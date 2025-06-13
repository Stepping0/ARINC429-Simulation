/* trend_dfa_sfunc.c - S-Function wrapper for Trend Analysis DFA */

#define S_FUNCTION_NAME  trend_dfa_sfunc
#define S_FUNCTION_LEVEL 2

#include "simstruc.h"
#include <string.h>

/* Include the DFA implementation */
extern int trend_analysis_dfa(double *vel_array, double *alt_array, double *temp_array, 
                             double *press_array, double *mach_array, 
                             double *confidence, double *trend_values, char *state_name);
extern void reset_dfa_state(void);

/* Input/Output port definitions */
#define NUM_INPUTS      5
#define NUM_OUTPUTS     4

#define INPUT_VEL       0
#define INPUT_ALT       1  
#define INPUT_TEMP      2
#define INPUT_PRESS     3
#define INPUT_MACH      4

#define OUTPUT_STATE    0
#define OUTPUT_CONF     1
#define OUTPUT_TRENDS   2
#define OUTPUT_NAME     3

#define SAMPLE_SIZE     10

/*====================*
 * S-function methods *
 *====================*/

/* Function: mdlInitializeSizes ===============================================
 * Abstract:
 *    The sizes information is used by Simulink to determine the S-function
 *    block's characteristics (number of inputs, outputs, states, etc.).
 */
static void mdlInitializeSizes(SimStruct *S)
{
    int i;
    
    ssSetNumSFcnParams(S, 0);  /* Number of expected parameters */
    if (ssGetNumSFcnParams(S) != ssGetSFcnParamsCount(S)) {
        return; /* Parameter mismatch will be reported by Simulink */
    }

    ssSetNumContStates(S, 0);
    ssSetNumDiscStates(S, 0);

    /* Configure input ports */
    if (!ssSetNumInputPorts(S, NUM_INPUTS)) return;
    
    for (i = 0; i < NUM_INPUTS; i++) {
        ssSetInputPortWidth(S, i, SAMPLE_SIZE);
        ssSetInputPortDataType(S, i, SS_DOUBLE);
        ssSetInputPortComplexSignal(S, i, COMPLEX_NO);
        ssSetInputPortDirectFeedThrough(S, i, 1);
        ssSetInputPortRequiredContiguous(S, i, 1);
    }

    /* Configure output ports */
    if (!ssSetNumOutputPorts(S, NUM_OUTPUTS)) return;
    
    /* Output 0: Current state (scalar) */
    ssSetOutputPortWidth(S, OUTPUT_STATE, 1);
    ssSetOutputPortDataType(S, OUTPUT_STATE, SS_DOUBLE);
    ssSetOutputPortComplexSignal(S, OUTPUT_STATE, COMPLEX_NO);
    
    /* Output 1: Confidence (scalar) */
    ssSetOutputPortWidth(S, OUTPUT_CONF, 1);
    ssSetOutputPortDataType(S, OUTPUT_CONF, SS_DOUBLE);
    ssSetOutputPortComplexSignal(S, OUTPUT_CONF, COMPLEX_NO);
    
    /* Output 2: Trend values (6 elements) */
    ssSetOutputPortWidth(S, OUTPUT_TRENDS, 6);
    ssSetOutputPortDataType(S, OUTPUT_TRENDS, SS_DOUBLE);
    ssSetOutputPortComplexSignal(S, OUTPUT_TRENDS, COMPLEX_NO);
    
    /* Output 3: State name (string - represented as double array) */
    ssSetOutputPortWidth(S, OUTPUT_NAME, 1);
    ssSetOutputPortDataType(S, OUTPUT_NAME, SS_DOUBLE);
    ssSetOutputPortComplexSignal(S, OUTPUT_NAME, COMPLEX_NO);

    ssSetNumSampleTimes(S, 1);
    ssSetNumRWork(S, 0);
    ssSetNumIWork(S, 0);
    ssSetNumPWork(S, 0);
    ssSetNumModes(S, 0);
    ssSetNumNonsampledZCs(S, 0);

    /* Take care when specifying exception free code - see sfuntmpl.doc */
    ssSetOptions(S, SS_OPTION_EXCEPTION_FREE_CODE);
}

/* Function: mdlInitializeSampleTimes =========================================
 * Abstract:
 *    This function is used to specify the sample time(s) for your
 *    S-function. You must register the same number of sample times as
 *    specified in ssSetNumSampleTimes.
 */
static void mdlInitializeSampleTimes(SimStruct *S)
{
    ssSetSampleTime(S, 0, INHERITED_SAMPLE_TIME);
    ssSetOffsetTime(S, 0, 0.0);
}

/* Function: mdlStart =======================================================
 * Abstract:
 *    This function is called once at start of model execution. If you
 *    have states that should be initialized once, this is the place
 *    to do it.
 */
#define MDL_START
#if defined(MDL_START) 
static void mdlStart(SimStruct *S)
{
    /* Initialize/reset the DFA state */
    reset_dfa_state();
}
#endif /*  MDL_START */

/* Function: mdlOutputs =======================================================
 * Abstract:
 *    In this function, you compute the outputs of your S-function
 *    block. Generally outputs are placed in the output vector, ssGetY(S).
 */
static void mdlOutputs(SimStruct *S, int_T tid)
{
    /* Input pointers */
    double *vel_input   = (double*)ssGetInputPortSignal(S, INPUT_VEL);
    double *alt_input   = (double*)ssGetInputPortSignal(S, INPUT_ALT);
    double *temp_input  = (double*)ssGetInputPortSignal(S, INPUT_TEMP);
    double *press_input = (double*)ssGetInputPortSignal(S, INPUT_PRESS);
    double *mach_input  = (double*)ssGetInputPortSignal(S, INPUT_MACH);
    
    /* Output pointers */
    double *state_output  = (double*)ssGetOutputPortSignal(S, OUTPUT_STATE);
    double *conf_output   = (double*)ssGetOutputPortSignal(S, OUTPUT_CONF);
    double *trends_output = (double*)ssGetOutputPortSignal(S, OUTPUT_TRENDS);
    double *name_output   = (double*)ssGetOutputPortSignal(S, OUTPUT_NAME);
    
    /* Local variables */
    int current_state;
    double confidence;
    double trend_values[6];
    char state_name[20];
    int i;
    
    /* Call the DFA function */
    current_state = trend_analysis_dfa(vel_input, alt_input, temp_input, 
                                      press_input, mach_input, 
                                      &confidence, trend_values, state_name);
    
    /* Set outputs */
    state_output[0] = (double)current_state;
    conf_output[0] = confidence;
    
    /* Copy trend values */
    for (i = 0; i < 6; i++) {
        trends_output[i] = trend_values[i];
    }
    
    /* Convert state name to numeric representation for Simulink */
    /* You can use a lookup table or hash the string */
    switch (current_state) {
        case 1: name_output[0] = 1.0; break; /* STABLE */
        case 2: name_output[0] = 2.0; break; /* INCREASING */
        case 3: name_output[0] = 3.0; break; /* DECREASING */
        case 4: name_output[0] = 4.0; break; /* OSCILLATING */
        case 5: name_output[0] = 5.0; break; /* ANOMALY */
        default: name_output[0] = 0.0; break; /* UNKNOWN */
    }
}

/* Function: mdlTerminate =====================================================
 * Abstract:
 *    In this function, you should perform any actions that are necessary
 *    at the termination of a simulation. For example, if memory was
 *    allocated in mdlStart, this is the place to free it.
 */
static void mdlTerminate(SimStruct *S)
{
    /* Reset DFA state on termination */
    reset_dfa_state();
}

/*======================================================*
 * See sfuntmpl.doc for the optional S-function methods *
 *======================================================*/

/*=============================*
 * Required S-function trailer *
 *=============================*/

#ifdef  MATLAB_MEX_FILE    /* Is this file being compiled as a MEX-file? */
#include "simulink.c"      /* MEX-file interface mechanism */
#else
#include "cg_sfun.h"       /* Code generation registration function */
#endif