/***************************************************************************
  This is a library for the LSM303 Accelerometer and magnentometer/compass
  Designed specifically to work with the Adafruit LSM303DLHC Breakout
  These displays use I2C to communicate, 2 pins are required to interface.
  Adafruit invests time and resources providing this open source code,
  please support Adafruit andopen-source hardware by purchasing products
  from Adafruit!
  Written by Kevin Townsend for Adafruit Industries.  
  BSD license, all text above must be included in any redistribution
***************************************************************************/
#include <util/delay.h>
#include "i2c_master.h"
#include "lsm303dlhc.h"
#include "74hc595.h" // for debugging with shift();

/***************************************************************************
 CONSTRUCTOR
***************************************************************************/
bool lsm303_begin()
{
  i2c_init();
  
  int ret = 0;

  // Enable the accelerometer
  ret = lsm303_write8(LSM303_ADDRESS_ACCEL, LSM303_REGISTER_ACCEL_CTRL_REG1_A, 0x27);
  
  // Enable the magnetometer
  //ret = lsm303_write8(LSM303_ADDRESS_MAG, LSM303_REGISTER_MAG_MR_REG_M, 0x00);
  
  if(ret != 0)
    {
      return true;
    }

  return false;
}

/***************************************************************************
 PUBLIC FUNCTIONS
***************************************************************************/
uint8_t lsm303_read()
{
  byte rx_data[6];

  // Read the accelerometer
  i2c_readReg(LSM303_ADDRESS_ACCEL, LSM303_REGISTER_ACCEL_OUT_X_L_A | 0x80, rx_data, (byte)6);  

  uint8_t xlo = rx_data[0];
  uint8_t xhi = rx_data[1];
  uint8_t ylo = rx_data[2];
  uint8_t yhi = rx_data[3];
  uint8_t zlo = rx_data[4];
  uint8_t zhi = rx_data[5];

  // Shift values to create properly formed integer (low byte first)
  lsm303accelData.x = (xlo | (xhi << 8)) >> 4;
  lsm303accelData.y = (ylo | (yhi << 8)) >> 4;
  lsm303accelData.z = (zlo | (zhi << 8)) >> 4;

  // Read the magnetometer
  i2c_readReg(LSM303_ADDRESS_MAG, LSM303_REGISTER_MAG_OUT_X_H_M, rx_data, (byte)6);  

  // Note high before low (different than accel)
  xhi = rx_data[0];
  xlo = rx_data[1];
  zhi = rx_data[2];
  zlo = rx_data[3];
  yhi = rx_data[4];
  ylo = rx_data[5];

  // Shift values to create properly formed integer (low byte first)
  lsm303magData.x = (xlo | (xhi << 8));
  lsm303magData.y = (ylo | (yhi << 8));
  lsm303magData.z = (zlo | (zhi << 8));

  // ToDo: Calculate orientation
  lsm303magData.orientation = 0.0;

  return 0;
}

void lsm303_setMagGain(lsm303MagGain gain)
{
  lsm303_write8(LSM303_ADDRESS_MAG, LSM303_REGISTER_MAG_CRB_REG_M, (byte)gain);
}

/***************************************************************************
 PRIVATE FUNCTIONS
 ***************************************************************************/
uint8_t lsm303_write8(byte address, byte reg, byte value)
{

  int ret = 0;
  byte buf[1] = {value};

  ret = i2c_writeReg(address, reg, buf, 0x01);

#ifdef DEBUG
  if(ret!=0)
    {
      shift(0xF0F0);
      _delay_ms(2000);
    }
#endif

  return 0;
}

uint8_t lsm303_read8(uint8_t address, uint8_t reg)
{
  byte value = 0;

  i2c_start(address);
  i2c_write(reg);
  i2c_stop();
//  Wire.requestFrom(address, (byte)1);
//  value = Wire.read();
//  Wire.endTransmission();
// todo this isn't done
  return value;
}
