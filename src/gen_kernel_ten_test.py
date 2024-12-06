"""
    Copyright (C) 2024, Chuck Garcia

    This file is part of libfqam and is available under the 3-Clause
    BSD license, which can be found in the LICENSE file at the top-level
    directory, or at http://opensource.org/licenses/BSD-3-Clause

Description: This file automatically generates kronecker product kernel C tests 
for random matrices using numpy as a reference.
"""

import numpy as np

def generate_matrix_code(matrix_a, matrix_b):
    # Get dimensions
    m = len(matrix_a)
    n = len(matrix_a[0])
    p = len(matrix_b)
    q = len(matrix_b[0])
    r = m * p
    c = n * q

    # Generate matrix initialization strings
    def matrix_to_string(array):
        array_str = np.array2string(
            array, separator=", ", formatter={"float_kind": lambda x: str(x)}, threshold=np.inf
        )
        formatted_str = array_str.replace("[", "{").replace("]", "}")
        return formatted_str

    # Calculate reference matrix dimensions and values
    c_ref = np.kron(matrix_a, matrix_b)
    # Note: In a real implementation, you would compute the Kronecker product here
    # This is a simplified version matching the example

    code = f"""
#include "FLAME.h"
#include "FQAM.h"
#include "__kernels.h"
#include "assertf.h"
  void test_tensor_1 (void)    
{{ 
FLA_Init();
  
// Turn on parameter checking
FLA_Check_error_level_set(FLA_FULL_ERROR_CHECKING);
FLA_Obj A, B, C_ref, C, ZERO;
int m, n, p, q, r, c;

// Square matrices dimension
m = {m};
n = {n};
p = {p};
q = {q};
r = {m*p};
c = {n * q};

// Create FLA objects
FLA_Obj_create_without_buffer(FLA_DOUBLE, m, n, &A);
FLA_Obj_create_without_buffer(FLA_DOUBLE, p, q, &B);
FLA_Obj_create_without_buffer(FLA_DOUBLE, r, c, &C_ref);
FLA_Obj_create(FLA_DOUBLE, r, c, 1, c, &C);
FLA_Obj_create_constant (0.0, &ZERO);
FLA_Set(ZERO, C); // Initialize C to all zeros

// Create buffer for operands and reference
double A_arr[{m}][{n}] = {matrix_to_string(matrix_a)};
double B_arr[{p}][{q}] = {matrix_to_string(matrix_b)};
double C_arr_ref[{r}][{c}] = {matrix_to_string(c_ref)};

FLA_Obj_attach_buffer(A_arr, 1, m, &A);
FLA_Obj_attach_buffer(B_arr, 1, p, &B);
FLA_Obj_attach_buffer(C_arr_ref, 1, r, &C_ref);

// Execute kernel being tested
kernel_kron_prod_rec(A, B, C, 2);

// Check success
bool success = FLA_Obj_equals(C, C_ref);
assertf(success, "Failed test");
printf ("Passed test! \\n");
}}
"""
    return code


# Example usage
if __name__ == "__main__":
    # Define input matrices
    # matrix_a = np.array([[1, 0], [0, 1]])
    # matrix_b = np.array ([[0, 1], [1, 0]])

    # A = np.eye (1)
    # B = np.eye ()
    # Anp.array (1, 0)
    # np.array (0, 1)
    
    A = np.random.rand(16, 16)
    B = np.random.rand(16, 16)

    # Generate and print the C code
    generated_code = generate_matrix_code(A, B)
    
    # print(generated_code)
    with open("src/generated_code.c", "w") as file:
      file.write(generated_code)

