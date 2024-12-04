/*
    Copyright (C) 2024, Chuck Garcia

    This file is part of libfqam and is available under the 3-Clause
    BSD license, which can be found in the LICENSE file at the top-level
    directory, or at http://opensource.org/licenses/BSD-3-Clause
*/

#include <math.h>
#include <stdbool.h>

#include "FQAM.h"
#include "arraylist.h"
#include "assertf.h"

static bool _FQAM_initialized = false;

/* Stage Struct */
struct stage
{
  FLA_Obj statevector; // Quantum statevector
  size_t dim;          // Dimension of hilbertspace
  size_t state_space;  // Statevector size
  arraylist *stage;    // Contain sequence applied computation steps
} main_stage;

void _debug_show_state_data (void);

/*
Arguments:
    size_t dim: Dimension of Hilbert space
    unsigned int initial_state: Nonnegative integer to initialize statevector to
*/
void FQAM_init (size_t dim, unsigned int initial_state)
{
  assertf (initial_state < pow (2, dim),
           "Error: Initial state must be within hilbert space");

  // Initialize Flame
  FLA_Init ();

  // Initialize Statevector buffer
  dcomplex *buf;
  int state_space = pow (2, dim);

  buf = (dcomplex *)calloc (sizeof (dcomplex), pow (2, dim));
  assertf (buf, "Error: Failed to allocate statevector");

  buf[initial_state].real = 1.0;
  buf[initial_state].imag = 0.0;

  // Initialize Stage
  FLA_Obj_create_without_buffer (FLA_DOUBLE_COMPLEX, state_space, 1,
                                 &main_stage.statevector);
  FLA_Obj_attach_buffer (buf, 1, state_space, &main_stage.statevector);

  main_stage.state_space = state_space;
  main_stage.dim = dim;
  main_stage.stage = arraylist_create ();
  _FQAM_initialized = true;

  FLA_Obj_buffer_at_view (main_stage.statevector);

  // TODO: Add way to pass if built in operators should be initialized
  // pauli_ops_init_ ();
  printf ("FQAM: Initialized\n");
}

typedef int Local;

void FQAM_Part_as_lattice (Local neigh) {
  //
};

/*
Free resources in FQAM. Includes all FQAM modules.
*/
void FQAM_finalize (void)
{
  assertf (FQAM_initialized (), "Error: Tried to finalize uninitialized FQAM");

  // Free all operator matrices
  for (int idx = 0; idx < main_stage.stage->size; idx++)
  {
    FQAM_Op *operator= arraylist_get (main_stage.stage, idx);
    FQAM_Operator_free (operator->stack_addr);
  }

  FLA_Set (FLA_ZERO, main_stage.statevector);
  FLA_Finalize ();
  arraylist_destroy (main_stage.stage);

  printf ("FQAM: Finalized\n");
  return FQAM_SUCCESS;
}

/* Print Current State vector*/
void FQAM_show_statevector (void)
{
  assertf (FQAM_initialized (), "Error: Expected stage initialized");
  assertf (FLA_Obj_buffer_is_null (main_stage.statevector) == false,
           "Error: Statevector FLA object found as NULL");

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
void FQAM_stage_append (FQAM_Op operator)
{
  // Ensure operator has been initialized
  assertf (FQAM_Operator_initialized (operator.stack_addr),
           "Error: Tried appending uninitialized operator object");
  assertf (FLA_Obj_buffer_is_null (operator.mat_repr) == 0,
           "Error: Tried appending operator with null matrix representation");

  arraylist_add (main_stage.stage, operator.stack_addr);

  return FQAM_SUCCESS;
}

bool FQAM_initialized (void) { return _FQAM_initialized; }

/* Applys operator to state vector */
void apply_operator (FLA_Obj A)
{
  FLA_Obj x, y_tmp;
  x = main_stage.statevector;
  FLA_Obj_create_conf_to (FLA_NO_TRANSPOSE, x, &y_tmp); // Temp

  /* y = A * x */
  FLA_Gemv (FLA_NO_TRANSPOSE, FLA_ONE, A, x, FLA_ZERO, y_tmp);

  // Copy y_tmp back to x
  FLA_Copy (y_tmp, x);

  // Free the temporary object
  FLA_Obj_free (&y_tmp);
}

void FQAM_compute_outcomes (void)
{
  assertf (FQAM_initialized (), "Error: Computing in uninitialized stage\n");

  for (int idx = 0; idx < main_stage.stage->size; idx++)
  {
    FQAM_Op *operator= arraylist_get (main_stage.stage, idx);
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
  dim_t i, j, m, n;
  dim_t rs, cs;

  datatype = FLA_Obj_datatype (main_stage.statevector);
  m = FLA_Obj_length (main_stage.statevector);
  n = FLA_Obj_width (main_stage.statevector);
  rs = FLA_Obj_row_stride (main_stage.statevector);
  cs = FLA_Obj_col_stride (main_stage.statevector);

  printf ("datatype: %d\n", datatype);
  printf ("length (m): %d\n", m);
  printf ("width (n): %d\n", n);
  printf ("row stride (rs): %ld\n", rs);
  printf ("col stride (cs): %ld\n", cs);
}

void _debug_show_fla_meta_data (FLA_Obj A)
{
  int datatype;
  dim_t i, j, m, n;
  dim_t rs, cs;
  bool is_vector, is_scalar;

  datatype = FLA_Obj_datatype (A);
  m = FLA_Obj_length (A);
  n = FLA_Obj_width (A);
  rs = FLA_Obj_row_stride (A);
  cs = FLA_Obj_col_stride (A);
  is_vector = FLA_Obj_is_vector (A);
  is_scalar = FLA_Obj_is_scalar (A);

  printf ("\ndatatype: %d\n", datatype);
  printf ("length (m): %d\n", m);
  printf ("width (n): %d\n", n);
  printf ("row stride (rs): %ld\n", rs);
  printf ("col stride (cs): %ld\n", cs);
  printf ("vector: %d\n", is_vector);
  printf ("scalar: %d\n", is_scalar);

  FLA_Obj_show ("---Printing FLA Object---", A, "%e", "---Done Printing---");
}
