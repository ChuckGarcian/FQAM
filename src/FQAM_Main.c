/*
    Copyright (C) 2024, Chuck Garcia

    This file is part of libfqam and is available under the 3-Clause
    BSD license, which can be found in the LICENSE file at the top-level
    directory, or at http://opensource.org/licenses/BSD-3-Clause
*/

#include <stdbool.h>
#include <math.h>

#include "arraylist.h"
#include "assertf.h"
#include "FQAM.h"

static bool _FQAM_initialized = false;

/* Stage Struct */
static struct stage {
  FLA_Obj statevector;   // Quantum statevector 
  size_t dim;            // Dimension of hilbertspace
  size_t state_space;     // Statevector size
  arraylist *stage;      // Contain sequence applied computation steps
} main_stage;

void _debug_show_state_data (void);

/*
Arguments:
    size_t dim: Dimension of Hilbert space
    unsigned int initial_state: Nonnegative integer to initialize statevector to
*/
void FQAM_init (size_t dim, unsigned int initial_state)
{
    assertf (initial_state < pow (2, dim), "Error: Initial state must be within hilbert space");
    
    // Initialize Flame
    FLA_Init ();
    
    // Initialize Statevector buffer
    dcomplex *buf;
    int state_space = pow (2, dim);
    
    buf = (dcomplex *) calloc (sizeof (dcomplex), pow (2, dim));
    assertf (buf, "Error: Failed to allocate statevector");
    
    buf[initial_state].real = 1.0;
    buf[initial_state].imag = 0.0;

    // Initialize Stage 
    FLA_Obj_create_without_buffer (FLA_DOUBLE_COMPLEX, state_space, 1, &main_stage.statevector); 
    FLA_Obj_attach_buffer (buf, 1, state_space, &main_stage.statevector);   

    main_stage.state_space = state_space;
    main_stage.dim        = dim;
    main_stage.stage      = arraylist_create ();
    _FQAM_initialized     = true;
    
    FLA_Obj_buffer_at_view (main_stage.statevector);

    // TODO: Add way to pass if built in operators should be initialized
    pauli_ops_init_ (); 
    printf ("FQAM: Initialized\n");
}

/*
Free resources in FQAM. Includes all FQAM modules.
*/
void FQAM_finalize (void)
{
    assertf (FQAM_initialized (), "Error: Tried to finalize uninitialized FQAM");

    // Free all operator matrices
    for (int idx = 0; idx < main_stage.stage->size; idx++)
    {
        FQAM_Op *operator = arraylist_get (main_stage.stage, idx);
        FQAM_Operator_free (operator);
    }

    FLA_Finalize ();
    arraylist_destroy (main_stage.stage);
    
    printf ("FQAM: Finalized\n");
    return FQAM_SUCCESS;
}

/* Print Current State vector*/
void FQAM_show_statevector (void)
{
  assertf (FQAM_initialized (), "Error: Expected stage initialized");
  assertf (FLA_Obj_buffer_is_null (main_stage.statevector) == false, "Error: Statevector FLA object found as NULL");
    
  FLA_Obj_show ("Statevector: \n", main_stage.statevector, "%11.3e", "");    
  printf ("\n\n");
}

/*
Appends operator to stage.

Returns: 
    FQAM_SUCCESS on success, FQAM_FAILURE otherwise

Arguments:
    operator: Operator object. Must be initialized, otherwise an assertion
             fault is triggered.
*/
void FQAM_stage_append (FQAM_Op *operator)
{
    // Ensure operator has been initialized
    assertf (FQAM_Operator_initialized (operator),
            "Error: Tried appending uninitialized operator object");
    assertf (FLA_Obj_buffer_is_null (operator->mat_repr) == 0,
            "Error: Tried appending operator with null matrix representation");
    
    arraylist_add (main_stage.stage, (void *)operator);
    
    return FQAM_SUCCESS;
}

bool FQAM_initialized (void)
{
  return _FQAM_initialized;
}

/* Applys operator to state vector */
void apply_operator(FLA_Obj A)
{
    FLA_Obj x, y_tmp;
    x = main_stage.statevector;
    FLA_Obj_create_conf_to(FLA_NO_TRANSPOSE, x, &y_tmp); // Temp

    /* y = A * x */
    FLA_Gemv(FLA_NO_TRANSPOSE, FLA_ONE, A, x, FLA_ZERO, y_tmp);

    // Copy y_tmp back to x 
    FLA_Copy(y_tmp, x);

    // Free the temporary object 
    FLA_Obj_free(&y_tmp);
}

void FQAM_compute_outcomes (void)
{
    assertf (FQAM_initialized (), "Error: Computing in uninitialized stage\n");

    for (int idx = 0; idx < main_stage.stage->size; idx++)
    {
        FQAM_Op *operator = arraylist_get (main_stage.stage, idx);
        apply_operator (operator->mat_repr);
    }
}

/*
Debug function which prints the stage list
*/
void _debug_FQAM_show_stage (void)
{
    printf ("----Debug: Showing stage----\n");
    
    assertf (FQAM_initialized (), "Error: FQAM expected to be in initialized state");

    for (int idx = 0; idx < main_stage.stage->size; idx++)
    {
        printf ("Index: %d\n", idx);
        
        FQAM_Op *op = arraylist_get (main_stage.stage, idx);
        FQAM_Operator_show (op);
    }
    
    printf ("----Debug: Done stage----\n");
}

void _debug_show_state_data (void)
{
    int datatype;
    dim_t        i, j, m, n;
    dim_t        rs, cs;

    
    datatype = FLA_Obj_datatype(main_stage.statevector);
    m        = FLA_Obj_length(main_stage.statevector);
    n        = FLA_Obj_width(main_stage.statevector);
    rs       = FLA_Obj_row_stride(main_stage.statevector);
    cs       = FLA_Obj_col_stride(main_stage.statevector);
    
    printf("datatype: %d\n", datatype);
    printf("length (m): %d\n", m);
    printf("width (n): %d\n", n);
    printf("row stride (rs): %ld\n", rs);
    printf("col stride (cs): %ld\n", cs);
}

/*
    Copyright (C) 2024, Chuck Garcia

    This file is part of libfqam and is available under the 3-Clause
    BSD license, which can be found in the LICENSE file at the top-level
    directory, or at http://opensource.org/licenses/BSD-3-Clause

*/

#include "FQAM_Rendering.h"
#include "assertf.h"
#include "raylib.h"
#include <stdio.h>

#include "raylib.h"

#define RECS_WIDTH 25
#define RECS_HEIGHT 25

#define SCREEN_WIDTH 800
#define SCREEN_HEIGHT 450

int gen_path_probility_matrix ( FLA_Obj A, FLA_Obj bt, FLA_Obj C );

void draw_transition_lines (Image *image, FLA_Obj adjacency_matrix, int time_step, const int spacing_x, const int spacing_y);
void draw_next_state (Image *image, FLA_Obj state, int time_step, const int spacing_x, const int spacing_y);

FQAM_Error FQAM_show_diagram (void)
{  
  assertf (FQAM_initialized (), "Error: Expected core initialized");
  
  int screenWidth, screenHeight; 
  const int spacing_x, spacing_y;
  float rotation;
  
  FLA_Obj adjacency_matrix;  

  screenWidth  = SCREEN_WIDTH;
  screenHeight = SCREEN_HEIGHT;
  
  dim_t input_states, output_states;    
  Image result_image;

  // Create result image object
  result_image = GenImageColor (screenWidth, screenHeight, RED);    
    
  // Input states are rows and columns outputs
  input_states = main_stage.state_space;
  output_states = main_stage.state_space;
  
  // Create adjacency matrix buffer and set to zeros
  FLA_Obj_create (FLA_DOUBLE_COMPLEX, input_states, output_states, 0, 0, &adjacency_matrix);
  FLA_set (FLA_ZERO, adjacency_matrix);

  // Compute and draw transition probabilities 
  for (int time_step = 0; time_step < main_stage.stage->size; )
  {    
    FLA_Obj operator, state, adjacency_matrix;
    
    // Compute next state and adjacency matrix
    operator = ((FQAM_Op *) arraylist_get (main_stage.stage, time_step))->mat_repr;
    state = main_stage.statevector;
    
    if (time_step) // Fence post
      gen_path_probility_matrix (operator, state, adjacency_matrix);
    apply_operator (operator);
    
    // Draw next state and adjacency matrix
    draw_next_state (&result_image, state, time_step, spacing_x, spacing_y);
    draw_transition_lines     (&result_image, adjacency_matrix, time_step, spacing_x, spacing_y);            
  }  

  ExportImage (result_image, "saved_image.png");
  UnloadImage (result_image);
  return 0;
}

/* Copyright 2024 The University of Texas at Austin  
 
   For licensing information see
                  http://www.cs.utexas.edu/users/flame/license.html 

   Programmed by: Name of author
                  Email of author
                                                                     */

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
    print ("c1t Length: %d", FLA_Obj_length (c1t));
    print ("c1t Width: %d", FLA_Obj_width (c1t));
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

void draw_transition_lines (Image *image, FLA_Obj adjacency_matrix, int time_step, const int spacing_x, const int spacing_y)
{
  int max_recs_x, max_recs_y;
  int length, width;
  
  max_recs_x = FLA_Obj_length (adjacency_matrix);
  
  length = FLA_Obj_length (adjacency_matrix);
  width  = FLA_Obj_width (adjacency_matrix);
  
  // float spacingX = (SCREEN_WIDTH - (max_recs_x * RECS_WIDTH)) / max_recs_x;
  float spacingY = (SCREEN_HEIGHT - (max_recs_y * RECS_HEIGHT)) / max_recs_y;

  // assertf (spacingX > 0, "Error: Unhandled negative spacing error");
  // assertf (spacingY > 0, "Error: Unhandled negative spacing error");

  BeginDrawing ();


  for (int x = 0; x < length; x++)
  {
    layer_recs->recs[y * max_recs_x + x].x = ((RECS_WIDTH + spacing_x) * x) + RECS_WIDTH * 2;
    layer_recs->recs[y * max_recs_x + x].y = ((RECS_HEIGHT + spacing_y) * y) + RECS_HEIGHT * 2;
    layer_recs->recs[y * max_recs_x + x].width = RECS_WIDTH;
    layer_recs->recs[y * max_recs_x + x].height = RECS_HEIGHT;
    
    if (main_stage.stage)
      layer_recs->color = GRAY
  }
  }    
  
  for (int i = 0; i < max_recs_x * max_recs_y; i++)
  {
    DrawRectanglePro (layers.recs[i], (Vector2){layers.recs[i].width, layers.recs[i].height}, rotation,
                      layers.color);
  }

}

void draw_next_state (Image *image, FLA_Obj state, int time_step, const int spacing_x, const int spacing_y)
{
  
  int width  = FLA_Obj_width (state)    
  
  // Render each basis state 
  for (int y = 0; y < width; y++)
  {
    Rectangle rec;
    
    rec.x      = ((RECS_WIDTH + spacingX) * time_step) + RECS_WIDTH * 2;
    rec.y      = ((RECS_HEIGHT + spacingY) * y) + RECS_HEIGHT * 2;
    rec.width  = RECS_WIDTH;
    rec.height = RECS_HEIGHT;    
    
    Color color = RED;    
    
    DrawRectanglePro (rec, (Vector2){rec.width, rec.height}, rotation, color);
    }
}    


// void color_grid (struct Stage_Layer *layer_recs, int layer_idx, )
// {
//   for (int y = 0; y < MAX_RECS_Y; y++) 
//   {
//     layer_recs->recs[y * MAX_RECS_X + layer_idx].x = ((RECS_WIDTH + spacingX) * x) + RECS_WIDTH * 2;
//     layer_recs->recs[y * MAX_RECS_X + layer_idx].y = ((RECS_HEIGHT + spacingY) * y) + RECS_HEIGHT * 2;
//     layer_recs->recs[y * MAX_RECS_X + layer_idx].width = RECS_WIDTH;
//     layer_recs->recs[y * MAX_RECS_X + layer_idx].height = RECS_HEIGHT;    
//   }
// }