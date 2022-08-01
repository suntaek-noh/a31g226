/*************************************************************************
* @file		A31G22x_it.c
* @brief	Contains all macro definitions and function prototypes
*           for system clock initialization
* @version	1.00
* @date		22 July 2020
* @author	ABOV PTS team
*
* Copyright(C) 2020, ABOV Semiconductor All rights reserved.
*
**************************************************************************/
#include "hw_def.h"

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config (void);
/* Private variables ---------------------------------------------------------*/

//#define USED_LSI		
//#define USED_LSE		
//#define USED_HSI		
//#define USED_HSE		
//#define USED_HSIPLL
#define USED_HSEPLL

/**************************************************************************
 * @brief			Initialize default clock for A33M11x Board
 * @param[in]		None
 * @return			None
 **************************************************************************/
void SystemClock_Config (void)
{

    HAL_SCU_ClockOutput(4, ENABLE);  // Enable Main CLK output via CLKO(PF4) port

#ifdef USED_LSI
    HAL_SCU_SetSystemClock(SYSCLK_LSI, SYSDIV_NONE);  // Use LSI clock (500kHz)
#endif

	#ifdef USED_LSE
    HAL_SCU_SetSystemClock(SYSCLK_LSE, SYSDIV_NONE);  // Use LSE clock (32.768kHz)
#endif

#ifdef USED_HSI
    HAL_SCU_SetSystemClock(SYSCLK_HSI, SYSDIV_NONE);  // Use HSI clock (32MHz)
#endif

#ifdef USED_HSE
    HAL_SCU_SetSystemClock(SYSCLK_HSE, SYSDIV_NONE);  // Use HSE clock (XTAL)
#endif


#ifdef USED_HSIPLL
        HAL_SCU_SetPLLFreq(IRC32MHz, PLL48MHz);     // Use PLL with HSI clock
#endif

#ifdef USED_HSEPLL
    HAL_SCU_SetPLLFreq(XTAL8MHz, PLL48MHz);   // Use PLL with HSE clock
#endif


}
