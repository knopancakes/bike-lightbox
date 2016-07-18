#ifndef _BUTTONS_H
#define _BUTTONS_H

#include "leds.h"

#define LEFT_TURN_PORT	PORTD
#define LEFT_TURN_SW	PD7
#define LEFT_TURN_PIN	PIND

#define RIGHT_TURN_PORT	PORTE
#define RIGHT_TURN_SW	PE6
#define RIGHT_TURN_PIN	PINE

#if defined (__cplusplus)
extern "C" {
#endif

indications get_signal_switch_status(void);
void buttons_init(void);

#if defined (__cplusplus)
}
#endif

#endif
