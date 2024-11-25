FQAM_Error FQAM_init (void);
FQAM_Error FQAM_finalize (void);

void FQAM_stage_show (void);
FQAM_Error FQAM_stage_append (FQAM_Op *operator); // Adds operator to staging list

bool FQAM_initialized (void);

void _debug_FQAM_show_stage (void);