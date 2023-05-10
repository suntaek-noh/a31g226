/*
 * i2c_sw.c
 *
 *  Created on: 2022. 11. 9.
 *      Author: sunta
 */

#include "i2c_sw.h"

#ifdef _USE_HW_I2C_SW

//#define SW_I2C_WAIT_TIME  100 //100Khz(11.4us)
//#define SW_I2C_WAIT_TIME  26 //100Khz(11.4us)
//#define SW_I2C_WAIT_TIME  25 //(11.0us)
//#define SW_I2C_WAIT_TIME  23 //(10.4us)
//#define SW_I2C_WAIT_TIME    22 //100Khz(10.0us) 100Khz == 10us
#define SW_I2C_WAIT_TIME  10 //195Khz
//#define SW_I2C_WAIT_TIME  9 //205Khz 200Khz == 5us
//#define SW_I2C_WAIT_TIME  8 //237Khz
//#define SW_I2C_WAIT_TIME  7 //240Khz 250Khz == 4us
//#define SW_I2C_WAIT_TIME  6 //275Khz
//#define SW_I2C_WAIT_TIME  5 //305Khz
//#define SW_I2C_WAIT_TIME  4 //350Khz(3.84us)
//#define SW_I2C_WAIT_TIME  3 //400Khz(3.44us)
//#define SW_I2C_WAIT_TIME  2 //425Khz(3.04us) 333Khz == 3us
//#define SW_I2C_WAIT_TIME  1 //425Khz(2.64us) 400Khz == 2.5us

#define I2C_READ      0x01
#define READ_CMD      1
#define WRITE_CMD     0

void TIMER__Wait_us(__IO uint32_t nCount)
{
  for (; nCount != 0;nCount--);
} /* internal static functions */


void SW_I2C_initial(i2c_tbl_t *p_i2c_sw)
{
  HAL_GPIO_ConfigOutput(p_i2c_sw->scl_port, p_i2c_sw->scl_pin, PCU_MODE_PUSH_PULL);
  HAL_GPIO_ConfigPullup(p_i2c_sw->scl_port, p_i2c_sw->scl_pin, PCU_PUPD_DISABLE);

  HAL_GPIO_ConfigOutput(p_i2c_sw->sda_port, p_i2c_sw->sda_pin, PCU_MODE_PUSH_PULL);
  HAL_GPIO_ConfigPullup(p_i2c_sw->sda_port, p_i2c_sw->sda_pin, PCU_PUPD_DISABLE);
}

// SDA High
void sda_high(i2c_tbl_t *p_i2c_sw)
{
  HAL_GPIO_SetPin(p_i2c_sw->sda_port, (1 << p_i2c_sw->sda_pin));
}

// SDA low
void sda_low(i2c_tbl_t *p_i2c_sw)
{
  HAL_GPIO_ClearPin(p_i2c_sw->sda_port, (1 << p_i2c_sw->sda_pin));
}

// SCL High
void scl_high(i2c_tbl_t *p_i2c_sw)
{
  HAL_GPIO_SetPin(p_i2c_sw->scl_port, (1 << p_i2c_sw->scl_pin));
}

// SCL low
void scl_low(i2c_tbl_t *p_i2c_sw)
{
  HAL_GPIO_ClearPin(p_i2c_sw->scl_port, (1 << p_i2c_sw->scl_pin));
}

void sda_out(i2c_tbl_t *p_i2c_sw, uint8_t out)
{
  if (out)
    sda_high(p_i2c_sw);
  else
    sda_low(p_i2c_sw);
}

void sda_in_mode(i2c_tbl_t *p_i2c_sw)
{
  HAL_GPIO_ConfigOutput(p_i2c_sw->sda_port, p_i2c_sw->sda_pin, PCU_MODE_INPUT);
  HAL_GPIO_ConfigPullup(p_i2c_sw->sda_port, p_i2c_sw->sda_pin, PCU_PUPD_DISABLE);
}

void sda_out_mode(i2c_tbl_t *p_i2c_sw)
{
  HAL_GPIO_ConfigOutput(p_i2c_sw->sda_port, p_i2c_sw->sda_pin, PCU_MODE_PUSH_PULL);
  HAL_GPIO_ConfigPullup(p_i2c_sw->sda_port, p_i2c_sw->sda_pin, PCU_PUPD_DISABLE);
}

#if 0
void scl_in_mode(uint8_t sel)
{
  GPIO_InitTypeDef GPIO_InitStructure;

  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU; //IPD->IPU

  if(sel == 1) {
    GPIO_InitStructure.GPIO_Pin = SW_I2C1_SCL_PIN;
    GPIO_Init(SW_I2C1_SCL_GPIO, &GPIO_InitStructure);
  } else if(sel == 2) {
    GPIO_InitStructure.GPIO_Pin = SW_I2C2_SCL_PIN;
    GPIO_Init(SW_I2C2_SCL_GPIO, &GPIO_InitStructure);
  } else if(sel == 3) {
    GPIO_InitStructure.GPIO_Pin = SW_I2C3_SCL_PIN;
    GPIO_Init(SW_I2C3_SCL_GPIO, &GPIO_InitStructure);
  } else if(sel == 4) {
    GPIO_InitStructure.GPIO_Pin = SW_I2C4_SCL_PIN;
    GPIO_Init(SW_I2C4_SCL_GPIO, &GPIO_InitStructure);
  } else if(sel == 5) {
    GPIO_InitStructure.GPIO_Pin = SW_I2C5_SCL_PIN;
    GPIO_Init(SW_I2C5_SCL_GPIO, &GPIO_InitStructure);
  } else if(sel == 6) {
    GPIO_InitStructure.GPIO_Pin = SW_I2C6_SCL_PIN;
    GPIO_Init(SW_I2C6_SCL_GPIO, &GPIO_InitStructure);
  } else if(sel == 7) {
    GPIO_InitStructure.GPIO_Pin = SW_I2C7_SCL_PIN;
    GPIO_Init(SW_I2C7_SCL_GPIO, &GPIO_InitStructure);
  } else if(sel == 8) {
    GPIO_InitStructure.GPIO_Pin = SW_I2C8_SCL_PIN;
    GPIO_Init(SW_I2C8_SCL_GPIO, &GPIO_InitStructure);
  } else if(sel == 9) {
    GPIO_InitStructure.GPIO_Pin = SW_I2C9_SCL_PIN;
    GPIO_Init(SW_I2C9_SCL_GPIO, &GPIO_InitStructure);
  } else if(sel == 10) {
    GPIO_InitStructure.GPIO_Pin = SW_I2C10_SCL_PIN;
    GPIO_Init(SW_I2C10_SCL_GPIO, &GPIO_InitStructure);
  }
}

void scl_out_mode(uint8_t sel)
{
  GPIO_InitTypeDef GPIO_InitStructure;

  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_OD; // error point GPIO_Mode_Out_PP

  if(sel == 1) {
    GPIO_InitStructure.GPIO_Pin = SW_I2C1_SCL_PIN;
    GPIO_Init(SW_I2C1_SCL_GPIO, &GPIO_InitStructure);
  } else if(sel == 2) {
    GPIO_InitStructure.GPIO_Pin = SW_I2C2_SCL_PIN;
    GPIO_Init(SW_I2C2_SCL_GPIO, &GPIO_InitStructure);
  } else if(sel == 3) {
    GPIO_InitStructure.GPIO_Pin = SW_I2C3_SCL_PIN;
    GPIO_Init(SW_I2C3_SCL_GPIO, &GPIO_InitStructure);
  } else if(sel == 4) {
    GPIO_InitStructure.GPIO_Pin = SW_I2C4_SCL_PIN;
    GPIO_Init(SW_I2C4_SCL_GPIO, &GPIO_InitStructure);
  } else if(sel == 5) {
    GPIO_InitStructure.GPIO_Pin = SW_I2C5_SCL_PIN;
    GPIO_Init(SW_I2C5_SCL_GPIO, &GPIO_InitStructure);
  } else if(sel == 6) {
    GPIO_InitStructure.GPIO_Pin = SW_I2C6_SCL_PIN;
    GPIO_Init(SW_I2C6_SCL_GPIO, &GPIO_InitStructure);
  } else if(sel == 7) {
    GPIO_InitStructure.GPIO_Pin = SW_I2C7_SCL_PIN;
    GPIO_Init(SW_I2C7_SCL_GPIO, &GPIO_InitStructure);
  } else if(sel == 8) {
    GPIO_InitStructure.GPIO_Pin = SW_I2C8_SCL_PIN;
    GPIO_Init(SW_I2C8_SCL_GPIO, &GPIO_InitStructure);
  } else if(sel == 9) {
    GPIO_InitStructure.GPIO_Pin = SW_I2C9_SCL_PIN;
    GPIO_Init(SW_I2C9_SCL_GPIO, &GPIO_InitStructure);
  } else if(sel == 10) {
    GPIO_InitStructure.GPIO_Pin = SW_I2C10_SCL_PIN;
    GPIO_Init(SW_I2C10_SCL_GPIO, &GPIO_InitStructure);
  }
}
#endif

void i2c_clk_data_out(i2c_tbl_t *p_i2c_sw)
{
  scl_high(p_i2c_sw);
  TIMER__Wait_us(SW_I2C_WAIT_TIME);
  scl_low(p_i2c_sw);
  // TIMER__Wait_us(SW_I2C_WAIT_TIME>>2);
}

void i2c_port_initial(i2c_tbl_t *p_i2c_sw)
{
  sda_high(p_i2c_sw);
  scl_high(p_i2c_sw);
}
void i2c_start_condition(i2c_tbl_t *p_i2c_sw)
{
  sda_high(p_i2c_sw);
  scl_high(p_i2c_sw);

  TIMER__Wait_us(SW_I2C_WAIT_TIME);
  sda_low(p_i2c_sw);
  TIMER__Wait_us(SW_I2C_WAIT_TIME);
  scl_low(p_i2c_sw);

  TIMER__Wait_us(SW_I2C_WAIT_TIME << 1);
}
void i2c_stop_condition(i2c_tbl_t *p_i2c_sw)
{
  sda_low(p_i2c_sw);
  scl_high(p_i2c_sw);

  TIMER__Wait_us(SW_I2C_WAIT_TIME);
  sda_high(p_i2c_sw);
  TIMER__Wait_us(SW_I2C_WAIT_TIME);
}

uint8_t i2c_check_ack(i2c_tbl_t *p_i2c_sw)
{
  uint8_t ack;
  int i;
  unsigned int temp;

  sda_in_mode(p_i2c_sw);

  scl_high(p_i2c_sw);

  ack = FALSE;
  TIMER__Wait_us(SW_I2C_WAIT_TIME);

  for (i = 10; i > 0; i--)
  {
    temp = !(SW_I2C_ReadVal_SDA(p_i2c_sw)); //0=ack , 1=nack
    if (temp) // if ack, enter
    {
      ack = TRUE;
      break;
    }
  }
  scl_low(p_i2c_sw);
  sda_out_mode(p_i2c_sw); //during setting, sda signal high

  TIMER__Wait_us(SW_I2C_WAIT_TIME);
  return ack;
}

void i2c_check_not_ack(i2c_tbl_t *p_i2c_sw)
{
  sda_in_mode(p_i2c_sw);
  i2c_clk_data_out(p_i2c_sw);
  sda_out_mode(p_i2c_sw);
  TIMER__Wait_us(SW_I2C_WAIT_TIME);
}

void i2c_check_not_ack_continue(i2c_tbl_t *p_i2c_sw)
{
  // sda_in_mode();
  i2c_clk_data_out(p_i2c_sw);
  // sda_out_mode();
  TIMER__Wait_us(SW_I2C_WAIT_TIME);
}

void i2c_slave_address(i2c_tbl_t *p_i2c_sw, uint8_t IICID, uint8_t readwrite)
{
  int x;

  if (readwrite)
  {
    IICID |= I2C_READ;
  }
  else
  {
    IICID &= ~I2C_READ;
  }

  scl_low(p_i2c_sw);

  for (x = 7; x >= 0; x--)
  {
    sda_out(p_i2c_sw, IICID & (1 << x));
    TIMER__Wait_us(SW_I2C_WAIT_TIME);
    i2c_clk_data_out(p_i2c_sw);
    // TIMER__Wait_us(SW_I2C_WAIT_TIME);
  }
}

void i2c_register_address(i2c_tbl_t *p_i2c_sw, uint8_t addr)
{
  int x;

  scl_low(p_i2c_sw);

  for (x = 7; x >= 0; x--)
  {
    sda_out(p_i2c_sw, addr & (1 << x));
    TIMER__Wait_us(SW_I2C_WAIT_TIME);
    i2c_clk_data_out(p_i2c_sw);
    // TIMER__Wait_us(SW_I2C_WAIT_TIME);
  }
}

void i2c_send_ack(i2c_tbl_t *p_i2c_sw)
{
  sda_out_mode(p_i2c_sw);
  sda_low(p_i2c_sw);

  TIMER__Wait_us(SW_I2C_WAIT_TIME);
  scl_high(p_i2c_sw);

  TIMER__Wait_us(SW_I2C_WAIT_TIME << 1);

  sda_low(p_i2c_sw);
  TIMER__Wait_us(SW_I2C_WAIT_TIME << 1);

  scl_low(p_i2c_sw);

  sda_out_mode(p_i2c_sw);

  TIMER__Wait_us(SW_I2C_WAIT_TIME);
}

/* external functions */
uint8_t SW_I2C_ReadVal_SDA(i2c_tbl_t *p_i2c_sw)
{
  uint8_t ret = 0;
  if(HAL_GPIO_ReadPin(p_i2c_sw->sda_port) & (1 << p_i2c_sw->sda_pin))
  {
    ret = 1;
  }

  return ret;
}

#if 0
uint8_t SW_I2C_ReadVal_SCL(uint8_t sel)
{
  if(sel == 1)
    return GPIO_ReadInputDataBit(SW_I2C1_SCL_GPIO, SW_I2C1_SCL_PIN);
  else if(sel == 2)
    return GPIO_ReadInputDataBit(SW_I2C2_SCL_GPIO, SW_I2C2_SCL_PIN);
  else if(sel == 3)
    return GPIO_ReadInputDataBit(SW_I2C3_SCL_GPIO, SW_I2C3_SCL_PIN);
  else if(sel == 4)
    return GPIO_ReadInputDataBit(SW_I2C4_SCL_GPIO, SW_I2C4_SCL_PIN);
  else if(sel == 5)
    return GPIO_ReadInputDataBit(SW_I2C5_SCL_GPIO, SW_I2C5_SCL_PIN);
  else if(sel == 6)
    return GPIO_ReadInputDataBit(SW_I2C6_SCL_GPIO, SW_I2C6_SCL_PIN);
  else if(sel == 7)
    return GPIO_ReadInputDataBit(SW_I2C7_SCL_GPIO, SW_I2C7_SCL_PIN);
  else if(sel == 8)
    return GPIO_ReadInputDataBit(SW_I2C8_SCL_GPIO, SW_I2C8_SCL_PIN);
  else if(sel == 9)
    return GPIO_ReadInputDataBit(SW_I2C9_SCL_GPIO, SW_I2C9_SCL_PIN);
  else if(sel == 10)
    return GPIO_ReadInputDataBit(SW_I2C10_SCL_GPIO, SW_I2C10_SCL_PIN);
  return 0;
}
#endif

void SW_I2C_Write_Data(i2c_tbl_t *p_i2c_sw, uint8_t data)
{
  int x;

  scl_low(p_i2c_sw);

  for (x = 7; x >= 0; x--)
  {
    sda_out(p_i2c_sw, data & (1 << x));
    TIMER__Wait_us(SW_I2C_WAIT_TIME);
    i2c_clk_data_out(p_i2c_sw);
    // TIMER__Wait_us(SW_I2C_WAIT_TIME);
  }
}

uint8_t SW_I2C_Read_Data(i2c_tbl_t *p_i2c_sw)
{
  int x;
  uint8_t readdata = 0;

  sda_in_mode(p_i2c_sw);

  for (x = 8; x--;)
  {
    scl_high(p_i2c_sw);

    readdata <<= 1;
    if (SW_I2C_ReadVal_SDA(p_i2c_sw))
      readdata |= 0x01;

    TIMER__Wait_us(SW_I2C_WAIT_TIME);
    scl_low(p_i2c_sw);

    TIMER__Wait_us(SW_I2C_WAIT_TIME);
  }

  sda_out_mode(p_i2c_sw);
  return readdata;
}

uint8_t SW_I2C_WriteControl_8Bit(i2c_tbl_t *p_i2c_sw, uint8_t IICID, uint8_t regaddr, uint8_t data)
{
  uint8_t returnack = TRUE;

  i2c_start_condition(p_i2c_sw);

  i2c_slave_address(p_i2c_sw, IICID, WRITE_CMD);
  if (!i2c_check_ack(p_i2c_sw))
  {
    returnack = FALSE;
  }

  TIMER__Wait_us(SW_I2C_WAIT_TIME);

  i2c_register_address(p_i2c_sw, regaddr);
  if (!i2c_check_ack(p_i2c_sw))
  {
    returnack = FALSE;
  }

  TIMER__Wait_us(SW_I2C_WAIT_TIME);

  SW_I2C_Write_Data(p_i2c_sw, data);
  if (!i2c_check_ack(p_i2c_sw))
  {
    returnack = FALSE;
  }

  TIMER__Wait_us(SW_I2C_WAIT_TIME);

  i2c_stop_condition(p_i2c_sw);

  return returnack;
}
uint8_t SW_I2C_WriteControl_8Bit_OnlyRegAddr(i2c_tbl_t *p_i2c_sw, uint8_t IICID, uint8_t regaddr)
{
  uint8_t returnack = TRUE;

  i2c_start_condition(p_i2c_sw);

  i2c_slave_address(p_i2c_sw, IICID, WRITE_CMD);
  if (!i2c_check_ack(p_i2c_sw))
  {
    returnack = FALSE;
  }

  i2c_register_address(p_i2c_sw, regaddr);
  if (!i2c_check_ack(p_i2c_sw))
  {
    returnack = FALSE;
  }

  // i2c_stop_condition(sel);

  return returnack;
}

uint8_t SW_I2C_WriteControl_16Bit(i2c_tbl_t *p_i2c_sw, uint8_t IICID, uint8_t regaddr, uint16_t data)
{
  uint8_t returnack = TRUE;

  i2c_start_condition(p_i2c_sw);

  i2c_slave_address(p_i2c_sw, IICID, WRITE_CMD);
  if (!i2c_check_ack(p_i2c_sw))
  {
    returnack = FALSE;
  }

  TIMER__Wait_us(SW_I2C_WAIT_TIME);

  i2c_register_address(p_i2c_sw, regaddr);
  if (!i2c_check_ack(p_i2c_sw))
  {
    returnack = FALSE;
  }

  TIMER__Wait_us(SW_I2C_WAIT_TIME);

  SW_I2C_Write_Data(p_i2c_sw, (data >> 8) & 0xFF);
  if (!i2c_check_ack(p_i2c_sw))
  {
    returnack = FALSE;
  }

  TIMER__Wait_us(SW_I2C_WAIT_TIME);

  SW_I2C_Write_Data(p_i2c_sw, data & 0xFF);
  if (!i2c_check_ack(p_i2c_sw))
  {
    returnack = FALSE;
  }

  TIMER__Wait_us(SW_I2C_WAIT_TIME);

  i2c_stop_condition(p_i2c_sw);

  return returnack;
}

bool i2cSwRead(i2c_tbl_t *p_i2c_sw, uint8_t dev_addr, uint8_t reg_addr, uint8_t *p_data, uint8_t length, uint32_t timeout)
{
  bool ret = true;

  i2c_port_initial(p_i2c_sw);

  i2c_start_condition(p_i2c_sw);

  i2c_slave_address(p_i2c_sw, dev_addr, WRITE_CMD);
  if (!i2c_check_ack(p_i2c_sw))
  {
    ret = false;
  }

  TIMER__Wait_us(SW_I2C_WAIT_TIME);

  i2c_register_address(p_i2c_sw, reg_addr);
  if (!i2c_check_ack(p_i2c_sw))
  {
    ret = false;
  }

  //i2c_stop_condition(sel);

  TIMER__Wait_us(SW_I2C_WAIT_TIME);

  i2c_start_condition(p_i2c_sw);

  i2c_slave_address(p_i2c_sw, dev_addr, READ_CMD);
  if (!i2c_check_ack(p_i2c_sw))
  {
    ret = false;
  }

  for (int i=0 ; i<length ; i++)
  {
    TIMER__Wait_us(SW_I2C_WAIT_TIME);
    p_data[i] = SW_I2C_Read_Data(p_i2c_sw);

    if(i == (length-1))
      i2c_check_not_ack(p_i2c_sw);
    else
      i2c_send_ack(p_i2c_sw);
  }
  //pdata[rcnt-1] = SW_I2C_Read_Data(sel);

  i2c_stop_condition(p_i2c_sw);

  return ret;
}

bool i2cSwWrite(i2c_tbl_t *p_i2c_sw, uint8_t dev_addr, uint8_t reg_addr, uint8_t *p_data, uint8_t length, uint32_t timeout)
{
  bool ret = TRUE;

  i2c_start_condition(p_i2c_sw);

  i2c_slave_address(p_i2c_sw, dev_addr, WRITE_CMD);
  if (!i2c_check_ack(p_i2c_sw))
  {
    ret = FALSE;
  }

  TIMER__Wait_us(SW_I2C_WAIT_TIME);

  i2c_register_address(p_i2c_sw, reg_addr);
  if (!i2c_check_ack(p_i2c_sw))
  {
    ret = FALSE;
  }

  for(int i=0; i<length; i++)
  {
    TIMER__Wait_us(SW_I2C_WAIT_TIME);
    SW_I2C_Write_Data(p_i2c_sw, p_data[i]);

    if (!i2c_check_ack(p_i2c_sw))
    {
      ret = FALSE;
    }
  }

  TIMER__Wait_us(SW_I2C_WAIT_TIME);

  i2c_stop_condition(p_i2c_sw);

  return ret;
}




uint8_t SW_I2C_WritenControl_8Bit(i2c_tbl_t *p_i2c_sw, uint8_t IICID, uint8_t regaddr, uint8_t *data, uint8_t rcnt)
{
  uint8_t returnack = TRUE;
  uint8_t index;

  i2c_start_condition(p_i2c_sw);

  i2c_slave_address(p_i2c_sw, IICID, WRITE_CMD);
  if (!i2c_check_ack(p_i2c_sw))
  {
    returnack = FALSE;
  }

  TIMER__Wait_us(SW_I2C_WAIT_TIME);

  i2c_register_address(p_i2c_sw, regaddr);
  if (!i2c_check_ack(p_i2c_sw))
  {
    returnack = FALSE;
  }

  for(index=0; index<rcnt; index++)
  {
    TIMER__Wait_us(SW_I2C_WAIT_TIME);
    SW_I2C_Write_Data(p_i2c_sw, data[index]);

    if (!i2c_check_ack(p_i2c_sw))
    {
      returnack = FALSE;
    }
  }

  TIMER__Wait_us(SW_I2C_WAIT_TIME);

  i2c_stop_condition(p_i2c_sw);

  return returnack;
}



uint8_t SW_I2C_ReadControl_8Bit_OnlyRegAddr(i2c_tbl_t *p_i2c_sw, uint8_t IICID, uint8_t regaddr)
{
  uint8_t returnack = TRUE;

  i2c_start_condition(p_i2c_sw);

  i2c_slave_address(p_i2c_sw, IICID, WRITE_CMD);
  if (!i2c_check_ack(p_i2c_sw))
  {
    returnack = FALSE;
  }

  TIMER__Wait_us(SW_I2C_WAIT_TIME);

  i2c_register_address(p_i2c_sw, regaddr);
  if (!i2c_check_ack(p_i2c_sw))
  {
    returnack = FALSE;
  }

  TIMER__Wait_us(SW_I2C_WAIT_TIME);

  i2c_stop_condition(p_i2c_sw);

  return returnack;
}

uint8_t SW_I2C_ReadControl_8Bit_OnlyData(i2c_tbl_t *p_i2c_sw, uint8_t IICID)
{
  uint8_t readdata = 0;

  i2c_port_initial(p_i2c_sw);

  i2c_start_condition(p_i2c_sw);

  i2c_slave_address(p_i2c_sw, IICID, READ_CMD);
  i2c_check_ack(p_i2c_sw);

  TIMER__Wait_us(SW_I2C_WAIT_TIME);

  readdata = SW_I2C_Read_Data(p_i2c_sw);

  i2c_check_not_ack(p_i2c_sw);

  i2c_stop_condition(p_i2c_sw);

  return readdata;
}

uint16_t SW_I2C_ReadControl_16Bit_OnlyData(i2c_tbl_t *p_i2c_sw, uint8_t IICID)
{
  uint8_t readimsi = 0;
  uint16_t readdata = 0;

  i2c_start_condition(p_i2c_sw);

  i2c_slave_address(p_i2c_sw, IICID, READ_CMD);
  i2c_check_not_ack(p_i2c_sw);

  TIMER__Wait_us(SW_I2C_WAIT_TIME);

  readimsi = SW_I2C_Read_Data(p_i2c_sw);
  i2c_check_not_ack_continue(p_i2c_sw);

  readdata = readimsi<<8;

  readimsi = SW_I2C_Read_Data(p_i2c_sw);
  i2c_check_not_ack(p_i2c_sw);

  readdata |= readimsi;

  i2c_stop_condition(p_i2c_sw);

  return readdata;
}

uint8_t SW_I2C_ReadControl_8Bit(i2c_tbl_t *p_i2c_sw, uint8_t IICID, uint8_t regaddr)
{
  uint8_t readdata = 0;

  i2c_port_initial(p_i2c_sw);

  i2c_start_condition(p_i2c_sw);

  i2c_slave_address(p_i2c_sw, IICID, WRITE_CMD);
  i2c_check_ack(p_i2c_sw);

  i2c_register_address(p_i2c_sw, regaddr);
  i2c_check_ack(p_i2c_sw);

  TIMER__Wait_us(SW_I2C_WAIT_TIME);

  i2c_start_condition(p_i2c_sw);

  i2c_slave_address(p_i2c_sw, IICID, READ_CMD);
  i2c_check_ack(p_i2c_sw);

  TIMER__Wait_us(SW_I2C_WAIT_TIME);

  readdata = SW_I2C_Read_Data(p_i2c_sw);

  i2c_check_not_ack(p_i2c_sw);

  i2c_stop_condition(p_i2c_sw);

  return readdata;
}

uint16_t SW_I2C_ReadControl_16Bit(i2c_tbl_t *p_i2c_sw, uint8_t IICID, uint8_t regaddr)
{
  uint16_t readdata = 0;

  i2c_port_initial(p_i2c_sw);

  i2c_start_condition(p_i2c_sw);

  i2c_slave_address(p_i2c_sw, IICID, WRITE_CMD);
  i2c_check_ack(p_i2c_sw);

  i2c_register_address(p_i2c_sw, regaddr);
  i2c_check_ack(p_i2c_sw);

  TIMER__Wait_us(SW_I2C_WAIT_TIME);

  i2c_start_condition(p_i2c_sw);

  i2c_slave_address(p_i2c_sw, IICID, READ_CMD);
  i2c_check_ack(p_i2c_sw);

  TIMER__Wait_us(SW_I2C_WAIT_TIME);

  readdata = SW_I2C_Read_Data(p_i2c_sw);
  i2c_send_ack(p_i2c_sw);
  TIMER__Wait_us(SW_I2C_WAIT_TIME);

  readdata = ((readdata << 8) | SW_I2C_Read_Data(p_i2c_sw));

  i2c_check_not_ack(p_i2c_sw);

  i2c_stop_condition(p_i2c_sw);

  return readdata;
}

uint8_t SW_I2C_ReadnControl_8Bit(i2c_tbl_t *p_i2c_sw, uint8_t IICID, uint8_t regaddr, uint8_t rcnt, uint8_t (*pdata))
{
  uint8_t returnack = TRUE;
  uint8_t index;

  i2c_port_initial(p_i2c_sw);

  i2c_start_condition(p_i2c_sw);

  i2c_slave_address(p_i2c_sw, IICID, WRITE_CMD);
  if (!i2c_check_ack(p_i2c_sw))
  {
    returnack = FALSE;
  }

  TIMER__Wait_us(SW_I2C_WAIT_TIME);

  i2c_register_address(p_i2c_sw, regaddr);
  if (!i2c_check_ack(p_i2c_sw))
  {
    returnack = FALSE;
  }

  //i2c_stop_condition(sel);

  TIMER__Wait_us(SW_I2C_WAIT_TIME);

  i2c_start_condition(p_i2c_sw);

  i2c_slave_address(p_i2c_sw, IICID, READ_CMD);
  if (!i2c_check_ack(p_i2c_sw))
  {
    returnack = FALSE;
  }

  for ( index=0 ; index<rcnt ; index++)
  {
    TIMER__Wait_us(SW_I2C_WAIT_TIME);
    pdata[index] = SW_I2C_Read_Data(p_i2c_sw);

    if(index == (rcnt-1))
      i2c_check_not_ack(p_i2c_sw);
    else
      i2c_send_ack(p_i2c_sw);
  }

  //pdata[rcnt-1] = SW_I2C_Read_Data(sel);



  i2c_stop_condition(p_i2c_sw);

  return returnack;
}

uint8_t SW_I2C_Multi_ReadnControl_8Bit(i2c_tbl_t *p_i2c_sw, uint8_t IICID, uint8_t regaddr, uint8_t rcnt, uint8_t (*pdata))
{
  uint8_t returnack = TRUE;
  uint8_t index;

  i2c_port_initial(p_i2c_sw);

  i2c_start_condition(p_i2c_sw);

  i2c_slave_address(p_i2c_sw, IICID, WRITE_CMD);
  if (!i2c_check_ack(p_i2c_sw))
  {
    returnack = FALSE;
  }

  TIMER__Wait_us(SW_I2C_WAIT_TIME);

  i2c_register_address(p_i2c_sw, regaddr);
  if (!i2c_check_ack(p_i2c_sw))
  {
    returnack = FALSE;
  }

  TIMER__Wait_us(SW_I2C_WAIT_TIME);

  i2c_start_condition(p_i2c_sw);

  i2c_slave_address(p_i2c_sw, IICID, READ_CMD);
  if (!i2c_check_ack(p_i2c_sw))
  {
    returnack = FALSE;
  }

  for ( index = 0 ; index < (rcnt-1) ; index++)
  {
    TIMER__Wait_us(SW_I2C_WAIT_TIME);
    pdata[index] = SW_I2C_Read_Data(p_i2c_sw);
    i2c_send_ack(p_i2c_sw);
    //if (!i2c_check_ack(sel))
    //{
    //  returnack = FALSE;
    //}
  }

  //pdata[rcnt-1] = SW_I2C_Read_Data(sel);

  i2c_check_not_ack(p_i2c_sw);

  i2c_stop_condition(p_i2c_sw);

  return returnack;
}

uint8_t SW_I2C_Check_SlaveAddr(i2c_tbl_t *p_i2c_sw, uint8_t IICID)
{
  uint8_t returnack = TRUE;

  i2c_start_condition(p_i2c_sw);

  i2c_slave_address(p_i2c_sw, IICID, WRITE_CMD);
  if (!i2c_check_ack(p_i2c_sw))
  {
    returnack = FALSE;
  }

  return returnack;
}

uint8_t SW_I2C_UTIL_WRITE(i2c_tbl_t *p_i2c_sw, uint8_t IICID, uint8_t regaddr, uint8_t data)
{
  return SW_I2C_WriteControl_8Bit(p_i2c_sw, IICID<<1, regaddr, data);
}

uint8_t SW_I2C_UTIL_Read(i2c_tbl_t *p_i2c_sw, uint8_t IICID, uint8_t regaddr)
{
  return SW_I2C_ReadControl_8Bit(p_i2c_sw, IICID<<1, regaddr);
}

uint8_t SW_I2C_UTIL_Read_Multi(i2c_tbl_t *p_i2c_sw, uint8_t IICID, uint8_t regaddr, uint8_t rcnt, uint8_t (*pdata))
{
  return SW_I2C_Multi_ReadnControl_8Bit(p_i2c_sw, IICID<<1, regaddr, rcnt, pdata);
}


#endif
