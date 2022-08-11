/*
 * ir_remote.c
 *
 *  Created on: 2022. 8. 11.
 *      Author: sunta
 */

#include "ir_remote.h"
#include "cli.h"
#include "qblock.h"


#define IR_REMOTE_BIT_BUF_MAX       32
#define US_TO_TIME(x)               (((x)*10L)/10)  // Clock 1Mhz


typedef enum
{
  IR_REMOTE_BIT_NORMAL,
  IR_REMOTE_BIT_REPEAT,
  IR_REMOTE_BIT_HIGH,
  IR_REMOTE_BIT_LOW,
  IR_REMOTE_BIT_LAST,
  IR_REMOTE_BIT_MAX,
} IrRemoteBitValue_t;



typedef struct
{
  uint16_t on_time;
  uint16_t total_time;
} ir_remote_bit_t;

typedef struct
{
  IrRemoteBitValue_t value;
  uint16_t on_time;
  uint16_t on_error;
  uint16_t total_time;
  uint16_t total_error;
} ir_remote_bit_ref_t;


static ir_remote_bit_t bit_buf[IR_REMOTE_BIT_BUF_MAX];
static qblock_t rx_bit_q;


#ifdef _USE_HW_CLI
static void cliIrRemote(cli_args_t *args);
#endif


ir_remote_bit_ref_t bit_ref[] =
{
  {IR_REMOTE_BIT_NORMAL, US_TO_TIME(9000), US_TO_TIME(500), US_TO_TIME(13500), US_TO_TIME(500)},
  {IR_REMOTE_BIT_REPEAT, US_TO_TIME(9000), US_TO_TIME(500), US_TO_TIME(11250), US_TO_TIME(500)},
  {IR_REMOTE_BIT_HIGH,   US_TO_TIME(560),  US_TO_TIME(100), US_TO_TIME(2250),  US_TO_TIME(100)},
  {IR_REMOTE_BIT_LOW,    US_TO_TIME(560),  US_TO_TIME(100), US_TO_TIME(1120),  US_TO_TIME(100)},
  {IR_REMOTE_BIT_LAST,   0,  0, 0, 0},
};


IrRemoteBitValue_t irRemoteGetBit(ir_remote_bit_ref_t *p_ref, ir_remote_bit_t *p_bit);

bool irRemoteInit(void)
{
  bool ret = true;

  TIMER1n_CFG_Type TIMER1n_Config;

  qblockCreateBySize(&rx_bit_q, (uint8_t *)bit_buf, sizeof(ir_remote_bit_t), IR_REMOTE_BIT_BUF_MAX);

  HAL_SCU_Timer1n_ClockConfig(T1NCLK_PCLK);

  HAL_SCU_MiscClockConfig(4, PD0_TYPE, CLKSRC_LSI, 5); // 1 clock = 10us
  //HAL_SCU_MiscClockConfig(1, TIMER1n_TYPE, CLKSRC_PLL, 5); // 1 clock = 10us


  TIMER1n_Config.StartSync = DISABLE;
  TIMER1n_Config.ClearSync = DISABLE;
  TIMER1n_Config.ExtClock = DISABLE; // PCLK
  TIMER1n_Config.CapturePolarity = TIMER1n_CAPTURE_POLARITY_FALLING;
  TIMER1n_Config.MatchInterrupt = DISABLE;
  TIMER1n_Config.CaptureInterrupt = ENABLE;

  TIMER1n_Config.AData = 0xFFFF;
  TIMER1n_Config.PrescalerData = 48 - 1; //   1.5Mhz

  HAL_GPIO_ConfigOutput  (PB, 11, PCU_MODE_ALT_FUNC);
  HAL_GPIO_ConfigFunction(PB, 11, PCU_ALT_FUNCTION_1);
  HAL_GPIO_SetDebouncePin(PB, 11, ENABLE);

  NVIC_SetPriority(TIMER15_IRQn, 5);
  NVIC_EnableIRQ(TIMER15_IRQn);

  if(HAL_TIMER1n_Init(TIMER15, TIMER1n_CAPTURE_MODE, &TIMER1n_Config) != HAL_OK)
  {
    ret = false;
  }

  HAL_TIMER1n_ClearCounter(TIMER15);
  HAL_TIMER1n_Start(TIMER15);

#ifdef _USE_HW_CLI
  cliAdd("ir_remote", cliIrRemote);
#endif

  return ret;
}


IrRemoteBitValue_t irRemoteGetBit(ir_remote_bit_ref_t *p_ref, ir_remote_bit_t *p_bit)
{
  IrRemoteBitValue_t ret = IR_REMOTE_BIT_LAST;

  for (int i=0; i<IR_REMOTE_BIT_LAST; i++)
  {
    bool in_range = true;

    //if (p_bit->on_time <= (p_ref[i].on_time - p_ref[i].on_error)) in_range = false;
    //if (p_bit->on_time >= (p_ref[i].on_time + p_ref[i].on_error)) in_range = false;

    //logPrintf("p_bit->total_time <= (p_ref[i].total_time - p_ref[i].total_error / %d <= %d-%d\r\n", p_bit->total_time, p_ref[i].total_time, p_ref[i].total_error);
    //logPrintf("p_bit->total_time >= (p_ref[i].total_time + p_ref[i].total_error / %d <= %d-%d\r\n", p_bit->total_time, p_ref[i].total_time, p_ref[i].total_error);

    if (p_bit->total_time <= (p_ref[i].total_time - p_ref[i].total_error)) in_range = false;
    if (p_bit->total_time >= (p_ref[i].total_time + p_ref[i].total_error)) in_range = false;

    if (in_range == true)
    {
      ret = p_ref[i].value;
      break;
    }
  }

  return ret;
}

void TIMER15_IRQHandler(void)
{
  ir_remote_bit_t bit_time;

  if (HAL_TIMER1n_GetStatus(TIMER15) & TIMER1n_STATUS_CAPTURE_INT)
  {
    bit_time.on_time = HAL_TIMER1n_GetCaptureData(TIMER15);
    bit_time.total_time = HAL_TIMER1n_GetCaptureData(TIMER15);


    //logPrintf("on_time : %d\r\n",bit_time.on_time);
    //logPrintf("total_time : %d\r\n",bit_time.total_time);

    qblockWrite(&rx_bit_q, (uint8_t *)&bit_time, 1);

    HAL_TIMER1n_ClearStatus(TIMER15, TIMER1n_STATUS_CAPTURE_INT);
  }


}


#ifdef _USE_HW_CLI
void cliIrRemote(cli_args_t *args)
{
  bool ret = false;


  if (args->argc == 1 && args->isStr(0, "bits") == true)
  {
    uint16_t index = 0;
    ir_remote_bit_t data;

    while(cliKeepLoop())
    {
      if (qblockAvailable(&rx_bit_q) > 0)
      {
        qblockRead(&rx_bit_q, (uint8_t *)&data, 1);

        IrRemoteBitValue_t bit_value;
        bit_value = irRemoteGetBit(bit_ref, &data);
        //logPrintf("bit_value : %d\r\n", bit_value);
        if (bit_value != IR_REMOTE_BIT_LAST)
        {
          if (bit_value == IR_REMOTE_BIT_NORMAL || bit_value == IR_REMOTE_BIT_REPEAT)
          {
            index = 0;
          }

          cliPrintf("%4d : on %5d, t %5d, bit : ", index++, data.on_time, data.total_time);

          if (bit_value == IR_REMOTE_BIT_NORMAL) cliPrintf("Normal");
          if (bit_value == IR_REMOTE_BIT_REPEAT) cliPrintf("Repeat");
          if (bit_value == IR_REMOTE_BIT_HIGH)   cliPrintf("1");
          if (bit_value == IR_REMOTE_BIT_LOW)    cliPrintf("0");
          cliPrintf("\n");
        }
      }
    }
    ret = true;
  }

  if (ret != true)
  {
    cliPrintf("ir_remote bits\n");
  }
}
#endif
