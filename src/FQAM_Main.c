/*
    Copyright (C) 2024, Chuck Garcia

    This file is part of libfqam and is available under the 3-Clause
    BSD license, which can be found in the LICENSE file at the top-level
    directory, or at http://opensource.org/licenses/BSD-3-Clause
*/


#include <stdbool.h>
#include "FQAM.h"
#include "list.h"

#define ASSERTF_DEF_ONCE
#include "assertf.h"

extern void pauli_ops_init_ (void);

static int _FQAM_initialized = false;

// Stage data structure; Opaque (User should not access explicitly)
struct list stage;

// Adds operator to staging list
FQAM_Error FQAM_init (void)
{
  list_init (&stage);
  FLA_Init ();
  printf ("Initialized! \n");
  pauli_ops_init_ ();

  _FQAM_initialized = true;
  return FQAM_SUCCESS;
}

/* Free resources fqam. Includes all FQAM modules */
FQAM_Error FQAM_finalize (void)
{
  if (_FQAM_initialized == 0) return FQAM_FAILURE;

  struct list_elem *e;

  // Free all operator matrices
  e = list_head (&stage);
  
  while ((e = list_next (e)) != list_end (&stage))
  {
    FQAM_Op *op = list_entry (e, FQAM_Op, elem);
    FLA_Obj_free (&op->mat_repr);
  }
  
  FLA_Finalize ();
  printf ("Finalized! \n");
  return FQAM_SUCCESS;
}

/* Appends operator to stage.

Returns: FQAM_SUCCESS on success, other FQAM_FAILURE
Args:
  operator: Operator object. Must be initialized otherwise an assertion
            fault is triggered.
*/
FQAM_Error FQAM_stage_append (FQAM_Op operator)
{
  // Ensure operator has been initialized
  assertf (FLA_Obj_buffer_is_null (operator.mat_repr),
           "Error: Tried appending uninitialized operator object");

  list_push_back (&stage, &operator.elem);
  return FQAM_SUCCESS;
}

bool FQAM_initialized (void)
{
  return _FQAM_initialized;
}