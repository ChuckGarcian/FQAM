#include "FLAME.h"
#include "list.h"

#define FQAM_SUCCESS           (-1)
#define FQAM_FAILURE           (-2)

typedef int FQAM_Error;

typedef struct 
{
  struct list_elem elem;  // Used in stage
  char *name;             // Operator Name    
  FLA_Obj mat_repr;       // Operator Matrix Representation
} FQAM_Op;

FQAM_Error FQAM_operator_init (void);
FQAM_Error FQAM_operator_create (FQAM_Op operator, char *name);
FQAM_Error FQAM_operator_finalize (void);
FQAM_Error FQAM_operator_generate_functions (FQAM_Op operator, char *name);