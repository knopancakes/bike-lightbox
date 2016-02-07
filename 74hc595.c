#include <avr/io.h>
#include "74hc595.h"

#define SRCLK_Pin 			PB6
#define RCLK_Pin 			PB5
#define SER_Pin 			PB4
#define SREG_PORT	 		PORTB
#define SREG_DDR			DDRB

void shifter_init()
{
	  SREG_DDR 	= 0x00;
	  SREG_PORT = (SRCLK_Pin | RCLK_Pin  | SER_Pin);
}

void shift(unsigned long data)
{

	SREG_PORT &= ~(1 << RCLK_Pin); // Set the register-clock pin low

	int i;

	for (i = 0; i < (8 * number_of_74hc595s); i++){// Now we are entering the loop to shift out 8+ bits
		// Set the serial-clock pin low
		SREG_PORT &= ~(1 << SRCLK_Pin);
		// Go through each bit of data and output it
		SREG_PORT |= (((data&(0x01<<i))>>i) << SER_Pin );
		// Set the serial-clock pin high
		SREG_PORT |= (1 << SRCLK_Pin);
		// Set the datapin low again
		SREG_PORT &= ~(((data&(0x01<<i))>>i) << SER_Pin );
	}

	// Set the register-clock pin high to update the output of the shift-register
	SREG_PORT |= (1 << RCLK_Pin);
}
