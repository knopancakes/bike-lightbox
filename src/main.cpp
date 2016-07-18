/*
 * Insert Description Here
 */

#include <stdio.h>
#include <stdbool.h>
#include <avr/io.h>
#include <util/delay.h>
#include <avr/interrupt.h>

#include "leds.h"
#include "buttons.h"
#include "serial.h"
#include "lsm303dlhc.h"
#include "74hc595.h"
#include "brakecontrol.h"


using namespace std;

int main()
{
  /* declarations */
  bool special_mode;
  indications command, last_command;
  indication_mode pattern;
  unsigned char buffer[32];

  Serial serial;
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
  serial.begin();

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
      special_mode = true;
    } 
  else 
    {
    special_mode = false;
    }

  while(true)
    {
      /* control brake light using mag/accel data */
      brake_light_control(&accel);

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


      if(serial.available()) {
        int size = serial.read(buffer);
        if (size!=0) {
          serial.write((const uint8_t*)buffer, size);
          serial.write('\n');
        }
      }
      serial.poll();


      /* debounce user input */
      last_command = command;
      _delay_ms(3);
      
    }
  
  return 0;
}




