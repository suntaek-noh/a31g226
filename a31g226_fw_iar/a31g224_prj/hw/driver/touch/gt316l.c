/*
 * gt316l.c
 *
 *  Created on: 2022. 8. 17.
 *      Author: sunta
 */

#include "touch/gt316l.h"
#include "i2c.h"
#include "gpio.h"

#ifdef _USE_HW_GT316L

#define TOUCH_RST_L()  gpioPinWrite(touch_rst, _DEF_LOW)        // active low
#define TOUCH_RST_H()  gpioPinWrite(touch_rst, _DEF_HIGH)


typedef struct
{
  uint8_t   touch_sn_num;
  uint8_t   led_num;
  char      *name;
}touch_tbl_t;

touch_tbl_t touch1_tbl[GT316L_MAX_TOUCHES] =
  {
    {1, 0,  "con3"},
    {2, 0,  "light2"},
    {3, 16, "con_total"},
    {4, 15, "light3"},
    {5, 14, "setting"},
    
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

static bool gt316lInit(void);
static uint8_t gt316lGetTouchStatus(uint8_t ic);
static bool gt316lGetTouchData(uint8_t ic, uint8_t *p_data);
bool gt316lTouchIoOut(uint8_t ch, uint8_t value);
void gt316lTouchReset(uint8_t ch);

bool gt316lInitDriver(tbutton_driver_t *p_driver)
{
  p_driver->init = gt316lInit;
  p_driver->getTocuhedStatus = gt316lGetTouchStatus;
  p_driver->getTouchedData = gt316lGetTouchData;
  p_driver->setTouchedData = gt316lTouchIoOut;
  return true;
}

bool gt316lInit(void)
{
  bool ret = true;

  i2cOpen(_DEF_I2C2, I2C_FREQ_100KHz);

  TOUCH_RST_L();
  delay(100);
  TOUCH_RST_H();

#if 0
  for(i=0; i<gt316L_MAX_TOUCHES; i++)
  {
    touch_dir |= (1 << (touch_tbl[i].touch_sn_num-1));      // touch : input
    touch_dir &= ~(1 << (touch_tbl[i].led_num-1));          // led   : output
  }
#endif

  if(i2cWriteBytes(_DEF_I2C2, GT316L_CHIP1_ADDR, GT316L_IO_DIR1, touch_cfg, 4, 1000) != true)
  {
    logPrintf("touch ic1 init GT21L_IO_DIR1 error \r\n");
    ret = false;
  }

  if(i2cWriteBytes(_DEF_I2C2, GT316L_CHIP1_ADDR, GT316L_SENSITIVITY_CONTROL, touch_sensitivity_cfg, 10, 1000) != true)
  {
    logPrintf("touch ic1 init GT316L_SENSITIVITY_CONTROL error \r\n");
    ret = false;
  }


  if(i2cWriteBytes(_DEF_I2C2, GT316L_CHIP2_ADDR, GT316L_IO_DIR1, touch_cfg, 4, 1000) != true)
  {
    logPrintf("touch ic2 init GT21L_IO_DIR1 error \r\n");
    ret = false;
  }

  if(i2cWriteBytes(_DEF_I2C2, GT316L_CHIP2_ADDR, GT316L_SENSITIVITY_CONTROL, touch_sensitivity_cfg, 10, 1000) != true)
  {
    logPrintf("touch ic2 init GT316L_SENSITIVITY_CONTROL error \r\n");
    ret = false;
  }

  return ret;
}

void gt316lTouchReset(uint8_t ch)
{
  i2cReset(_DEF_I2C2);
}

uint8_t gt316lGetTouchStatus(uint8_t ic)
{
  uint8_t ret;

  if(i2cReadBytes(_DEF_I2C2, GT316L_CHIP1_ADDR, GT316L_TOUCH_STATUS, &ret, 1, 1000) != true)
  {
    logPrintf("GT316L_TOUCH_STATUS error \r\n");
    ret = 0;
  }

  return ret;
}

bool gt316lGetTouchData(uint8_t ic, uint8_t *p_data)
{
  bool ret = true;

  switch(ic)
  {
    case 0:
      if(i2cReadBytes(_DEF_I2C2, GT316L_CHIP1_ADDR, GT316L_TOUCH_OUT, (uint8_t *)p_data, 2, 1000) != true)
      {
        logPrintf("touch1  GT316L_TOUCH_OUT error \r\n");
        ret = false;
      }
      break;

    case 1:
      if(i2cReadBytes(_DEF_I2C2, GT316L_CHIP2_ADDR, GT316L_TOUCH_OUT, (uint8_t *)p_data, 2, 1000) != true)
      {
        logPrintf("touch2  GT316L_TOUCH_OUT error \r\n");
        ret = false;
      }
      break;

  }
  return ret;
}

bool gt316lTouchIoOut(uint8_t ch, uint8_t value)
{
  bool ret = true;

#if 0
  if(SW_I2C_WriteControl(GT316L_CHIP_ADDR1, ch, &value, 1) != true)
  {
    debugprintf("touch Io ch : 0x%x out error\r\n", ch);
    ret = false;
  }
#endif
  return ret;
}

#endif
