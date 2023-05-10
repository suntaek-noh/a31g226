/***************************************************************************//**
* @file     A31G22x_hal_timer1n.h
* @brief    Contains all macro definitions and function prototypes support
*           for 16-bit timer1n driver on A31G22x
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

#ifndef _A31G22x_TIMER1N_H_
#define _A31G22x_TIMER1N_H_

#ifdef __cplusplus
extern "C" {
#endif


/*******************************************************************************
* Included File
*******************************************************************************/
#include "A31G22x.h"
#include "a31g22x_hal_aa_types.h"


/*******************************************************************************
* Public Macro
*******************************************************************************/


/*******************************************************************************
* Public Typedef
*******************************************************************************/

/**
 * @brief  External clock edge selection enumerated definition
 */
typedef enum {
    TIMER1n_EXT_CLOCK_EDGE_FALLING = 0x00UL, /*!< Falling edge */
    TIMER1n_EXT_CLOCK_EDGE_RISING = 0x01UL /*!< Rising edge */
} TIMER1n_EXT_CLOCK_EDGE_Type;

/**
 * @brief  Output polarity selection enumerated definition
 */
typedef enum {
    TIMER1n_OUTPUT_POLARITY_HIGH = 0x00UL, /*!< Start High level (Output is low level at disable) */
    TIMER1n_OUTPUT_POLARITY_LOW = 0x01UL /*!< Start Low level (Output is High level at disable) */
} TIMER1n_OUTPUT_POLARITY_Type;

/**
 * @brief  Capture polarity selection enumerated definition
 */
typedef enum {
    TIMER1n_CAPTURE_POLARITY_FALLING = 0x00UL, /*!< Capture on falling edge */
    TIMER1n_CAPTURE_POLARITY_RISING = 0x01UL, /*!< Capture on rising edge */
    TIMER1n_CAPTURE_POLARITY_BOTH = 0x02UL /*!< Capture on both of falling and rising edge */
} TIMER1n_CAPTURE_POLARITY_Type;

/**
 * @brief  Timer1n status enumerated definition
 */
typedef enum {
    TIMER1n_STATUS_CAPTURE_INT = (0x01UL << TIMER1n_CR_CIFLAG_Pos), /*!< Capture interrupt status */
    TIMER1n_STATUS_MATCH_INT = (0x01UL << TIMER1n_CR_MIFLAG_Pos) /*!< Match interrupt status */
} TIMER1n_STATUS_Type;

/** @brief counter clock source select */
typedef enum
{
	TIMER1n_MCCR1PCLK = 0,		/*!< clock source from pclk or mccr1 */
	TIMER1n_ECn = 1			/*!< clock source from ECn pin input. before setting, have to set ECn pin mode */	
} TIMER1n_CKSEL_MODE_OPT;

/**
 * @brief  Timer1n mode selection enumerated definition
 */
typedef enum
{
	TIMER1n_PERIODIC_MODE = 0,	/*!< PERIODIC mode */
	TIMER1n_CAPTURE_MODE,			/*!< CAPTURE mode */
	TIMER1n_ONESHOT_MODE,		/*!< ONE SHOT mode */
	TIMER1n_PWM_MODE				/*!< PWM mode */
} TIMER1n_MODE_OPT;

/**
 * @brief  Timer1n clr mode definition
 */
typedef enum
{
	TIMER1n_FALLING_EGDE=0,		/*!< falling edge clear mode */
	TIMER1n_RISING_EGDE,	/*!< rising edge clear mode */
	TIMER1n_BOTH_EGDE,			/*!< both edge clear  mode */
	TIMER1n_NONE						/*!< none clear mode */
} TIMER1n_CLR_MODE_OPT;

/** @brief start default level select: initial output value. */
typedef enum
{
	TIMER1n_START_HIGH=0,
	TIMER1n_START_LOW
} TIMER1n_STARTLVL_OPT;


/**
 * @brief  Timer1n IP index enumerated definition
 */
typedef enum {
    TIMER1n_IP_INDEX_TIMER10,
    TIMER1n_IP_INDEX_TIMER11,
    TIMER1n_IP_INDEX_TIMER12,
    TIMER1n_IP_INDEX_TIMER13,
    TIMER1n_IP_INDEX_TIMER14,
    TIMER1n_IP_INDEX_TIMER15,
    TIMER1n_IP_INDEX_TIMER16,
    TIMER1n_IP_INDEX_MAX
} TIMER1n_IP_INDEX_Type;

/**
 * @brief  Timer1n configuration structure definition
 */
typedef struct {
    /* CR */
    FunctionalState StartSync; /*!< Synchronized start counter with TIMER30 */
    FunctionalState ClearSync; /*!< Synchronized clear counter with TIMER30 */
    FunctionalState ExtClock; /*!< External clock */
    TIMER1n_EXT_CLOCK_EDGE_Type ExtClockEdge; /*!< External clock edge selection */
    TIMER1n_OUTPUT_POLARITY_Type OutputPolarity; /*!< Output polarity selection */
    TIMER1n_CAPTURE_POLARITY_Type CapturePolarity; /*!< Capture polarity selection */
    FunctionalState MatchInterrupt; /*!< Match interrupt */
    FunctionalState CaptureInterrupt; /*!< Capture interrupt */

    /* ADR */
    uint16_t AData; /*!< A data bit. The range is 0x0002 to 0xFFFF */
    /* BDR */
    uint16_t BData; /*!< B data bit. The range is 0x0000 to 0xFFFF */
    /* PREDR */
    uint16_t PrescalerData; /*!< Prescaler data bit. The range is 0x0000 to 0x0FFF. Clock = Fclk / (PRED + 1) */
} TIMER1n_CFG_Type;



/*********************************************************************
* @brief TIMER Interrupt Type definitions
**********************************************************************/
typedef enum {
	TIMER1n_INTCFG_MIE = 0,		/*!< Match Interrupt enable*/
	TIMER1n_INTCFG_CIE,			/*!< Capture Interrupt enable*/
} TIMER1n_INT_Type;


/*******************************************************************************
* Exported Public Function
*******************************************************************************/
HAL_Status_Type HAL_TIMER1n_Init(TIMER1n_Type *TIMER1n, TIMER1n_MODE_OPT TimerCounterMode, void *TIMER_ConfigStruct);
void HAL_TIMER1n_DeInit(TIMER1n_Type *pTIMER1x);

HAL_Status_Type HAL_TIMER1n_Cmd(TIMER1n_Type *TIMER1n, FunctionalState NewState);
HAL_Status_Type HAL_TIMER1n_ConfigInterrupt(TIMER1n_Type *TIMER1n, TIMER1n_INT_Type TIMERIntCfg, FunctionalState NewState);
HAL_Status_Type HAL_TIMER1n_ClearCounter(TIMER1n_Type *pTIMER1x);
uint32_t HAL_TIMER1n_GetMatchInterrupt(TIMER1n_Type *TIMER1n);
HAL_Status_Type HAL_TIMER1n_ClearMatchInterrupt(TIMER1n_Type *TIMER1n);

TIMER1n_STATUS_Type HAL_TIMER1n_GetStatus(TIMER1n_Type *pTIMER1x);
void HAL_TIMER1n_ClearStatus(TIMER1n_Type *pTIMER1x, TIMER1n_STATUS_Type Status);

uint16_t HAL_TIMER1n_GetCaptureData(TIMER1n_Type *pTIMER1x);
uint32_t HAL_TIMER1n_GetCaptureInterrupt(TIMER1n_Type *TIMER1n);
HAL_Status_Type HAL_TIMER1n_ClearCaptureInterrupt(TIMER1n_Type *TIMER1n);

uint16_t HAL_TIMER1n_GetAData(TIMER1n_Type *pTIMER1x);
void HAL_TIMER1n_SetAData(TIMER1n_Type *pTIMER1x, uint16_t Data);
uint16_t HAL_TIMER1n_GetBData(TIMER1n_Type *pTIMER1x);
void HAL_TIMER1n_SetBData(TIMER1n_Type *pTIMER1x, uint16_t Data);
uint16_t HAL_TIMER1n_GetPrescalerData(TIMER1n_Type *pTIMER1x);
void HAL_TIMER1n_SetPrescalerData(TIMER1n_Type *pTIMER1x, uint16_t Data);
uint16_t HAL_TIMER1n_GetCounter(TIMER1n_Type *pTIMER1x);
void HAL_TIMER1n_Pause(TIMER1n_Type *pTIMER1x, FunctionalState Pause);
void HAL_TIMER1n_Start(TIMER1n_Type *pTIMER1x);
void HAL_TIMER1n_Stop(TIMER1n_Type *pTIMER1x);


#ifdef __cplusplus
}
#endif

#endif /* _A31G22x_TIMER1N_H_ */
/* --------------------------------- End Of File ------------------------------ */
