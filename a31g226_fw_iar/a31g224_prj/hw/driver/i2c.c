/*
 * i2c.c
 *
 *  Created on: 2022. 8. 17.
 *      Author: sunta
 */

#include "i2c.h"
#include "i2c_sw.h"

#include "cli.h"


#ifdef _USE_HW_I2C

#ifdef _USE_HW_CLI
static void cliI2C(cli_args_t *args);
#endif

#define LOCK_BEGIN(x)
#define LOCK_END(x)

static uint32_t i2c_timeout[I2C_MAX_CH];
static uint32_t i2c_errcount[I2C_MAX_CH];
static uint32_t i2c_freq[I2C_MAX_CH];

static bool is_init = false;
static bool is_open[I2C_MAX_CH];


static i2c_tbl_t i2c_tbl[I2C_MAX_CH] =
    {
        { I2C1, PA, 1, PA, 0},        // ch 1
        { NULL, PA, 1, PA, 0},        // ch 2
      //{ &hi2c2, PD, 1, PD, 0},
    };

static const uint32_t i2c_freq_tbl[] =
    {
        100000, // 100Khz
        200000,
        300000,
        400000, // 400Khz
    };

static void delayUs(uint32_t us);

bool i2cInit(void)
{
  uint32_t i;

  for (i=0; i<I2C_MAX_CH; i++)
  {
    i2c_timeout[i] = 10;
    i2c_errcount[i] = 0;
    is_open[i] = false;
  }

#ifdef _USE_HW_CLI
  cliAdd("i2c", cliI2C);
#endif

  is_init = true;
  return true;
}

bool i2cIsInit(void)
{
  return is_init;
}

bool i2cOpen(uint8_t ch, uint32_t freq_khz)
{
  bool ret = false;

  if (ch >= I2C_MAX_CH)
  {
    return false;
  }

  switch(ch)
  {
    case _DEF_I2C1:

      i2c_freq[ch] = freq_khz;
      is_open[ch] = false;

      i2cReset(ch);   // i2c
      HAL_I2C_DeInit(i2c_tbl[ch].p_hi2c);
      /* MASTER CHANNEL INIT */
      // SCL port
      HAL_GPIO_ConfigOutput(i2c_tbl[ch].scl_port, i2c_tbl[ch].scl_pin , PCU_MODE_ALT_FUNC);
      HAL_GPIO_ConfigFunction(i2c_tbl[ch].scl_port, i2c_tbl[ch].scl_pin, PCU_ALT_FUNCTION_1);

      // SDA port
      HAL_GPIO_ConfigOutput(i2c_tbl[ch].sda_port, i2c_tbl[ch].sda_pin, PCU_MODE_ALT_FUNC);
      HAL_GPIO_ConfigFunction(i2c_tbl[ch].sda_port, i2c_tbl[ch].sda_pin, PCU_ALT_FUNCTION_1);

      HAL_I2C_Init(i2c_tbl[ch].p_hi2c, i2c_freq_tbl[freq_khz]);

      is_open[ch] = true;

      ret = true;

      break;

    case _DEF_I2C2:
      i2c_freq[ch] = freq_khz;
      is_open[ch]   = false;

      i2cReset(ch);   // i2c
    
      SW_I2C_initial(&i2c_tbl[ch]);

      is_open[ch] = true;

      ret = true;
      break;
  }

  return ret;
}

bool i2cIsOpen(uint8_t ch)
{
  return is_open[ch];
}

void i2cReset(uint8_t ch)
{
  i2c_tbl_t *p_pin = &i2c_tbl[ch];

  switch(ch)
 {
   case _DEF_I2C1:
   {

     HAL_GPIO_ConfigOutput(p_pin->scl_port, p_pin->scl_pin, PCU_MODE_PUSH_PULL);
     HAL_GPIO_ConfigPullup(p_pin->scl_port, p_pin->scl_pin, PCU_PUPD_DISABLE);

     HAL_GPIO_ConfigOutput(p_pin->sda_port, p_pin->sda_pin, PCU_MODE_PUSH_PULL);
     HAL_GPIO_ConfigPullup(p_pin->sda_port, p_pin->sda_pin, PCU_PUPD_DISABLE);


     HAL_GPIO_SetPin(p_pin->scl_port, (1 << p_pin->scl_pin));
     HAL_GPIO_SetPin(p_pin->sda_port, (1 << p_pin->sda_pin));

     delayUs(5);

     for (int i=0; i<9; i++)
     {
       HAL_GPIO_ClearPin(p_pin->scl_port, (1 <<  p_pin->scl_pin));
       delayUs(5);
       HAL_GPIO_SetPin(p_pin->scl_port, (1 << p_pin->scl_pin));
       delayUs(5);
     }

     HAL_GPIO_ClearPin(p_pin->scl_port, (1 <<  p_pin->scl_pin));
     delayUs(5);
     HAL_GPIO_ClearPin(p_pin->sda_port, (1 <<  p_pin->sda_pin));
     delayUs(5);

     HAL_GPIO_SetPin(p_pin->scl_port, (1 << p_pin->scl_pin));
     delayUs(5);
     HAL_GPIO_SetPin(p_pin->sda_port, (1 << p_pin->sda_pin));

     break;
   }


   case _DEF_I2C2:
   {
     HAL_GPIO_ConfigOutput(p_pin->scl_port, p_pin->scl_pin, PCU_MODE_PUSH_PULL);
     HAL_GPIO_ConfigPullup(p_pin->scl_port, p_pin->scl_pin, PCU_PUPD_DISABLE);

     HAL_GPIO_ConfigOutput(p_pin->sda_port, p_pin->sda_pin, PCU_MODE_PUSH_PULL);
     HAL_GPIO_ConfigPullup(p_pin->sda_port, p_pin->sda_pin, PCU_PUPD_DISABLE);


     HAL_GPIO_SetPin(p_pin->scl_port, (1 << p_pin->scl_pin));
     HAL_GPIO_SetPin(p_pin->sda_port, (1 << p_pin->sda_pin));

     delayUs(5);

     for (int i=0; i<9; i++)
     {
       HAL_GPIO_ClearPin(p_pin->scl_port, (1 <<  p_pin->scl_pin));
       delayUs(5);
       HAL_GPIO_SetPin(p_pin->scl_port, (1 << p_pin->scl_pin));
       delayUs(5);
     }

     HAL_GPIO_ClearPin(p_pin->scl_port, (1 <<  p_pin->scl_pin));
     delayUs(5);
     HAL_GPIO_ClearPin(p_pin->sda_port, (1 <<  p_pin->sda_pin));
     delayUs(5);

     HAL_GPIO_SetPin(p_pin->scl_port, (1 << p_pin->scl_pin));
     delayUs(5);
     HAL_GPIO_SetPin(p_pin->sda_port, (1 << p_pin->sda_pin));

     break;
   }

 }

}

bool i2cIsDeviceReady(uint8_t ch, uint8_t dev_addr)
{
  uint8_t tx_buf[1];
  tx_buf[0] = 0;

  I2C_Type *p_handle = i2c_tbl[ch].p_hi2c;

  switch(ch)
  {
    case _DEF_I2C1:

      i2c_tbl[ch].m_setup.sl_addr7bit = dev_addr>>1;
      i2c_tbl[ch].m_setup.tx_data = tx_buf;
      i2c_tbl[ch].m_setup.tx_length = 1;

      if(HAL_I2C_MasterTransmitData(p_handle, &i2c_tbl[ch].m_setup, I2C_TRANSFER_POLLING) == SUCCESS)
      {
        return true;
      }
      break;

    case _DEF_I2C2:


      break;
  }

  return false;
}

bool i2cRecovery(uint8_t ch)
{
  bool ret;

  i2cReset(ch);

  ret = i2cOpen(ch, i2c_freq[ch]);

  return ret;
}

bool i2cReadByte (uint8_t ch, uint16_t dev_addr, uint16_t reg_addr, uint8_t *p_data, uint32_t timeout)
{
  return i2cReadBytes(ch, dev_addr, reg_addr, p_data, 1, timeout);
}

bool i2cReadBytes(uint8_t ch, uint16_t dev_addr, uint16_t reg_addr, uint8_t *p_data, uint32_t length, uint32_t timeout)
{
  bool ret = false;
  uint8_t tx_buf[2];
  Status i2c_ret;

  if (ch >= I2C_MAX_CH)
  {
    return false;
  }

  switch(ch)
  {
    case  _DEF_I2C1:
    {
      I2C_Type *p_handle = i2c_tbl[ch].p_hi2c;

      tx_buf[0] = (uint8_t)reg_addr;

      i2c_tbl[ch].m_setup.sl_addr7bit = dev_addr>>1;
      i2c_tbl[ch].m_setup.tx_data = tx_buf;
      i2c_tbl[ch].m_setup.tx_length = 1;

      HAL_I2C_MasterTransmitData(p_handle, &i2c_tbl[ch].m_setup, I2C_TRANSFER_POLLING);

      i2c_tbl[ch].m_setup.sl_addr7bit = dev_addr>>1;
      i2c_tbl[ch].m_setup.rx_data = p_data;
      i2c_tbl[ch].m_setup.rx_length = length;

      i2c_ret = HAL_I2C_MasterReceiveData(p_handle, &i2c_tbl[ch].m_setup, I2C_TRANSFER_POLLING);

      if( i2c_ret == SUCCESS )
      {
        ret = true;
      }
      else
      {
        ret = false;
      }
    }
    break;

    case _DEF_I2C2:
    {
      ret = i2cSwRead(&i2c_tbl[ch], dev_addr, reg_addr, p_data, length, timeout);
    }
    break;


  }



  return ret;
}


bool i2cReadData(uint8_t ch, uint16_t dev_addr, uint8_t *p_data, uint32_t length, uint32_t timeout)
{
  bool ret = false;
  #if 0
  HAL_StatusTypeDef i2c_ret;
  I2C_HandleTypeDef *p_handle = i2c_tbl[ch].p_hi2c;

  if (ch >= I2C_MAX_CH)
  {
    return false;
  }

  i2c_ret = HAL_I2C_Master_Receive(p_handle, (uint16_t)(dev_addr << 1), p_data, length, timeout);

  if( i2c_ret == HAL_OK )
  {
    ret = true;
  }
  else
  {
    ret = false;
  }
  #endif
  return ret;
}


bool i2cWriteByte (uint8_t ch, uint16_t dev_addr, uint16_t reg_addr, uint8_t data, uint32_t timeout)
{
  return i2cWriteBytes(ch, dev_addr, reg_addr, &data, 1, timeout);
}

bool i2cWriteBytes(uint8_t ch, uint16_t dev_addr, uint16_t reg_addr, uint8_t *p_data, uint32_t length, uint32_t timeout)
{
  bool ret = false;
  //uint8_t tx_buf[length+1];   // "99을 채택한 컴파일러에서는 리스트의 크기를 변수로 선언이 가능하다. 그렇지 않은 컴파일러에서는 그런 기능을 지원하지 않는다"
  uint8_t tx_buf[20] = {0};
  Status i2c_ret;

  if (ch >= I2C_MAX_CH)
  {
    return false;
  }

  switch(ch)
  {
    case _DEF_I2C1:
    {
      I2C_Type *p_handle = i2c_tbl[ch].p_hi2c;

      tx_buf[0] = reg_addr;

      for (int i=0; i<length; i++)
      {
        tx_buf[1+i] = p_data[i];
      }

      i2c_tbl[ch].m_setup.sl_addr7bit = dev_addr>>1;
      i2c_tbl[ch].m_setup.tx_data = tx_buf;
      i2c_tbl[ch].m_setup.tx_length = length+1;

      i2c_ret = HAL_I2C_MasterTransmitData(p_handle, &i2c_tbl[ch].m_setup, I2C_TRANSFER_POLLING);

      if(i2c_ret == SUCCESS)
      {
        ret = true;
      }
      else
      {
        ret = false;
      }
    }
    break;

    case _DEF_I2C2:
    {
      ret = i2cSwWrite(&i2c_tbl[ch], dev_addr, reg_addr, p_data, length, timeout);
    }
    break;
  }


  return ret;
}


bool i2cWriteData(uint8_t ch, uint16_t dev_addr, uint8_t *p_data, uint32_t length, uint32_t timeout)
{
  bool ret = false;
  #if 0
  HAL_StatusTypeDef i2c_ret;
  I2C_HandleTypeDef *p_handle = i2c_tbl[ch].p_hi2c;

  if (ch >= I2C_MAX_CH)
  {
    return false;
  }


  i2c_ret = HAL_I2C_Master_Transmit(p_handle, (uint16_t)(dev_addr << 1), p_data, length, timeout);

  if(i2c_ret == HAL_OK)
  {
    ret = true;
  }
  else
  {
    ret = false;
  }
  #endif
  return ret;
}

void i2cSetTimeout(uint8_t ch, uint32_t timeout)
{
  i2c_timeout[ch] = timeout;
}

uint32_t i2cGetTimeout(uint8_t ch)
{
  return i2c_timeout[ch];
}

void i2cClearErrCount(uint8_t ch)
{
  i2c_errcount[ch] = 0;
}

uint32_t i2cGetErrCount(uint8_t ch)
{
  return i2c_errcount[ch];
}

void delayUs(uint32_t us)
{
  volatile uint32_t i;

  for (i=0; i<us*1000; i++)
  {

  }
}

#ifdef _USE_HW_CLI
void cliI2C(cli_args_t *args)
{
  bool ret = true;
  bool i2c_ret;

  uint8_t print_ch;
  uint8_t ch;
  uint16_t dev_addr;
  uint16_t reg_addr;
  uint16_t length;

  uint32_t i;
  uint8_t i2c_data[128];
  uint32_t pre_time;


  if (args->argc == 2)
  {
    print_ch = (uint16_t) args->getData(1);

    print_ch = constrain(print_ch, 1, I2C_MAX_CH);
    print_ch -= 1;

    if(args->isStr(0, "scan") == true)
    {
      for (i=0x00; i<= 0xff; i++)
      {
        if (i2cIsDeviceReady(print_ch, i) == true)
        {
          cliPrintf("I2C CH%d Addr 0x%X : OK\n", print_ch+1, i);
        }
        else
        {
          //cliPrintf("I2C CH%d Addr 0x%X : fail\n", print_ch+1, i);
        }
      }
    }
    else if(args->isStr(0, "open") == true)
    {
      i2c_ret = i2cOpen(print_ch, I2C_FREQ_100KHz);
      if (i2c_ret == true)
      {
        cliPrintf("I2C CH%d Open OK\n", print_ch + 1);
      }
      else
      {
        cliPrintf("I2C CH%d Open Fail\n", print_ch + 1);
      }
    }
  }
  else if (args->argc == 5)
  {
    print_ch = (uint16_t) args->getData(1);
    print_ch = constrain(print_ch, 1, I2C_MAX_CH);

    dev_addr = (uint16_t) args->getData(2);
    reg_addr = (uint16_t) args->getData(3);
    length   = (uint16_t) args->getData(4);
    ch       = print_ch - 1;

    if(args->isStr(0, "read") == true)
    {
      for (i=0; i<length; i++)
      {
        i2c_ret = i2cReadByte(ch, dev_addr, reg_addr+i, i2c_data, 100);

        if (i2c_ret == true)
        {
          cliPrintf("%d I2C - 0x%02X : 0x%02X\n", print_ch, reg_addr+i, i2c_data[0]);
        }
        else
        {
          cliPrintf("%d I2C - Fail \n", print_ch);
          break;
        }
      }
    }
    else if(args->isStr(0, "reads") == true)
    {

      i2c_ret = i2cReadBytes(ch, dev_addr, reg_addr, i2c_data, length, 100);


      if (i2c_ret == true)
      {
        for(int i=0; i<length; i++)
        {
          cliPrintf("%d I2C - 0x%02X : 0x%02X\n", print_ch, reg_addr+i, i2c_data[i]);
        }
      }
      else
      {
        cliPrintf("%d I2C - Fail \n", print_ch);
        //break;
      }

    }
    else if(args->isStr(0, "write") == true)
    {
      pre_time = millis();
      i2c_ret = i2cWriteByte(ch, dev_addr, reg_addr, (uint8_t)length, 100);

      if (i2c_ret == true)
      {
        cliPrintf("%d I2C - 0x%02X : 0x%02X, %d ms\n", print_ch, reg_addr, length, millis()-pre_time);
      }
      else
      {
        cliPrintf("%d I2C - Fail \n", print_ch);
      }
    }
    else
    {
      ret = false;
    }
  }
  else
  {
    ret = false;
  }

  if (ret == false)
  {
    cliPrintf( "i2c open channel[1~%d]\n", I2C_MAX_CH);
    cliPrintf( "i2c scan channel[1~%d]\n", I2C_MAX_CH);
    cliPrintf( "i2c read channel dev_addr reg_addr length\n");
    cliPrintf( "i2c reads channel dev_addr reg_addr length\n");
    cliPrintf( "i2c write channel dev_addr reg_addr data\n");
  }
}

#endif

#endif
