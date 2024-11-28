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
  size_t state_size;     // Statevector size
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
    int state_size = pow (2, dim);
    
    buf = (dcomplex *) calloc (sizeof (dcomplex), pow (2, dim));
    assertf (buf, "Error: Failed to allocate statevector");
    
    buf[initial_state].real = 1.0;
    buf[initial_state].imag = 0.0;

    // Initialize Stage 
    FLA_Obj_create_without_buffer (FLA_DOUBLE_COMPLEX, state_size, 1, &main_stage.statevector); 
    FLA_Obj_attach_buffer (buf, 1, state_size, &main_stage.statevector);   

    main_stage.state_size = state_size;
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
void apply_operator(FQAM_Op *op)
{
    FLA_Obj A, x, y_tmp;

    A = op->mat_repr;
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
        apply_operator (operator);
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

// #define MAX_RECS_X 4
// #define MAX_RECS_Y 2

#define PLAY_TIME_IN_FRAMES 240 // At 60 fps = 4 seconds

/* Stage Layer struct for rendering */
struct Stage_Layer {  
  Rectangle *recs; 
  Color color;
} layer_recs;

void init_grid (struct Stage_Layer *layer_recs, int max_recs_x, int max_recs_y);
void color_grid (struct Stage_Layer *layer_recs, int layer_idx);

FQAM_Error FQAM_show_diagram (void)
{
  assertf (FQAM_initialized (), "Error: Expected core initialized");
  
  const int screenWidth = SCREEN_WIDTH;
  const int screenHeight = SCREEN_HEIGHT;

  int max_recs_x = main_stage.stage->size + 1;
  int max_recs_y = main_stage.state_size;
  
  struct Stage_Layer layers = {0};
  layers.recs = (Rectangle *) calloc (sizeof (Rectangle), max_recs_x * max_recs_y);
  layers.color = BLUE; // Initial Color

  InitWindow (screenWidth, screenHeight,
              "raylib [shapes] example - easings rectangle array");
  printf ("Max_recs_x: %d \n", max_recs_x);
  printf ("Max_recs_y: %d \n", max_recs_y);    
  init_grid (&layers, max_recs_x, max_recs_y);

  float rotation = 0.0f;
  int framesCounter = 0;

  SetTargetFPS (60);
  int idx = 0;
    

  while (!WindowShouldClose ())
  {
    
    // if (idx < main_stage.stage->size)
    // {
    //   FQAM_Op *operator = arraylist_get (main_stage.stage, idx);
    //   apply_operator (operator);
    //   color_grid (&layer_recs, idx);
    // }
    
    BeginDrawing ();
    ClearBackground (RAYWHITE);

    
    for (int i = 0; i < max_recs_x * max_recs_y; i++)
    {
      DrawRectanglePro (layers.recs[i], (Vector2){layers.recs[i].width, layers.recs[i].height}, rotation,
                        layers.color);
    }

    EndDrawing ();
  }

  CloseWindow ();

  return 0;
}

void init_grid (struct Stage_Layer *layer_recs, int max_recs_x, int max_recs_y)
{
  float spacingX = (SCREEN_WIDTH - (max_recs_x * RECS_WIDTH)) / max_recs_x;
  float spacingY = (SCREEN_HEIGHT - (max_recs_y * RECS_HEIGHT)) / max_recs_y;

  assertf (spacingX > 0, "Error: Unhandled negative spacing error");
  assertf (spacingY > 0, "Error: Unhandled negative spacing error");

  for (int y = 0; y < max_recs_y; y++)
  {
    for (int x = 0; x < max_recs_x; x++)
    {
      layer_recs->recs[y * max_recs_x + x].x = ((RECS_WIDTH + spacingX) * x) + RECS_WIDTH * 2;
      layer_recs->recs[y * max_recs_x + x].y = ((RECS_HEIGHT + spacingY) * y) + RECS_HEIGHT * 2;
      layer_recs->recs[y * max_recs_x + x].width = RECS_WIDTH;
      layer_recs->recs[y * max_recs_x + x].height = RECS_HEIGHT;
    }
  }
}

// void color_grid (struct Stage_Layer *layer_recs, int layer_idx)
// {
//   for (int y = 0; y < MAX_RECS_Y; y++) 
//   {
//     layer_recs->recs[y * MAX_RECS_X + layer_idx].x = ((RECS_WIDTH + spacingX) * x) + RECS_WIDTH * 2;
//     layer_recs->recs[y * MAX_RECS_X + layer_idx].y = ((RECS_HEIGHT + spacingY) * y) + RECS_HEIGHT * 2;
//     layer_recs->recs[y * MAX_RECS_X + layer_idx].width = RECS_WIDTH;
//     layer_recs->recs[y * MAX_RECS_X + layer_idx].height = RECS_HEIGHT;    
//   }
// }