/*
    Copyright (C) 2024, Chuck Garcia

    This file is part of libfqam and is available under the 3-Clause
    BSD license, which can be found in the LICENSE file at the top-level
    directory, or at http://opensource.org/licenses/BSD-3-Clause
*/

#include "FQAM_Pauli_primitives.h"

static FQAM_Op x_op;
static FQAM_Op z_op;
static FQAM_Op y_op;
static FQAM_Op eye_op;

/* Initializes static allocated pauli operators */
void pauli_ops_init_ (void);  // Calls operator_generator_gen() for each
void pauli_gen_x (int state_space); // Call operator create, 
void pauli_gen_z (int state_space);