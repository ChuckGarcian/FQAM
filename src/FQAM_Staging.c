/*
    Copyright (C) 2024, Chuck Garcia

    This file is part of libfqam and is available under the 3-Clause
    BSD license, which can be found in the LICENSE file at the top-level
    directory, or at http://opensource.org/licenses/BSD-3-Clause
*/

#include "FQAM_Operator.h"
#include "list.h"

// Stage data structure; Opaque (User should not access explicitly)
static struct list stage; 

// Adds operator to staging list
FQAM_Error FQAM_operator_init (void); 

// Initialize an operator 
FQAM_Error FQAM_operator_create (FQAM_Op operator, char *name);

// Free stage and operator resources. 
FQAM_Error FQAM_operator_finalize (void);

// Placeholder for a set of different functions to assist in creating operator matrix representations
// functions could include: tensor_product, generate_projector, measure_op, ect
FQAM_Error FQAM_operator_generate_functions (FQAM_Op operator, char *name);