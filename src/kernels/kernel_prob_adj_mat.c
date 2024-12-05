/***
 *     Copyright (C) 2024, Chuck Garcia
 *
 *     This file is part of libfqam and is available under the 3-Clause
 *     BSD license, which can be found in the LICENSE file at the top-level
 *     directory, or at http://opensource.org/licenses/BSD-3-Clause
 */

#include "__kernels.h"
#include "assertf.h"

/***
 *  Computes the probability adjacency matrix using matrices A and B, storing the
 * result into matrix C. Arguments: FLA_Obj A : Operator Matrix FLA_Obj bt: Current
 * state vector as a row vector FLA_Obj C : Path probability adjacency matrix. Matrix
 * specifying the probability amplitudes of edges connecting the i-th and (i + 1)-th
 * state. Width is input state. Height are output states. For example: 'a' at 11 is
 * the probability state 01 transitions to 10 00  01 00 b   b 10 b   a
 */
int compute_probability_adjacency_matrix (FLA_Obj A, FLA_Obj state, FLA_Obj C)
{
  FLA_Obj AL, AR, A0, a1, A2;
  FLA_Obj B, BT, B0, BB, beta, B2;

  // Create copy to avoid modifying original state vector
  FLA_Obj_create_conf_to (FLA_NO_TRANSPOSE, state, &B);
  FLA_Copy (state, B);

  FLA_Obj CT, C0, CB, c1t, C2;

  FLA_Part_1x2 (A, &AL, &AR, 0, FLA_LEFT);
  FLA_Part_2x1 (B, &BT, &BB, 0, FLA_TOP);
  FLA_Part_2x1 (C, &CT, &CB, 0, FLA_TOP);

  while (FLA_Obj_width (AL) < FLA_Obj_width (A))
  {

    FLA_Repart_1x2_to_1x3 (AL, /**/ AR, &A0, /**/ &a1, &A2, 1, FLA_RIGHT);
    FLA_Repart_2x1_to_3x1 (BT, &B0, &beta, BB, &B2, 1, FLA_BOTTOM);
    FLA_Repart_2x1_to_3x1 (CT, &C0,
                           /* ** */ /* *** */
                           &c1t, CB, &C2, 1, FLA_BOTTOM);

    FLA_Axpyt (FLA_TRANSPOSE, beta, a1, c1t);

    FLA_Cont_with_1x3_to_1x2 (&AL, /**/ &AR, A0, a1, /**/ A2, FLA_LEFT);
    FLA_Cont_with_3x1_to_2x1 (&BT, B0, beta, &BB, B2, FLA_TOP);
    FLA_Cont_with_3x1_to_2x1 (&CT, C0, c1t,
                              /* ** */ /* *** */
                              &CB, C2, FLA_TOP);
  }

  FLA_Obj_free (&B);
  return FLA_SUCCESS;
}