/***************************************************************************//**
* @file     A31G22x_hal_wdt.h
* @brief    Contains all macro definitions and function prototypes support
*           for watchdog timer driver on A31G22x
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

#ifndef _A31G22x_HAL_WDT_H_
#define _A31G22x_HAL_WDT_H_

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
#define    WDT_SR_UNFIFLAG                     (0x1UL)
#define    WDT_SR_WINMIFLAG                    (0x2UL)

/*******************************************************************************
* Public Typedef
*******************************************************************************/

/**
 * @brief  watchdog timer clock divider selection enumerated definition
 */
typedef enum {
    WDT_DIVIDER_4 = 0x00UL, /*!< WDT Clock / 4 */
    WDT_DIVIDER_16 = 0x01UL, /*!< WDT Clock / 16 */
    WDT_DIVIDER_64 = 0x02UL, /*!< WDT Clock / 64 */
    WDT_DIVIDER_256 = 0x03UL /*!< WDT Clock / 256 */
} WDT_DIVIDER_Type;

/**
 * @brief  watchdog timer status enumerated definition
 */
typedef enum {
    WDT_STATUS_UNDERFLOW_INT = (0x01UL << WDT_SR_UNFIFLAG_Pos), /*!< Underflow occurred */
    WDT_STATUS_WINDOW_MATCH_INT = (0x01UL << WDT_SR_WINMIFLAG_Pos) /*!< Window match occurred */
} WDT_STATUS_Type;

/**
 * @brief  watchdog timer underflow interrupt enumerated definition
 */
typedef enum {
    WDT_INTCFG_UNFI_DISABLE = 0,            /*!< UNFIEN Interrupt disable*/
    WDT_INTCFG_UNFI_ENABLE = 1                /*!< UNFIEN Interrupt enable*/
} WDT_INT_UNFI_Type;

/**
 * @brief  watchdog timer window match interrupt enumerated definition
 */
typedef enum {
    WDT_INTCFG_WINMI_DISABLE = 0,            /*!< WINMIEN Interrupt disable*/
    WDT_INTCFG_WINMI_ENABLE = 1            /*!< WINMIEN Interrupt enable*/
} WDT_INT_WMI_Type;

/**
 * @brief  watchdog timer reset enumerated definition
 */
typedef enum {
    WDT_RESET_DISABLE = 0,
    WDT_RESET_ENABLE = 1
} WDT_RESET_Type;

/**
 * @brief  watch timer configuration structure definition
 */
typedef struct {
    Bool EnableReset; /*!< Enable reset by watchdog timer */
    WDT_DIVIDER_Type ClockDivider; /*!< watchdog timer clock divider selection */
    uint32_t Data; /*!< Data */
    uint32_t WindowData; /*!< Window data */
} WDT_CFG_Type;


/*******************************************************************************
* Exported Public Function
*******************************************************************************/
HAL_Status_Type HAL_WDT_Init(WDT_CFG_Type *pConfig);
HAL_Status_Type HAL_WDT_DeInit(void);
HAL_Status_Type HAL_WDT_ConfigureInterrupt(WDT_INT_WMI_Type EnableWindowMatch, WDT_INT_UNFI_Type EnableUnderflow);
HAL_Status_Type HAL_WDT_ReloadTimeCounter(FunctionalState Reload);
HAL_Status_Type HAL_WDT_ClearStatus(WDT_STATUS_Type clrbit);
HAL_Status_Type HAL_WDT_Start(void);
uint32_t HAL_WDT_GetCurrentCount(void);

WDT_STATUS_Type HAL_WDT_GetStatus(void);
void HAL_WDT_SetCountValue(uint32_t data);
void HAL_WDT_CountEnableInDebug(FunctionalState Mode);

#ifdef __cplusplus
}
#endif

#endif /* _A31G22x_WDT_H_ */
/* --------------------------------- End Of File ------------------------------ */
