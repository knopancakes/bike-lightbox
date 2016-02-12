
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

// Bits is set to one if a depounced press is detected.
volatile uint8_t buttons_down;
// Return non-zero if a button matching mask is pressed.
uint8_t button_down(uint8_t button_mask)
{
	// ATOMIC_BLOCK is needed if debounce() is called from within an ISR
	ATOMIC_BLOCK(ATOMIC_RESTORESTATE){
		// And with debounced state for a one if they match
		button_mask &= buttons_down;
		// Clear if there was a match
		buttons_down ^= button_mask;
	}
	// Return non-zero if there was a match
	return button_mask;
}
void debounce_init(void)
{
	// Button pins as input
	BUTTON_DDR &= ~(BUTTON_MASK);
	// Enable pullup on buttons
	BUTTON_PORT |= BUTTON_MASK;
}
