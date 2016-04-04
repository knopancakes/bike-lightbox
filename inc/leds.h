#ifndef _LEDS_H
#define _LEDS_H

#define HIBEAMS_PORT	PORTB
#define HIBEAMS_DDR		DDRB
#define HIBEAMS			PB6

typedef enum {
	ind_off = 0,
	ind_left,
	ind_right,
	ind_hazard
} indications ;

typedef enum {
	off = 0,
	flash,
	scroll,
	loop,
	random,
	pwm
} indication_mode ;

extern indications rear_lights;
extern indication_mode rear_lights_mode;

void leds_init();
void brake_lights(indication_mode mode);
void turn_signal(indications indication, indication_mode mode);

#endif
