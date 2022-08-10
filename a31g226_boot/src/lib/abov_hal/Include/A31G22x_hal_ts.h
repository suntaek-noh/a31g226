/***************************************************************************//**
* @file     A31G22x_hal_ts.h
* @brief    Contains all macro definitions and function prototypes support
*           for Temp Sensor driver on A31G22x
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

#ifndef _A31G22x_HAL_TS_H_
#define _A31G22x_HAL_TS_H_

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


/*******************************************************************************
* Public Typedef
*******************************************************************************/
/**
 * @brief  TS configuration structure definition
 */
typedef struct {
    /* CR */
    FunctionalState TsInterrupt; /*!< Ts Interrupt */
    /* REFPERIOD */
    uint32_t ReferencePeriod; /*!< Reference Period, 0x00000 ~ 0xFFFFF */
} TS_CFG_Type;

/*******************************************************************************
* Exported Public Function
*******************************************************************************/
void HAL_TS_Init(TS_CFG_Type *pConfig);
void HAL_TS_DeInit(void);
FlagStatus HAL_TS_GetStatus(void);
void HAL_TS_ClearStatus(void);
uint32_t HAL_TS_GetSensingCount(void);
void HAL_TS_Start(void);
void HAL_TS_Stop(void);


#ifdef __cplusplus
}
#endif

#endif /* _A31G22x_TS_H_ */
/* --------------------------------- End Of File ------------------------------ */
