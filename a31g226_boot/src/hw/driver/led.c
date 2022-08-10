/*
 * led.c
 *
 *  Created on: Jan 29, 2022
 *      Author: suntaek.noh
 */


#include "led.h"
#include "cli.h"

#ifdef _USE_HW_LED

typedef struct
{
  PORT_Type     *port;
  uint32_t       pin;
  uint8_t        on_state;
  uint8_t        off_state;
}led_tbl_t;

led_tbl_t led_tbl[LED_MAX_CH] =
    {
        {PE, 2,_DEF_HIGH,_DEF_LOW},
        //{PE, 3,_DEF_HIGH,_DEF_LOW},
        //{PE, 4,_DEF_HIGH,_DEF_LOW},
        //{PE, 5,_DEF_HIGH,_DEF_LOW},
        //{PE, 6,_DEF_HIGH,_DEF_LOW},
        //{PE, 7,_DEF_HIGH,_DEF_LOW},

    };

#ifdef _USE_HW_CLI
static void cliLed(cli_args_t *args);
#endif

bool ledInit(void)
{
  bool ret = true;

  for(int i=0; i<LED_MAX_CH; i++)
  {
    HAL_GPIO_ConfigOutput(led_tbl[i].port, led_tbl[i].pin, PCU_MODE_PUSH_PULL);      // PE2 to PE7 GPIO Output
    HAL_GPIO_ConfigPullup(led_tbl[i].port, led_tbl[i].pin, PCU_PUPD_DISABLE);       // Pull-up/Pull-down Disable
  }


  #ifdef _USE_HW_CLI
  cliAdd("led", cliLed);
  #endif
  return ret;
}
void ledOn(uint8_t ch)
{
  if(ch >= LED_MAX_CH) return;

  if(led_tbl[ch].on_state == _DEF_HIGH)
  {
    HAL_GPIO_SetPin(led_tbl[ch].port, (1 << led_tbl[ch].pin));
  }
  else
  {
    HAL_GPIO_ClearPin(led_tbl[ch].port, (1 << led_tbl[ch].pin));
  }
}
void ledOff(uint8_t ch)
{
  if(ch >= LED_MAX_CH) return;

  if(led_tbl[ch].off_state == _DEF_HIGH)
  {
    HAL_GPIO_SetPin(led_tbl[ch].port, (1 << led_tbl[ch].pin));
  }
  else
  {
    HAL_GPIO_ClearPin(led_tbl[ch].port, (1 << led_tbl[ch].pin));
  }
}
void ledToggle(uint8_t ch)
{
  uint32_t pin_value = 0;;

  if(ch >= LED_MAX_CH) return;

  pin_value = led_tbl[ch].port->OUTDR;   //HAL_GPIO_EXTI_GetState(led_tbl[ch].port);

  if(pin_value & (1 << led_tbl[ch].pin))
  {
    HAL_GPIO_ClearPin(led_tbl[ch].port, (1 << led_tbl[ch].pin));
  }
  else
  {
    HAL_GPIO_SetPin(led_tbl[ch].port, (1 << led_tbl[ch].pin));
  }
}

#ifdef _USE_HW_CLI

void cliLed(cli_args_t *args)
{
  bool ret = false;

  if (args->argc == 3 && args->isStr(0, "toggle") == true)
  {
    uint8_t  led_ch;
    uint32_t toggle_time;
    uint32_t pre_time;

    led_ch      = (uint8_t)args->getData(1);
    toggle_time = (uint32_t)args->getData(2);

    if (led_ch > 0)
    {
      led_ch--;
    }

    pre_time = millis();
    while(cliKeepLoop())
    {
      if (millis()-pre_time >= toggle_time)
      {
        pre_time = millis();
        ledToggle(led_ch);
      }
    }
    ret = true;
  }



  if (ret != true)
  {
    cliPrintf("led toggle ch[1~%d] time(ms)\n", LED_MAX_CH);
  }
}
#endif


#endif

