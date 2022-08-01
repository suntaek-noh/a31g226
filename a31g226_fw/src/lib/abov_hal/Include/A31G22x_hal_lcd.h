/***************************************************************************//**
* @file     A31G22x_hal_lcd.h
* @brief    Contains all macro definitions and function prototypes support
*           for LCD driver on A31G22x
* @author   AE Team, ABOV Semiconductor Co., Ltd.
* @version  V1.00
* @date     28. DEC. 2020
*
* Copyright(C) 2020, ABOV Semiconductor All rights reserved.
********************************************************************************
* DISCLAIMER
*
* THE PRESENT FIRMWARE WHICH IS FOR GUIDANCE ONLY AIMS AT PROVIDING CUSTOMERS
* WITH CODING INFORMATION REGARDING THEIR PRODUCTS IN ORDER FOR THEM TO SAVE
* TIME. AS A RESULT, ABOV SEMICONDUCTOR DISCLAIMS ALL LIABILITIES FROM ANY
* DIRECT, INDIRECT OR CONSEQUENTIAL DAMAGES WITH RESPECT TO ANY CLAIMS ARISING
* FROM THE CONTENT OF SUCH FIRMWARE AND/OR THE USE MADE BY CUSTOMERS OF THE
* CODING INFORMATION CONTAINED HEREIN IN CONNECTION WITH THEIR PRODUCTS.
*
*******************************************************************************/

#ifndef _A31G22x_LCD_H_
#define _A31G22x_LCD_H_

#ifdef __cplusplus
extern "C" {
#endif


/*******************************************************************************
* Included File
*******************************************************************************/
#include "A31G22x.h"
#include "A31G22x_hal_aa_types.h"


/*******************************************************************************
* Public Macro
*******************************************************************************/
#define LCD_IP_COM_INDEX_MAX    (8)
#define LCD_IP_SEG_INDEX_MAX    (44)
#define LCD_IP_VLC_INDEX_MAX    (4)

/*******************************************************************************
* Public Typedef
*******************************************************************************/
/**
 * @brief Internal LCD bias dividing resistor option type enumeration
 */
typedef enum {
    RLCD3_105_105_80,
    RLCD1_10_10_10,
    RLCD2_66_66_50,
    RLCD4_320_320_240
} LCD_IRSEL_Type;

/**
 * @brief LCD duty and bias option type enumeration
 */
typedef enum {
    DUTY1_8_BIAS1_4,
    DUTY1_6_BIAS1_4,
    DUTY1_5_BIAS1_3,
    DUTY1_4_BIAS1_3,
    DUTY1_3_BIAS1_3,
    DUTY1_3_BIAS1_2
} LCD_DUTYBIAS_Type;

/**
 * @brief LCD clock option type enumeration
 */
typedef enum {
    LCDCLK_128Hz,
    LCDCLK_256Hz,
    LCDCLK_512Hz,
    LCDCLK_1024Hz
} LCD_CLOCK_Type;
/**
 * @brief Bias Mode A” time option type enumeration
 */
typedef enum {
    fLCD_1CLK,
    fLCD_2CLK,
    fLCD_3CLK,
    fLCD_4CLK,
    fLCD_5CLK,
    fLCD_6CLK,
    fLCD_7CLK,
    fLCD_8CLK
} LCD_BIASMODE_Type;
/**
 * @brief VLC0 Voltage Control option type enumeration when the contrast is enabled.
 */
typedef enum {
    VDD16_31_STEP,
    VDD16_30_STEP,
    VDD16_29_STEP,
    VDD16_28_STEP,
    VDD16_27_STEP,
    VDD16_26_STEP,
    VDD16_25_STEP,
    VDD16_24_STEP,
    VDD16_23_STEP,
    VDD16_22_STEP,
    VDD16_21_STEP,
    VDD16_20_STEP,
    VDD16_19_STEP,
    VDD16_18_STEP,
    VDD16_17_STEP,
    VDD16_16_STEP,
} LCD_VLC0_Type;
/**
 * @brief External bias VLC0/1/2/3 Enable option type enumeration
 */
typedef enum {
    LCD_VLC0,
    LCD_VLC1,
    LCD_VLC2,
    LCD_VLC3
} LCD_VLC_Type;

/**
 * @brief  LCD Configuration type struct definition
 */
typedef struct {
    LCD_IRSEL_Type      InternalBiasResistor;
    LCD_DUTYBIAS_Type   LcdDutyandBias;
    LCD_CLOCK_Type      LcdClock;
    FunctionalState     LcdAutoBias;
    LCD_BIASMODE_Type   BiasModeClk;
    FunctionalState     LcdContrast;
    LCD_VLC0_Type       VlcVoltage;
    FunctionalState     LcdExtBias;
    FunctionalState     LcdExtBiasPath;
} LCD_CFG_Type;

/**
 * @brief  LCD pin option type struct definition
 */
typedef struct {
    PORT_Type   *port;
    uint8_t     pin_number;
    uint8_t     pin_Name[8];
    uint8_t     *databit;   
} LCD_PORT_Type;

/*******************************************************************************
* Exported Public Function
*******************************************************************************/
void HAL_LCD_Module(FunctionalState state);
void HAL_LCD_Configuration(LCD_CFG_Type *lcdCfg);
void HAL_LCD_SetVLC(LCD_VLC_Type vlc, FunctionalState state);
void HAL_LCD_Display(FunctionalState state);
void HAL_LCD_PortInit(LCD_PORT_Type *lcdport);

#ifdef __cplusplus
}
#endif

#endif /* _A31G22x_WT_H_ */
/* --------------------------------- End Of File ------------------------------ */
