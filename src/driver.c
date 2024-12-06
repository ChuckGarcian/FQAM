/*
    Copyright (C) 2024, Chuck Garcia

    This file is part of libfqam and is available under the 3-Clause
    BSD license, which can be found in the LICENSE file at the top-level
    directory, or at http://opensource.org/licenses/BSD-3-Clause
*/

#include "FLAME.h"
#include "FQAM.h"
#include "__kernels.h"
#include <stdio.h>

#define ASSERTF_DEF_ONCE
#include "assertf.h"

static int test_dim = 1;
// General usage tests
void test_1 (void);
void test_2 (void);
void test_3 (void);
void test_4 (void);
void test_5 (void);

// Operator creation tests
void example_eye (void);
void test_not (void);
void example_hadamard (void);

// Tensor product tests
extern void test_tensor_1 (void);

// Render tests
void test_render_1 (void);

int main (void)
{
  // test_1 ();
  // test_2 ();
  // test_3 ();
  // test_4 ();
  // test_5 ();

  // test_render_1 ();
  // example_hadamard ();
  // example_not ();
  test_tensor_1 ();
}

void test_1 (void)
{
  // Test: basic test ensuring FQAM can initialize and finalize
  printf ("Running Test 1 \n");
  FQAM_init (4, 3);
  FQAM_show_statevector (); // Should output 0010
  FQAM_finalize ();
  printf ("Passed Test 1\n\n");
}

void test_2 (void)
{
  FQAM_Op not;

  // Test: Checking basic stage appending and finalization
  printf ("Running Test 2 \n");
  FQAM_init (1, 0);
  FQAM_Pauli_x (&not);
  FQAM_stage_append (not);
  FQAM_finalize ();
  printf ("Passed Test 2 \n \n");
}

/* How would the following be implemented? */
typedef struct
{
  // Tuple w/ Coordinates
  // Operation
  // Neighborhood
} FQAM_Qubit;

void test_3 (void)
{
  FQAM_Op not, y_op, eye_op;

  printf ("Running Test 3 \n");
  FQAM_init (test_dim, 0);

  // Generate gates
  FQAM_Pauli_x (&not);
  FQAM_Pauli_y (&y_op);
  FQAM_Pauli_eye (&eye_op);

  // Create circuits
  FQAM_stage_append (not);
  FQAM_stage_append (y_op);
  FQAM_stage_append (eye_op);
  _debug_FQAM_show_stage ();

  FQAM_Render_feynman_diagram ();
  FQAM_show_statevector ();
  FQAM_finalize ();
  printf ("Passed Test 3 \n \n");
}

void test_4 (void)
{
  printf ("Running Test 4 \n");

  // Testing hadamard  Initial state 0
  FQAM_init (1, 0);
  FQAM_Op hadamard;

  FQAM_hadamard (&hadamard);
  FQAM_stage_append (hadamard);
  FQAM_Render_feynman_diagram ();

  _debug_FQAM_show_stage ();
  FQAM_show_statevector ();
  printf ("Passed Test 4 \n");
}

void test_5 (void)
{
  printf ("Running Test 5 \n");

  // Testing hadamard: Initial state 1
  FQAM_init (1, 0);
  FQAM_Op hadamard, y_op, z_op, eye_op, x_op, p_op;

  FQAM_hadamard (&hadamard);
  FQAM_Pauli_y (&y_op);
  FQAM_Pauli_z (&z_op);
  FQAM_Pauli_x (&x_op);
  FQAM_Pauli_eye (&eye_op);
  // FQAM_Phase_T (&p_op);

  FQAM_show_statevector ();
  // FQAM_stage_append (p_op);
  FQAM_stage_append (eye_op);
  // FQAM_stage_append (hadamard);
  FQAM_stage_append (hadamard);
  FQAM_stage_append (p_op);
  // FQAM_stage_append (x_op);
  FQAM_stage_append (hadamard);
  FQAM_stage_append (hadamard);
  FQAM_stage_append (p_op);
  FQAM_stage_append (hadamard);
  // FQAM_stage_append (y_op);

  FQAM_stage_append (eye_op);
  // FQAM_stage_append (hadamard);

  FQAM_Render_feynman_diagram ();

  _debug_FQAM_show_stage ();
  FQAM_show_statevector ();
  printf ("Passed Test 5 \n \n");
}

void test_render_1 (void)
{
  printf ("Running Render Test 1 \n");
  FQAM_init (test_dim, 0);
  FQAM_show_statevector ();

  // FQAM_Pauli_eye ();
  // FQAM_Pauli_x ();
  // FQAM_Pauli_x ();
  // FQAM_Pauli_eye ();
  // FQAM_Pauli_x ();
  // _debug_FQAM_show_stage ();
  FQAM_Render_feynman_diagram ();
  printf ("Passed Render Test 1 \n");
}

void example_hadamard (void)
{
  FQAM_init (1, 0);
  FQAM_Op hadamard;

  FQAM_hadamard (&hadamard);
  FQAM_stage_append (hadamard);
  FQAM_Render_feynman_diagram ();

  _debug_FQAM_show_stage ();
  FQAM_show_statevector ();
  printf ("Done \n");
}

void example_eye (void)
{
  FQAM_init (test_dim, 0);
  FQAM_Basis ket0, ket1;
  FQAM_Op outer0, outer1, res;

  FQAM_Op_create (&res, "Eye\0", test_dim);

  ket0 = FQAM_Basis_create (1, 0, 0);
  ket1 = FQAM_Basis_create (1, 0, 1);

  // |0><0| + |1><1|
  FQAM_Basis_outer (ket0, ket0, &outer0); // |1><1|
  FQAM_Basis_outer (ket1, ket1, &outer1); // |1><1|

  FQAM_Op_add (FQAM_ONE, outer0, &res);
  FQAM_Op_add (FQAM_ONE, outer1, &res);

  _debug_FQAM_show_stage ();
  FQAM_stage_append (res);

  _debug_FQAM_show_stage ();
  FQAM_Render_feynman_diagram ();
  printf ("Done \n");
}

void test_not (void)
{
  FQAM_Op not;

  FQAM_Pauli_x (&not);

  _debug_FQAM_show_stage ();
  FQAM_stage_append (not);
  FQAM_stage_append (not);
  FQAM_stage_append (not);

  _debug_FQAM_show_stage ();
  FQAM_Render_feynman_diagram ();
  printf ("Done \n");
}
