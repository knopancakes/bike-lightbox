#include <stdint.h>
#include <avr/interrupt.h>
#include <avr/io.h>
#include "buttons.h"
#include "leds.h"

void buttons_init()
{
	DDRD &= ~(1<<LEFT_TURN_SW);
	DDRE &= ~(1<<RIGHT_TURN_SW);

	/* enable pull up resistors. switches are pull down */
	LEFT_TURN_PORT |= (1<<LEFT_TURN_SW);
	RIGHT_TURN_PORT |= (1<<RIGHT_TURN_SW);
}


int get_signal_switch_status(void)
{
  int left = 0;
  int right = 0;
  int ret = 0;
  int i;
 
  /* take a few samples in case the input is noisy */
  for(i=0; i<20; i++) {
    if( (RIGHT_TURN_PIN & (1<< RIGHT_TURN_SW)) == 0x00 ) {
      right++;
    }
    if( (LEFT_TURN_PIN & (1<<LEFT_TURN_SW)) == 0x00 ) {
      left++;
    }
  }

  /* process samples */
  if( (left > right) && (left > 10) ) 
    {
      ret = ind_left;
    }
  else if( (right > left) && (right > 10))
    {
      ret = ind_right;
    } 
  else {
    ret = ind_off;
  }  
  
  return ret;
    
}
