
#define FQAM_SUCCESS           (-1)
#define FQAM_FAILURE           (-2)

typedef int FQAM_Error;

typedef struct 
{
  char name [10];         // Operator Name    
  FLA_Obj mat_repr;       // Operator Matrix Representation
  int initialized; 
} FQAM_Op;