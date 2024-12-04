/*
    Copyright (C) 2024, Chuck Garcia

    This file is part of libfqam and is available under the 3-Clause
    BSD license, which can be found in the LICENSE file at the top-level
    directory, or at http://opensource.org/licenses/BSD-3-Clause
*/

#include "FQAM.h"
#include "assertf.h"
#include "stdbool.h"

#define access(i, j) (buf + (i * rs) + (j * cs)) // Address incremental

#include "FLAME.h" // Assuming

// Initialize an operator
FQAM_Error FQAM_Op_create (FQAM_Op *operator, char * name, int dim)
{
  assertf (FQAM_initialized (),
           "Error: Stage must be initialized to create operator");

  strcpy (operator->name, name);
  operator->initialized = true;
  operator->stack_addr = operator;
  
  FLA_Obj_create (FLA_DOUBLE_COMPLEX, pow (2, dim), pow (2, dim), 0, 0,
                  &operator->mat_repr);

  return FQAM_SUCCESS;
}

void FQAM_Operator_show (FQAM_Op *operator)
{
  printf ("Operator: %s", operator->name);

  FLA_Obj_show ("", operator->mat_repr, "%11.3e", "");
}

/* Free operators resources and marks as uninitialized.
Calling of this function is idempotent. That is attempting to free an already
uninitialized operator is safe and simply does nothing.
*/
FQAM_Error FQAM_Operator_free (FQAM_Op *operator)
{
  assertf (FQAM_initialized (),
           "Error: Operator finalization requires main to be in initialized state");

  if (operator->initialized == true)
  {
    operator->initialized = false;
    FLA_Obj_free (&operator->mat_repr);
  }
}

bool FQAM_Operator_initialized (FQAM_Op *operator) { return operator->initialized; }

/* Returns basis state as FLA_Obj matrix */
void gen_basis_matrix (FQAM_Basis basis, FLA_Obj *basis_mat)
{
  int m, n, rs, cs;

  // Column vector
  m = basis.m;
  n = basis.n;

  // Create buffer
  FLA_Obj_create (FLA_DOUBLE_COMPLEX, m, n, 0, 0, basis_mat);
  FLA_Set (FLA_ZERO, *basis_mat);

  dcomplex *buf = FLA_Obj_buffer_at_view (*basis_mat);

  // Access's array elements and set to proper values
  rs = FLA_Obj_row_stride (*basis_mat);
  cs = FLA_Obj_col_stride (*basis_mat);
  
  access (basis.eigen_value, 0)->real = 1.0;
}

/* Returns basis vector
 */
FQAM_Basis FQAM_Basis_create (int num_qubits, double angle, int eigen_value)
{
  assertf (eigen_value <= num_qubits, "Error: Improper eigen");

  FQAM_Basis basis;
  basis.n = 1;
  basis.m = pow (2, num_qubits);
  basis.angle = angle;
  basis.eigen_value = eigen_value;
  return basis;
}

void FQAM_Basis_outer (FQAM_Basis ket0, FQAM_Basis ket1, FQAM_Op *outer)
{
  outer->outer_ket0 = ket0;
  outer->outer_ket1 = ket1;
  outer->outer = true;
}

void FQAM_Op_add (dcomplex alpha, FQAM_Op term, FQAM_Op *result)
{

  FLA_Obj x, y;

  // Get matrix representations of basis states
  FLA_Obj ket0_mat, ket1_mat, alpha_obj;

  // Generate FLA matrices
  gen_basis_matrix (term.outer_ket0, &ket0_mat);
  gen_basis_matrix (term.outer_ket1, &ket1_mat);

  // Generate ALPHA FLA
  FLA_Obj_create_complex_constant (alpha.real, alpha.imag, &alpha_obj);

  /* A =  alpha * x * y' + A */
  FLA_Gerc (FLA_NO_CONJUGATE, FLA_CONJUGATE, alpha_obj, ket0_mat, ket1_mat,
            result->mat_repr);
}

// bool FQAM_Op_check      (FQAM_Op op); // Asserts 'boundary conditions