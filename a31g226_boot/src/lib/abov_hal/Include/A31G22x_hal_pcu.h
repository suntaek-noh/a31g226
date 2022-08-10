/***************************************************************************//**
* @file     A31G22x_hal_pcu.h
* @brief    Contains all macro definitions and function prototypes support
*           for PCU(Port Control Unit) driver on A31G22x
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

#ifndef _A31G22x_HAL_PCU_H_
#define _A31G22x_HAL_PCU_H_

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
#define PORT_ACCESS_EN()			do { PCU2->PORTEN = 0x15; PCU2->PORTEN = 0x51; } while(0)	/*!< PORT Register Access Enable */
#define PORT_ACCESS_DIS()			do { PCU2->PORTEN = 0x00; } while(0)                        /*!< PORT Register Access Disable */
#define PORT_ACCESS_STATUS_MASK         (1 << 8)                                                /*!< PORT Register Access Mask Bit */

#define PCU_ISR_BIT_WIDTH			(0x02UL)												    /*!< PORT Interrupt Status Bit Width */
#define PCU_ISR_BIT_MASK			(0x03UL)													/*!< PORT Interrupt Status Bit Mask */


/*******************************************************************************
* Public Typedef
*******************************************************************************/
/**
 * @brief  PCU alternative function selection enumerated definition
 */
typedef enum {
	PCU_ALT_FUNCTION_0 = 0x00UL, /*!< Alternative function 0 */
	PCU_ALT_FUNCTION_1 = 0x01UL, /*!< Alternative function 1 */
	PCU_ALT_FUNCTION_2 = 0x02UL, /*!< Alternative function 2 */
	PCU_ALT_FUNCTION_3 = 0x03UL, /*!< Alternative function 3 */
	PCU_ALT_FUNCTION_4 = 0x04UL  /*!< Alternative function 4 */
} PCU_ALT_FUNCTION_Type;

/*
 * @brief  PCU mode selection enumerated definition
 */
typedef enum {
	PCU_MODE_INPUT = 0x00UL, /*!< Input mode */
	PCU_MODE_PUSH_PULL = 0x01UL, /*!< Push-pull output mode */
	PCU_MODE_ALT_FUNC = 0x02UL, /*!< Alternative function mode */
	PCU_MODE_OPEN_DRAIN = 0x03UL /*!< Open-drain output mode */
} PCU_MODE_Type;

/*
 * @brief  PCU pull-up/down selection enumerated definition
 */
typedef enum {
	PCU_PUPD_DISABLE = 0x00UL, /*!< Disable pull-up/down */
	PCU_PUPD_PULL_UP = 0x01UL, /*!< Enable pull-up */
	PCU_PUPD_PULL_DOWN = 0x02UL, /*!< Enable pull-down */
} PCU_PUPD_Type;

/*
 * @brief  PCU interrupt mode selection enumerated definition
 */
typedef enum {
	PCU_INTERRUPT_MODE_DISABLE = 0x00UL, /*!< Disable interrupt */
	PCU_INTERRUPT_MODE_LEVEL = 0x01UL, /*!< Level interrupt */
	PCU_INTERRUPT_MODE_EDGE = 0x03UL /*!< Edge interrupt */
} PCU_INTERRUPT_MODE_Type;

/*
 * @brief  PCU interrupt control selection enumerated definition
 */
typedef enum {
	PCU_INTERRUPT_CTRL_PROHIBIT = 0x00UL, /*!< Prohibit external interrupt */
	PCU_INTERRUPT_CTRL_EDGE_FALLING = 0x01UL, /*!< Falling edge interrupt */
	PCU_INTERRUPT_CTRL_EDGE_RISING = 0x02UL, /*!< Rising edge interrupt */
	PCU_INTERRUPT_CTRL_EDGE_BOTH = 0x03UL, /*!< Both edge interrupt */
	PCU_INTERRUPT_CTRL_LEVEL_LOW = PCU_INTERRUPT_CTRL_EDGE_FALLING, /*!< Low level interrupt */
	PCU_INTERRUPT_CTRL_LEVEL_HIGH = PCU_INTERRUPT_CTRL_EDGE_RISING /*!< High level interrupt */
} PCU_INTERRUPT_CTRL_Type;

/*
 * @brief  PCU ISEG status selection enumerated definition
 */
typedef enum {
	PCU_ISEG_STATUS_FLOATING = 0x00UL, /*!< Output low */
	PCU_ISEG_STATUS_HIGH_CURRENT_LOW = 0x01UL, /*!< Output high (open-drain) */
} PCU_ISEG_STATUS_Type;

/*
 * @brief  PCU ISEG Inversion status selection enumerated definition
 */
typedef enum {
    PCU_ISEGINV_STATUS_FLOATING = 0x00UL, /*!< Output low */
    PCU_ISEGINV_STATUS_HIGH_CURRENT_LOW = 0x01UL, /*!< Output high (open-drain) */
} PCU_ISEGINV_STATUS_Type;


/*******************************************************************************
* Exported Public Function
*******************************************************************************/
void HAL_GPIO_ConfigOutput(PORT_Type *pPCUx, uint32_t PortNumber, PCU_MODE_Type Mode);
void HAL_GPIO_ConfigFunction(PORT_Type *pPCUx, uint32_t PortNumber, PCU_ALT_FUNCTION_Type Function);
void HAL_GPIO_ConfigPullup(PORT_Type *pPCUx, uint32_t PortNumber, PCU_PUPD_Type PUPD);
void HAL_GPIO_ConfigDataMask(PORT_Type *pPCUx, uint32_t PortNumber, FunctionalState Mask);
void HAL_GPIO_SetDebouncePin(PORT_Type *pPCUx, uint32_t PortNumber, FunctionalState Debounce);

void HAL_GPIO_SetPin(PORT_Type *pPCUx, uint16_t BitValue);
void HAL_GPIO_ClearPin(PORT_Type *pPCUx, uint32_t BitValue);
uint32_t HAL_GPIO_ReadPin(PORT_Type *pPCUx);
void HAL_GPIO_WritePin(PORT_Type *pPCUx, uint32_t Value);

void HAL_GPIO_EXTI_Config(PORT_Type *pPCUx, uint32_t PortNumber, PCU_INTERRUPT_MODE_Type Mode, PCU_INTERRUPT_CTRL_Type Control);
uint32_t HAL_GPIO_EXTI_GetState(PORT_Type *pPCUx);
void HAL_GPIO_EXTI_ClearPin(PORT_Type *pPCUx, uint32_t Status);

void HAL_PCU_UseISEG(FunctionalState Output);
void HAL_PCU_ConfigISEG(uint32_t ISEG_Number, PCU_ISEG_STATUS_Type Status);
void HAL_PCU_ConfigISEGInv(uint32_t ISEG_Number, PCU_ISEGINV_STATUS_Type Status);
void HAL_PCU_ConfigISEGx(uint32_t ISEGx, PCU_ISEG_STATUS_Type Status);
void HAL_PCU_ConfigISEGxInv(uint32_t ISEGx, PCU_ISEGINV_STATUS_Type Status);
uint16_t HAL_PCU_GetISEGInvStatus(void);
uint16_t HAL_PCU_GetISEGStatus(void);

void HAL_PCU_SetStrength(PORT_Type *pPCUx, uint32_t PortNumber, FunctionalState Strength);
uint8_t HAL_GPIO_GetPortEnStatus(void);




#ifdef __cplusplus
}
#endif

#endif /* _A31G22x_PCU_H_ */
/* --------------------------------- End Of File ------------------------------ */
