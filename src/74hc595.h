#ifndef _75HC595_H
#define _75HC595_H

#define number_of_74hc595s 2 //How many of the shift registers are there daisey chained?

#define SRCLK_Pin 			PB6
#define RCLK_Pin 			PB5
#define SER_Pin 			PB4
#define SREG_PORT	 		PORTB
#define SREG_DDR			DDRB

void shifter_init();

void shift(unsigned long data);


#endif
