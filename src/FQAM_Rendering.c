/*
    Copyright (C) 2024, Chuck Garcia

    This file is part of libfqam and is available under the 3-Clause
    BSD license, which can be found in the LICENSE file at the top-level
    directory, or at http://opensource.org/licenses/BSD-3-Clause

*/
#include "FQAM.h"
#include "assertf.h"
#include "arraylist.h"
#include "raylib.h"

#include <stdio.h>

#define RECS_WIDTH 50
#define RECS_HEIGHT RECS_WIDTH

#define SCREEN_WIDTH 800
#define SCREEN_HEIGHT 450

#define ORIGIN_X 10
#define ORIGIN_Y 10

struct stage {
    FLA_Obj statevector;   // Quantum statevector 
    size_t dim;            // Dimension of hilbertspace
    size_t state_space;    // Statevector size
    arraylist *stage;      // Contain sequence applied computation steps
};

// Extern declaration of main_stage
extern struct stage main_stage;

int gen_path_probability_matrix(FLA_Obj A, FLA_Obj bt, FLA_Obj C);
void draw_transition_lines(Image *image, FLA_Obj adjacency_matrix, int time_step, const int spacing_x, const int spacing_y);
void draw_next_state(Image *image, FLA_Obj state, int time_step, const int spacing_x, const int spacing_y);
Color get_color_from_complex_amplitude(FLA_Obj amplitude);
double get_probability(FLA_Obj amplitude);

FQAM_Error FQAM_Render_feynman_diagram (void)
{  
    assertf(FQAM_initialized(), "Error: Expected core initialized");
      
    int screenWidth, screenHeight, depth, spacing_x, spacing_y;
    float rotation;
    FLA_Obj adjacency_matrix;  
    dim_t input_states, output_states;    
    Image result_image;
  
    InitWindow(1, 1, "FQAM Rendering: Feynman Paths");  
  
    depth = main_stage.stage->size + 1;
    spacing_x = RECS_WIDTH / 2;
    spacing_y = RECS_WIDTH / 2;

    assertf(spacing_x > 0, "Error: Unhandled negative spacing error");
    assertf(spacing_y > 0, "Error: Unhandled negative spacing error");

    screenWidth = depth * (RECS_WIDTH * 2.5 + spacing_x);                  
    screenHeight = main_stage.state_space * RECS_WIDTH * 1.5;

    // Create result image object
    result_image = GenImageColor(screenWidth, screenHeight, WHITE);    
    
    // Input states are rows and columns outputs
    input_states = main_stage.state_space;
    output_states = main_stage.state_space;
  
    // Create adjacency matrix buffer and set to zeros
    FLA_Obj_create(FLA_DOUBLE_COMPLEX, input_states, output_states, 0, 0, &adjacency_matrix);
    FLA_Set(FLA_ZERO, adjacency_matrix);

    // Draw initial state
    draw_next_state(&result_image, main_stage.statevector, 0, spacing_x, spacing_y);
  
    // Compute and draw transition probabilities 
    for (int time_step = 1; time_step < depth; time_step++) {    
        FLA_Obj operator, state, adjacency_matrix;
        
        // Compute next state and adjacency matrix
        operator = ((FQAM_Op *)arraylist_get(main_stage.stage, time_step - 1))->mat_repr;
        state = main_stage.statevector;
    
        apply_operator(operator);
        draw_next_state(&result_image, state, time_step, spacing_x, spacing_y);
    }  

    ExportImage(result_image, "saved_image.png");
    UnloadImage(result_image);
    return 0;
}

/*
Arguments:
  FLA_Obj A: Operator Matrix
  FLA_Obj bt: current state vector as row vector
    FLA_Obj C: Path probability adjacency matrix. Matrix specifying the probability
     amplitudes of edges connecting the i-th and (i + 1)-th state.
*/
int gen_path_probility_matrix ( FLA_Obj A, FLA_Obj bt, FLA_Obj C )
{
  FLA_Obj AL,    AR,       A0,  a1,  A2;

  FLA_Obj bLt,    bRt,       b0t,  beta1,  b2t;

  FLA_Obj CT,              C0,
          CB,              c1t,
                           C2;

  FLA_Part_1x2( A,    &AL,  &AR,      0, FLA_LEFT );

  FLA_Part_1x2( bt,    &bLt,  &bRt,      0, FLA_LEFT );

  FLA_Part_2x1( C,    &CT, 
                      &CB,            0, FLA_TOP );

  while ( FLA_Obj_width( AL ) < FLA_Obj_width( A ) ){

    FLA_Repart_1x2_to_1x3( AL,  /**/ AR,        &A0, /**/ &a1, &A2,
                           1, FLA_RIGHT );

    FLA_Repart_1x2_to_1x3( bLt,  /**/ bRt,        &b0t, /**/ &beta1, &b2t,
                           1, FLA_RIGHT );

    FLA_Repart_2x1_to_3x1( CT,                &C0, 
                        /* ** */            /* *** */
                                              &c1t, 
                           CB,                &C2,        1, FLA_BOTTOM );


    // Update 
    printf ("c1t Length: %d", FLA_Obj_length (c1t));
    printf ("c1t Width: %d", FLA_Obj_width (c1t));
    // FLA_Axpy (FLA_TRANSPOSE, beta1, a1, c1t);
    

    FLA_Cont_with_1x3_to_1x2( &AL,  /**/ &AR,        A0, a1, /**/ A2,
                              FLA_LEFT );

    FLA_Cont_with_1x3_to_1x2( &bLt,  /**/ &bRt,        b0t, beta1, /**/ b2t,
                              FLA_LEFT );

    FLA_Cont_with_3x1_to_2x1( &CT,                C0, 
                                                  c1t, 
                            /* ** */           /* *** */
                              &CB,                C2,     FLA_TOP );

  }

  return FLA_SUCCESS;
}

void draw_next_state(Image *image, FLA_Obj state, int time_step, const int spacing_x, const int spacing_y)
{
    assertf(FLA_Obj_is_vector(state), "Error: Expected statevector to be vector");
    _debug_show_state_data();

    int state_index = 0;
    int font_size = 13;
  
    FLA_Obj A, AT, A0, AB, a1t, A2;
  
    // Create copy to avoid modifying original state vector
    FLA_Obj_create_conf_to(FLA_NO_TRANSPOSE, state, &A);
    FLA_Copy(state, A);

    FLA_Part_2x1(A, &AT, &AB, 0, FLA_TOP);
  
    while (FLA_Obj_length(AT) < FLA_Obj_length(A)) {
        FLA_Repart_2x1_to_3x1(AT, &A0, &a1t, AB, &A2, 1, FLA_BOTTOM);
      
        Rectangle rec;
        int x_pos = ((RECS_WIDTH + spacing_x) * time_step) + ORIGIN_X;
        int y_pos = ((RECS_HEIGHT + spacing_y) * state_index) + ORIGIN_Y;
      
        // Create rectangle        
        rec.x = x_pos;
        rec.y = y_pos;
        rec.width = RECS_WIDTH;
        rec.height = RECS_HEIGHT;    
        Color color = get_color_from_complex_amplitude(a1t);

        // Create string to label state
        char state_label[100]; 
        sprintf(state_label, "|%d>", state_index);

        ImageDrawRectangleRec(image, rec, color);    
        ImageDrawText(image, state_label, x_pos + RECS_HEIGHT * 0.40, y_pos + RECS_HEIGHT * 0.40, font_size, BLUE);
    
        FLA_Cont_with_3x1_to_2x1(&AT, A0, a1t, &AB, A2, FLA_TOP);
        state_index++;
    }

    FLA_Obj_free(&a1t);
}

/* Returns scalar probability (Double) from given double complex FLA_Obj amplitude object */
double get_probability(FLA_Obj amplitude)
{
    double probability;
  
    FLA_Absolute_square(amplitude);
    probability = FLA_DOUBLE_COMPLEX_PTR(amplitude)->real;    
  
    assertf(probability <= 1.0, "Error: Somehow statevector has real probability > 1");
    assertf(probability >= 0.0, "Error: Somehow statevector has real probability < 0");
    return probability;
}

/* Returns raylib color from a FLA complex amplitude */
Color get_color_from_complex_amplitude(FLA_Obj amplitude)
{ 
    double probability = get_probability(amplitude);
    // Map the probability to a grayscale color intensity (reciprocal so darker areas mean greater probability)
    unsigned char intensity = (unsigned char)((1 - probability) * 150);
    return (Color){intensity, intensity, intensity, 255}; 
}
