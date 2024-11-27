
/*Life Cycle*/
FQAM_Error FQAM_Operator_init (FQAM_Op *operator, char *name);
FQAM_Error FQAM_operator_free (FQAM_Op *operator);

/* Operator Generation Functions */

FQAM_Error FQAM_Operator_add_term(FQAM_Op *operator, FLA_Datatype alpha, FQAM_Op *op_term);
FQAM_Operator_outer_product (FQAM_Op *op, int theta);

/*
FQAM_Error FQAM_Operator_outerprod (FQAM_Op *operator);
FQAM_Error FQAM_Op_kron (FQAM_Op *operator);

*/

/* Operator Helper Functions*/
void FQAM_Operator_show (FQAM_Op *operator);
bool FQAM_Operator_initialized (FQAM_Op *operator);
