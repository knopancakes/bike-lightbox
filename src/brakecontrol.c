#include <avr/io.h>
#include <util/delay.h>
#include <avr/interrupt.h>

#include "lsm303dlhc.h"
#include "74hc595.h"
#include "brakecontrol.h"
#include "leds.h"

void pid_zeroize(PID* pid)
{
  // set prev and integrated error to zero
  pid->prev_error = 0;
  pid->int_error = 0;
}

void pid_update(PID* pid, double curr_error, double dt)
{
  double diff;
  double p_term;
  double i_term;
  double d_term;

  // integration with windup guarding
  pid->int_error += (curr_error * dt);
  if (pid->int_error < -(pid->windup_guard))
    pid->int_error = -(pid->windup_guard);
  else if (pid->int_error > pid->windup_guard)
    pid->int_error = pid->windup_guard;

  // differentiation
  diff = ((curr_error - pid->prev_error) / dt);

  // scaling
  p_term = (pid->proportional_gain * curr_error);
  i_term = (pid->integral_gain     * pid->int_error);
  d_term = (pid->derivative_gain   * diff);

  // summation of terms
  pid->control = p_term + i_term + d_term;

  // save current error as previous error for next iteration
  pid->prev_error = curr_error;
}

void brake_light_control(PID* accel)
{

  /* update accel/mag data */
  lsm303_read();

  /* update brake lights based on accel data */
  // +- 17000
  uint16_t intensity = 0;
  pid_update(accel, normalize(lsm303accelData.z, -1.0f, 1.0f), 1);

  intensity = OCR1B;

  if( (accel->control > BRAKING_THRESHOLD))
    {
      intensity = PWM_MIN;
    }
  else if( (accel->control < ACCEL_THRESHOLD))
    {
      intensity = PWM_MIN;
    }
  else
    {
      /* return to idle */
      if( intensity > PWM_IDLE )
	{
	  intensity--;
	}
      else if (intensity < PWM_IDLE )
	{
	  intensity++;
	}
    }


  /*
  if( (accel->control > BRAKING_THRESHOLD) && (intensity < PWM_MAX))
    {
      intensity++;
    }
  else if( (accel->control < ACCEL_THRESHOLD) && (intensity > PWM_MIN))
    {
      intensity--;
    }
  else
    {
      if( intensity > PWM_IDLE )
	{
	  intensity--;
	}
      else if (intensity < PWM_IDLE )
	{
	  intensity++;
	}
    }
  */

  OCR1B = intensity;
}

double normalize( double input, double min, double max)
{
  return ((input-min)/(max - min));
}
