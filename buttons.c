
#include <avr/interrupt.h>

// Called at about 100Hz (122Hz)
ISR(TIMER0_OVF_vect)
{
// Debounce buttons. debounce() is declared static inline
// in debounce.h so we will not suffer from the added overhead
// of a (external) function call
debounce();
}

void buttons_init()
{
	// BORTB output
	DDRB = 0xFF;
	// High turns off LED’s on STK500
	PORTB = 0xFF;
	// Timer0 normal mode, presc 1:256
	TCCR0B = 1<<CS02;
	// Overflow interrupt. (at 8e6/256/256 = 122 Hz)
	TIMSK0 = 1<<TOIE0;

}
