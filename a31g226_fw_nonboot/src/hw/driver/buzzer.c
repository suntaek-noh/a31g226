/*
 * buzzer.c
 *
 *  Created on: 2022. 8. 10.
 *      Author: sunta
 */


#include "buzzer.h"
#include "cli.h"

static bool is_on = false;
static uint32_t on_time_cnt = 0;
static uint8_t buzzer_volume = 100;

#ifdef _USE_HW_CLI
static void cliBuzzer(cli_args_t *args);
#endif


void buzzerISR(void *arg)
{
  if (is_on && on_time_cnt > 0)
  {
    on_time_cnt--;

    if (on_time_cnt == 0)
    {
      buzzerOff();
    }
  }
}

bool buzzerInit(void)
{
  bool ret = true;

  TIMER1n_CFG_Type TIMER1n_Config;

  HAL_SCU_Timer1n_ClockConfig(T1NCLK_PCLK);

  TIMER1n_Config.StartSync = DISABLE;
  TIMER1n_Config.ClearSync = DISABLE;
  TIMER1n_Config.ExtClock = DISABLE;                              // PCLK 48MHz
  TIMER1n_Config.OutputPolarity = TIMER1n_OUTPUT_POLARITY_LOW;
  TIMER1n_Config.MatchInterrupt = ENABLE;
  TIMER1n_Config.CaptureInterrupt = DISABLE;
  TIMER1n_Config.PrescalerData = (SystemCoreClock/1000000)-1; // 1us

  TIMER1n_Config.AData = 5000;//200 * (Index + 1);          //주파수
  TIMER1n_Config.BData = 2500;//100 * (Index + 1);         // 두티 high

  HAL_TIMER1n_Stop(TIMER12);
  NVIC_DisableIRQ(TIMER12_IRQn);
  HAL_TIMER1n_DeInit(TIMER12);

  HAL_GPIO_ConfigOutput(PA, 5, PCU_MODE_ALT_FUNC);
  HAL_GPIO_ConfigFunction(PA, 5, PCU_ALT_FUNCTION_1);

  if(HAL_TIMER1n_Init(TIMER12, TIMER1n_PWM_MODE, &TIMER1n_Config) != HAL_OK)
  {
     ret = false;
  }

  //HAL_TIMER1n_Start(TIMER12);
  HAL_TIMER1n_Stop(TIMER12);


#ifdef _USE_HW_CLI
  cliAdd("buzzer", cliBuzzer);
#endif
  return ret;
}

void buzzerOn(uint16_t freq_hz, uint16_t time_ms)
{
  //TIMER1n_CFG_Type TIMER1n_Config;

  uint32_t pwm_top;
  //uint16_t pwm_duty;


  if (freq_hz == 0) return;

  pwm_top = 1000000/freq_hz;
  HAL_TIMER1n_SetAData(TIMER12, pwm_top);             //주파수
  //pwm_duty = HAL_TIMER1n_GetBData(TIMER12);         // 두티 high
  HAL_TIMER1n_SetBData(TIMER12,pwm_top/2);

  HAL_TIMER1n_Start(TIMER12);

  is_on = true;
  on_time_cnt = time_ms;
}

void buzzerBeep(uint16_t time_ms)
{
  buzzerOn(1000, time_ms);
}

void buzzerOff(void)
{
  HAL_TIMER1n_Stop(TIMER12);
  is_on = false;
}

void buzzerSetVolume(uint8_t volume)
{
  buzzer_volume = constrain(volume, 0, 100);
}

uint8_t buzzerGetVolume(void)
{
  return buzzer_volume;
}

#ifdef _USE_HW_CLI
void cliBuzzer(cli_args_t *args)
{
  bool ret = false;


  if (args->argc == 3 && args->isStr(0, "on"))
  {
    uint16_t freq_hz;
    uint16_t time_ms;

    freq_hz = args->getData(1);
    time_ms = args->getData(2);

    buzzerOn(freq_hz, time_ms);
    ret = true;
  }

  if (args->argc == 1 && args->isStr(0, "off"))
  {
    buzzerOff();
    ret = true;
  }

  if (args->argc == 1 && args->isStr(0, "test"))
  {
    uint16_t freq_tbl[8] = {261, 293, 329, 349, 391, 440, 493, 523};
    uint8_t freq_i;

    for (int i=0; i<16; i++)
    {
      if (i/8 == 0)
      {
        freq_i = i%8;
      }
      else
      {
        freq_i = 7 - (i%8);
      }
      buzzerOn(freq_tbl[freq_i], 150);
      cliPrintf("%dHz, %dms\n", freq_tbl[freq_i], 100);
      delay(300);
    }
    ret = true;
  }

  if (args->argc == 1 && args->isStr(0, "melody"))
  {
    uint16_t melody[] = {NOTE_C4, NOTE_G3, NOTE_G3, NOTE_A3, NOTE_G3, 0, NOTE_B3, NOTE_C4};
    int note_durations[] = { 4, 8, 8, 4, 4, 4, 4, 4 };

    for (int i=0; i<8; i++)
    {
      int note_duration = 1000 / note_durations[i];

      buzzerOn(melody[i], note_duration);
      delay(note_duration * 1.30);
    }
    ret = true;
  }
  if (ret != true)
  {
    cliPrintf("buzzer on freq(32~500000) time_ms\n");
    cliPrintf("buzzer off\n");
    cliPrintf("buzzer test\n");
    cliPrintf("buzzer melody\n");
  }
}
#endif
