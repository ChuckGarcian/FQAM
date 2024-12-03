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

static int test_dim = 1;

void test_1 (void);
void test_2 (void);
void test_3 (void);
void test_4 (void);
void test_5 (void);




#define FQAM_PI4  ((dcomplex){1/sqrt(2), 0})
#define FQAM_7PI4 ((dcomplex){-1 * ( 1/sqrt(2)), 0})
#define FQAM_ONE ((dcomplex){1, 0})

void example_hadamard (void)
      {
        FQAM_init (test_dim, 0);
        FQAM_Basis ket0, ket1;
        FQAM_Op outer0, outer1, res;
                 
        FQAM_Op_create (&res, "Not\0", test_dim);
        
        ket0 = FQAM_Basis_create (1, 0, 0);
        ket1 = FQAM_Basis_create (1, 0, 1);
        
        // |0><0| + |1><0|        
        FQAM_Basis_outer (ket0, ket0, &outer0); // |0><1|
        FQAM_Basis_outer (ket1, ket0, &outer1); // |1><0|
        
        FQAM_Op_add (FQAM_PI4, outer0, &res);
        FQAM_Op_add (FQAM_7PI4, outer1, &res);
      
        _debug_FQAM_show_stage ();  
        FQAM_stage_append (&res);
      
        _debug_FQAM_show_stage ();  
        FQAM_Render_feynman_diagram ();
        printf ("Done \n");
}

void example_eye (void)
      {
        FQAM_init (test_dim, 0);
        FQAM_Basis ket0, ket1;
        FQAM_Op outer0, outer1, res;
                 
        FQAM_Op_create (&res, "Eye\0", test_dim);
        
        ket0 = FQAM_Basis_create (1, 0, 0);
        ket1 = FQAM_Basis_create (1, 0, 1);
        
        // |0><0| + |1><1|        
        FQAM_Basis_outer (ket0, ket0, &outer0); // |1><1|
        FQAM_Basis_outer (ket1, ket1, &outer1); // |1><1|
        
        FQAM_Op_add (FQAM_ONE, outer0, &res);
        FQAM_Op_add (FQAM_ONE, outer1, &res);
              
        _debug_FQAM_show_stage ();  
        FQAM_stage_append (&res);
      
        _debug_FQAM_show_stage ();  
        FQAM_Render_feynman_diagram ();
        printf ("Done \n");
}


void example_not (void)
      {
        FQAM_init (test_dim, 0);
        FQAM_Basis ket0, ket1;
        FQAM_Op outer0, outer1, res;
                 
        FQAM_Op_create (&res, "Not\0", test_dim);
        
        ket0 = FQAM_Basis_create (1, 0, 0);
        ket1 = FQAM_Basis_create (1, 0, 1);
        
        // |0><1| + |1><0|        
        FQAM_Basis_outer (ket0, ket1, &outer0); // |0><1|
        FQAM_Basis_outer (ket1, ket0, &outer1); // |1><0|
        
        FQAM_Op_add (FQAM_ONE, outer0, &res);
        FQAM_Op_add (FQAM_ONE, outer1, &res);
              
        _debug_FQAM_show_stage ();  
        FQAM_stage_append (&res);
        FQAM_stage_append (&res);
        FQAM_stage_append (&res);
      
        _debug_FQAM_show_stage ();  
        FQAM_Render_feynman_diagram ();
        printf ("Done \n");
}

void test_render_1 (void);

int main (void)
{  
  // test_1();
  // test_2();
  // test_3();  
  // test_4();  
  // test_5 ();
  
  // test_render_1 ();
  example_hadamard ();
  // example_not ();
}

void test_1 (void)
{
  // Test: basic test ensuring FQAM can initialize and finalize
  printf ("Running Test 1 \n");
  FQAM_init (4, 3);
  FQAM_show_statevector (); // Should output 0010
  FQAM_finalize ();
  printf ("Passed Test 1\n\n");
}

void test_2 (void)
{
  // Test: Checking basic stage appending and finalization
  printf ("Running Test 2 \n");
  FQAM_init (test_dim, 0);
  FQAM_Pauli_x ();
  FQAM_finalize ();
  printf ("Passed Test 2 \n \n");
}

void test_3 (void)
{
  printf ("Running Test 3 \n");  
  FQAM_init (test_dim, 0);  
  
  FQAM_Pauli_x ();
  FQAM_Pauli_z ();
  FQAM_Pauli_eye ();
  
  _debug_FQAM_show_stage ();  
  FQAM_finalize ();  
  printf ("Passed Test 3 \n \n");
}

void test_4 (void)
{
  printf ("Running Test 4 \n");  
  FQAM_init (test_dim, 0);  
  FQAM_show_statevector (); 

  FQAM_Pauli_x ();
  FQAM_Pauli_z ();
  FQAM_Pauli_eye ();
  
  _debug_FQAM_show_stage ();  
  FQAM_compute_outcomes ();
  FQAM_show_statevector (); 
  FQAM_finalize ();  
  printf ("Passed Test 4 \n \n");
}

void test_5 (void)
{
  printf ("Running Test 5 \n");  
  FQAM_init (test_dim, 0);  
  FQAM_show_statevector (); 
  FQAM_compute_outcomes (); // Expected to not crash
  FQAM_finalize ();  
  printf ("Passed Test 5 \n \n");
}

void test_render_1 (void)
{
  printf ("Running Render Test 1 \n");  
  FQAM_init (test_dim, 0);  
  FQAM_show_statevector (); 

  FQAM_Pauli_eye ();
  // FQAM_Pauli_x ();
  // FQAM_Pauli_x ();
  // FQAM_Pauli_eye ();
  // FQAM_Pauli_x ();
  // _debug_FQAM_show_stage ();  
  FQAM_Render_feynman_diagram ();
  printf ("Passed Render Test 1 \n");  
}