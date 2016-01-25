#include <avr/io.h>
#include <util/delay.h>

#include "74hc595.h"

#define SERCLK PB6
#define RCLK PB5
#define SER PB4

#define sleep(sec) _delay_ms(sec * 1000)

int main(){

  DDRB = 0xFF;
  PORTB = 0x00;

  // Counter used for displaying a number in binary via the shift register
  char counter = 0;

  while(1){
    counter++; 
    shift(SERCLK, RCLK, SER, counter);
    
    shift(SERCLK, RCLK, SER, 0x00);
    
  }
  return 0;
}
