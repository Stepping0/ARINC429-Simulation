#define S_FUNCTION_NAME  arinc429_decimal_to_bcd
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

    /* Configure input port - a single double representing decimal value */
    ssSetInputPortWidth(S, 0, 1);
    ssSetInputPortDataType(S, 0, SS_DOUBLE);
    ssSetInputPortRequiredContiguous(S, 0, true);
    ssSetInputPortDirectFeedThrough(S, 0, 1);

    /* Configure output port - 19-bit array representing BCD data */
    ssSetOutputPortWidth(S, 0, 19);
    ssSetOutputPortDataType(S, 0, SS_UINT8);

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
 *    Encode decimal value to ARINC 429 BCD data according to the standard specification.
 *    BCD Character 1: 3 bits (0-7)
 *    BCD Characters 2-5: 4 bits each (0-9)
 */
static void mdlOutputs(SimStruct *S, int_T tid)
{
    /* Get input and output pointers */
    const real_T *u = (const real_T*) ssGetInputPortSignal(S, 0);
    uint8_T *y = (uint8_T*) ssGetOutputPortSignal(S, 0);
    
    /* Get decimal value and ensure it's in valid range (0-99999) */
    double decimal_value = u[0];
    if (decimal_value < 0) decimal_value = 0;
    if (decimal_value > 99999) decimal_value = 99999;
    
    /* Extract BCD digits */
    int bcd1 = (int)(decimal_value / 10000) % 10;    /* 10000's place */
    int bcd2 = (int)(decimal_value / 1000) % 10;     /* 1000's place */
    int bcd3 = (int)(decimal_value / 100) % 10;      /* 100's place */
    int bcd4 = (int)(decimal_value / 10) % 10;       /* 10's place */
    int bcd5 = (int)(decimal_value) % 10;            /* 1's place */
    
    /* Debug print the extracted BCD digits */
    #ifdef MATLAB_MEX_FILE
    ssPrintf("Decimal value: %f\n", decimal_value);
    ssPrintf("BCD digits (MSB to LSB): %d %d %d %d %d\n", bcd1, bcd2, bcd3, bcd4, bcd5);
    #endif
    
    /* Initialize output array to zeros */
    for (int i = 0; i < 19; i++) {
        y[i] = 0;
    }
    
    /* Encode BCD Character #1 (MSB, 3 bits) */
    /* Bits 0-2 */
    y[0] = (bcd1 >> 2) & 0x01;
    y[1] = (bcd1 >> 1) & 0x01;
    y[2] = bcd1 & 0x01;
    
    /* Encode BCD Character #2 (4 bits) */
    /* Bits 3-6 */
    y[3] = (bcd2 >> 3) & 0x01;
    y[4] = (bcd2 >> 2) & 0x01;
    y[5] = (bcd2 >> 1) & 0x01;
    y[6] = bcd2 & 0x01;
    
    /* Encode BCD Character #3 (4 bits) */
    /* Bits 7-10 */
    y[7] = (bcd3 >> 3) & 0x01;
    y[8] = (bcd3 >> 2) & 0x01;
    y[9] = (bcd3 >> 1) & 0x01;
    y[10] = bcd3 & 0x01;
    
    /* Encode BCD Character #4 (4 bits) */
    /* Bits 11-14 */
    y[11] = (bcd4 >> 3) & 0x01;
    y[12] = (bcd4 >> 2) & 0x01;
    y[13] = (bcd4 >> 1) & 0x01;
    y[14] = bcd4 & 0x01;
    
    /* Encode BCD Character #5 (LSB, 4 bits) */
    /* Bits 15-18 */
    y[15] = (bcd5 >> 3) & 0x01;
    y[16] = (bcd5 >> 2) & 0x01;
    y[17] = (bcd5 >> 1) & 0x01;
    y[18] = bcd5 & 0x01;
    
    /* Debug print the output bits */
    #ifdef MATLAB_MEX_FILE
    ssPrintf("Output bits: ");
    for (int i = 0; i < 19; i++) {
        ssPrintf("%d", y[i]);
    }
    ssPrintf("\n");
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