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

extern indications turn_lights;
extern indication_mode turn_lights_mode;
extern indication_mode tail_lights_mode;

#define NUM_FRAMES	10
#define NUM_LOOP_FRAMES	7
#define animate() shift( (turn_lights & ( (scroll_flash[animation_index] << 8 ) | scroll_flash[animation_index]) ));
#define animate_loop() shift( (turn_lights & ( (loop_pattern[animation_index] << 8 ) | loop_pattern[animation_index]) ));

void leds_init();
void leds_reset();
void brake_lights(indication_mode mode);
void turn_signal(indications indication, indication_mode mode);
void timer1_init();

// blink patterns
extern uint8_t scroll_pattern[4];
extern uint8_t scroll2_pattern[6];
extern uint8_t flash_pattern[2];
extern uint8_t scroll_flash[NUM_FRAMES];
extern uint8_t loop_pattern[7];


#endif
