/*
 * gt216l.c
 *
 *  Created on: 2022. 8. 17.
 *      Author: sunta
 */

#include "touch/gt216l.h"
#include "i2c.h"
#include "gpio.h"

#ifdef _USE_HW_GT216L

#define TOUCH_RST_L()  gpioPinWrite(touch_rst, _DEF_LOW)        // active low
#define TOUCH_RST_H()  gpioPinWrite(touch_rst, _DEF_HIGH)


typedef struct
{
  uint8_t   touch_sn_num;
  uint8_t   led_num;
  char      *name;
}touch_tbl_t;

touch_tbl_t touch1_tbl[GT216L_1_MAX_TOUCHES] =
  {
    {1, 0,  "con3"},
    {2, 0,  "light2"},
    {3, 16, "con_total"},
    {4, 15, "light3"},
    {5, 14, "setting"},
    {6, 13, "light4"},
    {7, 12, "ext"},
    {8, 11, "ext1"},
  };

uint8_t touch_cfg[] =
    {
        0xff, 0x03, 0x32, 0x0A
    };

uint8_t touch_sensitivity_cfg[] =
    {
        0x1F, 0x1F, 0x1F, 0x1F, 0x1F, 0x1F, 0x1F, 0x1F, 0x1F, 0x1F
    };

static uint8_t touch_rst = 10;

static bool gt216lInit(void);
static bool gt216lGetTouchData(uint16_t *p_data);
bool gt216lTouchIoOut(uint8_t ch, uint8_t value);

bool gt216lInitDriver(touch_driver_t *p_driver)
{
  p_driver->init = gt216lInit;
  p_driver->getTouchedData = gt216lGetTouchData;
  p_driver->setTouchedData = gt216lTouchIoOut;
  return true;
}

bool gt216lInit(void)
{
  bool ret = true;

  i2cOpen(_DEF_I2C1, I2C_FREQ_100KHz);

  TOUCH_RST_L();
  delay(100);
  TOUCH_RST_H();

#if 0
  for(i=0; i<GT216L_MAX_TOUCHES; i++)
  {
    touch_dir |= (1 << (touch_tbl[i].touch_sn_num-1));      // touch : input
    touch_dir &= ~(1 << (touch_tbl[i].led_num-1));          // led   : output
  }
#endif


  if(i2cWriteBytes(_DEF_I2C1, GT216L_CHIP1_ADDR, GT21L_IO_DIR1, touch_cfg, 4, 1000) != true)
  {
    logPrintf("touch init GT21L_IO_DIR1 error \r\n");
    ret = false;
  }

  if(i2cWriteBytes(_DEF_I2C1, GT216L_CHIP1_ADDR, GT216L_SENSITIVITY_CONTROL, touch_sensitivity_cfg, 10, 1000) != true)
  {
    logPrintf("touch init GT216L_SENSITIVITY_CONTROL error \r\n");
    ret = false;
  }

  logPrintf("touch init GT216L complete \r\n");
  return ret;
}

bool gt216lGetTouchData(uint16_t *p_data)
{
  bool ret = true;

#if 1
  if(i2cReadBytes(_DEF_I2C1, GT216L_CHIP1_ADDR, GT216L_TOUCH_OUT, (uint8_t *)p_data, 2, 1000) != true)
  {
    logPrintf("touch init GT216L_TOUCH_OUT error \r\n");
    ret = false;
  }
#endif

  return ret;
}

bool gt216lTouchIoOut(uint8_t ch, uint8_t value)
{
  bool ret = true;

#if 0
  if(SW_I2C_WriteControl(GT216L_CHIP_ADDR1, ch, &value, 1) != true)
  {
    debugprintf("touch Io ch : 0x%x out error\r\n", ch);
    ret = false;
  }
#endif
  return ret;
}

#endif
