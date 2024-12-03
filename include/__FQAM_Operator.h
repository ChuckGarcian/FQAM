
/*Life Cycle*/
FQAM_Error FQAM_Op_create (FQAM_Op *operator, char *name, int dim);
FQAM_Error FQAM_operator_free (FQAM_Op *operator);

/* Operator Generation Functions */


FQAM_Basis FQAM_Basis_create   (int num_qubits, double angle, int eigen_value);
void FQAM_Basis_outer (FQAM_Basis ket0, FQAM_Basis ket1, FQAM_Op *outer);
void FQAM_Op_add (dcomplex alpha, FQAM_Op term, FQAM_Op *result);


// FQAM_Basis FQAM_Basis_state   (int eigenstate, double angle);
// void FQAM_Basis_state_N (FQAM_Basis basis, int eigenstate, double angle, dim_t dim);    
// void FQAM_Op_outer      (FQAM_Op outer, FQAM_Basis ket0, FQAM_Basis ket1);
// void FQAM_Op_add        (FQAM_Op outer, double alpha, FQAM_Op term);
// bool FQAM_Op_check      (FQAM_Op op); // Asserts 'boundary conditions

/*
FQAM_Error FQAM_Operator_outerprod (FQAM_Op *operator);
FQAM_Error FQAM_Op_kron (FQAM_Op *operator);

*/

/* Operator Helper Functions*/
void FQAM_Operator_show (FQAM_Op *operator);
bool FQAM_Operator_initialized (FQAM_Op *operator);

