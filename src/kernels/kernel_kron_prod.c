/***
 *     Copyright (C) 2024, Chuck Garcia
 *
 *     This file is part of libfqam and is available under the 3-Clause
 *     BSD license, which can be found in the LICENSE file at the top-level
 *     directory, or at http://opensource.org/licenses/BSD-3-Clause
 */

#include "FLAME.h"
#include "FQAM.h"
#include "__kernels.h"
#include "assertf.h"

static int kernel_kron_prod_rec_base (FLA_Obj A, FLA_Obj B, FLA_Obj C);
static void check_dimensions (FLA_Obj A, FLA_Obj B, FLA_Obj C);

/***
 *  Computes the Kronecker product (tensor product) of matrices A and B, and stores
 * the result in C. Matrix C must be pre-initialized to be conformal to the
 * dimensions of A ⊗ B. Specifically, if A is an mxn matrix and B is a pxq matrix,
 * then C must be an (mp)x(nq) matrix.
 *
 * Notes: In the case function is called for vectors, it must be the case they
 * are row vectors (mx1).
 * Proof for this function is in docs 'tensor_proof.pdf'
 *
 * Arguments:
 *    FLA_Obj A: Matrix representation of the first operand.
 *    FLA_Obj B: Matrix representation of the second operand.
 *    FLA_Obj C: Matrix representation of the resulting Kronecker product.
 */
int kernel_kron_prod_rec (FLA_Obj A, FLA_Obj B, FLA_Obj C, int nb_alg)
{
  // Base case: A is a scalar
  if (FLA_Obj_length (A) <= 1)
  {
    kernel_kron_prod_rec_base (A, B, C);
    return 0;
  }

  // Assert conformality
  check_dimensions (A, B, C);

  FLA_Obj ATL, ATR, A00, A01, A02, ABL, ABR, A10, A11, A12, A20, A21, A22;
  FLA_Obj CTL, CTR, C00, C01, C02, CBL, CBR, C10, C11, C12, C20, C21, C22;

  FLA_Part_2x2 (A, &ATL, &ATR, &ABL, &ABR, 0, 0, FLA_TL);
  FLA_Part_2x2 (C, &CTL, &CTR, &CBL, &CBR, 0, 0, FLA_TL);

  // Recursively compute blocked kronecker products
  while (FLA_Obj_length (CTL) < FLA_Obj_length (C))
  {
    // Partition into subblocks
    int mb_A, nb_A, mb_C, nb_C;
    mb_A = min (FLA_Obj_length (ABR), FLA_Obj_length (A) / nb_alg);
    nb_A = min (FLA_Obj_width  (ABR), FLA_Obj_length (A) / nb_alg);
    mb_C = min (FLA_Obj_length (CBR), FLA_Obj_length (C) / nb_alg);
    nb_C = min (FLA_Obj_width  (CBR), FLA_Obj_length (C) / nb_alg);

    FLA_Repart_2x2_to_3x3 (ATL, ATR, &A00, &A01, &A02, &A10, &A11, &A12, ABL, ABR,
                           &A20, &A21, &A22, mb_A, nb_A, FLA_BR);
    FLA_Repart_2x2_to_3x3 (CTL, CTR, &C00, &C01, &C02, &C10, &C11, &C12, CBL, CBR,
                           &C20, &C21, &C22, mb_C, nb_C, FLA_BR);

    // Compute kronecker products of each sublock
    kernel_kron_prod_rec (A11, B, C11, nb_alg);
    kernel_kron_prod_rec (A01, B, C01, nb_alg);
    kernel_kron_prod_rec (A10, B, C10, nb_alg);

    FLA_Cont_with_3x3_to_2x2 (&ATL, &ATR, A00, A01, A02, A10, A11, A12, &ABL, &ABR,
                              A20, A21, A22, FLA_TL);
    FLA_Cont_with_3x3_to_2x2 (&CTL, &CTR, C00, C01, C02, C10, C11, C12, &CBL, &CBR,
                              C20, C21, C22, FLA_TL);
  }
  return FLA_SUCCESS;
}

/* Base case. TODO: Potentially add SIMD vectorization */
int kernel_kron_prod_rec_base (FLA_Obj alpha, FLA_Obj B, FLA_Obj C)
{
  /* Partion was along invalid axis*/
  if (FLA_Obj_length (alpha) == 0 || FLA_Obj_width (alpha) == 0)
    return 0;

  FLA_Axpy (alpha, B, C);
  return 0;
}

/* Ensures C is allocated the correct size */
static void check_dimensions (FLA_Obj A, FLA_Obj B, FLA_Obj C)
{
  int m, n, p, q, r, c;
  m = FLA_Obj_length (A);
  n = FLA_Obj_width (A);
  p = FLA_Obj_length (B);
  q = FLA_Obj_width (B);
  r = m * p;
  c = n * q;

  assertf (r == FLA_Obj_length (C),
           "Error: conformality issue, \n\tr: %d, FLA_Obj_length(C): %d\n\tc: %d, "
           "FLA_Obj_width(C): %d\n",
           r, FLA_Obj_length (C), c, FLA_Obj_width (C));
  assertf (c == FLA_Obj_width (C),
           "Error: conformality issue, \n\tr: %d, FLA_Obj_length(C): %d, "
           "FLA_Obj_width(C): %d\n\tc: %d\n",
           r, FLA_Obj_length (C), FLA_Obj_width (C), c);
}