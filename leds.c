#include <avr/io.h>
#include "leds.h"

void leds_init()
{

	HIBEAMS_PORT 	= 0x00;
	HIBEAMS_DDR	 	= (1<<HIBEAMS);

	LED_PORT |=  _BV(BTN_LED);
	LED_DDR = (_BV(RGB_LED_R) | _BV(RGB_LED_G) | _BV(RGB_LED_B) | _BV(BTN_LED));
}

void led_color(uint8_t color)
{

	// todo add pwm for more colors / brightness

/*	int i;
	for(i=0; i<3; i++) {
		rgb_led[i](color >> i) & 0x01;
	}
*/
	uint8_t color_out = LED_PORT;

	color_out &= !(_BV(RGB_LED_R) | _BV(RGB_LED_G) | _BV(RGB_LED_B) );

	if( 0x01 & (color >> 2)) {
		color_out |= _BV(RGB_LED_R);
	}

	if( 0x01 & (color >> 1)) {
		color_out |= _BV(RGB_LED_G);
	}

	if( 0x01 & (color >> 0)) {
		color_out |= _BV(RGB_LED_B);
	}

	LED_PORT = color_out;
}
