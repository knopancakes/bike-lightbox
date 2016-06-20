#include <stdio.h>
#include <avr/io.h>
#include <util/delay.h>
#include <avr/interrupt.h>
#include "leds.h"
#include "uart.h"
#include "buttons.h"

int main(){

	leds_init();
	buttons_init();
	timer1_init();
	//stdout = &uart_output;
    //stdin  = &uart_input;

	int special_mode = 0;
	indications command, last_command;
	indication_mode pattern;
    //char input;

	brake_lights(pwm);


	_delay_ms(1000);


	last_command = -1;
	command = get_signal_switch_status();

	if(command != ind_off) {
	  special_mode = 1;
	}


    while(1) {

////    	brake_lights(pwm);
//    	turn_signal(ind_left,flash);
//    	_delay_ms(4000);
//
//    	brake_lights(off);
//		turn_signal(ind_right,flash);
//    	_delay_ms(4000);


    	last_command = command;
    	command = get_signal_switch_status();
	if(special_mode && command == ind_off) {
	  command = ind_hazard;
	  pattern = loop;
	} else {
	  pattern = scroll;
	}


    	if (command != last_command) {
    		leds_reset();
    		turn_signal(command, pattern);
    	}


       	_delay_ms(100);


    }

	return 0;
}
