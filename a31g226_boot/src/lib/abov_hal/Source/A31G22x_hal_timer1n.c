/***************************************************************************//**
* @file     A31G22x_hal_timer1n.c
* @brief    Contains all functions support for 16-bit timer1n dirver on A31G22x
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
#ifdef _TIMER1n

#include "A31G22x_hal_scu.h"
#include "A31G22x_hal_timer1n.h"


/*******************************************************************************
* Private Pre-processor Definition & Macro
*******************************************************************************/
#define SCU_PER1_TIMER10_ENABLE_PERI            (0x01UL << SCU_PER1_TIMER10_Pos)
#define SCU_PCER1_TIMER10_ENABLE_CLOCK          (0x01UL << SCU_PCER1_TIMER10_Pos)

#define SCU_PER1_TIMER11_ENABLE_PERI            (0x01UL << SCU_PER1_TIMER11_Pos)
#define SCU_PCER1_TIMER11_ENABLE_CLOCK          (0x01UL << SCU_PCER1_TIMER11_Pos)

#define SCU_PER1_TIMER12_ENABLE_PERI            (0x01UL << SCU_PER1_TIMER12_Pos)
#define SCU_PCER1_TIMER12_ENABLE_CLOCK          (0x01UL << SCU_PCER1_TIMER12_Pos)

#define SCU_PER1_TIMER13_ENABLE_PERI            (0x01UL << SCU_PER1_TIMER13_Pos)
#define SCU_PCER1_TIMER13_ENABLE_CLOCK          (0x01UL << SCU_PCER1_TIMER13_Pos)

#define SCU_PER1_TIMER14_ENABLE_PERI            (0x01UL << SCU_PER1_TIMER14_Pos)
#define SCU_PCER1_TIMER14_ENABLE_CLOCK          (0x01UL << SCU_PCER1_TIMER14_Pos)

#define SCU_PER1_TIMER15_ENABLE_PERI            (0x01UL << SCU_PER1_TIMER15_Pos)
#define SCU_PCER1_TIMER15_ENABLE_CLOCK          (0x01UL << SCU_PCER1_TIMER15_Pos)

#define SCU_PER1_TIMER16_ENABLE_PERI            (0x01UL << SCU_PER1_TIMER16_Pos)
#define SCU_PCER1_TIMER16_ENABLE_CLOCK          (0x01UL << SCU_PCER1_TIMER16_Pos)


#define TIMER1n_CR_EN_ENABLE_TIMER              (0x01UL << TIMER1n_CR_EN_Pos)
#define TIMER1n_CR_EN_CLEAR_COUNTER             (0x01UL << TIMER1n_CR_CLR_Pos)

#define TIMER1n_CR_STATUS_GET_MASK              (TIMER1n_STATUS_MATCH_INT | TIMER1n_STATUS_CAPTURE_INT)
#define TIMER1n_CR_STATUS_CLEAR_MASK            (TIMER1n_STATUS_MATCH_INT | TIMER1n_STATUS_CAPTURE_INT)

/**  Timer 1n Match Interrupt Definition  */ 
#define TIMER1n_MATINTEN                    (0x1UL << 5)
#define TIMER1n_MATINTDIS                  	(0x0UL << 5)

/** Timer 1n Capture Interrupt Definition  */ 
#define TIMER1n_CAPINTEN                   	(0x1UL << 4)
#define TIMER1n_CAPINTDIS                   (0x0UL << 4)

#define TIMER1n_CR_MIFLAG                   (0x1UL << 3)
#define TIMER1n_CR_CIFLAG                   (0x1UL << 2)

/*******************************************************************************
* Private Typedef
*******************************************************************************/


/*******************************************************************************
* Private Variable
*******************************************************************************/


/*******************************************************************************
* Private Function Prototype
*******************************************************************************/

/***************************************************************************//**
* @brief      Initialize Timer1n peripheral
* @param      pTIMER1x : Timer1n peripheral selected, should be
*              - TIMER10 to TIMER16
* @param      pTIMER1x : Timer1n Mode selected, should be
*	           - TIMER1n_PERIODIC_MODE
*	           - TIMER1n_CAPTURE_MODE
*	           - TIMER1n_ONESHOT_MODE
*	           - TIMER1n_PWM_MODE
* @param      pConfig : Pointer contains configuration of Timer1n
* @return     None
*******************************************************************************/
//void HAL_TIMER1n_Init(TIMER1n_Type *pTIMER1x, TIMER1n_CFG_Type *pConfig) {
HAL_Status_Type HAL_TIMER1n_Init(TIMER1n_Type *TIMER1n, TIMER1n_MODE_OPT TimerCounterMode, void *TIMER_ConfigStruct) {

    TIMER1n_CFG_Type *pConfig;    
    
    if (TIMER1n == TIMER10) {
        SCU->PER1 &= ~SCU_PER1_TIMER10_Msk;
        SCU->PCER1 &= ~SCU_PCER1_TIMER10_Msk;

        SCU->PER1 |= SCU_PER1_TIMER10_ENABLE_PERI;
        SCU->PCER1 |= SCU_PCER1_TIMER10_ENABLE_CLOCK;

    } else if (TIMER1n == TIMER11) {
        SCU->PER1 &= ~SCU_PER1_TIMER11_Msk;
        SCU->PCER1 &= ~SCU_PCER1_TIMER11_Msk;

        SCU->PER1 |= SCU_PER1_TIMER11_ENABLE_PERI;
        SCU->PCER1 |= SCU_PCER1_TIMER11_ENABLE_CLOCK;

    } else if (TIMER1n == TIMER12) {
        SCU->PER1 &= ~SCU_PER1_TIMER12_Msk;
        SCU->PCER1 &= ~SCU_PCER1_TIMER12_Msk;

        SCU->PER1 |= SCU_PER1_TIMER12_ENABLE_PERI;
        SCU->PCER1 |= SCU_PCER1_TIMER12_ENABLE_CLOCK;

    } else if (TIMER1n == TIMER13) {
        SCU->PER1 &= ~SCU_PER1_TIMER13_Msk;
        SCU->PCER1 &= ~SCU_PCER1_TIMER13_Msk;

        SCU->PER1 |= SCU_PER1_TIMER13_ENABLE_PERI;
        SCU->PCER1 |= SCU_PCER1_TIMER13_ENABLE_CLOCK;

    } else if (TIMER1n == TIMER14) {
        SCU->PER1 &= ~SCU_PER1_TIMER14_Msk;
        SCU->PCER1 &= ~SCU_PCER1_TIMER14_Msk;

        SCU->PER1 |= SCU_PER1_TIMER14_ENABLE_PERI;
        SCU->PCER1 |= SCU_PCER1_TIMER14_ENABLE_CLOCK;

    } else if (TIMER1n == TIMER15) {
        SCU->PER1 &= ~SCU_PER1_TIMER15_Msk;
        SCU->PCER1 &= ~SCU_PCER1_TIMER15_Msk;

        SCU->PER1 |= SCU_PER1_TIMER15_ENABLE_PERI;
        SCU->PCER1 |= SCU_PCER1_TIMER15_ENABLE_CLOCK;

    } else if (TIMER1n == TIMER16) {
        SCU->PER1 &= ~SCU_PER1_TIMER16_Msk;
        SCU->PCER1 &= ~SCU_PCER1_TIMER16_Msk;

        SCU->PER1 |= SCU_PER1_TIMER16_ENABLE_PERI;
        SCU->PCER1 |= SCU_PCER1_TIMER16_ENABLE_CLOCK;
    }

    pConfig = (TIMER1n_CFG_Type *)TIMER_ConfigStruct;    
    
    // Control register
    TIMER1n->CR = 0x00UL
        | ((pConfig->StartSync << TIMER1n_CR_SSYNC_Pos) & TIMER1n_CR_SSYNC_Msk)
        | ((pConfig->ClearSync << TIMER1n_CR_CSYNC_Pos) & TIMER1n_CR_CSYNC_Msk)
        | ((pConfig->ExtClock << TIMER1n_CR_CLK_Pos) & TIMER1n_CR_CLK_Msk)
        | ((pConfig->ExtClockEdge << TIMER1n_CR_ECE_Pos) & TIMER1n_CR_ECE_Msk)
        | ((pConfig->OutputPolarity << TIMER1n_CR_OPOL_Pos) & TIMER1n_CR_OPOL_Msk)
        | ((pConfig->CapturePolarity << TIMER1n_CR_CPOL_Pos) & TIMER1n_CR_CPOL_Msk)
        | ((pConfig->MatchInterrupt << TIMER1n_CR_MIEN_Pos) & TIMER1n_CR_MIEN_Msk)
        | ((pConfig->CaptureInterrupt << TIMER1n_CR_CIEN_Pos) & TIMER1n_CR_CIEN_Msk)
        | ((0x01UL << TIMER1n_CR_CLR_Pos) & TIMER1n_CR_CLR_Msk);

    TIMER1n->CR &= ~(1 << TIMER1n_CR_MS_Pos);
    TIMER1n->CR |= (TimerCounterMode << TIMER1n_CR_MS_Pos);
    
	if (TimerCounterMode == TIMER1n_PERIODIC_MODE) {
        // A Data register
        HAL_TIMER1n_SetAData(TIMER1n, pConfig->AData);        
	} else if ((TimerCounterMode == TIMER1n_PWM_MODE) || (TimerCounterMode == TIMER1n_ONESHOT_MODE)) {        
        // A Data register
        HAL_TIMER1n_SetAData(TIMER1n, pConfig->AData);
        // B Data register
        HAL_TIMER1n_SetBData(TIMER1n, pConfig->BData);
    } else if (TimerCounterMode == TIMER1n_CAPTURE_MODE) {
        // A Data register
        HAL_TIMER1n_SetAData(TIMER1n, pConfig->AData);                
    }
    
    // Prescaler Data register
    HAL_TIMER1n_SetPrescalerData(TIMER1n, pConfig->PrescalerData);
    
    return HAL_OK;
}

/***************************************************************************//**
* @brief      De-Initialize Timer1n peripheral
* @param      pTIMER1x : Timer1n peripheral selected, should be
*              - TIMER10 to TIMER16
* @return     None
*******************************************************************************/
void HAL_TIMER1n_DeInit(TIMER1n_Type *pTIMER1x) {
    HAL_TIMER1n_Stop(pTIMER1x);

    if (pTIMER1x == TIMER10) {
        SCU->PER1 &= ~(SCU_PER1_TIMER10_Msk);
        SCU->PCER1 &= ~(SCU_PCER1_TIMER10_Msk);
    } else if (pTIMER1x == TIMER11) {
        SCU->PER1 &= ~(SCU_PER1_TIMER11_Msk);
        SCU->PCER1 &= ~(SCU_PCER1_TIMER11_Msk);
    } else if (pTIMER1x == TIMER12) {
        SCU->PER1 &= ~(SCU_PER1_TIMER12_Msk);
        SCU->PCER1 &= ~(SCU_PCER1_TIMER12_Msk);
    } else if (pTIMER1x == TIMER13) {
        SCU->PER1 &= ~(SCU_PER1_TIMER13_Msk);
        SCU->PCER1 &= ~(SCU_PCER1_TIMER13_Msk);
    } else if (pTIMER1x == TIMER14) {
        SCU->PER1 &= ~(SCU_PER1_TIMER14_Msk);
        SCU->PCER1 &= ~(SCU_PCER1_TIMER14_Msk);
    } else if (pTIMER1x == TIMER15) {
        SCU->PER1 &= ~(SCU_PER1_TIMER15_Msk);
        SCU->PCER1 &= ~(SCU_PCER1_TIMER15_Msk);
    } else if (pTIMER1x== TIMER16) {
        SCU->PER1 &= ~(SCU_PER1_TIMER16_Msk);
        SCU->PCER1 &= ~(SCU_PCER1_TIMER16_Msk);
    }
}

/***************************************************************************//**
* @brief      Get A data
* @param      pTIMER1x : Timer1n peripheral selected, should be
*              - TIMER10 to TIMER16
* @return     A data : 0x0002 to 0xFFFF
*******************************************************************************/
uint16_t HAL_TIMER1n_GetAData(TIMER1n_Type *pTIMER1x) {
    return pTIMER1x->ADR & TIMER1n_ADR_ADATA_Msk;
}

/***************************************************************************//**
* @brief      Set A data
* @param      pTIMER1x : Timer1n peripheral selected, should be
*              - TIMER10 to TIMER16
* @param      Data : A Data, should be
*              - 0x0002 to 0xFFFF
* @return     None
*******************************************************************************/
void HAL_TIMER1n_SetAData(TIMER1n_Type *pTIMER1x, uint16_t Data) {
    pTIMER1x->ADR = Data & TIMER1n_ADR_ADATA_Msk;
}

/***************************************************************************//**
* @brief      Get B data
* @param      pTIMER1x : Timer1n peripheral selected, should be
*              - TIMER10 to TIMER16
* @return     B data : 0x0000 to 0xFFFF
*******************************************************************************/
uint16_t HAL_TIMER1n_GetBData(TIMER1n_Type *pTIMER1x) {
    return pTIMER1x->BDR & TIMER1n_BDR_BDATA_Msk;
}

/***************************************************************************//**
* @brief      Set B data
* @param      pTIMER1x : Timer1n peripheral selected, should be
*              - TIMER10 to TIMER16
* @param      Data : B Data, should be
*              - 0x0000 to 0xFFFF
* @return     None
*******************************************************************************/
void HAL_TIMER1n_SetBData(TIMER1n_Type *pTIMER1x, uint16_t Data) {
    pTIMER1x->BDR = Data & TIMER1n_BDR_BDATA_Msk;
}

/***************************************************************************//**
* @brief      Get capture data
* @param      pTIMER1x : Timer1n peripheral selected, should be
*              - TIMER10 to TIMER16
* @return     Capture data : 0x0000 to 0xFFFF
*******************************************************************************/
uint16_t HAL_TIMER1n_GetCaptureData(TIMER1n_Type *pTIMER1x) {
    return pTIMER1x->CAPDR & TIMER1n_CAPDR_CAPD_Msk;
}

/***************************************************************************//**
* @brief      Get prescaler data
* @param      pTIMER1x : Timer1n peripheral selected, should be
*              - TIMER10 to TIMER16
* @return     Prescaler data : 0x0000 to 0x0FFF
*******************************************************************************/
uint16_t HAL_TIMER1n_GetPrescalerData(TIMER1n_Type *pTIMER1x) {
    return pTIMER1x->PREDR & TIMER1n_PREDR_PRED_Msk;
}

/***************************************************************************//**
* @brief      Set prescaler data
* @param      pTIMER1x : Timer1n peripheral selected, should be
*              - TIMER10 to TIMER16
* @param      Data : Prescaler Data, should be
*              - 0x0000 to 0x0FFF
* @return     None
*******************************************************************************/
void HAL_TIMER1n_SetPrescalerData(TIMER1n_Type *pTIMER1x, uint16_t Data) {
    pTIMER1x->PREDR = Data & TIMER1n_PREDR_PRED_Msk;
}

/***************************************************************************//**
* @brief      Get Counter data
* @param      pTIMER1x : Timer1n peripheral selected, should be
*              - TIMER10 to TIMER16
* @return     Counter data : 0x0000 to 0xFFFF
*******************************************************************************/
uint16_t HAL_TIMER1n_GetCounter(TIMER1n_Type *pTIMER1x) {
    return pTIMER1x->CNT & TIMER1n_CNT_CNT_Msk;
}

/***************************************************************************//**
* @brief      Clear counter and prescaler
*             Automatically cleared to “0b” after operation
* @param      pTIMER1x : Timer1n peripheral selected, should be
*              - TIMER10 to TIMER16
* @return     None
*******************************************************************************/
HAL_Status_Type HAL_TIMER1n_ClearCounter(TIMER1n_Type *pTIMER1x) {
    pTIMER1x->CR |= TIMER1n_CR_EN_CLEAR_COUNTER;
    
    return HAL_OK;
}

/***************************************************************************//**
* @brief      Get Timer1n status
* @param      pTIMER1x : Timer1n peripheral selected, should be
*              - TIMER10 to TIMER16
* @return     TIMER1n_STATUS_Type value
*              - TIMER1n_STATUS_CAPTURE_INT : Capture interrupt status
*              - TIMER1n_STATUS_MATCH_INT : Match interrupt status
*******************************************************************************/
TIMER1n_STATUS_Type HAL_TIMER1n_GetStatus(TIMER1n_Type *pTIMER1x) {
    return (TIMER1n_STATUS_Type)(pTIMER1x->CR & TIMER1n_CR_STATUS_GET_MASK);
}

/***************************************************************************//**
* @brief      Clear Timer1n status
* @param      pTIMER1x : Timer1n peripheral selected, should be
*              - TIMER10 to TIMER16
* @param      Status : Status selected, should be
*              - TIMER1n_STATUS_CAPTURE_INT : Capture interrupt status
*              - TIMER1n_STATUS_MATCH_INT : Match interrupt status
* @return     None
*******************************************************************************/
void HAL_TIMER1n_ClearStatus(TIMER1n_Type *pTIMER1x, TIMER1n_STATUS_Type Status) {
    pTIMER1x->CR |= (Status & TIMER1n_CR_STATUS_CLEAR_MASK);
}

/***************************************************************************//**
* @brief      Pause Timer1n
* @param      pTIMER1x : Timer1n peripheral selected, should be
*              - TIMER10 to TIMER16
* @param      Pause :
*              - ENABLE : Pause timer1n
*              - DISABLE : Continue timer1n
* @return     None
*******************************************************************************/
void HAL_TIMER1n_Pause(TIMER1n_Type *pTIMER1x, FunctionalState Pause) {
    volatile uint32_t Reg32;

    Reg32 = pTIMER1x->CR;
    Reg32 &= ~(TIMER1n_CR_EN_Msk);
    Reg32 |= (Pause << TIMER1n_CR_PAU_Pos);
    pTIMER1x->CR = Reg32;
}

/***************************************************************************//**
* @brief      Start Timer1n
* @param      pTIMER1x : Timer1n peripheral selected, should be
*              - TIMER10 to TIMER16
* @return     None
*******************************************************************************/
void HAL_TIMER1n_Start(TIMER1n_Type *pTIMER1x) {
    volatile uint32_t Reg32;

    Reg32 = pTIMER1x->CR;
    Reg32 &= ~TIMER1n_CR_EN_Msk;
    Reg32 |= TIMER1n_CR_EN_ENABLE_TIMER;
    pTIMER1x->CR = Reg32;
}

/***************************************************************************//**
* @brief      Stop Timer1n
* @param      pTIMER1x : Timer1n peripheral selected, should be
*              - TIMER10 to TIMER16
* @return     None
*******************************************************************************/
void HAL_TIMER1n_Stop(TIMER1n_Type *pTIMER1x) {
    pTIMER1x->CR &= ~TIMER1n_CR_EN_Msk;
}

/**********************************************************************//**
 * @brief	 	Start/Stop Timer/Counter device
 * @param[in]	TIMER1n Pointer to timer device, should be:
 * 					- TIMER1n peripheral n=0..6
 * @param[in]	NewState
 * 					- ENABLE  	:Set timer enable
 * 					- DISABLE 	:Disable timer
 * @return 		HAL Satus
 **********************************************************************/
HAL_Status_Type HAL_TIMER1n_Cmd(TIMER1n_Type *TIMER1n, FunctionalState NewState)
{
	if (NewState == ENABLE) {
		TIMER1n->CR |=  TIMER1n_CR_EN_Msk;	
	}
	else {
		TIMER1n->CR &= ~TIMER1n_CR_EN_Msk;
	}
	return HAL_OK;
}



/********************************************************************//**
 * @brief 		Enable or disable TIMER interrupt.
 * @param[in]	TIMER1n Pointer to timer device, should be:
 * 					- TIMER1n peripheral n=0..6
 * @param[in]	TIMERIntCfg	Specifies the interrupt flag,
 * 				should be one of the following:
 *						- TIMER1n_INTCFG_MIE : OVIE Interrupt enable
 *						- TIMER1n_INTCFG_CIE : MBIE Interrupt enable
 * @param[in]	NewState New state of specified TIMER1n interrupt type,
 * 				should be:
 * 					- ENALBE	:Enable this interrupt type.
 * 					- DISALBE :Disable this interrupt type.
 * @return 		HAL Satus
 *********************************************************************/
HAL_Status_Type HAL_TIMER1n_ConfigInterrupt(TIMER1n_Type *TIMER1n, TIMER1n_INT_Type TIMERIntCfg, FunctionalState NewState) {
	uint8_t tmp;

	switch(TIMERIntCfg){
		case TIMER1n_INTCFG_MIE:
			tmp = TIMER1n_MATINTEN;
			break;
		case TIMER1n_INTCFG_CIE:
			tmp = TIMER1n_CAPINTEN;
			break;			
	}

	if (NewState == ENABLE)
	{
		TIMER1n->CR |= tmp;
	}
	else // DISABLE
	{
		TIMER1n->CR &= (~tmp);
	}
	return HAL_OK;
}

/********************************************************************//**
   @brief        Get flags of Timer 1n Match Interrupt
   @param[in]    TIMER1n_Type           TIMER1n peripheral n=0..6
   @explain      This macro gets interrupt flag of Timer 1n Match Interrupt
 *********************************************************************/

uint32_t HAL_TIMER1n_GetMatchInterrupt(TIMER1n_Type *TIMER1n) {
	if (TIMER1n->CR & TIMER1n_CR_MIFLAG) {
		return (1);
	}
	else {
		return (0);
	}

}

/********************************************************************//**

   @brief        Clear flags of Timer 1n Match Interrupt
   @param[in]    TIMER1n_Type           TIMER1n peripheral n=0..6
   @explain      This macro clears interrupt flag of Timer 1n Match Interrupt
    @return 		HAL Satus
 *********************************************************************/
HAL_Status_Type HAL_TIMER1n_ClearMatchInterrupt(TIMER1n_Type *TIMER1n) {
	TIMER1n->CR |= TIMER1n_CR_MIFLAG;
    
	return HAL_OK;
}



/********************************************************************//**

   @brief        Get flags of Timer 1n Capture Interrupt
   @param[in]    TIMER1n_Type          TIMER1n peripheral n=0..6
   @explain      This macro gets interrupt flag of Timer 1n Capture Interrupt
*********************************************************************/
uint32_t HAL_TIMER1n_GetCaptureInterrupt(TIMER1n_Type *TIMER1n) {
	if (TIMER1n->CR & TIMER1n_CR_CIFLAG) {
		return 1;
	}
	else {
		return 0;
	}	
}

/********************************************************************//**

   @brief        Clear flags of Timer 1n Capture Interrupt
   @param[in]    TIMER1n_Type          TIMER1n peripheral n=0..6
   @explain      This macro clears interrupt flag of Timer 1n Capture Interrupt
   @return 		HAL Satus
 *********************************************************************/
HAL_Status_Type HAL_TIMER1n_ClearCaptureInterrupt(TIMER1n_Type *TIMER1n) {
	TIMER1n->CR |= TIMER1n_CR_CIFLAG;
    
	return HAL_OK;
}

#endif /* _TIMER1n */
/* --------------------------------- End Of File ------------------------------ */
