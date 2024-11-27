/*
    Copyright (C) 2024, Chuck Garcia

    This file is part of libfqam and is available under the 3-Clause
    BSD license, which can be found in the LICENSE file at the top-level
    directory, or at http://opensource.org/licenses/BSD-3-Clause
*/

#include "FQAM.h"
#include <string.h>

#define TMP_DATA_TYPE FLA_DOUBLE_COMPLEX

#define incr_addr(i, j) ((i * rs) + (j * cs)) // Address incremental
#define access(i, j) (buf + (i * rs) + (j * cs)) // Address incremental

int test_dim = 2;
FQAM_Op x_op;
FQAM_Op z_op;
FQAM_Op y_op;
FQAM_Op eye_op;

// Call operator create,
static void generate_x (void);
static void generate_z (void);
static void generate_y (void);
static void generate_eye (void);

/* Initializes static allocated pauli operators */
void pauli_ops_init_ (void)
{
  generate_x ();
  generate_z ();
  generate_eye ();
  generate_y (); 
}

FQAM_Op *FQAM_Pauli_x (void) {FQAM_stage_append (&x_op);} 
FQAM_Op *FQAM_Pauli_y (void) {FQAM_stage_append (&y_op);} 
FQAM_Op *FQAM_Pauli_z (void) {FQAM_stage_append (&z_op);}
FQAM_Op *FQAM_Pauli_eye (void) {FQAM_stage_append (&eye_op);} 

/*
  TODO: The way generated the pauli matrices bellow is just temporary. In the future I
  need to make them call into operators module
*/
static void generate_x (void)
{
  int rs, cs;
  char name[] = "not \0";
  
  // Initialize operator and set FLA matrix
  FQAM_Operator_init (&x_op, name);  
  FLA_Obj_create (TMP_DATA_TYPE, test_dim, test_dim, 0, 0, &x_op.mat_repr);

  // Get buffer address
  dcomplex *buf = FLA_Obj_buffer_at_view (x_op.mat_repr);

  // Access's array elements and set to proper values
  rs = FLA_Obj_row_stride (x_op.mat_repr);
  cs = FLA_Obj_col_stride (x_op.mat_repr);

  access (0, 0)->real = 0.0;
  access (1, 1)->real = 0.0;
  access (0, 1)->real = 1.0;
  access (1, 0)->real = 1.0;
  
  access (0, 0)->imag = 0.0;
  access (1, 1)->imag = 0.0;
  access (0, 1)->imag = 0.0;
  access (1, 0)->imag = 0.0;
}

static void generate_z (void)
{
  int rs, cs;

  char name[] = "Invert \0";
  
  FQAM_Operator_init (&z_op, name);  
  FLA_Obj_create (TMP_DATA_TYPE, test_dim, test_dim, 0, 0, &z_op.mat_repr);

  // Get buffer address
  dcomplex *buf = FLA_Obj_buffer_at_view (z_op.mat_repr);

  // Access's array elements and set to proper values
  rs = FLA_Obj_row_stride (z_op.mat_repr);
  cs = FLA_Obj_col_stride (z_op.mat_repr);

  access (0, 0)->real = 0.0;
  access (1, 1)->real = -1.0;
  access (0, 1)->real = 0.0;
  access (1, 0)->real = 0.0;
  
  access (0, 0)->imag = 0.0;
  access (1, 1)->imag = 0.0;
  access (0, 1)->imag = 0.0;
  access (1, 0)->imag = 0.0;
}

static void generate_eye (void)
{
  char name[] = "Identity \0";
  FQAM_Operator_init (&eye_op, name);  
  FLA_Obj_create (TMP_DATA_TYPE, test_dim, test_dim, 0, 0, &eye_op.mat_repr);
  FLA_Set_to_identity (eye_op.mat_repr);
}

static void generate_y (void) 
{ 
  int rs, cs;
  char name[] = "Y gate \0";
  
  // Initialize operator and set FLA matrix
  FQAM_Operator_init (&y_op, name);  
  FLA_Obj_create (TMP_DATA_TYPE, test_dim, test_dim, 0, 0, &y_op.mat_repr);

  // Get buffer address
  dcomplex *buf = FLA_Obj_buffer_at_view (y_op.mat_repr);

  // Access's array elements and set to proper values
  rs = FLA_Obj_row_stride (y_op.mat_repr);
  cs = FLA_Obj_col_stride (y_op.mat_repr);

  access (0, 0)->real = 0.0;
  access (1, 1)->real = 0.0;
  access (0, 1)->real = 1.0;
  access (1, 0)->real = 1.0;
  
  access (0, 0)->imag = 0.0;
  access (1, 1)->imag = 1.0;
  access (0, 1)->imag = 1.0;
  access (1, 0)->imag = 0.0;
}

