/***************************************************************************//**
* @file     A31G22x_hal_pcu.c
* @brief    Contains all functions support for PCU(Port Control Unit) dirver
*           on A31G22x
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
#ifdef _GPIO

#include "A31G22x_hal_pcu.h"


/*******************************************************************************
* Private Pre-processor Definition & Macro
*******************************************************************************/
#define PCU_MOD_BIT_WIDTH				(0x02UL)
#define PCU_MOD_BIT_MASK				(0x03UL)

#define PCU_TYP_BIT_MASK				(0x01UL)
#define PCU_TYP_OPEN_DRAIN				(0x01UL)

#define PCU_AFSR_BIT_WIDTH				(0x04UL)
#define PCU_AFSR_BIT_MASK				(0x0FUL)

#define PCU_PUPD_BIT_WIDTH				(0x02UL)
#define PCU_PUPD_BIT_MASK				(0x03UL)

#define PCU_OUTDMSK_BIT_MASK			(0x01UL)

#define PCU_DBCR_BIT_MASK				(0x01UL)

#define PCU_IER_BIT_WIDTH				(0x02UL)
#define PCU_IER_BIT_MASK				(0x03UL)

#define PCU_ICR_BIT_WIDTH				(0x02UL)
#define PCU_ICR_BIT_MASK				(0x03UL)

#define PCU2_ISEGKEY_ISEGENKEY_ENABLE	(0x0702)
#define PCU2_ISEGKEY_ISEGENKEY_DISABLE	(0x0000)
#define PCU2_ISEG_BIT_MASK			    (0x3FFUL)

#define PCU_STR_BIT_MASK                (0x03UL)
#define PCU_STR_BIT_WIDTH               (0x02UL)

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
* @brief      Configure mode of port n
* @param      pPCUx : Pointer to selected PCU peripheral, should be
*              - PA ~ PF : PCUx peripheral
* @param      PortNumber : Port number, should be 0 ~ 15
* @param      Mode : Port Mode, should be
*              - PCU_MODE_INPUT : Input mode
*              - PCU_MODE_PUSH_PULL : Push-pull output mode
*              - PCU_MODE_ALT_FUNC : Alternative function mode
*              - PCU_MODE_OPEN_DRAIN : Open-drain output mode
* @return     None
*******************************************************************************/
void HAL_GPIO_ConfigOutput(PORT_Type *pPCUx, uint32_t PortNumber, PCU_MODE_Type Mode)
{
	uint32_t Offset;
	volatile uint32_t Reg32;
	PCU_MODE_Type Temp;

	Temp = Mode;
	if (Mode == PCU_MODE_OPEN_DRAIN) {
		Mode = PCU_MODE_PUSH_PULL;
	}

    // Check PORT Control Access Status
    if (!HAL_GPIO_GetPortEnStatus()) {
    }

	// MOD
	Offset = PortNumber * PCU_MOD_BIT_WIDTH;
	Reg32 = pPCUx->MOD;
	Reg32 &= ~(PCU_MOD_BIT_MASK << Offset);
	Reg32 |= (Mode << Offset);
	pPCUx->MOD = Reg32;

	// TYP
	Reg32 = pPCUx->TYP;
	Reg32 &= ~(PCU_TYP_BIT_MASK << PortNumber);
	if (Temp == PCU_MODE_OPEN_DRAIN) {
		Reg32 |= (PCU_TYP_OPEN_DRAIN << PortNumber);
	}
	pPCUx->TYP = Reg32;
}

/***************************************************************************//**
* @brief      Configure alternative function of port n
* @param      pPCUx : Pointer to selected PCU peripheral, should be
*              - PA ~ PF : PCUx peripheral
* @param      PortNumber : Port number, should be 0 ~ 15
* @param      Function : function mode, should be
*              - PCU_ALT_FUNCTION_0 : Alternative function 0
*              - PCU_ALT_FUNCTION_1 : Alternative function 1
*              - PCU_ALT_FUNCTION_2 : Alternative function 2
*              - PCU_ALT_FUNCTION_3 : Alternative function 3
*              - PCU_ALT_FUNCTION_4 : Alternative function 4
* @return     None
*******************************************************************************/
void HAL_GPIO_ConfigFunction(PORT_Type *pPCUx, uint32_t PortNumber, PCU_ALT_FUNCTION_Type Function)
{
	uint32_t Offset;
	volatile uint32_t Reg32;

    // Check PORT Control Access Status
    if (!HAL_GPIO_GetPortEnStatus()) {
    }

	if (PortNumber < 8) { // 0 ~ 7
		Offset = PortNumber * PCU_AFSR_BIT_WIDTH;

		Reg32 = pPCUx->AFSR1;
		Reg32 &= ~(PCU_AFSR_BIT_MASK << Offset);
		Reg32 |= (Function << Offset);

		pPCUx->AFSR1 = Reg32;
	} else { // 8~15
		Offset = (PortNumber - 8) * PCU_AFSR_BIT_WIDTH;

		Reg32 = pPCUx->AFSR2;
		Reg32 &= ~(PCU_AFSR_BIT_MASK << Offset);
		Reg32 |= (Function << Offset);

		pPCUx->AFSR2 = Reg32;
	}
}

/***************************************************************************//**
* @brief      Configure pull-up/down of port n
* @param      pPCUx : Pointer to selected PCU peripheral, should be
*              - PA ~ PF : PCUx peripheral
* @param      PortNumber : Port number, should be 0 ~ 15
* @param      PUPD : Pull-up/down mode, should be
*              - PCU_PUPD_DISABLE : Disable pull-up/down
*              - PCU_PUPD_PULL_UP : Enable pull-up
*              - PCU_PUPD_PULL_DOWN : Enable pull-down
* @return     None
*******************************************************************************/
void HAL_GPIO_ConfigPullup(PORT_Type *pPCUx, uint32_t PortNumber, PCU_PUPD_Type PUPD)
{
	uint32_t Offset;
	volatile uint32_t Reg32;

    // Check PORT Control Access Status
    if (!HAL_GPIO_GetPortEnStatus()) {
    }

	Offset = PortNumber * PCU_PUPD_BIT_WIDTH;

	Reg32 = pPCUx->PUPD;
	Reg32 &= ~(PCU_PUPD_BIT_MASK << Offset);
	Reg32 |= (PUPD << Offset);
	pPCUx->PUPD = Reg32;
}

/***************************************************************************//**
* @brief      Configure output data mask of port n
* @param      pPCUx : Pointer to selected PCU peripheral, should be
*              - PA ~ PF : PCUx peripheral
* @param      PortNumber : Port number, should be 0 ~ 15
* @param      Mask :
*              - TRUE : Enable output data mask
*              - FALSE : Disable output data mask
* @return     None
*******************************************************************************/
void HAL_GPIO_ConfigDataMask(PORT_Type *pPCUx, uint32_t PortNumber, FunctionalState Mask)
{
	volatile uint32_t Reg32;

	Reg32 = pPCUx->OUTDMSK;
	Reg32 &= ~(PCU_OUTDMSK_BIT_MASK << PortNumber);
	Reg32 |= (Mask << PortNumber);
	pPCUx->OUTDMSK = Reg32;
}

/***************************************************************************//**
* @brief      Configure debounce of port n
* @param      pPCUx : Pointer to selected PCU peripheral, should be
*              - PA ~ PF : PCUx peripheral
* @param      PortNumber : Port number, should be 0 ~ 15
* @param      Debounce :
*              - TRUE : Enable debounce
*              - FALSE : Disable debounce
* @return     None
*******************************************************************************/
void HAL_GPIO_SetDebouncePin(PORT_Type *pPCUx, uint32_t PortNumber, FunctionalState Debounce)
{
	volatile uint32_t Reg32;

    // Check PORT Control Access Status
    if (!HAL_GPIO_GetPortEnStatus()) {
    }

	Reg32 = pPCUx->DBCR;
	Reg32 &= ~(PCU_DBCR_BIT_MASK << PortNumber);
	Reg32 |= (Debounce << PortNumber);
	pPCUx->DBCR = Reg32;
}

/***************************************************************************//**
* @brief      Configure ISEGR Registor to control a ISEG port
* @param      ISEG_Number : ISEG number, should be 0 ~ 9
* @param      Status : ISEG status, should be
*              - PCU_ISEG_STATUS_FLOATING : Floating
*              - PCU_ISEG_STATUS_HIGH_CURRENT_LOW : High Current low
* @return     None
*******************************************************************************/
void HAL_PCU_ConfigISEG(uint32_t ISEG_Number, PCU_ISEG_STATUS_Type Status)
{
	volatile uint32_t Reg32;

	Reg32 = PCU2->ISEGIR;
	Reg32 &= ~(1 << ISEG_Number);
	Reg32 |= (Status << ISEG_Number);
	PCU2->ISEGIR = Reg32;
}

/***************************************************************************//**
* @brief      Configure ISEGIR Registor to control a ISEG port
* @param      ISEG_Number : ISEG number, should be 0 ~ 9
* @param      Status : ISEG status, should be
*              - PCU_ISEG_STATUS_FLOATING : Floating
*              - PCU_ISEG_STATUS_HIGH_CURRENT_LOW : High Current low
* @return     None
*******************************************************************************/
void HAL_PCU_ConfigISEGInv(uint32_t ISEG_Number, PCU_ISEGINV_STATUS_Type Status)
{
	volatile uint32_t Reg32;

	Reg32 = PCU2->ISEGIR;
	Reg32 &= ~(1 << ISEG_Number);
	Reg32 |= (Status << ISEG_Number);
	PCU2->ISEGIR = Reg32;
}


/***************************************************************************//**
* @brief      Configure ISEGR Registor to control ISEGx ports
* @param      ISEG_Number : ISEG number, should be 0 ~ 9
* @param      Status : ISEG status, should be
*              - PCU_ISEG_STATUS_FLOATING : Floating
*              - PCU_ISEG_STATUS_HIGH_CURRENT_LOW : High Current low
* @return     None
*******************************************************************************/
void HAL_PCU_ConfigISEGx(uint32_t ISEGx, PCU_ISEG_STATUS_Type Status)
{
	volatile uint32_t Reg32;

	Reg32 = PCU2->ISEGR;
	Reg32 &= ~(ISEGx & PCU2_ISEG_BIT_MASK);
	Reg32 |= (Status << (ISEGx & PCU2_ISEG_BIT_MASK));
	PCU2->ISEGR = Reg32;
}

/***************************************************************************//**
* @brief      Configure ISEGIR Registor to control ISEGx ports
* @param      ISEG_Number : ISEG number, should be 0 ~ 9
* @param      Status : ISEG status, should be
*              - PCU_ISEG_STATUS_FLOATING : Floating
*              - PCU_ISEG_STATUS_HIGH_CURRENT_LOW : High Current low
* @return     None
*******************************************************************************/
void HAL_PCU_ConfigISEGxInv(uint32_t ISEGx, PCU_ISEGINV_STATUS_Type Status)
{
	volatile uint32_t Reg32;

	Reg32 = PCU2->ISEGIR;
	Reg32 &= ~(ISEGx & PCU2_ISEG_BIT_MASK);
	Reg32 |= (Status << (ISEGx & PCU2_ISEG_BIT_MASK));
	PCU2->ISEGIR = Reg32;
}


/***************************************************************************//**
* @brief      Enable/Disable Accessing ISEG, ISEGIR Registers
* @param      Output
*              - ENABLE : Enable ISEG ports by ISEG configuration
*              - DISABLE : Disable all ISEG (Floating)
* @return     None
*******************************************************************************/
void HAL_PCU_UseISEG(FunctionalState Output)
{
      if (Output == ENABLE) {
              PCU2->ISEGPEN = (PCU2_ISEGKEY_ISEGENKEY_ENABLE << PCU2_ISEGPEN_ISEGENKEY_Pos) & PCU2_ISEGPEN_ISEGENKEY_Msk;
      } else {
              PCU2->ISEGPEN = (PCU2_ISEGKEY_ISEGENKEY_DISABLE << PCU2_ISEGPEN_ISEGENKEY_Pos) & PCU2_ISEGPEN_ISEGENKEY_Msk;
      }
}

/***************************************************************************//**
* @brief      Get ISEG Pin Status
* @param      Output
*              - ENABLE : Enable ISEG ports by ISEG configuration
*              - DISABLE : Disable all ISEG (Floating)
* @return     ISEG Status
*******************************************************************************/
uint16_t HAL_PCU_GetISEGStatus(void)
{
    return ((PCU2->ISEGR) & PCU2_ISEG_BIT_MASK);
}

/***************************************************************************//**
* @brief      Get ISEG Pin Status
* @param      Output
*              - ENABLE : Enable ISEG ports by ISEG configuration
*              - DISABLE : Disable all ISEG (Floating)
* @return     ISEG Status
*******************************************************************************/
uint16_t HAL_PCU_GetISEGInvStatus(void)
{
    return ((PCU2->ISEGIR) & PCU2_ISEG_BIT_MASK);
}

/***************************************************************************//**
* @brief      Write value on port that have set stregnth function for high speed.
* @param      pPCUx : Pointer to selected PCU peripheral, should be
*              - PB ~ PC : PCUx peripheral
* @param      PortNumber : Write value
* @param      Strength : Strength On/Off
*              - ENABLE
*              - DISABLE
* @return     None
*******************************************************************************/
void HAL_PCU_SetStrength(PORT_Type *pPCUx, uint32_t PortNumber, FunctionalState Strength)
{
    uint32_t Reg32;
    uint32_t Offset;

    // Check PORT Control Access Status
    if (!HAL_GPIO_GetPortEnStatus()) {
    }

    Offset = (PortNumber * PCU_STR_BIT_WIDTH);

    Reg32 = pPCUx->STR;
    Reg32 &= ~(PCU_STR_BIT_MASK << Offset);
    if (Strength == ENABLE) {
        Reg32 |= (ENABLE << Offset);
    }
    pPCUx->STR = Reg32;
}


/***************************************************************************//**
* @brief      Configure GPIO interrupt of port n
* @param      pPCUx : Pointer to selected PCU peripheral, should be
*              - PA ~ PF : PCUx peripheral
* @param      PortNumber : Port number, should be 0 ~ 15
* @param      Mode : Interrupt mode, should be
*              - PCU_INTERRUPT_MODE_DISABLE : Disable interrupt
*              - PCU_INTERRUPT_MODE_LEVEL : Level interrupt
*              - PCU_INTERRUPT_MODE_EDGE : Edge interrupt
* @param      Control : interrupt control, should be:
*              | value | PCU_INTERRUPT_MODE_LEVEL      | PCU_INTERRUPT_MODE_EDGE         |
*              |:-----:|:-----------------------------:|:-------------------------------:|
*              | 0x00  | PCU_INTERRUPT_MODE_DISABLE    | PCU_INTERRUPT_CTRL_PROHIBIT     |
*              | 0x01  | PCU_INTERRUPT_MODE_LEVEL      | PCU_INTERRUPT_CTRL_EDGE_FALLING |
*              | 0x02  | Reserved                      | PCU_INTERRUPT_CTRL_EDGE_RISING  |
*              | 0x03  | PCU_INTERRUPT_MODE_EDGE       | PCU_INTERRUPT_CTRL_EDGE_BOTH    |
* @return     None
*******************************************************************************/
void HAL_GPIO_EXTI_Config(PORT_Type *pPCUx, uint32_t PortNumber, PCU_INTERRUPT_MODE_Type Mode, PCU_INTERRUPT_CTRL_Type Control)
{
	uint32_t Offset;
	volatile uint32_t Reg32;

    // Check PORT Control Access Status
    if (!HAL_GPIO_GetPortEnStatus()) {
    }

	Offset = PortNumber * PCU_IER_BIT_WIDTH;
	Reg32 = pPCUx->IER;
	Reg32 &= ~(PCU_IER_BIT_MASK << Offset);
	Reg32 |= (Mode << Offset);
	pPCUx->IER = Reg32;

	Offset = PortNumber * PCU_ICR_BIT_WIDTH;
	Reg32 = pPCUx->ICR;
	Reg32 &= ~(PCU_ICR_BIT_MASK << Offset);
	Reg32 |= (Control << Offset);
	pPCUx->ICR = Reg32;
}

/***************************************************************************//**
* @brief      Get GPIO status
* @param      pPCUx : Pointer to selected PCU peripheral, should be
*              - PA ~ PF : PCUx peripheral
* @return     Status (ISR register)
*******************************************************************************/
uint32_t HAL_GPIO_EXTI_GetState(PORT_Type *pPCUx)
{
	return pPCUx->ISR;
}

/***************************************************************************//**
* @brief      Clear GPIO status
* @param      pPCUx : Pointer to selected PCU peripheral, should be
*              - PA ~ PF : PCUx peripheral
* @param      Status : GPIO status, should be
*              - 0x0000 0000 ~ 0xFFFF FFFF
* @return     None
*******************************************************************************/
void HAL_GPIO_EXTI_ClearPin(PORT_Type *pPCUx, uint32_t Status)
{
    // Check PORT Control Access Status
    if (!HAL_GPIO_GetPortEnStatus()) {
    }

	pPCUx->ISR = Status;
}

/***************************************************************************//**
* @brief      Set value for bits that have output direction on GPIO
* @param      pPCUx : Pointer to selected PCU peripheral, should be
*              - PA ~ PF : PCUx peripheral
* @param      BitValue : Value that contains all bits to set on GPIO, should be
*              - 0x00 ~ 0xFF
* @return     None
*******************************************************************************/
void HAL_GPIO_SetPin(PORT_Type *pPCUx, uint16_t BitValue)
{
	pPCUx->BSR = BitValue;
}

/***************************************************************************//**
* @brief      Clear value for bits that have output direction on GPIO
* @param      pPCUx : Pointer to selected PCU peripheral, should be
*              - PA ~ PF : PCUx peripheral
* @param      BitValue : Value that contains all bits to clear on GPIO, should be
*              - 0x00 ~ 0xFF
* @return     None
*******************************************************************************/
void HAL_GPIO_ClearPin(PORT_Type *pPCUx, uint32_t BitValue)
{
	pPCUx->BCR = BitValue;
}

/***************************************************************************//**
* @brief      Read current state on port that have input direction on GPIO
* @param      pPCUx : Pointer to selected PCU peripheral, should be
*              - PA ~ PF : PCUx peripheral
* @return     Current value of GPIO
*******************************************************************************/
uint32_t HAL_GPIO_ReadPin(PORT_Type *pPCUx)
{
	return pPCUx->INDR;
}

/***************************************************************************//**
* @brief      Write value on port that have output direction on GPIO
* @param      pPCUx : Pointer to selected PCU peripheral, should be
*              - PA ~ PF : PCUx peripheral
* @param      Value : Write value
* @return     None
*******************************************************************************/
void HAL_GPIO_WriteValue(PORT_Type *pPCUx, uint32_t Value)
{
	pPCUx->OUTDR = Value;
}

uint8_t HAL_GPIO_GetPortEnStatus(void) {
    if ((PCU2->PORTEN & PORT_ACCESS_STATUS_MASK) == PORT_ACCESS_STATUS_MASK) {
        return 1;
    } else {
        return 0;
    }
}

#endif /* _GPIO */

/* --------------------------------- End Of File ------------------------------ */
