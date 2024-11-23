#include "FQAM.h"

static FQAM_Op x_op;
static FQAM_Op z_op;
static FQAM_Op y_op;
static FQAM_Op eye_op;

/* Initializes static allocated pauli operators */
void pauli_ops_init_ (void);  
void pauli_gen_x (void);
void pauli_gen_z (void);
void pauli_gen_eye (void);
