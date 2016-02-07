#ifndef _75HC595_H
#define _75HC595_H

#define number_of_74hc595s 2 //How many of the shift registers are there daisey chained?


void shifter_init();

void shift(unsigned long data);


#endif
