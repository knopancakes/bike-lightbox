#ifndef _BUTTONS_H
#define _BUTTONS_H

#define LEFT_TURN_PORT	PORTD
#define LEFT_TURN_SW	PD7
#define LEFT_TURN_PIN	PIND

#define RIGHT_TURN_PORT	PORTE
#define RIGHT_TURN_SW	PE6
#define RIGHT_TURN_PIN	PINE

int get_signal_switch_status(void);
void buttons_init(void);

#endif
