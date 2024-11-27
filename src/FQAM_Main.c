/*
    Copyright (C) 2024, Chuck Garcia

    This file is part of libfqam and is available under the 3-Clause
    BSD license, which can be found in the LICENSE file at the top-level
    directory, or at http://opensource.org/licenses/BSD-3-Clause
*/

#include <stdbool.h>
#include "FQAM.h"
#include "arraylist.h"
#include "assertf.h"

extern void _pauli_ops_init_ (void);  
static int _FQAM_initialized = false;

// Stage data structure; Opaque (User should not access explicitly)
arraylist *stage;

// Adds operator to staging list
FQAM_Error FQAM_init (void)
{  
  stage = arraylist_create ();  
  _FQAM_initialized = true;
  
  FLA_Init ();
  _pauli_ops_init_ ();
  
  printf ("FQAM: Initialized \n");
  return FQAM_SUCCESS;
}

/* Free resources FQAM. Includes all FQAM modules */
FQAM_Error FQAM_finalize (void)
{
  assertf (FQAM_initialized (), "Error: Tried appending uninitialized operator object");

  // Free all operator matrices
  for (int idx = 0; idx < stage->size; idx ++)
  {
    FQAM_Op *operator = arraylist_get (stage, idx);
    FQAM_Operator_free (operator);
  }

  FLA_Finalize ();  
  arraylist_destroy (stage);
  
  printf ("FQAM: Finalized\n");
  return FQAM_SUCCESS;
}

/* Appends operator to stage.

Returns: FQAM_SUCCESS on success, other FQAM_FAILURE
Args:
  operator: Operator object. Must be initialized otherwise an assertion
            fault is triggered.
*/
FQAM_Error FQAM_stage_append (FQAM_Op *operator)
{
  // Ensure operator has been initialized
  assertf (FQAM_Operator_initialized (operator), "Error: Tried appending uninitialized operator object");
  assertf (FLA_Obj_buffer_is_null (operator->mat_repr) == 0,
           "Error: Tried appending uninitialized operator object");

  
  arraylist_add (stage, (void *) operator);  
  
  return FQAM_SUCCESS;
}

bool FQAM_initialized (void)
{
  return _FQAM_initialized;
}

void FQAM_compute_outcomes (void)
{
  
}

/* Debug function which prints the stage list*/
void _debug_FQAM_show_stage (void)
{
  printf ("----Debug: Showing stage----\n");  
  assertf (FQAM_initialized (), "Error: FQAM expected to be in initialized state ");

  for (int idx = 0; idx < stage->size; idx ++)
  {
    printf ("Index: %d \n", idx); 
    FQAM_Op *op = arraylist_get (stage, idx);
    FQAM_Operator_show (op);    
  }

  printf ("----Debug: Done stage----\n");
}
