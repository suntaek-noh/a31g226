/*
 * i2c_sw.h
 *
 *  Created on: 2022. 11. 9.
 *      Author: sunta
 */

#ifndef SRC_COMMON_HW_INCLUDE_I2C_SW_H_
#define SRC_COMMON_HW_INCLUDE_I2C_SW_H_


#include "hw_def.h"
#include "i2c.h"

#ifdef _USE_HW_I2C_SW


/* functions */
void SW_I2C_initial(i2c_tbl_t *p_i2c_sw);
void i2c_port_initial(i2c_tbl_t *p_i2c_sw);
uint8_t SW_I2C_ReadVal_SDA(i2c_tbl_t *p_i2c_sw);
void SW_I2C_Write_Data(i2c_tbl_t *p_i2c_sw, uint8_t data);
uint8_t SW_I2C_Read_Data(i2c_tbl_t *p_i2c_sw);

bool i2cSwWrite(i2c_tbl_t *p_i2c_sw, uint8_t dev_addr, uint8_t reg_addr, uint8_t *p_data, uint8_t length, uint32_t timeout);
bool i2cSwRead(i2c_tbl_t *p_i2c_sw, uint8_t dev_addr, uint8_t reg_addr, uint8_t *p_data, uint8_t length, uint32_t timeout);

uint8_t SW_I2C_WriteControl_8Bit(i2c_tbl_t *p_i2c_sw, uint8_t IICID, uint8_t regaddr, uint8_t data);
uint8_t SW_I2C_WriteControl_8Bit_OnlyRegAddr(i2c_tbl_t *p_i2c_sw, uint8_t IICID, uint8_t regaddr);
uint8_t SW_I2C_WriteControl_16Bit(i2c_tbl_t *p_i2c_sw, uint8_t IICID, uint8_t regaddr, uint16_t data);
uint8_t SW_I2C_WritenControl_8Bit(i2c_tbl_t *p_i2c_sw, uint8_t IICID, uint8_t regaddr, uint8_t *data, uint8_t rcnt);

uint8_t SW_I2C_ReadControl_8Bit_OnlyRegAddr(i2c_tbl_t *p_i2c_sw, uint8_t IICID, uint8_t regaddr);
uint8_t SW_I2C_ReadControl_8Bit_OnlyData(i2c_tbl_t *p_i2c_sw, uint8_t IICID);
uint16_t SW_I2C_ReadControl_16Bit_OnlyData(i2c_tbl_t *p_i2c_sw, uint8_t IICID);
uint8_t SW_I2C_ReadControl_8Bit(i2c_tbl_t *p_i2c_sw, uint8_t IICID, uint8_t regaddr);
uint16_t SW_I2C_ReadControl_16Bit(i2c_tbl_t *p_i2c_sw, uint8_t IICID, uint8_t regaddr);

uint8_t SW_I2C_ReadnControl_8Bit(i2c_tbl_t *p_i2c_sw, uint8_t IICID, uint8_t regaddr, uint8_t rcnt, uint8_t (*pdata));
uint8_t SW_I2C_Multi_ReadnControl_8Bit(i2c_tbl_t *p_i2c_sw, uint8_t IICID, uint8_t regaddr, uint8_t rcnt, uint8_t (*pdata));
uint8_t SW_I2C_Check_SlaveAddr(i2c_tbl_t *p_i2c_sw, uint8_t IICID);

uint8_t SW_I2C_UTIL_WRITE(i2c_tbl_t *p_i2c_sw, uint8_t IICID, uint8_t regaddr, uint8_t data);
uint8_t SW_I2C_UTIL_Read(i2c_tbl_t *p_i2c_sw, uint8_t IICID, uint8_t regaddr);
uint8_t SW_I2C_UTIL_Read_Multi(i2c_tbl_t *p_i2c_sw, uint8_t IICID, uint8_t regaddr, uint8_t rcnt, uint8_t (*pdata));

#endif

#endif /* SRC_COMMON_HW_INCLUDE_I2C_SW_H_ */
