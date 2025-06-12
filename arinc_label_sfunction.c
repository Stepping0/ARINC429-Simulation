#define S_FUNCTION_NAME  arinc_label_sfunction
#define S_FUNCTION_LEVEL 2

#include "simstruc.h"

/* mdlInitializeSizes: Giriş/Çıkış portlarının tanımı */
static void mdlInitializeSizes(SimStruct *S)
{
    ssSetNumSFcnParams(S, 0); // Parametre yok
    if (!ssSetNumInputPorts(S, 1)) return;
    ssSetInputPortWidth(S, 0, 1);   // 1 giriş: label (uint8)
    ssSetInputPortDataType(S, 0, SS_UINT8);

    if (!ssSetNumOutputPorts(S, 1)) return;
    ssSetOutputPortWidth(S, 0, 1);  // 1 çıkış: label_out (uint8)
    ssSetOutputPortDataType(S, 0, SS_UINT8);

    ssSetNumSampleTimes(S, 1);
}

/* Zamanlama */
static void mdlInitializeSampleTimes(SimStruct *S)
{
    ssSetSampleTime(S, 0, INHERITED_SAMPLE_TIME);
    ssSetOffsetTime(S, 0, 0.0);
}

/* Çıkış Hesaplama */
static void mdlOutputs(SimStruct *S, int_T tid)
{
    InputUInt8PtrsType uPtrs = (InputUInt8PtrsType)ssGetInputPortSignalPtrs(S, 0);
    uint8_T label = *uPtrs[0];

    // ARINC 429 gereği label'ın LSB'si kelimenin MSB'sine gider (ters çevirme)
    uint8_T label_flipped = 0;
    for (int i = 0; i < 8; ++i)
    {
        label_flipped |= ((label >> i) & 0x01) << (7 - i);
    }

    uint8_T *y = (uint8_T *)ssGetOutputPortSignal(S, 0);
    y[0] = label_flipped;
}

/* Gerekli diğer fonksiyonlar */
static void mdlTerminate(SimStruct *S) {}

/* S-Function Makrosu */
#ifdef MATLAB_MEX_FILE
#include "simulink.c"
#else
#include "cg_sfun.h"
#endif
