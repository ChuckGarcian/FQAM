/*
    Copyright (C) 2024, Chuck Garcia

    This file is part of libfqam and is available under the 3-Clause
    BSD license, which can be found in the LICENSE file at the top-level
    directory, or at http://opensource.org/licenses/BSD-3-Clause

*/

#include "FQAM_Operator.h"
#include "stdbool.h"

// Returns 
FQAM_Error FQAM_stage_init (void);

// Adds operator to staging list
FQAM_Error FQAM_stage_append (FQAM_Op operator); 


// Adds operator to staging list
bool FQAM_stage_append (FQAM_Op operator); 

// Free stage and operator resources. 
FQAM_Error FQAM_stage_finalize (void); 