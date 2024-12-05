#include "FQAM.h"

void FQAM_Pauli_x (FQAM_Op *A);
void FQAM_Pauli_y (FQAM_Op *A);
void FQAM_Pauli_z (FQAM_Op *A);
void FQAM_Pauli_eye (FQAM_Op *A);
void FQAM_hadamard (FQAM_Op *A);

/* Phase Gates*/
void FQAM_PhaseA (double angle, FQAM_Op *A);
void inline FQAM_Phase (FQAM_Op *A) { FQAM_PhaseA (M_PI / 4, A); }
void inline FQAM_Phase_T (FQAM_Op *A) { FQAM_PhaseA (M_PI / 2, A); }
