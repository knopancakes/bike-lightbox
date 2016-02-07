#include <avr/io.h>
#include "leds.h"

#define LED_PORT		PORTF
#define LED_DDR			DDRF
#define RGB_LED_R		PF6
#define RGB_LED_G		PF5
#define RGB_LED_B		PF4
#define	BTN_LED			PF7


void leds_init()
{
	LED_PORT |= (RGB_LED_R | RGB_LED_G | RGB_LED_B | BTN_LED);
}
