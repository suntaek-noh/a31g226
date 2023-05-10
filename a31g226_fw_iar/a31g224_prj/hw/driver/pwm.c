/*
 * pwm.c
 *
 *  Created on: 2022. 8. 10.
 *      Author: sunta
 */

#include "pwm.h"
#include "cli.h"

#ifdef _USE_HW_PWM


static bool is_init = false;

#if 0
typedef struct
{
  TIM_HandleTypeDef  TimHandle;
  TIM_OC_InitTypeDef sConfig;
  uint32_t           channel;
} pwm_tbl_t;
#endif

//pwm_tbl_t  pwm_tbl[HW_PWM_MAX_CH];


#ifdef _USE_HW_CLI
static void cliPwm(cli_args_t *args);
#endif


bool pwmInit(void)
{
  bool ret = true;
  //pwm_tbl_t *p_pwm;
  uint32_t prescaler;

  TIMER1n_CFG_Type TIMER1n_Config;
  TIMER2n_CFG_Type TIMER2n_Config;

  prescaler = (uint32_t)((SystemCoreClock /1) / (4000 * 250)) - 1;        //48-1Mhz

  HAL_SCU_Timer1n_ClockConfig(T1NCLK_PCLK);

  TIMER1n_Config.StartSync = DISABLE;
  TIMER1n_Config.ClearSync = DISABLE;
  TIMER1n_Config.ExtClock = DISABLE;                              // PCLK 48MHz
  TIMER1n_Config.OutputPolarity = TIMER1n_OUTPUT_POLARITY_HIGH;
  TIMER1n_Config.MatchInterrupt = ENABLE;
  TIMER1n_Config.CaptureInterrupt = DISABLE;

  TIMER1n_Config.PrescalerData = prescaler;                 //

  TIMER1n_Config.AData = 250;                     //주파수 - 4Khz
  TIMER1n_Config.BData = 0;                     // 두티 high

  HAL_TIMER1n_Stop(TIMER16);
  NVIC_DisableIRQ(TIMER16_IRQn);
  HAL_TIMER1n_DeInit(TIMER16);

  HAL_GPIO_ConfigOutput(PB, 10, PCU_MODE_ALT_FUNC);             //backlight
  HAL_GPIO_ConfigFunction(PB, 10, PCU_ALT_FUNCTION_3);

  if(HAL_TIMER1n_Init(TIMER16, TIMER1n_PWM_MODE, &TIMER1n_Config) != HAL_OK)
  {
    ret = false;
  }


  HAL_SCU_Timer20_ClockConfig(T20CLK_PCLK);

  TIMER2n_Config.ExtClock = DISABLE;
  TIMER2n_Config.PrescalerData = prescaler;
  TIMER2n_Config.Mode = TIMER2n_MODE_PWM;
  TIMER2n_Config.OutputPolarity = TIMER2n_OUTPUT_POLARITY_HIGH;
  TIMER2n_Config.MatchInterrupt = ENABLE;
  TIMER2n_Config.CaptureInterrupt = DISABLE;

  TIMER2n_Config.AData = 250;                     //주파수
  TIMER2n_Config.BData = 0;                     // 두티 high

  HAL_GPIO_ConfigOutput  (PC, 0, PCU_MODE_ALT_FUNC);
  HAL_GPIO_ConfigFunction(PC, 0, PCU_ALT_FUNCTION_1);
  HAL_GPIO_ConfigOutput  (PC, 1, PCU_MODE_ALT_FUNC);
  HAL_GPIO_ConfigFunction(PC, 1, PCU_ALT_FUNCTION_1);

  HAL_TIMER2n_Init(TIMER20, &TIMER2n_Config);
  HAL_TIMER2n_Init(TIMER21, &TIMER2n_Config);

  HAL_TIMER1n_Start(TIMER16);
  HAL_TIMER2n_Start(TIMER20);
  HAL_TIMER2n_Start(TIMER21);


  is_init = ret;


#ifdef _USE_HW_CLI
  cliAdd("pwm", cliPwm);
#endif


  return ret;
}


bool pwmIsInit(void)
{
  return is_init;
}

void pwmWrite(uint8_t ch, uint16_t pwm_data)
{
  if (ch >= PWM_MAX_CH) return;

  switch(ch)
  {
    case _DEF_PWM1:
      HAL_TIMER1n_SetBData(TIMER16,pwm_data);
      //logPrintf("_DEF_PWM1 pwm_data : %d\r\n", pwm_data);
      break;

    case _DEF_PWM2:
      HAL_TIMER2n_SetBData(TIMER20, pwm_data);
      //logPrintf("_DEF_PWM2 pwm_data : %d\r\n", pwm_data);

      break;

    case _DEF_PWM3:
      HAL_TIMER2n_SetBData(TIMER21, pwm_data);
      //logPrintf("_DEF_PWM3 pwm_data : %d\r\n", pwm_data);
      break;

  }
}


uint16_t pwmRead(uint8_t ch)
{
  uint32_t ret = 0;

  if (ch >= HW_PWM_MAX_CH) return 0;


  switch(ch)
  {
    case _DEF_PWM1:
      ret = HAL_TIMER1n_GetBData(TIMER16);
      break;

    case _DEF_PWM2:
      ret = HAL_TIMER2n_GetBData(TIMER20);
      break;

    case _DEF_PWM3:
      ret = HAL_TIMER2n_GetBData(TIMER21);
      break;
  }

  return ret;
}

uint16_t pwmGetMax(uint8_t ch)
{
  uint16_t ret = 255;

  if (ch >= HW_PWM_MAX_CH) return 255;


  switch(ch)
  {
    case _DEF_PWM1:
      ret = 250;
      break;

    case _DEF_PWM2:
      ret = 250;
      break;

    case _DEF_PWM3:
       ret = 250;
       break;
  }

  return ret;
}




#ifdef _USE_HW_CLI
void cliPwm(cli_args_t *args)
{
  bool ret = true;
  uint8_t  ch;
  uint32_t pwm;


  if (args->argc == 3)
  {
    ch  = (uint8_t)args->getData(1);
    pwm = (uint8_t)args->getData(2);

    ch = constrain(ch, 0, PWM_MAX_CH);

    if(args->isStr(0, "set"))
    {
      pwm = constrain(pwm, 0, 100);
      pwmWrite(ch, CMAP(pwm, 0, 100, 0, 250));   // value 0~100%

      //pwmWrite(ch, pwm);
      cliPrintf("pwm ch%d %d\n", ch, pwm);
    }
    else
    {
      ret = false;
    }
  }
  else if (args->argc == 2)
  {
    ch = (uint8_t)args->getData(1);

    if(args->isStr(0, "get"))
    {
      cliPrintf("pwm ch%d %d\n", ch, pwmRead(ch));
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
    cliPrintf( "pwm set 0~%d 0~100%%\n", PWM_MAX_CH-1);
    cliPrintf( "pwm get 0~%d \n", PWM_MAX_CH-1);
  }

}

#endif
#endif
