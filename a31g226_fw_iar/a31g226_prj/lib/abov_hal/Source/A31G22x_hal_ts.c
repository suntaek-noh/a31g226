/***************************************************************************//**
* @file     A31G22x_hal_ts.c
* @brief    Contains all functions support for TS(Temperature Sensor) dirver on A31G22x
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

/*******************************************************************************
* Included File
*******************************************************************************/
#include "A31G22x_libcfg.h"
#ifdef _TS

#include "A31G22x_hal_scu.h"
#include "A31G22x_hal_ts.h"


/*******************************************************************************
* Private Pre-processor Definition & Macro
*******************************************************************************/


/*******************************************************************************
* Private Typedef
*******************************************************************************/


/*******************************************************************************
* Private Variable
*******************************************************************************/


/*******************************************************************************
* Private Function Prototype
*******************************************************************************/


/*******************************************************************************
* Public Function
*******************************************************************************/

/***************************************************************************//**
* @brief      Initialize TS(temperature sensor) peripheral
* @param      pConfig : Pointer contains configuration of TS
* @return     None
*******************************************************************************/
void HAL_TS_Init(TS_CFG_Type *pConfig) {
    // Disable TS peripheral & clock
    SCU->PER2 &= ~SCU_PER2_TS_Msk;
    SCU->PCER2 &= ~SCU_PCER2_TS_Msk;

    // Ensable TS peripheral & clock
    SCU->PER2 |= (0x01UL << SCU_PER2_TS_Pos);
    SCU->PCER2|= (0x01UL << SCU_PCER2_TS_Pos);

    TS->CR &= ~TS_CR_INTEN_Msk;
    TS->CR |= ((pConfig->TsInterrupt << TS_CR_INTEN_Pos) & TS_CR_INTEN_Msk);

    TS->RCCNT = pConfig->ReferencePeriod & TS_RCCNT_RCCV_Msk;
}

/***************************************************************************//**
* @brief      De-Initialize TS peripheral
* @return     None
*******************************************************************************/
void HAL_TS_DeInit(void) {
    HAL_TS_Stop();

    // Disable TS peripheral & clock
    SCU->PER2 &= ~SCU_PER2_TS_Msk;
    SCU->PCER2 &= ~SCU_PCER2_TS_Msk;
}

/***************************************************************************//**
* @brief      Get interrupt flag of TS
* @param      None
* @return     SET : Occurred, RESET : Not occurred
*******************************************************************************/
FlagStatus HAL_TS_GetStatus(void) {
    return (TS->SR & TS_SR_BUSY_Msk) ? RESET : SET;
}

/***************************************************************************//**
* @brief      Clear Done flag of TS
* @param      None
* @return     None
*******************************************************************************/
void HAL_TS_ClearStatus(void) {
    TS->SR |= (1 << TS_SR_DONE_Pos);   // Writing 1
}

/***************************************************************************//**
* @brief      Get sensing count of TS
* @param      None
* @return     Sensing Count : 0x00000000 to 0xFFFFFFFF
*******************************************************************************/
uint32_t HAL_TS_GetSensingCount(void) {
    return (TS->SCCNT & TS_SCCNT_SCCV_Msk);
}

/***************************************************************************//**
* @brief      Start temperature sensing
* @param      None
* @return     None
*******************************************************************************/
void HAL_TS_Start(void) {
    TS->CR |= (1 << TS_CR_START_Pos);
}

/***************************************************************************//**
* @brief      Stop temperature sensing
* @param      None
* @return     None
*******************************************************************************/
void HAL_TS_Stop(void) {
    TS->CR &= ~TS_CR_START_Msk;
}

#endif /* _TS */
/* --------------------------------- End Of File ------------------------------ */
