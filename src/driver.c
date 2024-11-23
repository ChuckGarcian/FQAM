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
  FQAM_init ();
  FQAM_finalize ();
}

int main2 (void)
{
  FLA_Obj r;
  int mn = 4;
  FLA_Obj_create (FLA_COMPLEX, mn, 1, 0, 0, &r);
  
  char *header = "Header!";
  char *format = "%d";
  char *footer = "Footer!";
  
  FLA_Obj_show (header, r, format, footer);
  int dim = FLA_Obj_vector_dim (r);
  printf ("dim: %d", dim);
  // FQAM_show_diagram ();
  printf ("\n\n----Finished Executing Main----\n");
}