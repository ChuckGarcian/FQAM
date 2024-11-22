
typedef int FQAM_Error;
typedef struct {
  char *name;
  // FLA_obj  
} FQAM_Op;

FQAM_Error FQAM_operator_init (void);
FQAM_Error FQAM_operator_create (FQAM_Op operator, char *name);
FQAM_Error FQAM_operator_finalize (void);
FQAM_Error FQAM_operator_generate_functions (FQAM_Op operator, char *name);