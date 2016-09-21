/*
 * Insert Description Here
 */

#include <stdio.h>
#include <stdbool.h>
#include <avr/io.h>
#include <util/delay.h>
#include <avr/interrupt.h>
#include "leds.h"
#include "uart.h"
#include "buttons.h"
#include "lsm303dlhc.h"

#ifdef DEBUG
#include "74hc595.h"
#endif

#define BRAKING_THRESHOLD  	 -900
#define ACCEL_THRESHOLD		 900

typedef struct {
  double windup_guard;
  double proportional_gain;
  double integral_gain;
  double derivative_gain;
  double prev_error;
  double int_error;
  double control;
} PID;
 
void pid_zeroize(PID* pid) {
  // set prev and integrated error to zero
  pid->prev_error = 0;
  pid->int_error = 0;
}
 
void pid_update(PID* pid, double curr_error, double dt) {
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

int main()
{
  /* declarations */
  bool special_mode;
  indications command, last_command;
  indication_mode pattern;
  //char input;
  PID accel;

  /* initialize hardware */
  leds_init();
  buttons_init();
  timer1_init();

  /* setup control loop for brake light */
  pid_zeroize(&accel);
  accel.proportional_gain = 1.0f;
  accel.integral_gain = 0.0f;
  accel.derivative_gain = 0.0f;

  /* initialize and read the lsm303 */
  lsm303_begin();
  lsm303_read();
  
  /* setup usb serial port */
  //stdout = &uart_output;
  //stdin  = &uart_input;

  /* initialize pattern settings */
  last_command = ind_off;
  command = ind_off;
  pattern = off;
  
  /* set light/indication mode to PWM driven */
  brake_lights(pwm);
  //brake_lights(off);
  
  /* reset leds and animation driver */
  turn_signal(command, pattern);
  leds_reset();

  /* check to see if switch is depressed on startup */
  command = get_signal_switch_status();
  if(command != ind_off) 
    {
      //special_mode = true;
      special_mode = false;
    } 
  else 
    {
    special_mode = false;
    }

  while(true)
    {
      /* update accel/mag data */
      lsm303_read();
      
      /* update brake lights based on accel data */
      // +- 17000
      uint16_t _intensity = OCR1B;
      uint16_t intensity = 0;
      //uint8_t ebb_direction = 0;
      pid_update(&accel, (double)lsm303accelData.z, 1);
 
      if( (int)accel.control < BRAKING_THRESHOLD )
	{
	  intensity = 0x04FF;

	  /* direct, hard output */
	  OCR1B = intensity;
	}
      else {
	if( (int)accel.control > ACCEL_THRESHOLD )
	  {	
	    intensity = 0x01CF;
	  }
	else 
	  {
	    intensity = 0x004F;
	  }
	/* reset from braking */
	if( _intensity > 0x01CF){
	  OCR1B = 0x004F;
	}
	/* smooth transition */
	else if( _intensity < intensity ) {
	  OCR1B = _intensity + 1;
	}
	else if( _intensity > intensity ) {
	  OCR1B = _intensity - 1;
	}
      }

      /* check to see if the user input has changed state */
      command = get_signal_switch_status();

      /* if "special mode" is enabled, play a fancy animation on idle */
      if(special_mode && (command == ind_off)) 
	{
	  /* animation selection */
	  command = ind_hazard;
	  pattern = loop;
	} 
      else 
	{
	  /* default pattern, no command override */
	  pattern = scroll;
	}
      
      /* if the user input has changed, update the pattern driver state */
      if (command != last_command) 
	{
	  leds_reset();
	  turn_signal(command, pattern);
    	}

      /* debounce user input */
      last_command = command;
      
    }
  
  return 0;
}

