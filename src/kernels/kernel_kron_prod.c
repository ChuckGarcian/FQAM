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
 *  Computes the Kronecker product (tensor product) of matrices A and B, and stores
 * the result in C. Matrix C must be pre-initialized to be conformal to the
 * dimensions of A ⊗ B. Specifically, if A is an mxn matrix and B is a pxq matrix,
 * then C must be an (mp)x(nq) matrix.
 *
 * Proof for this function is in docs 'tensor_proof.pdf'
 *
 * Arguments:
 *    FLA_Obj A: Matrix representation of the first operand.
 *    FLA_Obj B: Matrix representation of the second operand.
 *    FLA_Obj C: Matrix representation of the resulting Kronecker product.
 */
int kernel_kron_prod (FLA_Obj A, FLA_Obj B, FLA_Obj C)
{
  // Check preconditions
  int m, n, p, q;
  m = FLA_Obj_width (A);
  n = FLA_Obj_length (A);
  p = FLA_Obj_width (B);
  q = FLA_Obj_length (B);

  assertf (m * p == FLA_Obj_length (C), "Error: conformality issue");
  assertf (n * q == FLA_Obj_width (C), "Error: conformality issue");

  FLA_Set (FLA_ZERO, C);

  FLA_Obj ATL, ATR, A00, A01, A02, ABL, ABR, A10, A11, A12, A20, A21, A22;
  FLA_Obj CTL, CTR, C00, C01, C02, CBL, CBR, C10, C11, C12, C20, C21, C22;

  FLA_Part_2x2 (A, &ATL, &ATR, &ABL, &ABR, 0, 0, FLA_TL);
  FLA_Part_2x2 (C, &CTL, &CTR, &CBL, &CBR, 0, 0, FLA_TL);

  while (FLA_Obj_length (ATL) < FLA_Obj_length (A))
  {

    FLA_Repart_2x2_to_3x3 (ATL, ATR, &A00, &A01, &A02, &A10, &A11, &A12, ABL, ABR,
                           &A20, &A21, &A22, 1, 1, FLA_BR);

    FLA_Repart_2x2_to_3x3 (CTL, CTR, &C00, &C01, &C02, &C10, &C11, &C12, CBL, CBR,
                           &C20, &C21, &C22, 1, 1, FLA_BR);

    /* C01 = A01 */
    FLA_Gemm (FLA_NO_TRANSPOSE, FLA_NO_TRANSPOSE, FLA_ZERO, A01, B, FLA_ONE, C01);

    /* C11 = A11B */
    FLA_Gemm (FLA_NO_TRANSPOSE, FLA_NO_TRANSPOSE, FLA_ZERO, A11, B, FLA_ONE, C11);

    /* C10 = A10B */
    FLA_Gemm (FLA_NO_TRANSPOSE, FLA_NO_TRANSPOSE, FLA_ZERO, A10, B, FLA_ONE, C10);

    FLA_Cont_with_3x3_to_2x2 (&ATL, &ATR, A00, A01, A02, A10, A11, A12, &ABL, &ABR,
                              A20, A21, A22, FLA_TL);
    FLA_Cont_with_3x3_to_2x2 (&CTL, &CTR, C00, C01, C02, C10, C11, C12,

                              &CBL, &CBR, C20, C21, C22, FLA_TL);
  }

  return FLA_SUCCESS;
}