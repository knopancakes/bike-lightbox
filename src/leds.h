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

typedef enum indications {
	left,
	right,
	both,
	hazard,
	random
};

typedef enum indication_mode {
	flash,
	scroll,
	marquee,
	loop
};

void leds_init();
void indicator_color(uint8_t red, uint8_t green, uint8_t blue);
void turn_signal(enum indications indication, enum indication_mode mode);

#endif
