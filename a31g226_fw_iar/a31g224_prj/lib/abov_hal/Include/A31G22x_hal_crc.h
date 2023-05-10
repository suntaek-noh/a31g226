/***************************************************************************//**
* @file     A31G22x_hal_crc.h
* @brief    Contains all macro definitions and function prototypes support
*           for CRC driver on A31G22x
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

#ifndef _A31G22x_CRC_H_
#define _A31G22x_CRC_H_

#ifdef __cplusplus
extern "C" {
#endif


/*******************************************************************************
* Included File
*******************************************************************************/
#include "A31G22x.h"
#include "a31G22x_hal_aa_types.h"


/*******************************************************************************
* Public Macro
*******************************************************************************/


/*******************************************************************************
* Public Typedef
*******************************************************************************/
/**
 * @brief  CRC polynomial enumerated definition. CRC mode only
 */
typedef enum {
    CRC_POLY_CRC32, /*!< CRC-32 */
    CRC_POLY_CRC16, /*!< CRC-16 */
    CRC_POLY_CRC8, /*!< CRC-8 */
    CRC_POLY_CRC7, /*!< CRC-7 */
} CRC_POLY_Type;

/**
 * @brief  CRC first bit selection enumerated definition
 */
typedef enum {
    CRC_FIRST_BIT_LSB, /*!< LSB-first */
    CRC_FIRST_BIT_MSB, /*!< MSB-first */
} CRC_FIRST_BIT_Type;

/**
 * @brief  CRC configuration structure definition
 */
typedef struct {
    /* CR */
    FunctionalState InverseOutput; /*! Inverse CRC Output data */
    FunctionalState ReverseOutput; /*! Reverse CRC Output data */
    CRC_FIRST_BIT_Type FirstBitInput; /*! First Bit of Input Data */
    FunctionalState DMADoneInterrupt; /*! DMA Done Interrupt */
    CRC_POLY_Type Polynomial; /*!< CRC Polynomial (CRC Mode only) */
} CRC_CFG_Type;


/*******************************************************************************
* Exported Public Function
*******************************************************************************/
void HAL_CRC_Init(CRC_CFG_Type *pConfig);
void HAL_CRC_DeInit(void);
void HAL_CRC_SetInitVal(uint32_t Init);
void HAL_CRC_ApplyInitVal(void);
void HAL_CRC_SetInputData8(uint8_t Data);
uint32_t HAL_CRC_GetOutputData(void);
FlagStatus HAL_CRC_GetInterruptStatus(void);
void HAL_CRC_ClearInterruptStatus(void);

#ifdef __cplusplus
}
#endif

#endif /* _A31G22x_CRC_H_ */
/* --------------------------------- End Of File ------------------------------ */
