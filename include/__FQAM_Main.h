/*Life Cycle */
void FQAM_init (size_t dim, unsigned int initial_state);
void FQAM_finalize (void);

/* Stage Commands */
void FQAM_stage_append (FQAM_Op operator); // Adds operator to staging list
void FQAM_stage_show (void);

void FQAM_compute_outcomes (void);
void _debug_FQAM_show_stage (void);

/*FQAM States*/
bool FQAM_initialized (void);
void FQAM_show_statevector (void);

/* Debug commands (TODO: delete) */
void _debug_FQAM_show_stage (void);
void _debug_show_fla_meta_data (FLA_Obj A);
void _debug_show_state_data (void);
