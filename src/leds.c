#include <avr/io.h>
#include "74hc595.h"
#include "leds.h"

indications turn_lights;
indication_mode turn_lights_mode;
indication_mode brake_light;

ISR(TIMER1_OVF_vect)
{
	// PWM

	// blinky delay
	//shift(rear_lights_active);
}

void leds_init()
{
	shifter_init();
	turn_signal(0, 0);

	HIBEAMS_PORT 	|= (1<<HIBEAMS); 	// set pin high
	HIBEAMS_DDR 	|= (1<<HIBEAMS);	// set output
	//brake_lights(0);

}


void brake_lights(indication_mode mode)
{
	switch(mode){
	case pwm:
		HIBEAMS_PORT 	&= ~(1<<HIBEAMS);
		break;
	case flash:
		HIBEAMS_PORT 	&= ~(1<<HIBEAMS);
		break;
	default:
		HIBEAMS_PORT	|= (1<<HIBEAMS);
		break;
	}
}

void turn_signal(indications indication, indication_mode mode)
{
	switch(indication) {
	case ind_left:
		turn_lights = 0xFF00;
		break;
	case ind_right:
		turn_lights = 0x00FF;
		break;
	case ind_hazard:
		turn_lights = 0xFFFF;
		break;
	default:
		turn_lights = 0x0000;
		break;
	}

	switch(mode) {
	case scroll:
		turn_lights_mode = scroll;
		break;
	case flash:
		turn_lights_mode = flash;
		break;
	case loop:
		turn_lights_mode = loop;
		break;
	case random:
		turn_lights_mode = random;
		break;
	default:
		turn_lights_mode = off;
		break;
	}

	shift(turn_lights);
}
