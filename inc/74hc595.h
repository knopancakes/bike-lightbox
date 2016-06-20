#ifndef _75HC595_H
#define _75HC595_H

#define number_of_74hc595s 2 //How many of the shift registers are there daisey chained?

#define SRCLK_Pin 			PF6
#define RCLK_Pin 			PF5
#define SER_Pin 			PF4
#define SREG_PORT	 		PORTF
#define SREG_DDR			DDRF


void shifter_init();

void shift(unsigned long data);


#endif
