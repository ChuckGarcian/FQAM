/*
    Copyright (C) 2024, Chuck Garcia

    This file is part of libfqam and is available under the 3-Clause
    BSD license, which can be found in the LICENSE file at the top-level
    directory, or at http://opensource.org/licenses/BSD-3-Clause
*/

#include "FQAM.h"
#include "stdbool.h"

// Initialize an operator 
FQAM_Error FQAM_operator_create (FQAM_Op operator, char *name)
{
  if (FQAM_initialized () == false) return FQAM_FAILURE;
  
  // FLA_Obj_create (operator.mat_repr, );
  
  printf ("FQAM_operator_create Called \n"); 
  
  return FQAM_SUCCESS;
};

void FQAM_Operator_show (FQAM_Op operator)
{
  FLA_Obj_show (&operator.name, operator.mat_repr, "%d", "---Done---");  
}

// Free stage and operator resources. 
FQAM_Error FQAM_operator_finalize (void);

// Placeholder for a set of different functions to assist in creating operator matrix representations
// functions could include: tensor_product, generate_projector, measure_op, ect
FQAM_Error FQAM_operator_generate_functions (FQAM_Op operator, char *name);