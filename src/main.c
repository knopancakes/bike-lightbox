
#include <stdio.h>
#include <stdbool.h>
#include <avr/io.h>
#include <util/delay.h>
#include <avr/interrupt.h>
#include "leds.h"
#include "uart.h"
#include "buttons.h"
#include "lsm303dlhc.h"

int main(){

  /* declarations */
  bool special_mode = 0;
  indications command, last_command;
  indication_mode pattern;
  //char input;

  /* initialize hardware */
  leds_init();
  buttons_init();
  timer1_init();

  /* initialize and read the lsm303 */
  lsm303_begin();
  lsm303_read();
  
  /* setup usb serial port */
  //stdout = &uart_output;
  //stdin  = &uart_input;

  /* initialize pattern settings */
  last_command = ind_off;
  command = ind_off;
  pattern = loop;
  
  /* set light/indication mode to PWM driven */
  brake_lights(pwm);
  
  /* reset leds and animation driver */
  turn_signal(command, pattern);
  leds_reset();

  /* check to see if switch is depressed on startup */
  _delay_ms(100);
  command = get_signal_switch_status();
  if(command != ind_off) 
    {
      special_mode = true;
    }

  while(true)
    {
      /* check to see if the user input has changed state */
      last_command = command;
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
	  turn_signal(command, pattern);
	  leds_reset();
    	}

      /* debounce user input */
      _delay_ms(100);
      
    }
  
  return 0;
}
