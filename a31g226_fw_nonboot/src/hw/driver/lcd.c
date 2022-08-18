/*
 * lcd.c
 *
 *  Created on: 2022. 8. 14.
 *      Author: sunta
 */


#include "lcd.h"
#include "cli.h"

#ifdef _USE_HW_PWM
#include "pwm.h"
#endif


#define LCD_7SEG1A  10
#define LCD_7SEG1B  0

#define LCD_7SEG2A  9
#define LCD_7SEG2B  1

#define LCD_7SEG3A  7
#define LCD_7SEG3B  8

#define LCD_7SEG4A  5
#define LCD_7SEG4B  6


// 실제 lcd seg 순번이 A(ABCD)B(FGE) or 해서 계산
const uint8_t lcd_7seg_tab[10] =
    {
        //0   //1   //2   //3   //4   //5   //6   //7   //8   //9
        0x5F, 0x06, 0x6B, 0x2f, 0x36, 0x3D, 0x7E, 0x17, 0x7F, 0x37
    };





#define _PIN_DEF_BL_CTL       9
#define LCD_DR_OFFSET_ADDR   (0x40005010UL)     // LCD->DR0

uint8_t * DATA_BUFFER2 = (uint8_t *)LCD_DR_OFFSET_ADDR;

uint8_t * DATA_BUFFER[LCD_SEG_MAX_CH];   // LCD DR BUFFER to display

typedef struct
{
  PORT_Type   *port;
  uint8_t     pin;
  char        *pin_Name;
  uint8_t     seg_num;
}lcd_t;

lcd_t lcd_com_tbl[LCD_COM_MAX_CH] =
    {
        {PE, 0, "COM0"},
        {PE, 1, "COM1"},
        {PE, 2, "COM2"},
        {PE, 3, "COM3"},
    };

lcd_t lcd_seg_tbl[LCD_SEG_MAX_CH] =
    {

#if 0
        {PE, 4, "SEG1", 1},
        {PE, 5, "SEG2", 2},
        {PE, 6, "SEG3", 3},
        {PE, 7, "SEG4", 4},
#endif
        {PD, 5, "SEG1", 7},
        {PD, 1, "SEG2", 11},
        {PD, 0, "SEG3", 12},
        {PA, 6, "SEG4", 43},

        {PC, 11, "SEG5", 14},
        {PC, 6,  "SEG6", 19},
        {PC, 5,  "SEG7", 20},
        {PC, 4,  "SEG8", 21},
        {PC, 3,  "SEG9", 22},
        {PC, 2,  "SEG10", 23},

        {PB, 9,  "SEG11", 32},
    };




static bool is_init = false;
static uint8_t backlight_value = 100;

LCD_CFG_Type          lcdConfig;

#ifdef _USE_HW_CLI
static void cliLcd(cli_args_t *args);
#endif



bool lcdInit(void)
{
  bool ret = true;

  backlight_value = 0;

  HAL_LCD_Module(ENABLE);
  HAL_SCU_LCD_ClockConfig(LCDCLK_MCCR5);          //LCDCLK_MCCR5
  HAL_SCU_MiscClockConfig(5, LCD_TYPE, CLKSRC_HSE, 8);

  HAL_LCD_Display(DISABLE);

#if 1
  for(int i=0; i<LCD_COM_MAX_CH; i++)
  {
    HAL_GPIO_ConfigOutput(lcd_com_tbl[i].port, lcd_com_tbl[i].pin, PCU_MODE_ALT_FUNC);        //
    HAL_GPIO_ConfigFunction(lcd_com_tbl[i].port, lcd_com_tbl[i].pin, PCU_ALT_FUNCTION_0);       //
    //HAL_GPIO_ConfigPullup(lcd_com_tbl[i].port, lcd_com_tbl[i].pin, PCU_PUPD_PULL_UP);
  }
#endif


  for(int i=0; i<LCD_SEG_MAX_CH; i++)
  {
    HAL_GPIO_ConfigOutput(lcd_seg_tbl[i].port, lcd_seg_tbl[i].pin, PCU_MODE_ALT_FUNC);        //
    HAL_GPIO_ConfigFunction(lcd_seg_tbl[i].port, lcd_seg_tbl[i].pin, PCU_ALT_FUNCTION_0);       //
    //HAL_GPIO_ConfigPullup(lcd_seg_tbl[i].port, lcd_seg_tbl[i].pin, PCU_PUPD_PULL_UP);


    //logPrintf("addr 0x%X\r\n", (uint8_t *)LCD_DR_OFFSET_ADDR+lcd_seg_tbl[i].seg_num);

    DATA_BUFFER[i] = (uint8_t *)(LCD_DR_OFFSET_ADDR+lcd_seg_tbl[i].seg_num);

    logPrintf("DATA_BUFFER[%d] : 0x%x\r\n", i, DATA_BUFFER[i]);

  }

  HAL_GPIO_ConfigOutput(PE, 8, PCU_MODE_ALT_FUNC);        //
  HAL_GPIO_ConfigFunction(PE, 8, PCU_ALT_FUNCTION_0);       //

  HAL_GPIO_ConfigOutput(PE, 9, PCU_MODE_ALT_FUNC);        //
  HAL_GPIO_ConfigFunction(PE, 9, PCU_ALT_FUNCTION_0);       //

  HAL_GPIO_ConfigOutput(PE, 10, PCU_MODE_ALT_FUNC);        //
  HAL_GPIO_ConfigFunction(PE, 10, PCU_ALT_FUNCTION_0);       //

  HAL_GPIO_ConfigOutput(PE, 11, PCU_MODE_ALT_FUNC);        //
  HAL_GPIO_ConfigFunction(PE, 11, PCU_ALT_FUNCTION_0);       //




  lcdConfig.InternalBiasResistor    = RLCD4_320_320_240;
  lcdConfig.LcdDutyandBias          = DUTY1_4_BIAS1_3;
  lcdConfig.LcdClock                = LCDCLK_128Hz;
  lcdConfig.LcdAutoBias             = DISABLE;
  lcdConfig.BiasModeClk             = fLCD_4CLK;
  lcdConfig.LcdExtBias              = DISABLE;            // ENABLE: External Bias, DISABLE: Internal Bias
  lcdConfig.LcdContrast             = ENABLE;            // User for Internal Bias Only
  lcdConfig.VlcVoltage              = VDD16_16_STEP;     // When VDD = 5.0V and LCD Contrast Enabled, VLC0 = (5.0 V * (16 / 25)) = 3.2 V


  if (lcdConfig.LcdExtBias == ENABLE)
  {
    lcdConfig.LcdExtBiasPath          = ENABLE;
    // Use External Bias
    HAL_LCD_SetVLC(LCD_VLC0, ENABLE);
    HAL_LCD_SetVLC(LCD_VLC1, ENABLE);
    HAL_LCD_SetVLC(LCD_VLC2, ENABLE);
    HAL_LCD_SetVLC(LCD_VLC3, ENABLE);
  }
  else
  {
    // Use Internal Bias
    lcdConfig.LcdExtBiasPath          = DISABLE;

    HAL_LCD_SetVLC(LCD_VLC0, DISABLE);
    HAL_LCD_SetVLC(LCD_VLC1, DISABLE);
    HAL_LCD_SetVLC(LCD_VLC2, DISABLE);
    HAL_LCD_SetVLC(LCD_VLC3, DISABLE);
  }


  HAL_LCD_Configuration(&lcdConfig);                      // update LCD Setting
  HAL_LCD_Display(ENABLE);

  lcdSetBackLight(backlight_value);

  is_init = true;

  lcdClearBuffer();


  #ifdef _USE_HW_CLI
  cliAdd("lcd", cliLcd);
  #endif

  return ret;
}

bool lcdIsInit(void)
{
  return is_init;
}

void lcdReset(void)
{
  //lcd.reset();
}

uint8_t lcdGetBackLight(void)
{
  return backlight_value;
}

void lcdSetBackLight(uint8_t value)
{
  value = constrain(value, 0, 100);

  if (value != backlight_value)
  {
    backlight_value = value;
  }

#ifdef _USE_HW_PWM
  pwmWrite(0, CMAP(value, 0, 100, 0, 250));   // value 0~100%
#else
  if (backlight_value > 0)
  {
    gpioPinWrite(_PIN_DEF_BL_CTL, _DEF_HIGH);
  }
  else
  {
    gpioPinWrite(_PIN_DEF_BL_CTL, _DEF_LOW);
  }
#endif
}

void lcdWrite(uint8_t index, uint8_t data)
{
  *DATA_BUFFER[index] = 0;
  *DATA_BUFFER[index] = data;
}

void lcdClearBuffer(void)
{
  for (int i=0; i<LCD_SEG_MAX_CH; i++)
  {
    lcdWrite(i, 0x00);
  }
}

void lcdAllDisp(void)
{
  for (int i=0; i<LCD_SEG_MAX_CH; i++)
  {
    lcdWrite(i, 0x0F);
  }
}


void lcd7SegDis(uint8_t pos, uint8_t data)
{

  switch(pos)
  {
    case 1:
      lcdWrite(LCD_7SEG1A, lcd_7seg_tab[data]&0x0f);
      lcdWrite(LCD_7SEG1B, (lcd_7seg_tab[data] >> 4)&0x0f);
      //logPrintf("*DATA_BUFFER[%d] : 0x%x", LCD_7SEG1A, *DATA_BUFFER[LCD_7SEG1A]);
      break;

    case 2:
      lcdWrite(LCD_7SEG2A, lcd_7seg_tab[data]&0x0f);
      lcdWrite(LCD_7SEG2B, (lcd_7seg_tab[data]&0xf0)>>4);
      break;

    case 3:
      lcdWrite(LCD_7SEG3A, lcd_7seg_tab[data]&0x0f);
      lcdWrite(LCD_7SEG3B, (lcd_7seg_tab[data]&0xf0)>>4);
      break;

    case 4:
      lcdWrite(LCD_7SEG4A, lcd_7seg_tab[data]&0x0f);
      lcdWrite(LCD_7SEG4B, (lcd_7seg_tab[data]&0xf0)>>4);
      break;
  }
}

void cliLcd(cli_args_t *args)
{
  bool ret = false;
  uint8_t index;
  uint8_t data;

  if (args->argc == 1 && args->isStr(0, "allon") == true)       //InternalBiasResistor
  {
    lcdAllDisp();
    ret = true;
  }

  if (args->argc == 1 && args->isStr(0, "alloff") == true)       //InternalBiasResistor
  {
    lcdClearBuffer();
    ret = true;
  }

  if (args->argc == 3 && args->isStr(0, "write") == true)       //InternalBiasResistor
  {
    index      = (uint8_t)args->getData(1);
    data       = (uint8_t)args->getData(2);

    lcdWrite(index, data);

    ret = true;
  }

  if (args->argc == 3 && args->isStr(0, "7seg") == true)       //InternalBiasResistor
  {
    uint8_t pos;

    pos = (uint8_t)args->getData(1);
    data = (uint8_t)args->getData(2);

    lcd7SegDis(pos, data);

    ret = true;
  }

#if 1
  if (args->argc == 2 && args->isStr(0, "internalbiasresistor") == true)       //InternalBiasResistor
  {
    index      = (uint8_t)args->getData(1);
    lcdConfig.InternalBiasResistor    = (LCD_IRSEL_Type)index;

    ret = true;
  }

  if (args->argc == 2 && args->isStr(0, "dutyandbias") == true)
  {
    index      = (uint8_t)args->getData(1);
    lcdConfig.LcdDutyandBias    = (LCD_DUTYBIAS_Type)index;

    ret = true;
  }

  if (args->argc == 2 && args->isStr(0, "clock") == true)
  {
    index      = (uint8_t)args->getData(1);
    lcdConfig.LcdClock    = (LCD_CLOCK_Type)index;

    ret = true;
  }

  if (args->argc == 2 && args->isStr(0, "biasmodeclk") == true)
  {
    index      = (uint8_t)args->getData(1);
    lcdConfig.BiasModeClk    = (LCD_BIASMODE_Type)index;

    ret = true;
  }

  if (args->argc == 2 && args->isStr(0, "vlcvoltage") == true)
  {
    index      = (uint8_t)args->getData(1);
    lcdConfig.VlcVoltage    = (LCD_VLC0_Type)index;

    ret = true;
  }

  if (args->argc == 2 && args->isStr(0, "extbias") == true)
  {
    index      = (uint8_t)args->getData(1);
    lcdConfig.LcdExtBias    = (FunctionalState)index;

    ret = true;
  }


  if (args->argc == 2 && args->isStr(0, "autobias") == true)
  {
    index      = (uint8_t)args->getData(1);
    lcdConfig.LcdAutoBias    = (FunctionalState)index;

    ret = true;
  }

  if (args->argc == 2 && args->isStr(0, "contrast") == true)
  {
    index      = (uint8_t)args->getData(1);
    lcdConfig.LcdContrast    = (FunctionalState)index;

    ret = true;
  }

  HAL_LCD_Display(DISABLE);
  HAL_LCD_Configuration(&lcdConfig);
  HAL_LCD_Display(ENABLE);
#endif

  if (ret != true)
  {
    cliPrintf("lcd allon\r\n");
    cliPrintf("lcd alloff\r\n");
    cliPrintf("lcd 7seg [1~4] data\r\n");
    cliPrintf("lcd write [0~%d], data\r\n", LCD_SEG_MAX_CH-1);

#if 1
    cliPrintf("lcd InternalBiasResistor [0~3]\r\n");
    cliPrintf("\t0 : RLCD3_105_105_80 , 1 : RLCD1_10_10_10 \r\n");
    cliPrintf("\t2 : RLCD2_66_66_50   , 3 : RLCD4_320_320_240 \r\n");

    cliPrintf("lcd dutyandbias [0~5]\r\n");
    cliPrintf("\t0 : DUTY1_8_BIAS1_4  , 1 : DUTY1_6_BIAS1_4 \r\n");
    cliPrintf("\t2 : DUTY1_5_BIAS1_3  , 3 : DUTY1_4_BIAS1_3 \r\n");
    cliPrintf("\t4 : DUTY1_3_BIAS1_3  , 5 : DUTY1_3_BIAS1_2 \r\n");

    cliPrintf("lcd clock [0~3]\r\n");
    cliPrintf("\t0 : LCDCLK_128Hz    ,  1 : LCDCLK_256Hz \r\n");
    cliPrintf("\t2 : LCDCLK_512Hz    ,  3 : LCDCLK_1024Hz \r\n");

    cliPrintf("lcd biasmodeclk [0~7]\r\n");
    cliPrintf("\t0 : fLCD_1CLK       ,  1 : fLCD_2CLK \r\n");
    cliPrintf("\t2 : fLCD_3CLK       ,  3 : fLCD_4CLK \r\n");
    cliPrintf("\t4 : fLCD_5CLK       ,  5 : fLCD_6CLK \r\n");
    cliPrintf("\t6 : fLCD_7CLK       ,  7 : fLCD_8CLK \r\n");

    cliPrintf("lcd vlcvoltage [0~15]\r\n");
    cliPrintf("\t0 : VDD16_31_STEP   ,  1 : VDD16_30_STEP \r\n");
    cliPrintf("\t2 : VDD16_29_STEP   ,  3 : VDD16_28_STEP \r\n");
    cliPrintf("\t4 : VDD16_27_STEP   ,  5 : VDD16_26_STEP \r\n");
    cliPrintf("\t6 : VDD16_25_STEP   ,  7 : VDD16_24_STEP \r\n");
    cliPrintf("\t8 : VDD16_23_STEP   ,  9 : VDD16_22_STEP \r\n");
    cliPrintf("\t10 : VDD16_21_STEP  ,  11 : VDD16_20_STEP \r\n");
    cliPrintf("\t12 : VDD16_19_STEP  ,  13 : VDD16_18_STEP \r\n");
    cliPrintf("\t14 : VDD16_17_STEP  ,  15 : VDD16_16_STEP \r\n");

    cliPrintf("lcd extbias [0~1]\r\n");
    cliPrintf("\t0 : DISABLE         ,  1 : ENABLE \r\n");

    cliPrintf("lcd autobias [0~1]\r\n");
    cliPrintf("\t0 : DISABLE         ,  1 : ENABLE \r\n");

    cliPrintf("lcd contrast [0~1]\r\n");
    cliPrintf("\t0 : DISABLE         ,  1 : ENABLE \r\n");
#endif
  }

#if 1
  cliPrintf("lcdConfig.InternalBiasResistor \t: %d\r\n", lcdConfig.InternalBiasResistor);
  cliPrintf("lcdConfig.LcdDutyandBias \t: %d\r\n", lcdConfig.LcdDutyandBias);
  cliPrintf("lcdConfig.LcdClock \t: %d\r\n", lcdConfig.LcdClock);
  cliPrintf("lcdConfig.LcdAutoBias \t: %d\r\n", lcdConfig.LcdAutoBias);
  cliPrintf("lcdConfig.BiasModeClk \t: %d\r\n", lcdConfig.BiasModeClk);
  cliPrintf("lcdConfig.LcdExtBias \t: %d\r\n", lcdConfig.LcdExtBias);
  cliPrintf("lcdConfig.LcdContrast \t: %d\r\n", lcdConfig.LcdContrast);
  cliPrintf("lcdConfig.VlcVoltage \t: %d\r\n", lcdConfig.VlcVoltage);
#endif

}

