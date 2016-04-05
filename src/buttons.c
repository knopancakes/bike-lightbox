#include <stdint.h>
#include <avr/interrupt.h>
#include <avr/io.h>
#include "buttons.h"
#include "leds.h"

void buttons_init()
{
	// switch inputs
	DDRF &= ~((1<<TURN_SW_LEFT) | (1<<TURN_SW_RIGHT));

	// pull up
	TURN_SW_PORT |= ((1<<TURN_SW_LEFT) + (1<<TURN_SW_RIGHT));


}


int get_signal_switch_status(void)
{
	if( (TURN_SW & (1<< TURN_SW_LEFT)) == 0x00 ) {
		return ind_left;
	}
	else if( (TURN_SW & (1<<TURN_SW_RIGHT)) == 0x00 ) {
		return ind_right;
	}
	else {
		return ind_off;
	}


}
