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

int main()
{
  /* declarations */
  bool special_mode;
  indications command, last_command;
  indication_mode pattern;
  //char input;

  /* initialize hardware */
  leds_init();
  buttons_init();
  timer1_init();

  /* initialize and read the lsm303 */

  lsm303_begin();
  //lsm303_read();
  
  /* setup usb serial port */
  //stdout = &uart_output;
  //stdin  = &uart_input;

  /* initialize pattern settings */
  last_command = ind_off;
  command = ind_off;
  pattern = off;
  
  /* set light/indication mode to PWM driven */
  //brake_lights(pwm);
  brake_lights(off);
  
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
      _delay_ms(200);
      
    }
  
  return 0;
}
