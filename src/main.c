#include <stdio.h>
#include <avr/io.h>
#include <util/delay.h>
#include <avr/interrupt.h>
#include "leds.h"
#include "uart.h"



int main(){

	leds_init();
	//uart_init();
	//sei();

	//stdout = &uart_output;
    //stdin  = &uart_input;

    //char input;

    while(1) {

    	brake_lights(pwm);
    	turn_signal(ind_left,flash);
    	_delay_ms(4000);

    	brake_lights(off);
		turn_signal(ind_right,flash);
    	_delay_ms(4000);

    }

	return 0;
}
