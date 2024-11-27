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
int dim_m = 2; 
int dim_n = 2;

// Initialize an operator 
FQAM_Error FQAM_Operator_init (FQAM_Op *operator, char *name)
{
  assertf (FQAM_initialized (), "Error: Stage must be initialized to create operator");

  // FLA_Obj_create (FLA_Complex, dim_m, dim_n, 0, 0, &operator->mat_repr);
  strcpy (operator->name, name);
  operator->initialized = true;
  
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
};

// FQAM_Error FQAM_Operator_add(FQAM_Op *operator, FLA_Datatype alpha, FQAM_Op *op_term)
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
FQAM_Error FQAM_operator_generate_functions (FQAM_Op operator, char *name);


