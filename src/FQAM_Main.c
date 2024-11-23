/*
    Copyright (C) 2024, Chuck Garcia

    This file is part of libfqam and is available under the 3-Clause
    BSD license, which can be found in the LICENSE file at the top-level
    directory, or at http://opensource.org/licenses/BSD-3-Clause
*/

#include "FQAM.h"
#include "list.h"

// Stage data structure; Opaque (User should not access explicitly)
struct list stage; 

// Adds operator to staging list
FQAM_Error FQAM_init (void)
{
  list_init (&stage);
  FLA_Init ();
  printf ("Initialized! \n");
  return FQAM_SUCCESS;
}


/* Free resources fqam. Includes all FQAM modules */
FQAM_Error FQAM_finalize(void)
{
  struct list_elem *e;
  
  // Free all operator matrices 
  for (e = list_begin (&stage); e != list_end (&stage); e = list_next (e))
  {
    FQAM_Op *op = list_entry (e, FQAM_Op, elem);
    FLA_Obj_free (&op->mat_repr);
  }

  FLA_Finalize ();
  printf ("Finalized! \n");
  return FQAM_SUCCESS;
}