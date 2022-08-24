/*
 * led.c
 *
 *  Created on: Jan 29, 2022
 *      Author: suntaek.noh
 */


#include "gpio.h"
#include "cli.h"

#ifdef _USE_HW_GPIO

typedef struct
{
  PORT_Type     *port;
  uint32_t       pin;

  uint8_t        mode;

  uint8_t        on_state;
  uint8_t        off_state;

  bool           init_value;
}gpio_tbl_t;

gpio_tbl_t gpio_tbl[GPIO_MAX_CH] =
    {
        {(PORT_Type *)PF, 4,  _DEF_OUTPUT,_DEF_HIGH, _DEF_LOW, _DEF_HIGH},      //0
        {(PORT_Type *)PF, 5,  _DEF_OUTPUT,_DEF_HIGH, _DEF_LOW, _DEF_HIGH},      //1
        {(PORT_Type *)PF, 6,  _DEF_OUTPUT,_DEF_HIGH, _DEF_LOW, _DEF_HIGH},      //2
        {(PORT_Type *)PF, 7,  _DEF_OUTPUT,_DEF_HIGH, _DEF_LOW, _DEF_HIGH},      //3

        {             PA, 11, _DEF_OUTPUT,_DEF_HIGH, _DEF_LOW, _DEF_HIGH},      //4
        {             PA, 10, _DEF_OUTPUT,_DEF_HIGH, _DEF_LOW, _DEF_HIGH},      //5
        {             PA, 9,  _DEF_OUTPUT,_DEF_HIGH, _DEF_LOW, _DEF_HIGH},      //6
        {             PA, 8,  _DEF_OUTPUT,_DEF_HIGH, _DEF_LOW, _DEF_HIGH},      //7

        {             PC, 1,  _DEF_INPUT, _DEF_LOW,  _DEF_HIGH, _DEF_HIGH},     //8     sw
        {             PB, 10, _DEF_OUTPUT,_DEF_HIGH, _DEF_LOW,  _DEF_LOW},      //9     backligh

        {             PA, 3, _DEF_OUTPUT,_DEF_HIGH, _DEF_LOW,  _DEF_HIGH},      //10    touch reset
        {             PA, 2, _DEF_INPUT, _DEF_LOW, _DEF_HIGH,  _DEF_HIGH},      //11    touch1 int
        {             PC, 12, _DEF_INPUT,_DEF_LOW, _DEF_HIGH,  _DEF_HIGH},      //12    touch2 int



#if 0
        {             PE, 0, _DEF_OUTPUT,_DEF_HIGH, _DEF_LOW,  _DEF_LOW},       //10     com0
        {             PE, 1, _DEF_OUTPUT,_DEF_HIGH, _DEF_LOW,  _DEF_LOW},       //11     com1
        {             PE, 2, _DEF_OUTPUT,_DEF_HIGH, _DEF_LOW,  _DEF_LOW},       //12     com2
        {             PE, 3, _DEF_OUTPUT,_DEF_HIGH, _DEF_LOW,  _DEF_LOW},       //13     com3

        {             PE, 4, _DEF_OUTPUT,_DEF_HIGH, _DEF_LOW,  _DEF_HIGH},       //14     seg1
        {             PE, 5, _DEF_OUTPUT,_DEF_HIGH, _DEF_LOW,  _DEF_HIGH},       //15     seg2
        {             PE, 6, _DEF_OUTPUT,_DEF_HIGH, _DEF_LOW,  _DEF_HIGH},       //16     seg3
        {             PE, 7, _DEF_OUTPUT,_DEF_HIGH, _DEF_LOW,  _DEF_HIGH},       //17     seg4

        {             PC, 11, _DEF_OUTPUT,_DEF_HIGH, _DEF_LOW,  _DEF_HIGH},       //18     seg5

        {             PC, 6, _DEF_OUTPUT,_DEF_HIGH, _DEF_LOW,  _DEF_HIGH},       //19     seg5
        {             PC, 5, _DEF_OUTPUT,_DEF_HIGH, _DEF_LOW,  _DEF_HIGH},       //20     seg5
        {             PC, 4, _DEF_OUTPUT,_DEF_HIGH, _DEF_LOW,  _DEF_HIGH},       //21     seg5
        {             PC, 3, _DEF_OUTPUT,_DEF_HIGH, _DEF_LOW,  _DEF_HIGH},       //22     seg5
        {             PC, 2, _DEF_OUTPUT,_DEF_HIGH, _DEF_LOW,  _DEF_HIGH},       //23     seg5

        {             PB, 9, _DEF_OUTPUT,_DEF_HIGH, _DEF_LOW,  _DEF_HIGH},       //24     seg5
#endif










    };

#ifdef _USE_HW_CLI
static void cliGpio(cli_args_t *args);
#endif



bool gpioInit(void)
{
  bool ret = true;

  for (int i=0; i<GPIO_MAX_CH; i++)
  {
    gpioPinMode(i, gpio_tbl[i].mode);
    gpioPinWrite(i, gpio_tbl[i].init_value);
  }

#ifdef _USE_HW_CLI
  cliAdd("gpio", cliGpio);
#endif

  return ret;
}


bool gpioPinMode(uint8_t ch, uint8_t mode)
{
  bool ret = true;

  if (ch >= GPIO_MAX_CH)
  {
    return false;
  }

  switch(mode)
  {
    case _DEF_INPUT:
      HAL_GPIO_ConfigOutput(gpio_tbl[ch].port, gpio_tbl[ch].pin, PCU_MODE_INPUT);
      HAL_GPIO_ConfigPullup(gpio_tbl[ch].port, gpio_tbl[ch].pin, PCU_PUPD_DISABLE);
      break;

    case _DEF_INPUT_PULLUP:
      HAL_GPIO_ConfigOutput(gpio_tbl[ch].port, gpio_tbl[ch].pin, PCU_MODE_INPUT);
      HAL_GPIO_ConfigPullup(gpio_tbl[ch].port, gpio_tbl[ch].pin, PCU_PUPD_PULL_UP);
      break;

    case _DEF_INPUT_PULLDOWN:
      HAL_GPIO_ConfigOutput(gpio_tbl[ch].port, gpio_tbl[ch].pin, PCU_MODE_INPUT);
      HAL_GPIO_ConfigPullup(gpio_tbl[ch].port, gpio_tbl[ch].pin, PCU_PUPD_PULL_DOWN);
      break;

    case _DEF_OUTPUT:
      HAL_GPIO_ConfigOutput(gpio_tbl[ch].port, gpio_tbl[ch].pin, PCU_MODE_PUSH_PULL);
      HAL_GPIO_ConfigPullup(gpio_tbl[ch].port, gpio_tbl[ch].pin, PCU_PUPD_DISABLE);
      break;

    case _DEF_OUTPUT_PULLUP:
      HAL_GPIO_ConfigOutput(gpio_tbl[ch].port, gpio_tbl[ch].pin, PCU_MODE_PUSH_PULL);
      HAL_GPIO_ConfigPullup(gpio_tbl[ch].port, gpio_tbl[ch].pin, PCU_PUPD_PULL_UP);
      break;

    case _DEF_OUTPUT_PULLDOWN:
      HAL_GPIO_ConfigOutput(gpio_tbl[ch].port, gpio_tbl[ch].pin, PCU_MODE_PUSH_PULL);
      HAL_GPIO_ConfigPullup(gpio_tbl[ch].port, gpio_tbl[ch].pin, PCU_PUPD_PULL_DOWN);
      break;
  }

  return ret;
}



void gpioPinWrite(uint8_t ch, bool value)
{
  if (ch >= GPIO_MAX_CH)
  {
    return;
  }

  if (value)
  {
    HAL_GPIO_SetPin(gpio_tbl[ch].port, (1 << gpio_tbl[ch].pin));
  }
  else
  {
    HAL_GPIO_ClearPin(gpio_tbl[ch].port, (1 << gpio_tbl[ch].pin));
  }
}


bool gpioPinRead(uint8_t ch)
{
  bool ret = false;

  uint32_t pin_value = 0;

  if (ch >= GPIO_MAX_CH)
  {
    return false;
  }


  if(gpio_tbl[ch].mode == _DEF_OUTPUT || gpio_tbl[ch].mode == _DEF_OUTPUT_PULLUP || gpio_tbl[ch].mode == _DEF_OUTPUT_PULLDOWN)
  {
    pin_value = gpio_tbl[ch].port->OUTDR;   //HAL_GPIO_EXTI_GetState(led_tbl[ch].port);

    if(gpio_tbl[ch].on_state == _DEF_HIGH)
    {
      if(pin_value & (1 << gpio_tbl[ch].pin))
      {
        ret = true;
      }
    }
    else
    {
      if(!(pin_value & (1 << gpio_tbl[ch].pin)))
      {
        ret = true;
      }
    }
  }
  else
  {
    if(gpio_tbl[ch].on_state == _DEF_HIGH)
    {
      if(HAL_GPIO_ReadPin(gpio_tbl[ch].port) & (1 << gpio_tbl[ch].pin))
      {
        ret = true;
      }
    }
    else
    {
      if(!(HAL_GPIO_ReadPin(gpio_tbl[ch].port) & (1 << gpio_tbl[ch].pin)))
      {
        ret = true;
      }
    }
  }

  return ret;
}


void gpioPinToggle(uint8_t ch)
{
  uint32_t pin_value = 0;

  if (ch >= GPIO_MAX_CH)
  {
    return;
  }

  pin_value = gpio_tbl[ch].port->OUTDR;   //HAL_GPIO_EXTI_GetState(led_tbl[ch].port);

  if(pin_value & (1 << gpio_tbl[ch].pin))
  {
    HAL_GPIO_ClearPin(gpio_tbl[ch].port, (1 << gpio_tbl[ch].pin));
  }
  else
  {
    HAL_GPIO_SetPin(gpio_tbl[ch].port, (1 << gpio_tbl[ch].pin));
  }
}





#ifdef _USE_HW_CLI
void cliGpio(cli_args_t *args)
{
  bool ret = false;


  if (args->argc == 1 && args->isStr(0, "show") == true)
  {
    while(cliKeepLoop())
    {
      for (int i=0; i<GPIO_MAX_CH; i++)
      {
        cliPrintf("%d", gpioPinRead(i));
      }
      cliPrintf("\n");
      delay(100);
    }
    ret = true;
  }

  if (args->argc == 2 && args->isStr(0, "read") == true)
  {
    uint8_t ch;

    ch = (uint8_t)args->getData(1);

    while(cliKeepLoop())
    {
      cliPrintf("gpio read %d : %d\n", ch, gpioPinRead(ch));
      delay(100);
    }

    ret = true;
  }

  if (args->argc == 3 && args->isStr(0, "write") == true)
  {
    uint8_t ch;
    uint8_t data;

    ch   = (uint8_t)args->getData(1);
    data = (uint8_t)args->getData(2);

    gpioPinWrite(ch, data);

    cliPrintf("gpio write %d : %d\n", ch, data);
    ret = true;
  }

  if (ret != true)
  {
    cliPrintf("gpio show\n");
    cliPrintf("gpio read ch[0~%d]\n", GPIO_MAX_CH-1);
    cliPrintf("gpio write ch[0~%d] 0:1\n", GPIO_MAX_CH-1);
  }
}
#endif



#endif

