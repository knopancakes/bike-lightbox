#include <avr/io.h>
#include "74hc595.h"
#include "leds.h"

ISR(TIMER1_OVF_vect)
{
	// PWM

	// blinky delay
	// shift(rear_lights_active);
}

void leds_init()
{

	HIBEAMS_PORT 	= 0x00;
	HIBEAMS_DDR	 	= (1<<HIBEAMS);

	LED_PORT |=  _BV(BTN_LED);
	LED_DDR = (_BV(RGB_LED_R) | _BV(RGB_LED_G) | _BV(RGB_LED_B) | _BV(BTN_LED));
}

void indicator_color(uint8_t red, uint8_t green, uint8_t blue)
{
//	uint8_t color_out = LED_PORT;
//	color_out &= !(_BV(RGB_LED_R) | _BV(RGB_LED_G) | _BV(RGB_LED_B) );
//	if( 0x01 & (color >> 2)) {
//		color_out |= _BV(RGB_LED_R);
//	}
//	if( 0x01 & (color >> 1)) {
//		color_out |= _BV(RGB_LED_G);
//	}
//	if( 0x01 & (color >> 0)) {
//		color_out |= _BV(RGB_LED_B);
//	}
//	LED_PORT = color_out;
}

void turn_signal(enum indications indication)
{
	// todo account for brake lights active or no
	switch(indication) {
	case off:
		rear_lights_active = 0x0000;
		rear_lights_mode = flash;
		break;
	case left_flash:
		rear_lights = 0xFF00;
		break;
	case right_flash:
		rear_lights = 0x00FF;
		break;
	case both_flash:
		rear_lights = 0xFFFF;
		break;
	case left_scroll:

		break;
	case right_scroll:

		break;
	case both_scroll:

		break;
	case circular_scroll:

		break;
	case random:

		break;
	default:
		break;
	}
}
