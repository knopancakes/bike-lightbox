#include <avr/io.h>
#include <util/delay.h>
#include "leds.h"
#include "74hc595.h"

int main(){

	leds_init();
	shifter_init();

	// Counter used for displaying a number in binary via the shift register
	char counter = 0;

	while(1){
		counter++;
		shift(counter);
		_delay_ms(100);
	}
	return 0;
}
