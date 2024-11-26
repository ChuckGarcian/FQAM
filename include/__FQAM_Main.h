/*Life Cycle */
FQAM_Error FQAM_init (void);
FQAM_Error FQAM_finalize (void);

/* Stage Commands */
FQAM_Error FQAM_stage_append (FQAM_Op *operator); // Adds operator to staging list
void FQAM_stage_show (void);
void _debug_FQAM_show_stage (void);

void FQAM_compute_outcomes (void);

/*FQAM States*/
bool FQAM_initialized (void);
