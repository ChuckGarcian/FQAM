/*
    Copyright (C) 2024, Chuck Garcia

    This file is part of libfqam and is available under the 3-Clause
    BSD license, which can be found in the LICENSE file at the top-level
    directory, or at http://opensource.org/licenses/BSD-3-Clause
*/

#include "FLAME.h"
#include "FQAM.h"
#include <stdio.h>

#define ASSERTF_DEF_ONCE
#include "assertf.h"

void test_1 (void);
void test_2 (void);
void test_3 (void);

int main (void)
{
  test_1();
  test_2();
  test_3();  
}

void test_1 (void)
{
  // Test: basic test ensuring FQAM can initialize and finalize
  printf ("Running Test 1 \n");
  FQAM_init ();
  FQAM_finalize ();
  printf ("Passed Test 1\n\n");
}

void test_2 (void)
{
  // Test: Checking basic stage appending and finalization
  printf ("Running Test 2 \n");
  FQAM_init ();
  FQAM_Pauli_x ();
  FQAM_finalize ();
  printf ("Passed Test 2 \n \n");
}

void test_3 (void)
{
  printf ("Running Test 3 \n");  
  FQAM_init ();  
  
  FQAM_Pauli_x ();
  FQAM_Pauli_z ();
  FQAM_Pauli_eye ();
  
  _debug_FQAM_show_stage ();  
  FQAM_finalize ();  
  printf ("Passed Test 3 \n \n");
}