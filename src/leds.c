#include <avr/io.h>
#include <avr/interrupt.h>
#include <inttypes.h>
#include "74hc595.h"
#include "leds.h"

// blink patterns
uint8_t scroll_pattern[4] = {0b01000001, 0b00100010, 0b00010100, 0b00001000};
uint8_t scroll2_pattern[6] = { 0b01000001, 0b01100011, 0b00110110, 0b00011100, 0b00001000, 0b00000000};
uint8_t flash_pattern[2] = { 0xFF, 0x00 };
uint8_t loop_pattern[7] = { 0b01000000, 0b00100000, 0b00010000, 0b00001000, 0b00000100, 0b00000010, 0b00000001 };

// commands
indications turn_lights;
indication_mode turn_lights_mode;
indication_mode brake_light;

// pwm
uint16_t animation_delay;
const uint16_t animation_period = 300;
uint8_t animation_index;


void leds_init()
{
	shifter_init();
	turn_signal(0, 0);

	HIBEAMS_PORT 	|= (1<<HIBEAMS); 	// set pin high
	HIBEAMS_DDR 	|= (1<<HIBEAMS);	// set output
	//brake_lights(0);

	animation_delay = 0;
	animation_index = 0;
}

void leds_reset(){
	shift(0x0000);
	animation_delay = 0;
	animation_index = 0;
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

//	shift(turn_lights);
}


void timer1_init()
{
	cli();

	TCCR1B |= (1 << CS10) | (1 << WGM12);
	OCR1A = 0x0FFF;
	OCR1B = 0x00FF;
    TCNT1 = 0;
    TIMSK1 = (1 << OCIE1A) | (1 << OCIE1B);

    // disable usb connection interrupt (for now, later we can use this to trigger bootloader)
    USBCON = 0;

    // enable global interrupts
    sei();
}


ISR(TIMER1_COMPB_vect){
//	// PWM
//	if (1) {
//		if(pwm_delay == 0) {
//			HIBEAMS_PORT &= ~(1<<HIBEAMS);
//			HIBEAMS_PORT ^= (1<<HIBEAMS);
//
//		} else if(pwm_delay == pwm_duty_cycle) {
//			HIBEAMS_PORT |= (1<<HIBEAMS);
//		}
//		if(++pwm_delay >= pwm_period) {
//			pwm_delay = 0;
//		}
//	} else {
//		HIBEAMS_PORT |= (1<<HIBEAMS);
//	}

	HIBEAMS_PORT	|= (1<<HIBEAMS);
}

ISR(TIMER1_COMPA_vect)
{
	// Turn Lights
	if (turn_lights_mode == scroll ) {
		if(++animation_delay >= animation_period) {
			shift( (turn_lights & ( (scroll2_pattern[animation_index] << 8 ) | scroll2_pattern[animation_index]) ));
			animation_delay = 0;
			if(++animation_index >= 6) {
				animation_index = 0;
			}

		}
	}
	HIBEAMS_PORT	&= ~(1<<HIBEAMS);
}

// USB general interrupt. it breaks everything -_- disabled above
ISR(_VECTOR(10)) {
	shift(0x0A00);
}


ISR(__vector_default){
	shift(0xFFFF);
}



