/* trend_dfa.c - Trend Analysis DFA Implementation */

#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>

/* State definitions */
#define STATE_STABLE      1
#define STATE_INCREASING  2  
#define STATE_DECREASING  3
#define STATE_OSCILLATING 4
#define STATE_ANOMALY     5

/* Thresholds */
#define STABLE_THRESHOLD      0.5
#define INCREASE_THRESHOLD    2.0
#define DECREASE_THRESHOLD   -2.0
#define OSCILLATION_THRESHOLD 100.0
#define ANOMALY_THRESHOLD     1000.0

/* Data array size */
#define SAMPLE_SIZE 10

/* Static variables for state persistence */
static int prev_state = STATE_STABLE;
static int state_counter = 0;

/* Function prototypes */
double calculate_slope(double *x, double *y, int n);
double calculate_variance(double *data, int n);
int detect_anomaly(double *vel, double *alt, double *temp, double *press, double *mach);
const char* get_state_name(int state);

/* Main DFA function */
int trend_analysis_dfa(double *vel_array, double *alt_array, double *temp_array, 
                       double *press_array, double *mach_array, 
                       double *confidence, double *trend_values, char *state_name) {
    
    int i;
    double x_points[SAMPLE_SIZE];
    double vel_trend, alt_trend, temp_trend, press_trend, mach_trend;
    double vel_var, alt_var, temp_var, press_var, mach_var;
    double weighted_trend, max_variance;
    int new_state, current_state;
    
    /* Initialize x points for slope calculation */
    for (i = 0; i < SAMPLE_SIZE; i++) {
        x_points[i] = (double)(i + 1);
    }
    
    /* Calculate trends (slopes) */
    vel_trend = calculate_slope(x_points, vel_array, SAMPLE_SIZE);
    alt_trend = calculate_slope(x_points, alt_array, SAMPLE_SIZE);
    temp_trend = calculate_slope(x_points, temp_array, SAMPLE_SIZE);
    press_trend = calculate_slope(x_points, press_array, SAMPLE_SIZE);
    mach_trend = calculate_slope(x_points, mach_array, SAMPLE_SIZE);
    
    /* Calculate variances */
    vel_var = calculate_variance(vel_array, SAMPLE_SIZE);
    alt_var = calculate_variance(alt_array, SAMPLE_SIZE);
    temp_var = calculate_variance(temp_array, SAMPLE_SIZE);
    press_var = calculate_variance(press_array, SAMPLE_SIZE);
    mach_var = calculate_variance(mach_array, SAMPLE_SIZE);
    
    /* Find maximum variance */
    max_variance = vel_var;
    if (alt_var > max_variance) max_variance = alt_var;
    if (temp_var > max_variance) max_variance = temp_var;
    if (press_var > max_variance) max_variance = press_var;
    if (mach_var > max_variance) max_variance = mach_var;
    
    /* Calculate weighted trend (velocity and altitude more important) */
    weighted_trend = 0.4 * vel_trend + 0.3 * alt_trend + 0.1 * temp_trend + 
                     0.1 * press_trend + 0.1 * mach_trend;
    
    /* State decision logic */
    if (detect_anomaly(vel_array, alt_array, temp_array, press_array, mach_array)) {
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
    
    /* Change state only if consistent for 2+ cycles (except anomaly) */
    if (state_counter >= 2 || new_state == STATE_ANOMALY) {
        current_state = new_state;
        prev_state = new_state;
    } else {
        current_state = prev_state;
    }
    
    /* Fill trend values array for output */
    trend_values[0] = vel_trend;
    trend_values[1] = alt_trend;
    trend_values[2] = temp_trend;
    trend_values[3] = press_trend;
    trend_values[4] = mach_trend;
    trend_values[5] = weighted_trend;
    
    /* Copy state name */
    strcpy(state_name, get_state_name(current_state));
    
    return current_state;
}

/* Calculate linear regression slope */
double calculate_slope(double *x, double *y, int n) {
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
        slope = 0.0; /* Avoid division by zero */
    } else {
        slope = (n * sum_xy - sum_x * sum_y) / denominator;
    }
    
    return slope;
}

/* Calculate variance */
double calculate_variance(double *data, int n) {
    double mean = 0, variance = 0;
    int i;
    
    /* Calculate mean */
    for (i = 0; i < n; i++) {
        mean += data[i];
    }
    mean /= n;
    
    /* Calculate variance */
    for (i = 0; i < n; i++) {
        variance += (data[i] - mean) * (data[i] - mean);
    }
    variance /= (n - 1);
    
    return variance;
}

/* Detect anomalies in data */
int detect_anomaly(double *vel, double *alt, double *temp, double *press, double *mach) {
    int i;
    
    for (i = 0; i < SAMPLE_SIZE; i++) {
        if (fabs(vel[i]) > ANOMALY_THRESHOLD ||
            fabs(alt[i]) > ANOMALY_THRESHOLD ||
            fabs(temp[i]) > ANOMALY_THRESHOLD ||
            fabs(press[i]) > ANOMALY_THRESHOLD ||
            fabs(mach[i]) > ANOMALY_THRESHOLD) {
            return 1; /* Anomaly detected */
        }
    }
    return 0; /* No anomaly */
}

/* Get state name string */
const char* get_state_name(int state) {
    switch (state) {
        case STATE_STABLE:      return "STABLE";
        case STATE_INCREASING:  return "INCREASING";
        case STATE_DECREASING:  return "DECREASING";
        case STATE_OSCILLATING: return "OSCILLATING";
        case STATE_ANOMALY:     return "ANOMALY";
        default:                return "UNKNOWN";
    }
}

/* Reset function for S-function */
void reset_dfa_state(void) {
    prev_state = STATE_STABLE;
    state_counter = 0;
}