/*
    Copyright (C) 2024, Chuck Garcia

    This file is part of libfqam and is available under the 3-Clause
    BSD license, which can be found in the LICENSE file at the top-level
    directory, or at http://opensource.org/licenses/BSD-3-Clause
*/

#include <stdbool.h>
#include <math.h>

#include "arraylist.h"
#include "assertf.h"
#include "FQAM.h"

static bool _FQAM_initialized = false;

// Stage Struct
static struct stage {
  double _Complex *statevector; // Quantum statevector 
  size_t dim;                   // Dimension of hilbertspace
  arraylist *stage;             // Contain sequence applied computation steps
} main_stage;

/*
Arguments:
    size_t dim: Dimension of Hilbert space
    unsigned int initial_state: Nonnegative integer to initialize statevector to
*/
void FQAM_init (size_t dim, unsigned int initial_state)
{
    // Initialize Flame
    FLA_Init ();
    
    // Initialize Statevector
    int i = 0;
    main_stage.statevector = (double _Complex *)calloc (sizeof (double _Complex), pow (2, dim));
    assertf (main_stage.statevector, "Error: Failed to allocate statevector");

    while (initial_state > 0)
    {
        main_stage.statevector[i++] = initial_state % 2 + 0.0 * I;
        initial_state /= 2;
    }

    // Initialize stage
    main_stage.stage = arraylist_create ();
    _FQAM_initialized = true;
    
    pauli_ops_init_ (); // TODO: Add way to pass if built in operators should be initialized
    printf ("FQAM: Initialized\n");
}

/*
Free resources in FQAM. Includes all FQAM modules.
*/
void FQAM_finalize (void)
{
    assertf (FQAM_initialized (), "Error: Tried to finalize uninitialized FQAM");

    // Free all operator matrices
    for (int idx = 0; idx < main_stage.stage->size; idx++)
    {
        FQAM_Op *operator = arraylist_get (main_stage.stage, idx);
        FQAM_Operator_free (operator);
    }

    FLA_Finalize ();
    arraylist_destroy (main_stage.stage);
    
    printf ("FQAM: Finalized\n");
    return FQAM_SUCCESS;
}

/*
Appends operator to stage.

Returns: 
    FQAM_SUCCESS on success, FQAM_FAILURE otherwise

Arguments:
    operator: Operator object. Must be initialized, otherwise an assertion
             fault is triggered.
*/
void FQAM_stage_append (FQAM_Op *operator)
{
    // Ensure operator has been initialized
    assertf (FQAM_Operator_initialized (operator),
            "Error: Tried appending uninitialized operator object");
    assertf (FLA_Obj_buffer_is_null (operator->mat_repr) == 0,
            "Error: Tried appending operator with null matrix representation");
    
    arraylist_add (main_stage.stage, (void *)operator);
    
    return FQAM_SUCCESS;
}

bool FQAM_initialized (void)
{
    return _FQAM_initialized;
}

void FQAM_compute_outcomes (void)
{
    assertf (FQAM_initialized (), "Error: Computing in uninitialized stage\n");

    for (int idx = 0; idx < main_stage.stage->size; idx++)
    {
        FQAM_Op *operator = arraylist_get (main_stage.stage, idx);
        FQAM_Operator_free (operator);
    }
}

/*
Debug function which prints the stage list
*/
void _debug_FQAM_show_stage (void)
{
    printf ("----Debug: Showing stage----\n");
    
    assertf (FQAM_initialized (), "Error: FQAM expected to be in initialized state");

    for (int idx = 0; idx < main_stage.stage->size; idx++)
    {
        printf ("Index: %d\n", idx);
        
        FQAM_Op *op = arraylist_get (main_stage.stage, idx);
        FQAM_Operator_show (op);
    }
    
    printf ("----Debug: Done stage----\n");
}