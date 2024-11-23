#ifndef __FQAM_H
#define __FQAM_H

#include "FQAM_Operator.h"

FQAM_Error FQAM_init (void);
FQAM_Error FQAM_finalize (void);

void FQAM_stage_show (void);
FQAM_Error FQAM_stage_append (FQAM_Op operator); // Adds operator to staging list

#endif 