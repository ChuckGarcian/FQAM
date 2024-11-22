#include "FQAM_Operator.h"

FQAM_Error FQAM_stage_init (void);
FQAM_Error FQAM_stage_append (FQAM_Op operator); // Adds operator to staging list
FQAM_Error FQAM_stage_finalize (void); // Free stage and operator resources. 