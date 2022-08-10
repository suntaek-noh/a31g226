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
        {(PORT_Type *)PF, 4, _DEF_OUTPUT,_DEF_HIGH, _DEF_LOW, _DEF_HIGH},
        {(PORT_Type *)PF, 5, _DEF_OUTPUT,_DEF_HIGH, _DEF_LOW, _DEF_HIGH},
        {(PORT_Type *)PF, 6, _DEF_OUTPUT,_DEF_HIGH, _DEF_LOW, _DEF_HIGH},
        {(PORT_Type *)PF, 7, _DEF_OUTPUT,_DEF_HIGH, _DEF_LOW, _DEF_HIGH},

        {PA, 11, _DEF_OUTPUT,_DEF_HIGH, _DEF_LOW, _DEF_HIGH},
        {PA, 10, _DEF_OUTPUT,_DEF_HIGH, _DEF_LOW, _DEF_HIGH},
        {PA, 9, _DEF_OUTPUT,_DEF_HIGH, _DEF_LOW, _DEF_HIGH},
        {PA, 8, _DEF_OUTPUT,_DEF_HIGH, _DEF_LOW, _DEF_HIGH},


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
    if(HAL_GPIO_ReadPin(gpio_tbl[ch].port) & (1 << gpio_tbl[ch].pin))
     {
       ret = true;
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

