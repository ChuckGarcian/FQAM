/*
    Copyright (C) 2024, Chuck Garcia

    This file is part of libfqam and is available under the 3-Clause
    BSD license, which can be found in the LICENSE file at the top-level
    directory, or at http://opensource.org/licenses/BSD-3-Clause
*/

#include "FQAM.h"
#include "stdbool.h"

#include "assertf.h"


// Initialize an operator 
FQAM_Error FQAM_Operator_init (FQAM_Op *operator, char *name)
{
  assertf (FQAM_initialized (), "Error: Stage must be initialized to create operator");
  
  strcpy (operator->name, name);
  operator->initialized = true;
  
  return FQAM_SUCCESS;
};

void FQAM_Operator_show (FQAM_Op *operator)
{
  printf ("Operator: %s", operator->name);
  FLA_Obj_show ("", operator->mat_repr, "%d", "");  
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



