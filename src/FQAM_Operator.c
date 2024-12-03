/*
    Copyright (C) 2024, Chuck Garcia

    This file is part of libfqam and is available under the 3-Clause
    BSD license, which can be found in the LICENSE file at the top-level
    directory, or at http://opensource.org/licenses/BSD-3-Clause
*/

#include "FQAM.h"
#include "stdbool.h"
#include "assertf.h"

#define incr_addr(i, j) ((i * rs) + (j * cs)) // Address incremental
// Initial dimension
// int dim_m = 2; 
// int dim_n = 2;
#include "FLAME.h" // Assuming

// Initialize an operator 
FQAM_Error FQAM_Op_create (FQAM_Op *operator, char *name, int dim)
{
  assertf (FQAM_initialized (), "Error: Stage must be initialized to create operator");

  // FLA_Obj_create (FLA_Complex, dim_m, dim_n, 0, 0, &operator->mat_repr);
  strcpy (operator->name, name);
  operator->initialized = true;
  
  FLA_Obj_create (FLA_DOUBLE_COMPLEX, pow (2, dim), pow (2, dim),0, 0, &operator->mat_repr);

  // // Set Matrix to zeros
  // FLA_Obj_create (FLA_COMPLEX, dim_m, dim_n, 0, 0, operator);

  // int *buf = FLA_Obj_buffer_at_view (operator->mat_repr);

  // // Access's array elements and set to proper values
  // int rs, cs;
  // rs = FLA_Obj_row_stride (operator->mat_repr);
  // cs = FLA_Obj_col_stride (operator->mat_repr);

  // *(buf + incr_addr (0, 0)) = 0;  // Set X[0,0] = 0
  // *(buf + incr_addr (1, 1)) = 0; // Set X[1,1] = 0
  // *(buf + incr_addr (0, 1)) = 0;  // Set X[0,1] = 0
  // *(buf + incr_addr (1, 0)) = 0;  // Set X[1,0] = 0
  
  return FQAM_SUCCESS;
}

// void FQAM_Op_add (FQAM_Op op, FLA_Datatype alpha, FQAM_Op *op_term);
// void FQAM_Op_outer (FQAM_Op op, FLA_Basis a, FLA_Basis b);
// void FQAM_

// VOID 
// 


// FQAM_Error FQAM_Operator_add (FQAM_Op *operator, FLA_Datatype alpha, FQAM_Op *op_term)
// {
//   assertf (FQAM_Operator_initialized (operator), "Error: Trying to modify uninitialized operator object");
//   assertf (operator->dimension == op_term->dimension, "Error: Mismatching hilbert dimensions in operator decomposition");
  
//   FLA_Obj alpha_obj;
//   FLA_Obj_create (FLA_COMPLEX, 1, 1, 0, 0, &alpha_obj);
  
//   int *buf = FLA_Obj_buffer_at_view (alpha_obj);

//   // Access's array elements and set to proper values
//   int rs, cs;
//   rs = FLA_Obj_row_stride (alpha_obj);
//   cs = FLA_Obj_col_stride (alpha_obj);

//   *(buf + incr_addr (0, 0)) = alpha;  // Set X[0,0] = 0
//   FLA_Axpy (alpha_obj, op_term->mat_repr, operator->mat_repr); 
  
// }

// FQAM_Operator_outer_product (FQAM_Op *op, int theta)
// {  
//   assertf (FQAM_Operator_initialized (operator), "Error: Trying to modify uninitialized operator object");
//   assertf (operator->dimension == op_term->dimension, "Error: Mismatching hilbert dimensions in operator decomposition")
// }


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
  assertf (FQAM_initialized (), "Error: Operator finalization requires main to be in initialized state");

  if (operator->initialized == true)
  {
    operator->initialized = false;
    FLA_Obj_free (&operator->mat_repr);
  }
}

bool FQAM_Operator_initialized (FQAM_Op *operator)
{
  return operator->initialized;
}

// Placeholder for a set of different functions to assist in creating operator matrix representations
// functions could include: tensor_product, generate_projector, measure_op, ect
// FQAM_Error FQAM_operator_generate_functions (FQAM_Op operator, char *name);

#define access(i, j) (buf + (i * rs) + (j * cs)) // Address incremental

/* Returns basis state as FLA_Obj matrix */
void gen_basis_matrix (FQAM_Basis basis, FLA_Obj *basis_mat)
{
  int m, n, rs, cs;
  
  // Column vector
  m = basis.m;
  n = basis.n;

  // Create buffer
  FLA_Obj_create(FLA_DOUBLE_COMPLEX, m, n, 0, 0, basis_mat);
  FLA_Set (FLA_ZERO, *basis_mat);
  
  dcomplex *buf = FLA_Obj_buffer_at_view (*basis_mat);

  // Access's array elements and set to proper values
  rs = FLA_Obj_row_stride (*basis_mat);
  cs = FLA_Obj_col_stride (*basis_mat);

  access (basis.eigen_value, 0)->real = 1.0;
}

// -------
/* Returns basis vector 
*/
FQAM_Basis FQAM_Basis_create   (int num_qubits, double angle, int eigen_value)
{
  assertf (eigen_value <= num_qubits, "Error: Improper eigen");
  
  FQAM_Basis basis;  
  basis.n = 1;
  basis.m = pow (2, num_qubits);
  basis.angle = angle;
  basis.eigen_value = eigen_value;
  return basis;  
}    

/*Returns basis state in an 'n' dimensional hilbert space */
// FQAM_Basis FQAM_Basis_state_N (int eigenstate, double angle, dim_t dim)
// {
  // FQAM_basis
// }

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
  FLA_Gerc (FLA_NO_CONJUGATE, FLA_CONJUGATE, alpha_obj, ket0_mat, ket1_mat, result->mat_repr);
  
}

// bool FQAM_Op_check      (FQAM_Op op); // Asserts 'boundary conditions