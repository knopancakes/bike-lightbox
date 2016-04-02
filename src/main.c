#include <stdio.h>
#include <avr/io.h>
#include <util/delay.h>
#include <avr/interrupt.h>
#include "leds.h"
#include "uart.h"



int main(){

	leds_init();
	shifter_init();
	uart_init();
	sei();

    stdout = &uart_output;
    stdin  = &uart_input;

    char input;

    while(1) {

//    	led_color(0);
//        shift(0xFFFF);
//        _delay_ms(1000);
//
//        led_color(1);
//        shift(0xFF00);
//        _delay_ms(1000);
//
//        led_color(2);
//        shift(0x00FF);
//        _delay_ms(1000);
//
//        led_color(3);
//        shift(0xF0F0);
//        _delay_ms(1000);
//
//    	led_color(4);
//        shift(0x0F0F);
//        _delay_ms(1000);
//
//    	led_color(5);
//        shift(0x5555);
//        _delay_ms(1000);
//
//        led_color(6);
//        shift(0xAAAA);
//        _delay_ms(1000);
//
//        led_color(7);
//        shift(0x0000);
//        _delay_ms(1000);

    }

	return 0;
}
