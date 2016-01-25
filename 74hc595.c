#include <avr/io.h>
#include "74hc595.h"


void shift(int SRCLK_Pin, int RCLK_Pin, int SER_Pin, unsigned long data){

  PORTB &= ~(1 << RCLK_Pin); // Set the register-clock pin low

  int i;

  for (i = 0; i < (8 * number_of_74hc595s); i++){// Now we are entering the loop to shift out 8+ bits
    // Set the serial-clock pin low
    PORTB &= ~(1 << SRCLK_Pin); 
    // Go through each bit of data and output it
    PORTB |= (((data&(0x01<<i))>>i) << SER_Pin );
    // Set the serial-clock pin high
    PORTB |= (1 << SRCLK_Pin);
    // Set the datapin low again
    PORTB &= ~(((data&(0x01<<i))>>i) << SER_Pin );
  }

  // Set the register-clock pin high to update the output of the shift-register
  PORTB |= (1 << RCLK_Pin);
}
