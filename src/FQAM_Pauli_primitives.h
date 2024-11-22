#include "FQAM_Operator.h"

static FQAM_Op x_op;
static FQAM_Op z_op;
static FQAM_Op y_op;
static FQAM_Op eye_op;

/* Initializes static allocated pauli operators */
void pauli_ops_init_ (void);  
void pauli_gen_x (int state_space); 
void pauli_gen_z (int state_space);