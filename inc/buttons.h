#ifndef _BUTTONS_H
#define _BUTTONS_H

#define TURN_SW_PORT	PORTD
#define TURN_SW_LEFT	PD1
#define TURN_SW_RIGHT	PD0
#define TURN_SW			PIND

int get_signal_switch_status(void);
void buttons_init(void);

#endif
