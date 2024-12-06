/*
    Copyright (C) 2024, Chuck Garcia

    This file is part of libfqam and is available under the 3-Clause
    BSD license, which can be found in the LICENSE file at the top-level
    directory, or at http://opensource.org/licenses/BSD-3-Clause

*/
#include "FQAM.h"
#include "__kernels.h"
#include "arraylist.h"
#include "assertf.h"
#include "raylib.h"

#include <stdio.h>

#define RECS_SIZE 50

#define SCREEN_WIDTH 800
#define SCREEN_HEIGHT 450

#define ORIGIN_X 10
#define ORIGIN_Y 10

#define GET_X_POS(X) (((RECS_SIZE + spacing_x) * (X)) + ORIGIN_X)
#define GET_Y_POS(Y) (((RECS_SIZE + spacing_y) * (Y)) + ORIGIN_Y)

struct stage
{
  FLA_Obj statevector; // Quantum statevector
  size_t dim;          // Dimension of hilbertspace
  size_t state_space;  // Statevector size
  arraylist *stage;    // Contain sequence applied computation steps
};

// Extern declaration of main_stage
extern struct stage main_stage;
extern Color get_color_from_complex_amplitude (FLA_Obj amplitude);

int compute_probability_adjacency_matrix (FLA_Obj A, FLA_Obj state, FLA_Obj C);
void draw_transition_lines (Image *image, FLA_Obj adjacency_matrix, char *op_name,
                            int time_step, const int spacing_x, const int spacing_y,
                            const int thickness);
void draw_next_state (Image *image, FLA_Obj state, int time_step,
                      const int spacing_x, const int spacing_y);
// Color get_color_from_complex_amplitude (FLA_Obj amplitude);
double get_probability (FLA_Obj amplitude);
void ImageDrawLineEx (Image *dst, Vector2 start, Vector2 end, int thick,
                      Color color);
Color get_color_from_probability (FLA_Obj amplitude);

FQAM_Error FQAM_Render_feynman_diagram_no_lines (void)
{
  assertf (FQAM_initialized (), "Error: Expected core initialized");

  int screenWidth, screenHeight, depth, spacing_x, spacing_y, thickness;
  float rotation;

  FLA_Obj adjacency_matrix;

  dim_t input_states, output_states;
  Image result_image;

  InitWindow (1, 1, "FQAM Rendering: Feynman paths");

  // Rendering settings
  depth = main_stage.stage->size + 1;
  thickness = 10;

  spacing_x = 0;
  spacing_y = 0;

  screenWidth = depth * (RECS_SIZE * 2);
  screenHeight = main_stage.state_space * (RECS_SIZE * 2);

  // Create result image object
  result_image = GenImageColor (screenWidth, screenHeight, WHITE);

  // Draw initial state
  draw_next_state (&result_image, main_stage.statevector, 0, spacing_x, spacing_y);

  // Compute and draw transition probabilities
  for (int time_step = 1; time_step < depth; time_step++)
  {
    FLA_Obj operator, state;

    // Compute next state and adjacency matrix
    operator= ((FQAM_Op *)arraylist_get (main_stage.stage, time_step - 1))->mat_repr;
    state = main_stage.statevector;
    apply_operator (operator);
    draw_next_state (&result_image, state, time_step, spacing_x, spacing_y);

    printf ("Drew state: %d\n", time_step);
  }

  ExportImage (result_image, "saved_image.png");
  UnloadImage (result_image);
  return 0;
}

FQAM_Error FQAM_Render_feynman_diagram (void)
{
  assertf (FQAM_initialized (), "Error: Expected core initialized");

  int screenWidth, screenHeight, depth, spacing_x, spacing_y, thickness;
  float rotation;

  FLA_Obj adjacency_matrix;

  dim_t input_states, output_states;
  Image result_image;

  InitWindow (1, 1, "FQAM Rendering: Feynman paths");

  // Rendering settings
  depth = main_stage.stage->size + 1;
  thickness = 10;

  spacing_x = RECS_SIZE * 1.2;
  spacing_y = RECS_SIZE * 1.2;

  screenWidth = depth * (RECS_SIZE + spacing_x);
  screenHeight = main_stage.state_space * (RECS_SIZE + spacing_y);

  assertf (spacing_x > 0, "Error: Unhandled negative spacing error");
  assertf (spacing_y > 0, "Error: Unhandled negative spacing error");

  // Create result image object
  result_image = GenImageColor (screenWidth, screenHeight, WHITE);

  // Input states are rows and columns outputs
  input_states = main_stage.state_space;
  output_states = main_stage.state_space;

  // Create adjacency matrix buffer and set to zeros
  FLA_Obj_create (FLA_DOUBLE_COMPLEX, output_states, input_states, 0, 0,
                  &adjacency_matrix);

  // Draw initial state
  draw_next_state (&result_image, main_stage.statevector, 0, spacing_x, spacing_y);

  // Compute and draw transition probabilities
  for (int time_step = 1; time_step < depth; time_step++)
  {
    FLA_Obj state;
    FQAM_Op *operator;

    // Compute next state and adjacency matrix
    operator= ((FQAM_Op *)arraylist_get (main_stage.stage, time_step - 1));

    state = main_stage.statevector;

    FLA_Set (FLA_ZERO, adjacency_matrix);
    compute_probability_adjacency_matrix (operator->mat_repr, state,
                                          adjacency_matrix);
    apply_operator (operator->mat_repr);

    // TODO: Rethink ordering computation so transpose is completely avoided
    FLA_Transpose (adjacency_matrix);
    printf ("---Showing Adjacency---\n");
    _debug_show_fla_meta_data (adjacency_matrix);
    draw_transition_lines (&result_image, adjacency_matrix, operator->name,
                           time_step, spacing_x, spacing_y, thickness);
    draw_next_state (&result_image, state, time_step, spacing_x, spacing_y);
    printf ("Drew state: %d\n", time_step);
  }

  ExportImage (result_image, "saved_image.png");
  UnloadImage (result_image);
  return 0;
}

void draw_next_state (Image *image, FLA_Obj state, int time_step,
                      const int spacing_x, const int spacing_y)
{
  assertf (FLA_Obj_is_vector (state), "Error: Expected statevector to be vector");
  _debug_show_state_data ();

  int state_index = 0;
  int font_size = 13;

  FLA_Obj A, AT, A0, AB, a1t, A2;

  // Create copy to avoid modifying original state vector
  FLA_Obj_create_conf_to (FLA_NO_TRANSPOSE, state, &A);
  FLA_Copy (state, A);

  FLA_Part_2x1 (A, &AT, &AB, 0, FLA_TOP);

  while (FLA_Obj_length (AT) < FLA_Obj_length (A))
  {
    FLA_Repart_2x1_to_3x1 (AT, &A0, &a1t, AB, &A2, 1, FLA_BOTTOM);

    Rectangle rec;
    int x_pos = GET_X_POS (time_step) + ORIGIN_X;
    int y_pos = GET_Y_POS (state_index) + ORIGIN_Y;

    // Create rectangle
    rec.x = x_pos;
    rec.y = y_pos;
    rec.width = RECS_SIZE;
    rec.height = RECS_SIZE;

    Color color = get_color_from_probability (a1t);

    // Create string to label state
    char state_label[100];
    sprintf (state_label, "|%d>", state_index);

    ImageDrawRectangleRec (image, rec, color);
    ImageDrawText (image, state_label, x_pos + RECS_SIZE * 0.40,
                   y_pos + RECS_SIZE * 0.40, font_size, BLUE);

    FLA_Cont_with_3x1_to_2x1 (&AT, A0, a1t, &AB, A2, FLA_TOP);
    state_index++;
  }

  FLA_Obj_free (&a1t);
}
#define access(i, j) (buf + (i * rs) + (j * cs)) // Address incremental
/* Draws transition lines/edges between state layers from adjacent matrix 'A'
Arguments:
  image: Raylib image to draw to
  FLA_Obj A: Probability adjacency matrix. Contains resulting probability for each
state transition
*/
void draw_transition_lines (Image *image, FLA_Obj A, char *op_name, int time_step,
                            const int spacing_x, const int spacing_y,
                            const int thickness)
{

  int state_index = 0;
  int centering_delta = RECS_SIZE * 0.40;
  int font_size = 13;

  FLA_Obj AL, AR, A0, a1, A2;

  FLA_Part_1x2 (A, &AL, &AR, 0, FLA_LEFT);

  // Iterate through inputs (width of adjacency)
  while (FLA_Obj_width (AL) < FLA_Obj_width (A))
  {
    FLA_Repart_1x2_to_1x3 (AL, AR, &A0, &a1, &A2, 1, FLA_RIGHT);

    int next_state_index = 0;
    FLA_Obj B, BT, B0, BB, beta, B2;

    FLA_Part_2x1 (a1, &BT, &BB, 0, FLA_TOP);

    // Iterate through length of a column.
    // Draw edges in a1 which connect input and out
    while (FLA_Obj_length (BT) < FLA_Obj_length (a1))
    {
      FLA_Repart_2x1_to_3x1 (BT, &B0, &beta, BB, &B2, 1, FLA_BOTTOM);
      Rectangle rec;

      // Center transition lines (origin is top left)
      int curr_x_pos = GET_X_POS (time_step - 1) +
                       RECS_SIZE; // Don't draw over previous state recs
      int next_pos_x = GET_X_POS (time_step) + RECS_SIZE / 2;

      int curr_y_pos = GET_Y_POS (state_index) + RECS_SIZE / 2;
      int next_pos_y = GET_Y_POS (next_state_index) + RECS_SIZE / 2;

      _debug_show_fla_meta_data (a1);
      _debug_show_fla_meta_data (beta);

      // Only non-zero amplitudes
      if (get_probability (beta) > 0.01)
      {
        _debug_show_fla_meta_data (beta);
        Color color = get_color_from_complex_amplitude (beta);
        ImageDrawLineEx (image, (Vector2){curr_x_pos, curr_y_pos},
                         (Vector2){next_pos_x, next_pos_y}, thickness, color);
      }

      ImageDrawText (image, op_name, curr_x_pos + RECS_SIZE * 0.40, RECS_SIZE*.1, font_size, BLUE);
      FLA_Cont_with_3x1_to_2x1 (&BT, B0, beta, &BB, B2, FLA_TOP);
      next_state_index++;
    }

    state_index++;
    FLA_Cont_with_1x3_to_1x2 (&AL, &AR, A0, a1, A2, FLA_LEFT);
  }
}

/* Returns scalar probability (Double) from given double complex FLA_Obj amplitude
 * object */
double get_probability (FLA_Obj amplitude)
{
  double probability;
  // FLA_Absolute_square (amplitude);

  // Convert to C complex
  double x, y;
  x = FLA_DOUBLE_COMPLEX_PTR (amplitude)->real;
  y = FLA_DOUBLE_COMPLEX_PTR (amplitude)->imag;

  probability = x * x + y * y;

  assertf (probability <= 1.0,
           "Error: Somehow statevector has real probability > 1");
  assertf (probability >= 0.0,
           "Error: Somehow statevector has real probability < 0");

  return probability;
}

/* Normalize a value to a new range */
double normalize (double value, double min_old, double max_old, double min_new,
                  double max_new)
{
  return min_new + (value - min_old) * (max_new - min_new) / (max_old - min_old);
}

/* Returns raylib color from a FLA complex amplitude */
Color get_color_from_probability (FLA_Obj amplitude)
{
  double probability = get_probability (amplitude);

  // Map the probability to a grayscale color intensity (reciprocal so darker areas
  // mean greater probability)
  unsigned char intensity = (unsigned char)normalize (1 - probability, 0, 1, 0, 200);

  // double real_part = FLA_DOUBLE_COMPLEX_PTR(amplitude)->real;
  // double imag_part = FLA_DOUBLE_COMPLEX_PTR(amplitude)->imag;

  // real_part = real_part * real_part;
  // imag_part = imag_part * imag_part;

  // unsigned char r_intensity = (unsigned char)normalize(real_part, 0.0, 1.0, 50.0,
  // 200.0); unsigned char i_intensity = (unsigned char)normalize(imag_part,
  // 0.0, 1.0, 50.0, 200.0);

  return (Color){intensity, intensity, intensity, 250};
}

// Draw a line defining thickness within an image
void ImageDrawLineEx (Image *dst, Vector2 start, Vector2 end, int thick, Color color)
{
  // Round start and end positions to nearest integer coordinates
  int x1 = (int)(start.x + 0.5f);
  int y1 = (int)(start.y + 0.5f);
  int x2 = (int)(end.x + 0.5f);
  int y2 = (int)(end.y + 0.5f);

  // Calculate differences in x and y coordinates
  int dx = x2 - x1;
  int dy = y2 - y1;

  // Draw the main line between (x1, y1) and (x2, y2)
  ImageDrawLine (dst, x1, y1, x2, y2, color);

  // Determine if the line is more horizontal or vertical
  if (dx != 0 && abs (dy / dx) < 1)
  {
    // Line is more horizontal
    // Calculate half the width of the line
    int wy = (thick - 1) * (int)sqrtf ((float)(dx * dx + dy * dy)) / (2 * abs (dx));

    // Draw additional lines above and below the main line
    for (int i = 1; i <= wy; i++)
    {
      ImageDrawLine (dst, x1, y1 - i, x2, y2 - i, color); // Draw above the main line
      ImageDrawLine (dst, x1, y1 + i, x2, y2 + i, color); // Draw below the main line
    }
  }
  else if (dy != 0)
  {
    // Line is more vertical or perfectly horizontal
    // Calculate half the width of the line
    int wx = (thick - 1) * (int)sqrtf ((float)(dx * dx + dy * dy)) / (2 * abs (dy));

    // Draw additional lines to the left and right of the main line
    for (int i = 1; i <= wx; i++)
    {
      ImageDrawLine (dst, x1 - i, y1, x2 - i, y2,
                     color); // Draw left of the main line
      ImageDrawLine (dst, x1 + i, y1, x2 + i, y2,
                     color); // Draw right of the main line
    }
  }
}
