#ifndef BRAKE_CONTROL_H
#define BRAKE_CONTROL_H

typedef struct PID {
  double windup_guard;
  double proportional_gain;
  double integral_gain;
  double derivative_gain;
  double prev_error;
  double int_error;
  double control;
} PID;

#define BRAKING_THRESHOLD  	-0.2F
#define ACCEL_THRESHOLD		0.2F

#if defined (__cplusplus)
extern "C" {
#endif

void pid_zeroize(PID* pid);
void pid_update(PID* pid, double curr_error, double dt);
void brake_light_control(PID* accel);
double normalize( double input, double min, double max);

#if defined (__cplusplus)
}
#endif

#endif //BRAKE_CONTROL_H
