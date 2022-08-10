/***************************************************************************//**
* @file     system_A31G22x.h
* @brief    Contains all macro definitions and function prototypes support
*           for System Initialization
* @author   AE Team, ABOV Semiconductor Co., Ltd.
* @version  V0.0.1
* @date     13. May. 2019
*
* Copyright(C) 2019, ABOV Semiconductor
* All rights reserved.
*
*
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
#ifndef SYSTEM_A31G22x_H
#define SYSTEM_A31G22x_H

#ifdef __cplusplus
extern "C" {
#endif

#include <stdint.h>

/*----------------------------------------------------------------------------
  DEFINES
 *----------------------------------------------------------------------------*/
#define LSI_CLOCK           (500000)    // The clock speed of Internal Low Speed Oscillator
#define LSE_CLOCK           (32768)     // The clock speed of External Low Speed Oscillator (SXTAL)
#define HSI_CLOCK           (32000000)  // The clock speed of Internal High Speed Oscillator
#define HSE_CLOCK           (8000000)   // The clock speed of External High Speed Oscillator(XTAL)


/*----------------------------------------------------------------------------
  Clock Variable definitions
 *----------------------------------------------------------------------------*/
extern uint32_t SystemCoreClock;     /*!< System Clock Frequency (Core Clock)  */
extern uint32_t SystemPeriClock;     /*!< System Clock Frequency (Peri Clock)  */

/**
 * Initialize the system
 *
 * @param  none
 * @return none
 *
 * @brief  Setup the microcontroller system.
 *         Initialize the System and update the SystemCoreClock variable.
 */
extern void SystemInit(void);

///**
// * Initialize the system
// *
// * @param  none
// * @return none
// *
// * @brief  Setup the microcontroller system.
// *         Initialize the System and update the SystemCoreClock variable.
// */
//extern void SystemCoreClockUpdate(void);

#ifdef __cplusplus
}
#endif

#endif /* SYSTEM_A31G22x_H */
