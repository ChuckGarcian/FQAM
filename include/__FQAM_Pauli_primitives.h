#include "FQAM.h"

/* Initializes static allocated pauli operators */
void pauli_ops_init_ (void);  

FQAM_Op * pauli_gen_x (void);
FQAM_Op * pauli_gen_z (void);
FQAM_Op * pauli_gen_y (void);
FQAM_Op * pauli_gen_eye (void);
