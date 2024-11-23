/*
    Copyright (C) 2024, Chuck Garcia

    This file is part of libfqam and is available under the 3-Clause
    BSD license, which can be found in the LICENSE file at the top-level
    directory, or at http://opensource.org/licenses/BSD-3-Clause
*/

#include <stdio.h>
#include "FLAME.h"
#include "FQAM.h"

int main (void)
{  
  FQAM_Op op;

  FQAM_init ();
  
  // FQAM_operator_create (op, "Hello");
  // FQAM_stage_append (op);
  FQAM_finalize ();
}

int test_1 (void)
{  
  FQAM_Op op;

  FQAM_init ();
  // FQAM_stage_append (op);
  FQAM_finalize ();
}