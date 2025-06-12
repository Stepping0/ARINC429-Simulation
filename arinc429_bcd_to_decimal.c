#define S_FUNCTION_NAME  arinc429_bcd_to_decimal
#define S_FUNCTION_LEVEL 2

#include "simstruc.h"
#include <stdio.h>

/* Function: mdlInitializeSizes ===============================================
 * Abstract:
 *    The sizes information is used by Simulink to determine the S-function
 *    block's characteristics (number of inputs, outputs, states, etc.).
 */
static void mdlInitializeSizes(SimStruct *S)
{
    /* Set number of expected parameters */
    ssSetNumSFcnParams(S, 0);
    if (ssGetNumSFcnParams(S) != ssGetSFcnParamsCount(S)) {
        return; /* Parameter mismatch reported by Simulink */
    }

    /* Set number of input and output ports */
    if (!ssSetNumInputPorts(S, 1)) return;
    if (!ssSetNumOutputPorts(S, 1)) return;

    /* Configure input port - 19-bit array representing BCD data */
    ssSetInputPortWidth(S, 0, 19);
    ssSetInputPortDataType(S, 0, SS_UINT8);
    ssSetInputPortRequiredContiguous(S, 0, true);
    ssSetInputPortDirectFeedThrough(S, 0, 1);

    /* Configure output port - a single double value */
    ssSetOutputPortWidth(S, 0, 1);
    ssSetOutputPortDataType(S, 0, SS_DOUBLE);

    /* Set sample times */
    ssSetNumSampleTimes(S, 1);

    /* No work vectors needed */
    ssSetNumRWork(S, 0);
    ssSetNumIWork(S, 0);
    ssSetNumPWork(S, 0);
    ssSetNumModes(S, 0);
    ssSetNumNonsampledZCs(S, 0);

    /* Specify the operating point save/restore compliance to be same as a 
     * built-in block */
    ssSetOperatingPointCompliance(S, USE_DEFAULT_OPERATING_POINT);

    /* Set options */
    ssSetOptions(S, 0);
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
    ssSetModelReferenceSampleTimeDefaultInheritance(S);
}

/* Function: mdlOutputs =======================================================
 * Abstract:
 *    Decode ARINC 429 BCD data according to the standard specification.
 *    BCD Character 1: 3 bits
 *    BCD Characters 2-5: 4 bits each
 */
static void mdlOutputs(SimStruct *S, int_T tid)
{
    /* Get input and output pointers */
    const uint8_T *u = (const uint8_T*) ssGetInputPortSignal(S, 0);
    real_T *y = (real_T*) ssGetOutputPortSignal(S, 0);
    
    /* ARINC 429 BCD decoding according to specification */
    /* Note: The index mapping here assumes bit 0 is the MSB as per the specification */
    
    /* Extract BCD digits */
    /* Per the spec: 3-4-4-4-4 pattern from MSB to LSB */
    
    /* BCD Character #1 (MSB, 3 bits) */
    int bcd1 = (u[0] << 2) | (u[1] << 1) | u[2];
    
    /* BCD Character #2 (4 bits) */
    int bcd2 = (u[3] << 3) | (u[4] << 2) | (u[5] << 1) | u[6];
    
    /* BCD Character #3 (4 bits) */
    int bcd3 = (u[7] << 3) | (u[8] << 2) | (u[9] << 1) | u[10];
    
    /* BCD Character #4 (4 bits) */
    int bcd4 = (u[11] << 3) | (u[12] << 2) | (u[13] << 1) | u[14];
    
    /* BCD Character #5 (LSB, 4 bits) */
    int bcd5 = (u[15] << 3) | (u[16] << 2) | (u[17] << 1) | u[18];
    
    /* Debug print the extracted BCD digits */
    #ifdef MATLAB_MEX_FILE
    ssPrintf("Input bits: ");
    for (int i = 0; i < 19; i++) {
        ssPrintf("%d", u[i]);
    }
    ssPrintf("\n");
    
    ssPrintf("BCD digits (MSB to LSB): %d %d %d %d %d\n", bcd1, bcd2, bcd3, bcd4, bcd5);
    #endif
    
    /* Combine digits to form decimal value */
    double decimal = bcd5 + bcd4*10 + bcd3*100 + bcd2*1000 + bcd1*10000;
    
    /* Set output */
    y[0] = decimal;
    
    /* Debug print result */
    #ifdef MATLAB_MEX_FILE
    ssPrintf("Final decimal value: %f\n", decimal);
    #endif
}

/* Function: mdlTerminate =====================================================
 * Abstract:
 *    No termination needed.
 */
static void mdlTerminate(SimStruct *S)
{
}

/* Required S-function trailer */
#ifdef  MATLAB_MEX_FILE    /* Is this file being compiled as a MEX-file? */
#include "simulink.c"      /* MEX-file interface mechanism */
#else
#include "cg_sfun.h"       /* Code generation registration function */
#endif