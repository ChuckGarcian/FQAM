
/*Life Cycle*/
FQAM_Error FQAM_Operator_init (FQAM_Op *operator, char *name);
FQAM_Error FQAM_operator_free (FQAM_Op *operator);

/* Operator Helper Functions*/
void FQAM_Operator_show (FQAM_Op *operator);
bool FQAM_Operator_initialized (FQAM_Op *operator);