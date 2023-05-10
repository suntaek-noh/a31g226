/**********************************************************************//**
* @file		A31G22x_hal_usart1n.c
* @brief	Contains all functions support for USART1n firmware library on A31G22x
* @author   AE Team, ABOV Semiconductor Co., Ltd.
* @version  V1.00
* @date     28. DEC. 2020
*
* Copyright(C) 2020, ABOV Semiconductor All rights reserved.
************************************************************************
* ABOV Disclaimer
*
*IMPORTANT NOTICE ? PLEASE READ CAREFULLY
*ABOV Semiconductor ("ABOV") reserves the right to make changes, corrections, enhancements,
*modifications, and improvements to ABOV products and/or to this document at any time without notice.
*ABOV does not give warranties as to the accuracy or completeness of the information included herein.
*Purchasers should obtain the latest relevant information of ABOV products before placing orders.
*Purchasers are entirely responsible for the choice, selection, and use of ABOV products and
*ABOV assumes no liability for application assistance or the design of purchasers’ products. No license,
*express or implied, to any intellectual property rights is granted by ABOV herein.
*ABOV disclaims all express and implied warranties and shall not be responsible or
*liable for any injuries or damages related to use of ABOV products in such unauthorized applications.
*ABOV and the ABOV logo are trademarks of ABOV.
*All other product or service names are the property of their respective owners.
*Information in this document supersedes and replaces the information previously
*supplied in any former versions of this document.
*2020 ABOV Semiconductor  All rights reserved
*
**********************************************************************/


#include "A31G22x_hal_usart1n.h"
#include "A31G22x_hal_scu.h"

uint32_t UsartBaseClock;

/********************************************************************//**
 * @brief		Initializes the USART1x peripheral according to the specified
 *               parameters in the USART_ConfigStruct.
 * @param[in]	USARTx	USART peripheral selected, should be:
 * 					- USART10	:USART10 peripheral
 * 					- USART11	:USART11 peripheral
 * 					- USART12	:USART12 peripheral
 * 					- USART13	:USART13 peripheral
 * @param[in]	USART_ConfigStruct Pointer to a USART_CFG_Type structure
 *              that contains the configuration information for the
 *              specified USART peripheral.
 * @return 		 HAL Satus
 *********************************************************************/
HAL_Status_Type HAL_USART_Init(USART_Type *USARTx, USART_CFG_Type *USART_ConfigStruct)
{
	uint32_t tmp ;

	if(USARTx == USART10) {
		/* Set up peripheral clock for USART10 module */
        // USART10 Block Enable
        HAL_SCU_PeripheralCmd(PERI_USART10, ENABLE);
	}
	else if(USARTx == USART11) {
		/* Set up peripheral clock for USART11 module */
        // USART11 Block Enable
        HAL_SCU_PeripheralCmd(PERI_USART11, ENABLE);
	}
	else if(USARTx == USART12) {
		/* Set up peripheral clock for USART12 module */
        // USART12 Block Enable
        HAL_SCU_PeripheralCmd(PERI_USART12, ENABLE);
	}
	else if(USARTx == USART13) {
		/* Set up peripheral clock for USART13 module */
        // USART13 Block Enable
        HAL_SCU_PeripheralCmd(PERI_USART13, ENABLE);
	}
	else
	{
		return HAL_ERROR;
	}


	HAL_USART_Enable(USARTx, ENABLE);

    if(USART_ConfigStruct->Mode == USART_UART_MODE){
		tmp = 0
			| ((USART_ConfigStruct->Mode & 0x3) << 14)
			| ((USART_ConfigStruct->Parity & 0x3) << 12)
			| ((USART_ConfigStruct->Databits & 0x7) << 9)
			| (USART_CR1_TXE_Msk)  //Tx Enable
			| (USART_CR1_RXE_Msk)  //Rx Enable
			;
		USARTx->CR1 = tmp;

		tmp = 0
			| (USART_CR2_EN_Msk)  // EN
			| ((USART_ConfigStruct->Stopbits & 0x1)<<2)
			;
		USARTx->CR2 = tmp;

        UsartBaseClock = SystemCoreClock >> 4;  //     UsartBaseClock = SystemCoreClock / 16
		HAL_USART_set_divisors(USARTx, USART_ConfigStruct->Baud_rate);

    }
	else if(USART_ConfigStruct->Mode == USART_SYNC_MODE){
		tmp = 0
			| ((USART_ConfigStruct->Mode & 0x3) << 14)
			| ((USART_ConfigStruct->Parity & 0x3) << 12)
			| ((USART_ConfigStruct->Databits & 0x7) << 9)
			| (USART_CR1_TXE_Msk)  //Tx Enable
			| (USART_CR1_RXE_Msk)  //Rx Enable
			;
		USARTx->CR1 = tmp;

		tmp = 0
			| (USART_CR2_EN_Msk)  // EN
			| ((USART_ConfigStruct->Stopbits & 0x1) << 2)
			;
		USARTx->CR2 = tmp;

        UsartBaseClock = SystemCoreClock >> 1;  //     UsartBaseClock = SystemCoreClock / 2
		HAL_USART_set_divisors(USARTx, USART_ConfigStruct->Baud_rate);
    }
	else if(USART_ConfigStruct->Mode == USART_SPI_MODE){
		tmp = 0
			| ((USART_ConfigStruct->Mode & 0x3) << 14)
			| ((USART_ConfigStruct->Databits & 0x7) << 9)
			| ((USART_ConfigStruct->Order & 0x1) << 8)
			| ((USART_ConfigStruct->ACK & 0x1) << 7)
			| ((USART_ConfigStruct->Edge & 0x1)  << 6)
			| (USART_CR1_TXE_Msk)  //Tx Enable
			| (USART_CR1_RXE_Msk)  //Rx Enable
			;
		USARTx->CR1 = tmp;

		USARTx->CR2 |= USART_CR2_EN_Msk;	// EN
		USARTx->CR2 &= ~USART_CR2_FXCH_Msk;   // FXCHn reset
	    USARTx->CR2 |= USART_CR2_FXCH_Msk;    // FXCHn

		UsartBaseClock = SystemCoreClock >> 1;
		HAL_USART_set_divisors(USARTx, USART_ConfigStruct->Baud_rate);

		//Dummy Read
		tmp = HAL_USART_ReceiveByte(USARTx);
		tmp = HAL_USART_ReceiveByte(USARTx);
    }

    usart_set_buffer(USARTx, USART_ConfigStruct->RxTxBuffer, USART_ConfigStruct->RxBufferSize, USART_ConfigStruct->TxBufferSize);

	//Dummy Read and flush buffer
	usflush(USARTx);

	if (USART_ConfigStruct->RxTxBuffer != NULL) {
		HAL_USART_ConfigInterrupt(USARTx, USART_INTCFG_RXC, ENABLE);

		 if (USARTx == USART10) {
			NVIC_EnableIRQ(USART10_IRQn);
		} else if (USARTx == USART11) {
			NVIC_EnableIRQ(USART11_IRQn);
		} else if (USARTx == USART12) {
			NVIC_EnableIRQ(USART12_13_SPI21_IRQn);
		} else if (USARTx == USART13) {
			NVIC_EnableIRQ(USART12_13_SPI21_IRQn);
		}
	}

    return HAL_OK;
}


/*********************************************************************//**
 * @brief		De-initializes the USARTx peripheral registers to their
 *              default reset values.
 * @param[in]	USARTx	USART peripheral selected, should be:
 * 					- USART10	:USART10 peripheral
 * 					- USART11	:USART11 peripheral
 * 					- USART12	:USART12 peripheral
 * 					- USART13	:USART13 peripheral
 * @return 		 HAL Satus
 **********************************************************************/
HAL_Status_Type HAL_USART_DeInit(USART_Type* USARTx)
{
	if(USARTx == USART10) {
        HAL_SCU_PeripheralCmd(PERI_USART10, DISABLE);
	}
	else if(USARTx == USART11) {
        HAL_SCU_PeripheralCmd(PERI_USART11, DISABLE);
	}
	else if(USARTx == USART12) {
        HAL_SCU_PeripheralCmd(PERI_USART12, DISABLE);
	}
	else if(USARTx == USART13) {
        HAL_SCU_PeripheralCmd(PERI_USART13, DISABLE);
	}
	else
	{
		return HAL_ERROR;
	}
	return HAL_OK;

}


/*********************************************************************//**
 * @brief		USARTx enable control
 *              default reset values.
 * @param[in]	USARTx	USART peripheral selected, should be:
 * 					- USART10	:USART10 peripheral
 * 					- USART11	:USART11 peripheral
 * 					- USART12	:USART12 peripheral
 * 					- USART13	:USART13 peripheral
 *              state ENABLE / DISABLE
 * @return 		 HAL Satus
 **********************************************************************/
HAL_Status_Type HAL_USART_Enable(USART_Type* USARTx, FunctionalState state)
{
    if(state == ENABLE){
        USARTx->CR2 |= USART_CR2_EN_Msk;     // EN
    }else{
        USARTx->CR2 &= ~USART_CR2_EN_Msk;     // EN
    }
    return HAL_OK;
}


/******************************************************************************//**
 * @brief		Fills each HAL_USART_InitStruct member with its default value:
 * 					- 38400 bps
 * 					- 8-bit data
 * 					- 1 Stopbit
 * 					- None Parity
 * @param[in]	HAL_USART_InitStruct Pointer to a USART_CFG_Type structure which will
 * 				be initialized.
 * @return		 HAL Satus
 *******************************************************************************/
HAL_Status_Type HAL_USART_UART_Mode_Config(USART_CFG_Type *HAL_USART_InitStruct)
{
	HAL_USART_InitStruct->Mode = USART_UART_MODE;
	HAL_USART_InitStruct->Baud_rate = 38400;
	HAL_USART_InitStruct->Databits = USART_DATABIT_8;
	HAL_USART_InitStruct->Parity = USART_PARITY_NONE;
	HAL_USART_InitStruct->Stopbits = USART_STOPBIT_1;
	return HAL_OK;
}

/******************************************************************************//**
 * @brief		Fills each HAL_USART_InitStruct member with its default value:
 * @param[in]	HAL_USART_InitStruct Pointer to a USART_CFG_Type structure which will
 * 				be initialized.
 * @return		 HAL Satus
 *******************************************************************************/
HAL_Status_Type HAL_USART_SPI_Mode_Config(USART_CFG_Type *HAL_USART_InitStruct)
{
    HAL_USART_InitStruct->Mode = USART_SPI_MODE;
	HAL_USART_InitStruct->Baud_rate = 38400;
	HAL_USART_InitStruct->Databits = USART_DATABIT_8;

    //only SPI
    HAL_USART_InitStruct->Order = USART_SPI_LSB;
#if 1 // CPOLn : 0, CPHAn : 0
    HAL_USART_InitStruct->ACK = USART_SPI_TX_RISING;
    HAL_USART_InitStruct->Edge = USART_SPI_TX_LEADEDGE_SAMPLE;
#endif
#if 0 // CPOLn : 0, CPHAn : 1
    HAL_USART_InitStruct->ACK = USART_SPI_TX_RISING;
    HAL_USART_InitStruct->Edge = USART_SPI_TX_LEADEDGE_SETUP;
#endif

#if 0 // CPOLn : 1, CPHAn : 0
    HAL_USART_InitStruct->ACK = USART_SPI_TX_FALLING;
    HAL_USART_InitStruct->Edge = USART_SPI_TX_LEADEDGE_SAMPLE;
#endif

#if 0 // CPOLn : 1, CPHAn : 1
    HAL_USART_InitStruct->ACK = USART_SPI_TX_FALLING;
    HAL_USART_InitStruct->Edge = USART_SPI_TX_LEADEDGE_SETUP;
#endif
    return HAL_OK;


}

/******************************************************************************//**
 * @brief		Fills each HAL_USART_InitStruct member with its default value:
 * 					- 38400 bps
 * 					- 8-bit data
 * 					- 1 Stopbit
 * 					- None Parity
 * @param[in]	HAL_USART_InitStruct Pointer to a USART_CFG_Type structure which will
 * 				be initialized.
 * @return		 HAL Satus
 *******************************************************************************/
HAL_Status_Type HAL_USART_USRT_Mode_Config(USART_CFG_Type *HAL_USART_InitStruct)
{
    HAL_USART_InitStruct->Mode = USART_SYNC_MODE;
	HAL_USART_InitStruct->Baud_rate = 38400;
	HAL_USART_InitStruct->Databits = USART_DATABIT_8;
	HAL_USART_InitStruct->Parity = USART_PARITY_NONE;
	HAL_USART_InitStruct->Stopbits = USART_STOPBIT_1;
	return HAL_OK;
}

/* USART Send/Recieve functions -------------------------------------------------*/
/**********************************************************************//**
 * @brief		Transmit a single data through USART peripheral
 * @param[in]	USARTx	USART peripheral selected, should be:
 * 					- USART10	:USART10 peripheral
 * 					- USART11	:USART11 peripheral
 * @param[in]	Data	Data to transmit (must be 8-bit long)
 * @return 		 HAL Satus
 **********************************************************************/
HAL_Status_Type HAL_USART_TransmitByte(USART_Type* USARTx, uint8_t Data)
{
	USARTx->DR = Data;
	return HAL_OK;
}

/**********************************************************************//**
 * @brief		Receive a single data from USART peripheral
 * @param[in]	USARTx	USART peripheral selected, should be:
 * 					- USART10	:USART10 peripheral
 * 					- USART11	:USART11 peripheral
 * @return 		Data received
 **********************************************************************/
uint8_t HAL_USART_ReceiveByte(USART_Type* USARTx)
{
	return (USARTx->DR);
}

/**********************************************************************//**
 * @brief		Send a block of data via USART peripheral
 * @param[in]	USARTx	Selected USART peripheral used to send data, should be:
 * 					- USART10	:USART10 peripheral
 * 					- USART11	:USART11 peripheral
 * 					- USART12	:USART12 peripheral
 * 					- USART13	:USART13 peripheral
 * @param[in]	txbuf 	Pointer to Transmit buffer
 * @param[in]	buflen 	Length of Transmit buffer
 * @return 		Number of bytes sent.
 *
 **********************************************************************/
uint32_t HAL_USART_Transmit(USART_Type *USARTx, uint8_t *txbuf, uint32_t buflen)
{
	uint32_t bToSend, bSent;
	uint8_t *pChar = txbuf;

	bToSend = buflen;
	bSent = 0;
	while (bToSend){
		while(!(USARTx->ST & 0x80)){ }

		HAL_USART_TransmitByte(USARTx, (*pChar));

		pChar++;
		bToSend--;
		bSent++;
	}

	return bSent;
}

/*********************************************************************//**
 * @brief		Receive a block of data via USART peripheral
 * @param[in]	USARTx	Selected USART peripheral used to send data,
 * 				should be:
 * 					- USART10	:USART10 peripheral
 * 					- USART11	:USART11 peripheral
 * 					- USART12	:USART12 peripheral
 * 					- USART13	:USART13 peripheral
 * @param[out]	rxbuf 	Pointer to Received buffer
 * @param[in]	buflen 	Length of Received buffer
 * @param[in] 	flag 	Flag mode, should be:
 * 					- NONE_BLOCKING
 * 					- BLOCKING
 * @return 		Number of bytes received
 **********************************************************************/
uint32_t HAL_USART_Receive(USART_Type *USARTx, uint8_t *rxbuf, uint32_t buflen)
{
	uint32_t bToRecv, bRecv;
	uint8_t *pChar = rxbuf;

	bToRecv = buflen;
	bRecv = 0;
	while (bToRecv) {
		while(!(USARTx->ST & USART_ST_RXC)){ }

		(*pChar++) = HAL_USART_ReceiveByte(USARTx);
		bRecv++;
		bToRecv--;
	}
	return bRecv;
}


/********************************************************************//**
 * @brief 		Enable or disable specified USART interrupt.
 * @param[in]	USARTx	USART peripheral selected, should be
 * 					- USART10	:USART10 peripheral
 * 					- USART11	:USART11 peripheral
 * 					- USART12	:USART12 peripheral
 * 					- USART13	:USART13 peripheral
 * @param[in]	USTIntCfg	Specifies the interrupt flag,
 * 				should be one of the following:
 * 					- USART_INTCFG_DR 	:DR Interrupt enable
 * 					- USART_INTCFG_TXC 	:TXC Interrupt enable
 * 					- USART_INTCFG_RXC 	:RXC interrupt enable
 * 					- USART_INTCFG_WAKE 	:WAKE Interrupt enable
 * @param[in]	NewState New state of specified USART interrupt type,
 * 				should be:
 * 					- ENALBE	:Enable this USART interrupt type.
 * 					- DISALBE	:Disable this USART interrupt type.
 * @return 		 HAL Satus
 *********************************************************************/
HAL_Status_Type HAL_USART_ConfigInterrupt(USART_Type *USARTx, USART_INT_Type USTIntCfg, FunctionalState NewState)
{
	uint32_t tmp=0;


	switch(USTIntCfg){
//		case USART_INTCFG_WAKE :        // A31G226 doens't support
//			tmp = USART_IER_WAKEINT_EN;
//			break;
		case USART_INTCFG_RXC:
			tmp = USART_IER_RXCINT_EN;
			break;
		case USART_INTCFG_TXC:
			tmp = USART_IER_TXCINT_EN;
			break;
		case USART_INTCFG_DR:
			tmp = USART_IER_DR_EN;
			break;
	}

	if (NewState == ENABLE)
	{
		USARTx->CR1 |= tmp;
	}
	else
	{
		USARTx->CR1 &= ~(tmp & USART_IER_BITMASK) ;
	}
	return HAL_OK;
}


/*********************************************************************//**
 * @brief 		Get current value of Line Status register in USART peripheral.
 * @param[in]	USARTx	USART peripheral selected, should be:
 * 					- USART10	:USART10 peripheral
 * 					- USART11	:USART11 peripheral
 * 					- USART12	:USART12 peripheral
 * 					- USART13	:USART13 peripheral
 * @return		Current value of Status register in USART peripheral.
 *********************************************************************/
uint8_t HAL_USART_GetStatus(USART_Type* USARTx)
{
	return ((USARTx->ST) & USART_ST_BITMASK);
}

/*********************************************************************//**
 * @brief 		Clear Status register in USART peripheral.
 * @param[in]	USARTx	USART peripheral selected, should be:
 * 					- USART10	:USART10 peripheral
 * 					- USART11	:USART11 peripheral
 * 					- USART12	:USART12 peripheral
 * 					- USART13	:USART13 peripheral
  * @return 		 HAL Satus
 *********************************************************************/
HAL_Status_Type HAL_USART_ClearStatus(USART_Type* USARTx, USART_STATUS_Type Status)
{
    uint32_t tmp;

    switch(Status){
//        case USART_STATUS_WAKE:
//            tmp = USART_SR_WAKE;
//            break;
        case USART_STATUS_RXC:
            tmp = USART_ST_RXC;
            break;
        case USART_STATUS_TXC:
            tmp = USART_ST_TXC;
            break;
        case USART_STATUS_DRE:
            tmp = USART_ST_DRE;
            break;
        default:
            break;
    }

    USARTx->ST = tmp;
    return HAL_OK;
}

/**********************************************************************//**
 * @brief		Check whether if USART is busy or not
 * @param[in]	USARTx	USART peripheral selected, should be:
 * 					- USART10	:USART10 peripheral
 * 					- USART11	:USART11 peripheral
 * 					- USART12	:USART12 peripheral
 * 					- USART13	:USART13 peripheral
 * @return		RESET if USART is not busy, otherwise return SET.
 **********************************************************************/
FlagStatus HAL_USART_CheckBusy(USART_Type *USARTx)
{
	if (USARTx->ST & USART_ST_DRE){
		return RESET;
	} else {
		return SET;
	}
}

/*********************************************************************//**
 * @brief		Configure Data Control mode for USART peripheral
 * @param[in]	USARTx
 * 					- USART10	:USART10 peripheral
 * 					- USART11	:USART11 peripheral
 * 					- USART12	:USART12 peripheral
 * 					- USART13	:USART13 peripheral
 * @param[in]	Data Control mode, should be:
 * 					- USART_CR_USTEN     :Activate USARTn Block by supplying.
 * 					- USART_CR_DBLS      :Selects receiver sampling rate. (only UART mode)
 * 					- USART_CR_MASTER    :Selects master or slave in SPIn or Synchronous mode and controls the direction of SCKn pin.
 * 					- USART_CR_LOOPS     :Control the Loop Back mode of USARTn for test mode.
 * 					- USART_CR_DISSCK    :In synchronous mode operation, selects the waveform of SCKn output.
 * 					- USART_CR_USTSSEN   :This bit controls the SSn pin operation. (only SPI mode)
 * 					- USART_CR_FXCH      :SPIn port function exchange control bit. (only SPI mode)
 * 					- USART_CR_USTSB     :Selects the length of stop bit in Asynchronous or Synchronous mode.
 * 					- USART_CR_USTTX8    :The ninth bit of data frame in Asynchronous or Synchronous mode of operation. Write this bit first before loading the DR register.
 * 					- USART_CR_USTRX8    :The ninth bit of data frame in Asynchronous or Synchronous mode of operation. Read this bit first before reading the receive buffer (only UART mode)
 * @param[in]	NewState New State of this mode, should be:
 * 					- ENABLE	:Enable this mode.
					- DISABLE	:Disable this mode.
 * @return  HAL Satus
 **********************************************************************/
HAL_Status_Type HAL_USART_DataControlConfig(USART_Type *USARTx, USART_CONTROL_Type Mode, FunctionalState NewState)
{
	uint16_t tmp=0;

	switch(Mode){
	case USART_CONTROL_USTRX8:
		tmp = USART_CR2_USTRX8;
		break;
	case USART_CONTROL_USTTX8:
		tmp = USART_CR2_USTTX8;
		break;
	case USART_CONTROL_USTSB:
		tmp = USART_CR2_USTSB;
		break;
	case USART_CONTROL_FXCH:
		tmp = USART_CR2_FXCH;
		break;
	case USART_CONTROL_USTSSEN:
		tmp = USART_CR2_USTSSEN;
		break;
	case USART_CONTROL_DISSCK:
		tmp = USART_CR2_DISSCK;
		break;
	case USART_CONTROL_LOOPS:
		tmp = USART_CR2_LOOPS;
		break;
	case USART_CONTROL_MASTER:
		tmp = USART_CR2_MASTER;
		break;
	case USART_CONTROL_DBLS:
		tmp = USART_CR2_DBLS;
		break;
	case USART_CONTROL_USTEN:
		tmp = USART_CR2_USTEN;
		break;
	default:
		break;
	}

	if (NewState == ENABLE)
	{
		USARTx->CR2 |= tmp;
	}
	else
	{
		USARTx->CR2 &= ~(tmp & USART_CR2_BITMASK) ;
	}
	return HAL_OK;
}
/*********************************************************************//**
 * @brief		Determines best dividers to get a target clock rate
 * @param[in]	USARTx	Pointer to selected USART peripheral, should be:
 * 					- USART10	:USART10 peripheral
 * 					- USART11	:USART11 peripheral
 * 					- USART12	:USART12 peripheral
 * 					- USART13	:USART13 peripheral
 * @param[in]	baudrate Desired USART baud rate.
 * @return 		None
 **********************************************************************/
void HAL_USART_set_divisors(USART_Type *USARTx, uint32_t baudrate)
{
	uint32_t numerator;
	uint32_t denominator;
	uint32_t bdr;
	uint32_t fd;
    uint32_t bfr;

	//------------------------------------------------------------------------------
	// numerator & denominator
	//
	// bdr = (UsartBaseClock) / n / baudrate  - 1
	//------------------------------------------------------------------------------
    numerator = UsartBaseClock;
    denominator = baudrate;

    bdr = numerator / denominator - 1;      // get BDR value

    fd = numerator % denominator;           // Fraction
    bfr = (fd * 256) / denominator;         // Get bfr value

    USARTx->CR2 |= USART_CR2_BFREN;         // Enable Fraction Counter Register.

    USARTx->BDR= (uint16_t)(bdr & 0xffff);    // Updates BDR value
    USARTx->BFR= (uint8_t)(bfr & 0xff);     // Updated BFR value
}

/**********************************************************************
* @brief        Check whether if USART is busy or not
* @param[in]    USARTx    USART peripheral selected, should be:
*                     - USART10    :USART10 peripheral
*                     - USART11    :USART11 peripheral
*                     - USART12    :USART12 peripheral
*                     - USART13    :USART13 peripheral
* @param[in]    USARTx    USART Receive Time-out function, should be:
*                     - DISABLE    :Disable USART Receive Time-out
*                     - ENABLE    :Enable USART Receive Time-out
* @param[in]    USARTx    USART Receive Time-out Interrupt, should be:
*                     - DISABLE    :Disable USART Receive Time-out interrupt
*                     - ENABLE    :Enable USART Receive Time-out interrupt
* @param[in]    USARTx    USART Receive Time-out value, should be:
*                     - 0x0 ~ 0xFFFFFF
*
* @return       None
**********************************************************************/
void HAL_USART_ReceiveTimeOut(USART_Type* USTx, FunctionalState rtoen, FunctionalState Intr, uint32_t TimeOut) {
    uint32_t Reg32;

    Reg32 = USTx->CR2;

    if (rtoen == ENABLE) {
        Reg32 |= USART_CR2_RTOEN;
    } else {
        Reg32 &= ~(USART_CR2_RTOEN);
    }

    if (Intr == ENABLE) {
        Reg32 |= USART_CR2_RTOIE;
    } else {
        Reg32 &= ~(USART_CR2_RTOIE);
    }

    USTx->CR2 = Reg32; // Update the settings of receive time out
    USTx->RTO = (TimeOut & USART_RTO_RTOMASK); // Update the time-out value
}

/**********************************************************************
* @brief        Check whether if USART is busy or not
* @param[in]    USARTx    USART peripheral selected, should be:
*                     - USART10    :USART10 peripheral
*                     - USART11    :USART11 peripheral
*                     - USART12    :USART12 peripheral
*                     - USART13    :USART13 peripheral
* @param[in]    DmaTx    USART Tx DMA Interrupt, should be:
*                     - DISABLE    :Disable USART Tx Interrupt
*                     - ENABLE    :Enable USART Tx Interrupt
* @param[in]    DmaRx    USART Rx DMA Interrupt, should be:
*                     - DISABLE    :Disable USART Rx Interrupt
*                     - ENABLE    :Enable USART Rx Interrupt
* @return       None
**********************************************************************/
void HAL_USART_DmaConfig(USART_Type* USTx, uint32_t DmaInterrupType, FunctionalState en) {
    uint32_t Reg32;

    Reg32 = USTx->CR2;

    if (DmaInterrupType == USART_CR2_DMATXIE) {
        if (en == ENABLE) {
            Reg32 |= USART_CR2_DMATXIE;
        } else {
            Reg32 &= ~(USART_CR2_DMATXIE);
        }
    } else if (DmaInterrupType == USART_CR2_DMARXIE) {
        if (en == ENABLE) {
            Reg32 |= USART_CR2_DMARXIE;
        } else {
            Reg32 &= ~(USART_CR2_DMARXIE);
        }
    }
    USTx->CR2 = Reg32; // Update the settings of USART1n DMA Intererupt
}

/*********************************************************************
* @brief        USART SPI SS pin control in master mode
*               default reset values.
* @param[in]    USTx    USART_SPI_PORT_Type
* @param[in]    Type    USART SPI Operation Type
*                - 0 : Slave
*                - 1 : Master
* @param[in]    UsartConfig      USART SPI Configuration
* @return       None
**********************************************************************/
void HAL_USART_SPI_SS_control(USART_Type * USTx, char Dir) {
	if (USTx == USART10) {
		if (Dir == 0)
			PB->OUTDR &= ~(1 << 3);
		else
			PB->OUTDR |= (1 << 3);
	}
	if (USTx == USART11) {
		if (Dir == 0)
			PD->OUTDR &= ~(1 << 5);
		else
			PD->OUTDR |= (1 << 5);
	}
	if (USTx == USART12) {
		if (Dir == 0)
			PE->OUTDR &= ~(1 << 15);
		else
			PE->OUTDR |= (1 << 15);
	}
	if (USTx == USART13) {
		if (Dir == 0)
			PE->OUTDR &= ~(1 << 11);
		else
			PE->OUTDR |= (1 << 11);
	}
}

/* Private Functions --------------------------------------------------------- */
//******************************************************************************
// Private Function
//******************************************************************************

USARTBUFFER	USART_Buffer[4];
volatile uint8_t     usart_mode = USART_POLLING;  // 0: Polling (default), 1: Interrupt,

USARTBUFFER *usart_get_buffer(USART_Type *USART1n)
{
	USARTBUFFER	*ub;

	if (USART1n == USART10) {
        if (usart_mode) {
            ub = &USART_Buffer[0];
        } else {
            ub = NULL;
        }
	} else if (USART1n == USART11) {
        if (usart_mode) {
            ub = &USART_Buffer[1];
        } else {
            ub = NULL;
        }
	} else if (USART1n == USART12) {
        if (usart_mode) {
            ub = &USART_Buffer[2];
        } else {
            ub = NULL;
        }
	} else if (USART1n == USART13) {
        if (usart_mode) {
            ub = &USART_Buffer[3];
        } else {
            ub = NULL;
        }
	}
	else {
		ub = NULL;
	}

	return ub;
}

void	usart_set_buffer(USART_Type *USART1n, uint8_t *buffer, uint32_t rxqsize, uint32_t txqsize)
{
	USARTBUFFER	*ub;

	ub = usart_get_buffer(USART1n);

	if (ub != NULL) {
		ub->status = 0;
		ub->rxhead = ub->rxtail = ub->rxsize = 0;
		ub->txhead = ub->txtail = ub->txsize = 0;

		if (buffer == NULL) {
			ub->buffer = NULL;
			ub->rxqsize = 0;
			ub->txqsize = 0;
		} else {
			ub->buffer = buffer;
			ub->rxqsize = rxqsize;
			ub->txqsize = txqsize;
		}
	}
}

void	usflush(USART_Type* USARTn)
{
//	uint32_t	temp;
	USARTBUFFER	*ub;

	ub = usart_get_buffer(USARTn);

//	temp = USARTn->DR;
//	temp = USARTn->DR;
    USARTn->DR;
    USARTn->DR;

	if (ub != NULL) {
		ub->status=0;
		ub->rxhead=ub->rxtail=ub->rxsize=0;
		ub->txhead=ub->txtail=ub->txsize=0;
	}
}


void usputc(USART_Type* USARTn, uint8_t Data)
{

	USARTBUFFER	*ub;

	ub = usart_get_buffer(USARTn);

	if (ub != NULL) {

		if (ub->txqsize > 0) {
			if (ub->txsize == 0 && (USARTn->ST & USART_ST_DRE_Msk) != 0) {
				USARTn->DR = Data;
                USARTn->ST |= USART_ST_DRE_Msk;
			}
			else {
				while (ub->txsize == ub->txqsize) ;
				__disable_irq();
				ub->buffer[ub->rxqsize + ub->txtail] = Data;
				if (++ub->txtail >= ub->txqsize)
					ub->txtail = 0;
				ub->txsize++;
				USARTn->CR1 |= USART_CR1_DRIE_Msk;
				__enable_irq();
			}
		}
		else {
			while ((USARTn->ST & USART_ST_DRE_Msk) == 0);
			USARTn->DR = Data;
		}
	} else {
		while ((USARTn->ST & USART_ST_DRE_Msk) == 0);
		USARTn->DR = Data;
	}
}

int32_t	uswrite(USART_Type *USART1n,void *p,int size)
{
	int32_t i;
	uint8_t	*dp = p;
	for (i = 0; i < size; i++) {
		usputc(USART1n, *dp++);
	}
	return i;
}

int32_t	usread(USART_Type *USARTn,void *p,int size)
{
	int32_t i;
	uint8_t	*dp = p;
	for (i = 0; i < size; i++) {
		*dp++ = usgetc(USARTn);
	}
	return i;
}

int32_t	usputs(USART_Type *USARTn,char *s)
{
	int	i;
	int	ch;
	for (i = 0; ;i++) {
		ch = *s++;
		if (ch == 0)
			break;
		usputc(USARTn, ch);
	}
	return i;
}

int32_t usprintf(USART_Type *USARTn, const char* format, ...)
{
  int ret;
  va_list ap;

  // TODO: rewrite it to no longer use newlib, it is way too heavy

  static char buf[OS_INTEGER_USART_PRINTF_TMP_ARRAY_SIZE];

  va_start (ap, format);

  // Print to the local buffer
  ret = vsnprintf (buf, sizeof(buf), format, ap);
  if (ret > 0)
    {
      // Transfer the buffer to the device
      uswrite(USARTn, buf, (size_t)ret);
    }

  va_end (ap);
  return ret;
}

int32_t	usgetq(USARTBUFFER *ub)
{
	int	ch;

	if (ub->rxsize == 0)
		return -1;
	__disable_irq();
	ch = ub->buffer[ub->rxhead];
	if (++ub->rxhead>=ub->rxqsize)
		ub->rxhead=0;
	ub->rxsize--;
	__enable_irq();

	return ch;
}

int32_t	uscheck(USART_Type* USARTn)
{
	USARTBUFFER *ub;

	ub = usart_get_buffer(USARTn);
	if (ub != NULL) {
		return (ub->rxsize);
	}
	return USARTn->ST & USART_ST_RXC_Msk;
}

int32_t	usgetc(USART_Type* USARTn)
{
	USARTBUFFER *ub;

	ub = usart_get_buffer(USARTn);
	if (ub != NULL) {
		int	ch;
		while ((ch = usgetq(ub)) < 0);
		return ch;
	}

	while ((USARTn->ST & USART_ST_RXC_Msk) == 0);
	return USARTn->DR;
}

int32_t	usgetchar(USART_Type* USARTn)
{
	int	ch = -1;

	if (uscheck(USARTn))
		ch = usgetc(USARTn);

	return ch;
}

int32_t	usgets(USART_Type* USARTn, char *strin, int32_t maxsize)
{
	int i;

	for (i = 0; i < maxsize-1; i++) {
		int32_t	ch;

		ch = usgetc(USARTn);
		if (ch == '\r')
			break;
		strin[i] = ch;
	}
	strin[i++] = 0;
	return i;
}

/*********************************************************************
 * @brief		UART Handler routine
 * @param[in]	UARTn	Pointer to selected UART peripheral, should be:
 * 					- UART0	:UART0 peripheral
 * 					- UART1	:UART1 peripheral
 * 				ub		UART buffer
 * @return 		None
 **********************************************************************/
void USART_Handler(USART_Type *USARTn)
{
	int			ch;
	uint32_t	sr;
	USARTBUFFER	*ub;

	ub = usart_get_buffer(USARTn);
	sr = USARTn->ST;

	if (sr & USART_ST_RXC_Msk) {
		ch = USARTn->DR;
		if (ub->rxsize < ub->rxqsize) {
			ub->rxsize++;
			ub->buffer[ub->rxtail] = ch;
			if (++ub->rxtail >= ub->rxqsize)
				ub->rxtail = 0;
		}
		//USARTn->USARTn_ST_b.RXCn = 1;
	}
	if (sr & USART_ST_DRE_Msk) {
		if (ub->txsize == 0) {
			USARTn->CR1 &= ~USART_CR1_DRIE_Msk;	//_b.DRIEn = 0;	// = 0uawrite(port,IA_IER,uaread(port,IA_IER)&~IER_TIE);
		} else if (ub->txqsize > 0) {
			ch = ub->buffer[ub->rxqsize + ub->txhead];
			if (++ub->txhead >= ub->txqsize)
				ub->txhead = 0;
			USARTn->DR = ch;	//uawrite(port,IA_THR,ch);
			ub->txsize--;
		}
	}
	if (sr & USART_ST_DOR_Msk) {
		ub->status |= USART_ST_DOR_Msk;		//Read-Only
	}
	if (sr & USART_ST_FE_Msk) {
		ub->status |= USART_ST_FE_Msk;
		USARTn->ST |= USART_ST_FE_Msk;	//_b.FEn = 1;
	}
	if (sr & USART_ST_PE_Msk) {
		ub->status |= USART_ST_PE_Msk;
		USARTn->ST |= USART_ST_PE_Msk;	//_b.PEn = 1;
	}
}
