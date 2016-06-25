#ifndef  F_CPU
#define F_CPU 16000000UL
#endif

#include <avr/io.h>
#include <util/delay.h>
#include <util/twi.h>

#include "i2c_master.h"
#include "74hc595.h" // for debugging with shift()

#define F_SCL 100000UL // SCL frequency
#define TWBR_val (((F_CPU / F_SCL) - 16 ) / 2)

#ifdef DEBUG
# define RTIMEOUT() {						\
    shift(0xFFFF);						\
    _delay_ms(300);						\
    shift(0x0000);						\
    _delay_ms(300);						\
    shift(0xFFFF);						\
    _delay_ms(300);						\
    shift(0x0000);						\
    _delay_ms(1000);						\
  }

# define WTIMEOUT() {						\
    shift(0x3333);						\
    _delay_ms(300);						\
    shift(0x0000);						\
    _delay_ms(300);						\
    shift(0x3333);						\
    _delay_ms(300);						\
    shift(0x0000);						\
    _delay_ms(1000);						\
  }
#endif //DEBUG

void i2c_init(void)
{
  // Open drain
  
  PORTD &= ~(1 << PORTD0); // SCL
  DDRD |= (1<<PORTD0);
  PORTD &= ~(1 << PORTD1); // SDA
  DDRD |= (1<<PORTD1);
  
  TWBR = (uint8_t)TWBR_val;
  TWCR = (1<<TWEN);

  //_delay_ms(400);
}

uint8_t i2c_start(uint8_t address)
{
  uint8_t   twst;

  // send START condition
  TWCR = (1<<TWINT) | (1<<TWSTA) | (1<<TWEN);

  // wait until transmission completed
  while(!(TWCR & (1<<TWINT)));

  // check value of TWI Status Register. Mask prescaler bits.
  twst = TW_STATUS & 0xF8;
  if ( (twst != TW_START) && (twst != TW_REP_START)) 
    {
#ifdef DEBUG
      shift(0x0110);
      _delay_ms(1000);
#endif
      return 1;
    }
  // send device address
  TWDR = address;
  TWCR = (1<<TWINT) | (1<<TWEN);

  // wail until transmission completed and ACK/NACK has been received
  while(!(TWCR & (1<<TWINT)));

  // check value of TWI Status Register. Mask prescaler bits.
  twst = TW_STATUS & 0xF8;
  if ( (twst != TW_MT_SLA_ACK) && (twst != TW_MR_SLA_ACK) ) 
    {
#ifdef DEBUG
      shift(0x0220);
      _delay_ms(1000);
#endif
      //return 1;
    } 
  return 0;
}

void i2c_start_wait(uint8_t address)
{
  uint8_t twst;
   
  
  while ( 1 )
    {
      // send START condition
      TWCR = (1<<TWINT) | (1<<TWSTA) | (1<<TWEN);
      
      // wait until transmission completed
      while(!(TWCR & (1<<TWINT)));
      
      // check value of TWI Status Register. Mask prescaler bits.
      twst = TW_STATUS & 0xF8;
      if ( (twst != TW_START) && (twst != TW_REP_START)) continue;
      
      // send device address
      TWDR = address;
      TWCR = (1<<TWINT) | (1<<TWEN);
      
      // wail until transmission completed
      while(!(TWCR & (1<<TWINT)));
      
      // check value of TWI Status Register. Mask prescaler bits.
      twst = TW_STATUS & 0xF8;
      if ( (twst == TW_MT_SLA_NACK )||(twst ==TW_MR_DATA_NACK) ) 
	{        
	  /* device busy, send stop condition to terminate write operation */
	  TWCR = (1<<TWINT) | (1<<TWEN) | (1<<TWSTO);
	  
	  shift(0x8888);
	  // wait until stop condition is executed and bus released
	  while(TWCR & (1<<TWSTO));
	  
	  continue;
	}
      //if( twst != TW_MT_SLA_ACK) return 1;
      break;
    }
}


uint8_t i2c_write(uint8_t data)
{
  int i = 0;
  // load data into data register
  TWDR = data;
  // start transmission of data
  TWCR = (1<<TWINT) | (1<<TWEN);
  // wait for end of transmission
  while( !(TWCR & (1<<TWINT)) ) ;
  /*    {
      if(i++>30000)
	{
#ifdef DEBUG
	  WTIMEOUT();
#endif
	  return 1;
	}
    }
  */
  if( (TWSR & 0xF8) != TW_MT_DATA_ACK )
    { 
      return 1; 
    }
  
  return 0;
}

uint8_t i2c_read_ack(void)
{
  
  // sOAtart TWI module and acknowledge data after reception
  TWCR = (1<<TWINT) | (1<<TWEN) | (1<<TWEA); 

  int i = 0;
  // wait for end of transmission
  while( !(TWCR & (1<<TWINT)) )
    {
      if(i++>30000)
	{
#ifdef DEBUG
	  RTIMEOUT();
#endif
	  return 1;
	}
    }
  // return received data from TWDR
  return TWDR;
}

uint8_t i2c_read_nack(void)
{
  
  // start receiving without acknowledging reception
  TWCR = (1<<TWINT) | (1<<TWEN);
  int i = 0;
  // wait for end of transmission
  while( !(TWCR & (1<<TWINT)) )
    {
      if(i++ > 30000)
	{
#ifdef DEBUG
	  RTIMEOUT();
#endif
	  return 1;
	}
    }
  // return received data from TWDR
  return TWDR;
}

uint8_t i2c_transmit(uint8_t address, uint8_t* data, uint16_t length)
{
  if (i2c_start(address | I2C_WRITE)) return 1;
  
  uint16_t i;
  for (i = 0; i < length; i++)
    {
      if (i2c_write(data[i])) return 1;
    }
  
  i2c_stop();
  
  return 0;
}

uint8_t i2c_receive(uint8_t address, uint8_t* data, uint16_t length)
{
  if (i2c_start(address | I2C_READ)) return 1;
  
  uint16_t i;
  for (i = 0; i < (length-1); i++)
    {
      data[i] = i2c_read_ack();
    }
  data[(length-1)] = i2c_read_nack();
  
  i2c_stop();
  
  return 0;
}

uint8_t i2c_writeReg(uint8_t devaddr, uint8_t regaddr, uint8_t* data, uint16_t length)
{
  if (i2c_start(devaddr | 0x00)) return 1;

  i2c_write(regaddr);

  uint16_t i;
  for (i = 0; i < length; i++)
    {
      if (i2c_write(data[i])) return 1;
    }

  i2c_stop();

  return 0;
}

uint8_t i2c_readReg(uint8_t devaddr, uint8_t regaddr, uint8_t* data, uint16_t length)
{
  if (i2c_start(devaddr)) return 1;

  i2c_write(regaddr);

  if (i2c_start(devaddr | 0x01)) return 1;

  uint16_t i;
  for (i = 0; i < (length-1); i++)
    {
      data[i] = i2c_read_ack();
    }
  data[(length-1)] = i2c_read_nack();

  i2c_stop();

  return 0;
}

void i2c_stop(void)
{
  // transmit STOP condition
  TWCR = (1<<TWINT) | (1<<TWEN) | (1<<TWSTO);
}
