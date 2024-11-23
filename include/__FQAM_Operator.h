
// Returns
FQAM_Error FQAM_stage_init(void);

// Initialize an operator 
FQAM_Error FQAM_operator_create (FQAM_Op operator, char *name);

// Placeholder for a set of different functions to assist in creating operator matrix representations
// functions could include: tensor_product, generate_projector, measure_op, ect
FQAM_Error FQAM_operator_generate_functions (FQAM_Op operator, char *name);

void FQAM_Operator_show (FQAM_Op operator);