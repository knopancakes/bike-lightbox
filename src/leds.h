#ifndef _LEDS_H
#define _LEDS_H

#define LED_PORT		PORTF
#define LED_DDR			DDRF
#define RGB_LED_R		PF6
#define RGB_LED_G		PF5
#define RGB_LED_B		PF4
#define	BTN_LED			PF7

#define HIBEAMS_PORT	PORTD
#define HIBEAMS_DDR		DDRD
#define HIBEAMS			PD6

void leds_init();
void led_color(uint8_t color);

#endif
