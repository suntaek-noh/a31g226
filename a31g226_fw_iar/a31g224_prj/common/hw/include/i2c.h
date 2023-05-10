/*
 * i2c.h
 *
 *  Created on: 2020. 12. 30.
 *      Author: baram
 */

#ifndef SRC_COMMON_HW_INCLUDE_I2C_H_
#define SRC_COMMON_HW_INCLUDE_I2C_H_


#include "hw_def.h"

#ifdef _USE_HW_I2C
#define I2C_MAX_CH       HW_I2C_MAX_CH
#define I2C_SW_MAX_CH    HW_I2C_SW_MAX_CH


typedef enum
{
  I2C_FREQ_100KHz,
  I2C_FREQ_200KHz,
  I2C_FREQ_300KHz,
  I2C_FREQ_400KHz,
} i2c_freq_t;

typedef struct
{
  PORT_Type   *scl_port;
  uint32_t     scl_pin;

  PORT_Type   *sda_port;
  uint32_t     sda_pin;  

}i2c_sw_t;


typedef struct
{
  I2C_Type    *p_hi2c;

  PORT_Type   *scl_port;
  uint32_t     scl_pin;

  PORT_Type   *sda_port;
  uint32_t     sda_pin;

  I2C_M_SETUP_Type m_setup;
  
} i2c_tbl_t;

bool i2cInit(void);
bool i2cIsInit(void);
bool i2cOpen(uint8_t ch, uint32_t freq_khz);
bool i2cIsBegin(uint8_t ch);
void i2cReset(uint8_t ch);
bool i2cIsDeviceReady(uint8_t ch, uint8_t dev_addr);
bool i2cRecovery(uint8_t ch);
bool i2cReadByte (uint8_t ch, uint16_t dev_addr, uint16_t reg_addr, uint8_t *p_data, uint32_t timeout);
bool i2cReadBytes(uint8_t ch, uint16_t dev_addr, uint16_t reg_addr, uint8_t *p_data, uint32_t length, uint32_t timeout);

bool i2cWriteByte (uint8_t ch, uint16_t dev_addr, uint16_t reg_addr, uint8_t data, uint32_t timeout);
bool i2cWriteBytes(uint8_t ch, uint16_t dev_addr, uint16_t reg_addr, uint8_t *p_data, uint32_t length, uint32_t timeout);


bool i2cReadData(uint8_t ch, uint16_t dev_addr, uint8_t *p_data, uint32_t length, uint32_t timeout);
bool i2cWriteData(uint8_t ch, uint16_t dev_addr, uint8_t *p_data, uint32_t length, uint32_t timeout);


void     i2cSetTimeout(uint8_t ch, uint32_t timeout);
uint32_t i2cGetTimeout(uint8_t ch);

void     i2cClearErrCount(uint8_t ch);
uint32_t i2cGetErrCount(uint8_t ch);


#endif


#endif /* SRC_COMMON_HW_INCLUDE_I2C_H_ */
