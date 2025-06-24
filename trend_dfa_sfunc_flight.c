/* trend_dfa_sfunc_flight.c - Flight data version with velocity, baroaltitude, lat, lon, vertare */

#define S_FUNCTION_NAME  trend_dfa_sfunc_flight
#define S_FUNCTION_LEVEL 2

#include "simstruc.h"
#include <string.h>
#include <math.h>

/* Copy the DFA functions directly into this file to avoid linking issues */

/* State definitions */
#define STATE_STABLE      1
#define STATE_INCREASING  2  
#define STATE_DECREASING  3
#define STATE_OSCILLATING 4
#define STATE_ANOMALY     5

/* Thresholds - adjusted for flight data */
#define STABLE_THRESHOLD      0.5
#define INCREASE_THRESHOLD    2.0
#define DECREASE_THRESHOLD   -2.0
#define OSCILLATION_THRESHOLD 100.0
#define ANOMALY_THRESHOLD     1000.0

/* Flight-specific anomaly thresholds */
#define VEL_ANOMALY_THRESHOLD     500.0   /* m/s - extreme velocity */
#define ALT_ANOMALY_THRESHOLD     50000.0 /* m - extreme altitude */
#define LAT_ANOMALY_THRESHOLD     90.0    /* degrees - invalid latitude */
#define LON_ANOMALY_THRESHOLD     180.0   /* degrees - invalid longitude */
#define VERTARE_ANOMALY_THRESHOLD 1000.0  /* vertical area threshold */

#define SAMPLE_SIZE 10

/* Static variables for DFA state persistence */
static int prev_state = STATE_STABLE;
static int state_counter = 0;

/* Internal DFA functions */
static double calculate_slope(double *x, double *y, int n) {
    double sum_x = 0, sum_y = 0, sum_xy = 0, sum_x2 = 0;
    double denominator, slope;
    int i;
    
    for (i = 0; i < n; i++) {
        sum_x += x[i];
        sum_y += y[i];
        sum_xy += x[i] * y[i];
        sum_x2 += x[i] * x[i];
    }
    
    denominator = n * sum_x2 - sum_x * sum_x;
    
    if (fabs(denominator) < 1e-10) {
        slope = 0.0;
    } else {
        slope = (n * sum_xy - sum_x * sum_y) / denominator;
    }
    
    return slope;
}

static double calculate_variance(double *data, int n) {
    double mean = 0, variance = 0;
    int i;
    
    for (i = 0; i < n; i++) {
        mean += data[i];
    }
    mean /= n;
    
    for (i = 0; i < n; i++) {
        variance += (data[i] - mean) * (data[i] - mean);
    }
    variance /= (n - 1);
    
    return variance;
}

static int detect_flight_anomaly(double *vel, double *baroalt, double *lat, double *lon, double *vertare) {
    int i;
    
    for (i = 0; i < SAMPLE_SIZE; i++) {
        /* Check velocity anomalies */
        if (fabs(vel[i]) > VEL_ANOMALY_THRESHOLD) {
            return 1;
        }
        
        /* Check altitude anomalies */
        if (fabs(baroalt[i]) > ALT_ANOMALY_THRESHOLD) {
            return 1;
        }
        
        /* Check latitude bounds */
        if (fabs(lat[i]) > LAT_ANOMALY_THRESHOLD) {
            return 1;
        }
        
        /* Check longitude bounds */
        if (fabs(lon[i]) > LON_ANOMALY_THRESHOLD) {
            return 1;
        }
        
        /* Check vertical area anomalies */
        if (fabs(vertare[i]) > VERTARE_ANOMALY_THRESHOLD) {
            return 1;
        }
    }
    return 0;
}

static int internal_flight_trend_analysis_dfa(double *vel_array, double *baroalt_array, double *lat_array, 
                                             double *lon_array, double *vertare_array, 
                                             double *confidence, double *trend_values) {
    
    int i;
    double x_points[SAMPLE_SIZE];
    double vel_trend, baroalt_trend, lat_trend, lon_trend, vertare_trend;
    double vel_var, baroalt_var, lat_var, lon_var, vertare_var;
    double weighted_trend, max_variance;
    int new_state, current_state;
    
    /* Initialize x points */
    for (i = 0; i < SAMPLE_SIZE; i++) {
        x_points[i] = (double)(i + 1);
    }
    
    /* Calculate trends for flight parameters */
    vel_trend = calculate_slope(x_points, vel_array, SAMPLE_SIZE);
    baroalt_trend = calculate_slope(x_points, baroalt_array, SAMPLE_SIZE);
    lat_trend = calculate_slope(x_points, lat_array, SAMPLE_SIZE);
    lon_trend = calculate_slope(x_points, lon_array, SAMPLE_SIZE);
    vertare_trend = calculate_slope(x_points, vertare_array, SAMPLE_SIZE);
    
    /* Calculate variances */
    vel_var = calculate_variance(vel_array, SAMPLE_SIZE);
    baroalt_var = calculate_variance(baroalt_array, SAMPLE_SIZE);
    lat_var = calculate_variance(lat_array, SAMPLE_SIZE);
    lon_var = calculate_variance(lon_array, SAMPLE_SIZE);
    vertare_var = calculate_variance(vertare_array, SAMPLE_SIZE);
    
    /* Find maximum variance */
    max_variance = vel_var;
    if (baroalt_var > max_variance) max_variance = baroalt_var;
    if (lat_var > max_variance) max_variance = lat_var;
    if (lon_var > max_variance) max_variance = lon_var;
    if (vertare_var > max_variance) max_variance = vertare_var;
    
    /* Calculate weighted trend - prioritizing velocity and altitude for flight analysis */
    weighted_trend = 0.4 * vel_trend + 0.3 * baroalt_trend + 0.1 * lat_trend + 
                     0.1 * lon_trend + 0.1 * vertare_trend;
    
    /* State decision logic */
    if (detect_flight_anomaly(vel_array, baroalt_array, lat_array, lon_array, vertare_array)) {
        new_state = STATE_ANOMALY;
        *confidence = 0.95;
    }
    else if (max_variance > OSCILLATION_THRESHOLD) {
        new_state = STATE_OSCILLATING;
        *confidence = 0.8;
    }
    else if (weighted_trend > INCREASE_THRESHOLD) {
        new_state = STATE_INCREASING;
        *confidence = 0.85;
    }
    else if (weighted_trend < DECREASE_THRESHOLD) {
        new_state = STATE_DECREASING;
        *confidence = 0.85;
    }
    else if (fabs(weighted_trend) <= STABLE_THRESHOLD) {
        new_state = STATE_STABLE;
        *confidence = 0.9;
    }
    else {
        new_state = prev_state;
        *confidence = 0.6;
    }
    
    /* State transition with hysteresis */
    if (new_state == prev_state) {
        state_counter++;
    } else {
        state_counter = 1;
    }
    
    if (state_counter >= 2 || new_state == STATE_ANOMALY) {
        current_state = new_state;
        prev_state = new_state;
    } else {
        current_state = prev_state;
    }
    
    /* Fill trend values - flight parameters */
    trend_values[0] = vel_trend;
    trend_values[1] = baroalt_trend;
    trend_values[2] = lat_trend;
    trend_values[3] = lon_trend;
    trend_values[4] = vertare_trend;
    trend_values[5] = weighted_trend;
    
    return current_state;
}

/* S-Function implementation */
#define NUM_INPUTS      5
#define NUM_OUTPUTS     4
#define TOTAL_RWORK_SIZE    51

static void mdlInitializeSizes(SimStruct *S)
{
    int i;
    
    ssSetNumSFcnParams(S, 0);
    if (ssGetNumSFcnParams(S) != ssGetSFcnParamsCount(S)) {
        return;
    }

    ssSetNumContStates(S, 0);
    ssSetNumDiscStates(S, 0);

    if (!ssSetNumInputPorts(S, NUM_INPUTS)) return;
    
    for (i = 0; i < NUM_INPUTS; i++) {
        ssSetInputPortWidth(S, i, 1);
        ssSetInputPortDataType(S, i, SS_DOUBLE);
        ssSetInputPortComplexSignal(S, i, COMPLEX_NO);
        ssSetInputPortDirectFeedThrough(S, i, 1);
        ssSetInputPortRequiredContiguous(S, i, 1);
    }

    if (!ssSetNumOutputPorts(S, NUM_OUTPUTS)) return;
    
    ssSetOutputPortWidth(S, 0, 1);
    ssSetOutputPortDataType(S, 0, SS_DOUBLE);
    ssSetOutputPortComplexSignal(S, 0, COMPLEX_NO);
    
    ssSetOutputPortWidth(S, 1, 1);
    ssSetOutputPortDataType(S, 1, SS_DOUBLE);
    ssSetOutputPortComplexSignal(S, 1, COMPLEX_NO);
    
    ssSetOutputPortWidth(S, 2, 6);
    ssSetOutputPortDataType(S, 2, SS_DOUBLE);
    ssSetOutputPortComplexSignal(S, 2, COMPLEX_NO);
    
    ssSetOutputPortWidth(S, 3, 1);
    ssSetOutputPortDataType(S, 3, SS_DOUBLE);
    ssSetOutputPortComplexSignal(S, 3, COMPLEX_NO);

    ssSetNumSampleTimes(S, 1);
    ssSetNumRWork(S, TOTAL_RWORK_SIZE);
    ssSetNumIWork(S, 0);
    ssSetNumPWork(S, 0);
    ssSetNumModes(S, 0);
    ssSetNumNonsampledZCs(S, 0);

    ssSetOptions(S, SS_OPTION_EXCEPTION_FREE_CODE);
}

static void mdlInitializeSampleTimes(SimStruct *S)
{
    ssSetSampleTime(S, 0, INHERITED_SAMPLE_TIME);
    ssSetOffsetTime(S, 0, 0.0);
}

#define MDL_START
#if defined(MDL_START) 
static void mdlStart(SimStruct *S)
{
    real_T *rwork = ssGetRWork(S);
    int i;
    
    if (rwork == NULL) {
        ssSetErrorStatus(S, "RWork allocation failed");
        return;
    }
    
    for (i = 0; i < TOTAL_RWORK_SIZE; i++) {
        rwork[i] = 0.0;
    }
    
    /* Reset DFA state */
    prev_state = STATE_STABLE;
    state_counter = 0;
}
#endif

static void mdlOutputs(SimStruct *S, int_T tid)
{
    /* Get inputs - flight data parameters */
    double *vel_input     = (double*)ssGetInputPortSignal(S, 0);  /* velocity */
    double *baroalt_input = (double*)ssGetInputPortSignal(S, 1);  /* baroaltitude */
    double *lat_input     = (double*)ssGetInputPortSignal(S, 2);  /* latitude */
    double *lon_input     = (double*)ssGetInputPortSignal(S, 3);  /* longitude */
    double *vertare_input = (double*)ssGetInputPortSignal(S, 4);  /* vertare */
    
    /* Get outputs */
    double *state_output  = (double*)ssGetOutputPortSignal(S, 0);
    double *conf_output   = (double*)ssGetOutputPortSignal(S, 1);
    double *trends_output = (double*)ssGetOutputPortSignal(S, 2);
    double *name_output   = (double*)ssGetOutputPortSignal(S, 3);
    
    /* Safety checks */
    if (!vel_input || !baroalt_input || !lat_input || !lon_input || !vertare_input ||
        !state_output || !conf_output || !trends_output || !name_output) {
        ssSetErrorStatus(S, "Null pointer detected");
        return;
    }
    
    real_T *rwork = ssGetRWork(S);
    if (!rwork) {
        ssSetErrorStatus(S, "RWork is null");
        return;
    }
    
    /* Buffer management for flight data */
    double *vel_buffer     = &rwork[0];   /* velocity buffer */
    double *baroalt_buffer = &rwork[10];  /* baroaltitude buffer */
    double *lat_buffer     = &rwork[20];  /* latitude buffer */
    double *lon_buffer     = &rwork[30];  /* longitude buffer */
    double *vertare_buffer = &rwork[40];  /* vertare buffer */
    
    int buffer_idx = (int)rwork[50];
    int i;
    
    /* Shift buffers */
    for (i = 0; i < SAMPLE_SIZE - 1; i++) {
        vel_buffer[i]     = vel_buffer[i + 1];
        baroalt_buffer[i] = baroalt_buffer[i + 1];
        lat_buffer[i]     = lat_buffer[i + 1];
        lon_buffer[i]     = lon_buffer[i + 1];
        vertare_buffer[i] = vertare_buffer[i + 1];
    }
    
    /* Add new values */
    vel_buffer[SAMPLE_SIZE - 1]     = vel_input[0];
    baroalt_buffer[SAMPLE_SIZE - 1] = baroalt_input[0];
    lat_buffer[SAMPLE_SIZE - 1]     = lat_input[0];
    lon_buffer[SAMPLE_SIZE - 1]     = lon_input[0];
    vertare_buffer[SAMPLE_SIZE - 1] = vertare_input[0];
    
    buffer_idx++;
    rwork[50] = (double)buffer_idx;
    
    /* Process when enough samples */
    if (buffer_idx >= SAMPLE_SIZE) {
        int current_state;
        double confidence;
        double trend_values[6];
        
        current_state = internal_flight_trend_analysis_dfa(vel_buffer, baroalt_buffer, lat_buffer, 
                                                          lon_buffer, vertare_buffer, 
                                                          &confidence, trend_values);
        
        state_output[0] = (double)current_state;
        conf_output[0] = confidence;
        
        for (i = 0; i < 6; i++) {
            trends_output[i] = trend_values[i];
        }
        
        name_output[0] = (double)current_state;
    } else {
        /* Initial values */
        state_output[0] = 1.0;
        conf_output[0] = 0.0;
        for (i = 0; i < 6; i++) {
            trends_output[i] = 0.0;
        }
        name_output[0] = 1.0;
    }
}

static void mdlTerminate(SimStruct *S)
{
    prev_state = STATE_STABLE;
    state_counter = 0;
}

#ifdef  MATLAB_MEX_FILE
#include "simulink.c"
#else
#include "cg_sfun.h"
#endif