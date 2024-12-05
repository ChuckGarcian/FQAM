/*
    Copyright (C) 2024, Chuck Garcia

    This file is part of libfqam and is available under the 3-Clause
    BSD license, which can be found in the LICENSE file at the top-level
    directory, or at http://opensource.org/licenses/BSD-3-Clause
*/

#include "FQAM.h"
#include <string.h>

static int pauli_dim = 1;

/* Stores identity into operator A*/
void FQAM_Pauli_eye (FQAM_Op *A)
{
  FQAM_Basis ket0, ket1;
  FQAM_Op outer0, outer1;

  FQAM_Op_create (A, "Eye\0", pauli_dim);

  ket0 = FQAM_Basis_create (1, 0, 0);
  ket1 = FQAM_Basis_create (1, 0, 1);

  // |0><0| + |1><1|
  FQAM_Basis_outer (ket0, ket0, &outer0); // |1><1|
  FQAM_Basis_outer (ket1, ket1, &outer1); // |1><1|

  FQAM_Op_add (FQAM_ONE, outer0, A);
  FQAM_Op_add (FQAM_ONE, outer1, A);
}

/* Parameterized Phase */
void FQAM_PhaseA (double angle, FQAM_Op *A)
{
  FQAM_Basis ket0, ket1;
  FQAM_Op outer0, outer1;

  FQAM_Op_create (A, "Phase\0", pauli_dim);

  ket0 = FQAM_Basis_create (1, 0, 0);
  ket1 = FQAM_Basis_create (1, 0, 1);

  // |0><0| + |1><1|
  FQAM_Basis_outer (ket0, ket0, &outer0); // |1><1|
  FQAM_Basis_outer (ket1, ket1, &outer1); // |1><1|

  FQAM_Op_add (FQAM_CMPX (1, 0), outer0, A);
  FQAM_Op_add (FQAM_CMPXA (angle), outer1, A);
}

/* Stores Pauli X into operator A*/
void FQAM_Pauli_x (FQAM_Op *A)
{
  FQAM_Basis ket0, ket1;
  FQAM_Op outer0, outer1;

  FQAM_Op_create (A, "Not\0", pauli_dim);

  ket0 = FQAM_Basis_create (1, 0, 0);
  ket1 = FQAM_Basis_create (1, 0, 1);

  // |0><1| + |1><0|
  FQAM_Basis_outer (ket0, ket1, &outer0); // |0><1|
  FQAM_Basis_outer (ket1, ket0, &outer1); // |1><0|

  FQAM_Op_add (FQAM_ONE, outer0, A);
  FQAM_Op_add (FQAM_ONE, outer1, A);
}

#define FQAM_COMPLEX(real, imag) ((dcomplex){real, imag})

void FQAM_Pauli_y (FQAM_Op *A)
{
  FQAM_Basis ket0, ket1;
  FQAM_Op outer0, outer1;

  FQAM_Op_create (A, "Y\0", pauli_dim);

  ket0 = FQAM_Basis_create (1, 0, 0);
  ket1 = FQAM_Basis_create (1, 0, 1);

  // |0><1| + |1><0|
  FQAM_Basis_outer (ket0, ket1, &outer0); // |0><1|
  FQAM_Basis_outer (ket1, ket0, &outer1); // |1><0|

  FQAM_Op_add (FQAM_COMPLEX (0, 1), outer0, A);
  FQAM_Op_add (FQAM_COMPLEX (0, -1), outer1, A);
}

void FQAM_Pauli_z (FQAM_Op *A)
{
  FQAM_Basis ket0, ket1;
  FQAM_Op outer0, outer1;

  FQAM_Op_create (A, "Z\0", pauli_dim);

  ket0 = FQAM_Basis_create (1, 0, 0);
  ket1 = FQAM_Basis_create (1, 0, 1);

  // |0><0| + |1><1|
  FQAM_Basis_outer (ket0, ket0, &outer0); // |1><1|
  FQAM_Basis_outer (ket1, ket1, &outer1); // |1><1|

  FQAM_Op_add (FQAM_ONE, outer0, A);
  FQAM_Op_add (FQAM_CMPX (-1, 0), outer1, A);
}

void FQAM_hadamard (FQAM_Op *A)
{
  FQAM_Basis ket0, ket1;
  FQAM_Op outer0, outer1, outer2, outer3;

  FQAM_Op_create (A, "Hadamard\0", pauli_dim);

  ket0 = FQAM_Basis_create (1, 0, 0);
  ket1 = FQAM_Basis_create (1, 0, 1);

  // |0><0| + |1><0| + |0><1| + |1><1|
  FQAM_Basis_outer (ket0, ket0, &outer0); // |0><0|
  FQAM_Basis_outer (ket1, ket0, &outer1); // |1><0|
  FQAM_Basis_outer (ket0, ket1, &outer2); // |0><1|
  FQAM_Basis_outer (ket1, ket1, &outer3); // |1><1|

  // Alpha set as hadamard coefficients
  FQAM_Op_add (FQAM_PI4, outer0, A);
  FQAM_Op_add (FQAM_PI4, outer1, A);
  FQAM_Op_add (FQAM_PI4, outer2, A);
  FQAM_Op_add (FQAM_7PI4, outer3, A);
}