/*
    Copyright (C) 2024, Chuck Garcia

    This file is part of libfqam and is available under the 3-Clause
    BSD license, which can be found in the LICENSE file at the top-level
    directory, or at http://opensource.org/licenses/BSD-3-Clause
*/

#include "FQAM_Pauli_primitives.h"
#include <string.h>

#define TMP_DATA_TYPE FLA_INT

#define incr_addr(i, j) ((i * rs) + (j * cs)) // Address incremental

static int dim = 2;
static FQAM_Op x_op;
static FQAM_Op z_op;
static FQAM_Op y_op;
static FQAM_Op eye_op;

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

// Call operator create,
void pauli_gen_x (void);
void pauli_gen_z (void);
void pauli_gen_y (void);
void pauli_gen_eye (void);
/*
  TODO: The way generated the pauli matrices bellow is just temparary. In the future I
  need to make them call into operators module
*/
static void generate_x (void)
{
  int rs, cs;

  char name[] = "Not";
  strcpy (eye_op.name, name);

  // Create matrix
  FLA_Obj_create (TMP_DATA_TYPE, dim, dim, 0, 0, &x_op.mat_repr);

  // Get buffer address
  int *buf = FLA_Obj_buffer_at_view (x_op.mat_repr);

  // Access's array elements and set to proper values
  rs = FLA_Obj_row_stride (x_op.mat_repr);
  cs = FLA_Obj_col_stride (x_op.mat_repr);

  *(buf + incr_addr (0, 0)) = 0; // Set X[0,0] = 0
  *(buf + incr_addr (1, 1)) = 0; // Set X[1,1] = 0
  *(buf + incr_addr (0, 1)) = 1; // Set X[0,1] = 1
  *(buf + incr_addr (1, 0)) = 1; // Set X[1,0] = 1

  // Show Operator information
  FQAM_Operator_show (x_op);
}

static void generate_z (void)
{
  int rs, cs;

  char name[] = "Invert";
  strcpy (eye_op.name, name);

  // Create matrix
  FLA_Obj_create (TMP_DATA_TYPE, dim, dim, 0, 0, &z_op.mat_repr);

  // Get buffer address
  int *buf = FLA_Obj_buffer_at_view (z_op.mat_repr);

  // Access's array elements and set to proper values
  rs = FLA_Obj_row_stride (z_op.mat_repr);
  cs = FLA_Obj_col_stride (z_op.mat_repr);

  *(buf + incr_addr (0, 0)) = 1; // Set X[0,0] = 0
  *(buf + incr_addr (1, 1)) = -1; // Set X[1,1] = 0
  *(buf + incr_addr (0, 1)) = 0; // Set X[0,1] = 1
  *(buf + incr_addr (1, 0)) = 0; // Set X[1,0] = 1

  // Show operator data
  FQAM_Operator_show (z_op);
};

static void generate_eye (void)
{
  char name[] = "Identity";
  strcpy (eye_op.name, name);
  FLA_Obj_create (TMP_DATA_TYPE, dim, dim, 0, 0, &eye_op.mat_repr);
  FLA_Set_to_identity (eye_op.mat_repr);
}

static void generate_y (void) { FLA_Obj_create (TMP_DATA_TYPE, dim, dim, 0, 0, &y_op); }