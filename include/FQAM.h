#ifndef __FQAM_H
#define __FQAM_H

#include "FLAME.h"
#include "list.h"

#define FQAM_SUCCESS           (-1)
#define FQAM_FAILURE           (-2)

typedef int FQAM_Error;

typedef struct 
{
  struct list_elem elem;  // Used in stage
  char name [10];         // Operator Name    
  FLA_Obj mat_repr;       // Operator Matrix Representation
} FQAM_Op;

#include "__FQAM_Operator.h"

FQAM_Error FQAM_init (void);
FQAM_Error FQAM_finalize (void);

void FQAM_stage_show (void);
FQAM_Error FQAM_stage_append (FQAM_Op operator); // Adds operator to staging list

bool FQAM_initialized (void);

#endif 