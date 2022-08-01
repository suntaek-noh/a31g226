/***************************************************************************//**
* @file     A31G22x_hal_cfmc_SelfWriteErase.h
* @brief    Contains all macro definitions and function prototypes support
*           for CFMC(Code Flash Main Controller) driver on A31G22x
* @author   ABOV AE Team, ABOV Semiconductor Co., Ltd.
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

#ifndef _A31G22x_HAL_CFMC_SELFWRITEERASE_H_
#define _A31G22x_HAL_CFMC_SELFWRITEERASE_H_

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

/*******************************************************************************
* Exported Public Function
*******************************************************************************/


int HAL_CFMC_SelfWrite (unsigned long addr, unsigned long size, unsigned char *buf);
int HAL_CFMC_SelfErase (unsigned long addr, unsigned long size);

#ifdef __cplusplus
}
#endif

#endif /* _A31G22x_DFMC_H_ */
/* --------------------------------- End Of File ------------------------------ */
