/**********************************************************************//**
* @file		A31G22x_hal_uartn.c
* @brief	Contains all functions support for UART firmware library on A31G22x
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
*ABOV assumes no liability for application assistance or the design of purchasers�� products. No license, 
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


/* Includes -------------------------------------------------------- */
#include "A31G22x_hal_uartn.h"
#include "A31G22x_hal_scu.h"

/* Private Functions --------------------------------------------------- */
uint32_t UartBaseClock; 

/********************************************************************//**
 * @brief		Initializes the UARTx peripheral according to the specified
 *               parameters in the UART_ConfigStruct.
 * @param[in]	UARTx	UART peripheral selected, should be:
 * 					- UART0	:UART0 peripheral
 * 					- UART1	:UART1 peripheral
 * @param[in]	UART_ConfigStruct Pointer to a UART_CFG_Type structure
 *              that contains the configuration information for the
 *              specified UART peripheral.
 * @return 		 HAL Satus
 *********************************************************************/
HAL_Status_Type HAL_UART_Init(UART_Type *UARTx, UART_CFG_Type *UART_ConfigStruct)
{
	uint8_t tmp;

	if(UARTx == UART0)
	{	
        // UART0 Block Enable
        HAL_SCU_PeripheralCmd(PERI_UART0, ENABLE);
	}

	if(UARTx == UART1)
	{
        // UART1 Block Enable        
        HAL_SCU_PeripheralCmd(PERI_UART1, ENABLE);        
	}

	// Dummy reading
	while (UARTx->LSR & UART_LSR_RDR)
	{
		tmp = UARTx->RBR;
	}
	// Wait for current transmit complete
	while (!(UARTx->LSR & UART_LSR_THRE));

	// Disable interrupt
	UARTx->IER = 0;
	// Set LCR, DCR to default state
	UARTx->LCR = 0;
	UARTx->DCR = 0;
	
	tmp = UARTx->LSR;       // Dummy reading

	tmp = UARTx->IIR;	    // Clear Interrupt ID State Register

	// uart clock set
	UartBaseClock=SystemCoreClock/2;

	// Set Line Control register ----------------------------	
	HAL_UART_set_divisors(UARTx, (UART_ConfigStruct->Baud_rate));

	tmp = (UARTx->LCR & UART_LCR_BREAK_EN) & UART_LCR_BITMASK;

	switch (UART_ConfigStruct->Databits){
	case UART_DATABIT_5:
		tmp |= UART_LCR_WLEN5;
		break;
	case UART_DATABIT_6:
		tmp |= UART_LCR_WLEN6;
		break;
	case UART_DATABIT_7:
		tmp |= UART_LCR_WLEN7;
		break;
	case UART_DATABIT_8:
	default:
		tmp |= UART_LCR_WLEN8;
		break;
	}

	if (UART_ConfigStruct->Parity == UART_PARITY_NONE)
	{
		// Do nothing...
	}
	else
	{
		tmp |= UART_LCR_PARITY_EN;
		switch (UART_ConfigStruct->Parity)
		{
		case UART_PARITY_ODD:
			tmp |= UART_LCR_PARITY_ODD;
			break;

		case UART_PARITY_EVEN:
			tmp |= UART_LCR_PARITY_EVEN;
			break;

		case UART_PARITY_SP_1:
			tmp |= UART_LCR_PARITY_F_1;
			break;

		case UART_PARITY_SP_0:
			tmp |= UART_LCR_PARITY_F_0;
			break;
		default:
			break;
		}
	}

	switch (UART_ConfigStruct->Stopbits){
	case UART_STOPBIT_2:
		tmp |= UART_LCR_STOPBIT_SEL;
		break;
	case UART_STOPBIT_1:
	default:
		// Do no thing
		break;
	}

	UARTx->LCR = (uint8_t)(tmp & UART_LCR_BITMASK);

    
	uart_set_buffer(UARTx, UART_ConfigStruct->RxTxBuffer, UART_ConfigStruct->RxBufferSize, UART_ConfigStruct->TxBufferSize);

	if (UART_ConfigStruct->RxTxBuffer != NULL) {
		HAL_UART_ConfigInterrupt(UARTx, UART_INTCFG_DR, ENABLE);

		if (UARTx == UART0) {
			NVIC_EnableIRQ(UART0_IRQn);
		} else if (UARTx == UART1) {
			NVIC_EnableIRQ(UART1_IRQn);
		}
	}


	return HAL_OK;
}

/*********************************************************************//**
 * @brief		De-initializes the UARTx peripheral registers to their
 *              default reset values.
 * @param[in]	UARTx	UART peripheral selected, should be:
 * 					- UART0	:UART0 peripheral
 * 					- UART1	:UART1 peripheral
 * @return 		 HAL Satus
 **********************************************************************/
HAL_Status_Type HAL_UART_DeInit(UART_Type* UARTx)
{
	if (UARTx == UART0)
	{
		/* Set up peripheral clock for UART0 module */
        HAL_SCU_PeripheralCmd(PERI_UART0, DISABLE);
	}

	if (UARTx == UART1)
	{
		/* Set up peripheral clock for UART1 module */
        HAL_SCU_PeripheralCmd(PERI_UART1, DISABLE);

	}
	return HAL_OK;
}

/******************************************************************************//**
 * @brief		Fills each HAL_UART_InitStruct member with its default value:
 * 					- 38400 bps
 * 					- 8-bit data
 * 					- 1 Stopbit
 * 					- None Parity
 * @param[in]	HAL_UART_InitStruct Pointer to a UART_CFG_Type structure which will
 * 				be initialized.
 * @return		 HAL Satus
 *******************************************************************************/
HAL_Status_Type HAL_UART_ConfigStructInit(UART_CFG_Type *HAL_UART_InitStruct)
{
	HAL_UART_InitStruct->Baud_rate = 38400;
	HAL_UART_InitStruct->Databits = UART_DATABIT_8;
	HAL_UART_InitStruct->Parity = UART_PARITY_NONE;
	HAL_UART_InitStruct->Stopbits = UART_STOPBIT_1;
	return HAL_OK;
}

/* UART Send/Recieve functions -------------------------------------------------*/
/**********************************************************************//**
 * @brief		Transmit a single data through UART peripheral
 * @param[in]	UARTx	UART peripheral selected, should be:
 * 					- UART0	:UART0 peripheral
 * 					- UART1	:UART1 peripheral
 * @param[in]	Data	Data to transmit (must be 8-bit long)
 * @return 		 HAL Satus
 **********************************************************************/
HAL_Status_Type HAL_UART_TransmitByte(UART_Type* UARTx, uint8_t Data)
{
	UARTx->THR = Data;
	return HAL_OK;
}

/**********************************************************************//**
 * @brief		Receive a single data from UART peripheral
 * @param[in]	UARTx	UART peripheral selected, should be:
 * 					- UART0	:UART0 peripheral
 * 					- UART1	:UART1 peripheral
 * @return 		Data received
 **********************************************************************/
uint8_t HAL_UART_ReceiveByte(UART_Type* UARTx)
{
	return (UARTx->RBR);
}

/**********************************************************************//**
 * @brief		Send a block of data via UART peripheral
 * @param[in]	UARTx	Selected UART peripheral used to send data, should be:
 * 					- UART0	:UART0 peripheral
 * 					- UART1	:UART1 peripheral
 * @param[in]	txbuf 	Pointer to Transmit buffer
 * @param[in]	buflen 	Length of Transmit buffer
 * @param[in] 	flag 	Flag used in  UART transfer, should be
 * 					- NONE_BLOCKING
 * 					- BLOCKING
 * @return 		Number of bytes sent.
 *
 * Note: when using UART in BLOCKING mode, a time-out condition is used
 * via defined symbol UART_BLOCKING_TIMEOUT.
 **********************************************************************/
uint32_t HAL_UART_Transmit(UART_Type *UARTx, uint8_t *txbuf, uint32_t buflen, TRANSFER_BLOCK_Type flag)
{
	uint32_t bToSend, bSent, timeOut;
	uint8_t *pChar = txbuf;

	bToSend = buflen;

	// blocking mode
	if (flag == BLOCKING) {
		bSent = 0;
		while (bToSend){
			timeOut = UART_BLOCKING_TIMEOUT;
			// Wait for THR empty with timeout
			while (!(UARTx->LSR & UART_LSR_THRE)) {
				if (timeOut == 0) break;
				timeOut--;
			}
			// Time out!
			if(timeOut == 0) break;
			HAL_UART_TransmitByte(UARTx, (*pChar++));
			bToSend--;
			bSent++;
		}
	}
	// None blocking mode
	else {
		bSent = 0;
		while (bToSend) {
			if (!(UARTx->LSR & UART_LSR_THRE)){
				//continue;
				break;
			}
			HAL_UART_TransmitByte(UARTx, (*pChar++));
			bToSend--;
			bSent++;
		}
	}
	return bSent;
}

/*********************************************************************//**
 * @brief		Receive a block of data via UART peripheral
 * @param[in]	UARTx	Selected UART peripheral used to send data,
 * 				should be:
 * 					- UART0	:UART0 peripheral
 * 					- UART1	:UART1 peripheral
 * @param[out]	rxbuf 	Pointer to Received buffer
 * @param[in]	buflen 	Length of Received buffer
 * @param[in] 	flag 	Flag mode, should be:
 * 					- NONE_BLOCKING
 * 					- BLOCKING
 * @return 		Number of bytes received
 *
 * Note: when using UART in BLOCKING mode, a time-out condition is used
 * via defined symbol UART_BLOCKING_TIMEOUT.
 **********************************************************************/
uint32_t HAL_UART_Receive(UART_Type *UARTx, uint8_t *rxbuf, uint32_t buflen, TRANSFER_BLOCK_Type flag)
{
	uint32_t bToRecv, bRecv, timeOut;
	uint8_t *pChar = rxbuf;

	bToRecv = buflen;

	// Blocking mode
	if (flag == BLOCKING) {
		bRecv = 0;
		while (bToRecv){
			timeOut = UART_BLOCKING_TIMEOUT;
			while (!(UARTx->LSR & UART_LSR_RDR)){
				if (timeOut == 0) break;
				timeOut--;
			}
			// Time out!
			if(timeOut == 0) break;
			// Get data from the buffer
			(*pChar++) = HAL_UART_ReceiveByte(UARTx);
			bToRecv--;
			bRecv++;
		}
	}
	// None blocking mode
	else {
		bRecv = 0;
		while (bToRecv) {
			if (!(UARTx->LSR & UART_LSR_RDR)) {
				break;
			} else {
				(*pChar++) = HAL_UART_ReceiveByte(UARTx);
				bRecv++;
				bToRecv--;
			}
		}
	}
	return bRecv;
}

/*********************************************************************//**
 * @brief		Force BREAK character on UART line, output pin UARTx TXD is
				forced to logic 0.
 * @param[in]	UARTx	UART peripheral selected, should be:
 * 					- UART0	:UART0 peripheral
 * 					- UART1	:UART1 peripheral
 * @return 		None
 **********************************************************************/
HAL_Status_Type HAL_UART_ForceBreak(UART_Type* UARTx)
{
	UARTx->LCR |= UART_LCR_BREAK_EN;
	return HAL_OK;
}

/********************************************************************//**
 * @brief 		Enable or disable specified UART interrupt.
 * @param[in]	UARTx	UART peripheral selected, should be
 * 					- UART0	:UART0 peripheral
 * 					- UART1	:UART1 peripheral
 * @param[in]	UARTIntCfg	Specifies the interrupt flag,
 * 				should be one of the following:
 * 					- UART_INTCFG_DR 	:Data Receive interrupt enable
 * 					- UART_INTCFG_THRE 	:Transmit holding Register Empty interrupt enable
 * 					- UART_INTCFG_RLS 	:Receiver Line Status interrupt enable
 * 					- UART_INTCFG_TXE 	:Transmit Register Empty interrupt enable
 * 					- UART_INTCFG_DRX 	:DMA Rx interrupt enable
 * 					- UART_INTCFG_DTX 	:DMA Tx interrupt enable
 * @param[in]	NewState New state of specified UART interrupt type,
 * 				should be:
 * 					- ENALBE	:Enable this UART interrupt type.
 * 					- DISALBE	:Disable this UART interrupt type.
 * @return 		 HAL Satus
 *********************************************************************/
HAL_Status_Type HAL_UART_ConfigInterrupt(UART_Type *UARTx, UART_INT_Type UARTIntCfg, FunctionalState NewState)
{
	uint32_t tmp;

	switch(UARTIntCfg){
		case UART_INTCFG_DR:
			tmp = UART_IER_DREINT_EN;
			break;
		case UART_INTCFG_THRE:
			tmp = UART_IER_THREINT_EN;
			break;
		case UART_INTCFG_RLS:
			tmp = UART_IER_RLSINT_EN;
			break;
		case UART_INTCFG_TXE:
			tmp = UART_IER_TXEINT_EN;
			break;	
		case UART_INTCFG_DRX:
			tmp = UART_IER_DRXINT_EN;
			break;	
		case UART_INTCFG_DTX:
			tmp = UART_IER_DTXINT_EN;
			break;	        
	}

	if (NewState == ENABLE)
	{
		UARTx->IER |= tmp;
	}
	else
	{
		UARTx->IER &= (~tmp) & UART_IER_BITMASK;
	}
	return HAL_OK;
}


/*********************************************************************//**
 * @brief 		Get current value of Line Status register in UART peripheral.
 * @param[in]	UARTx	UART peripheral selected, should be:
 * 					- UART0	:UART0 peripheral
 * 					- UART1	:UART1 peripheral
 * @return		Current value of Line Status register in UART peripheral.
 * Note:	The return value of this function must be ANDed with each member in
 * 			UART_LS_Type enumeration to determine current flag status
 * 			corresponding to each Line status type. Because some flags in
 * 			Line Status register will be cleared after reading, the next reading
 * 			Line Status register could not be correct. So this function used to
 * 			read Line status register in one time only, then the return value
 * 			used to check all flags.
 *********************************************************************/
uint8_t HAL_UART_GetLineStatus(UART_Type* UARTx)
{
	return ((UARTx->LSR) & UART_LSR_BITMASK);
}

uint16_t HAL_UART_GetInterruptStatus(UART_Type* UARTx) {
    return (UARTx->IIR & UART_IIR_BITMASK);  // Get UART Interrupt status
}

/**********************************************************************//**
 * @brief		Check whether if UART is busy or not
 * @param[in]	UARTx	UART peripheral selected, should be:
 * 					- UART0	:UART0 peripheral
 * 					- UART1	:UART1 peripheral
 * @return		RESET if UART is not busy, otherwise return SET.
 **********************************************************************/
FlagStatus HAL_UART_CheckBusy(UART_Type *UARTx)
{
	if (UARTx->LSR & UART_LSR_TEMT){
		return RESET;
	} else {
		return SET;
	}
}

/*********************************************************************//**
 * @brief		Configure Data Control mode for UART peripheral
 * @param[in]	UARTx	
 * @param[in]	Data Control mode, should be:
 * 					- UART_DATA_CONTROL_LOOPBACK	:Loop back mode.
 * 					- UART_DATA_CONTROL_RXINV	: RX Data inversion mode.
 * 					- UART_DATA_CONTROL_TXINV	:TX Data inversion mode.
 * 					- UART_DATA_CONTROL_RTXINV	:TX RX Data inversion mode.
 * @param[in]	NewState New State of this mode, should be:
 * 					- ENABLE	:Enable this mode.
					- DISABLE	:Disable this mode.
 * @return  HAL Satus
 **********************************************************************/
HAL_Status_Type HAL_UART_DataControlConfig(UART_Type *UARTx, UART_DATA_CONTROL_Type Mode, FunctionalState NewState)
{
	uint8_t tmp;

	switch(Mode){
	case UART_DATA_CONTROL_LOOPBACK:
		tmp = UART_DCR_LBON;
		break;
	case UART_DATA_CONTROL_RXINV:
		tmp = UART_DCR_RXINV;
		break;
	case UART_DATA_CONTROL_TXINV:
		tmp = UART_DCR_TXINV;
		break;
	case UART_DATA_CONTROL_RTXINV:
		tmp = UART_DCR_RXINV | UART_DCR_TXINV;
		break;	
	default:
		break;
	}

	if (NewState == ENABLE)
	{
		UARTx->DCR |= tmp;
	}
	else
	{
		UARTx->DCR &= (~tmp) & UART_DCR_BITMASK;
	}
	return HAL_OK;
}

/*********************************************************************//**
 * @brief		Configure inter-frame delay time for UART peripheral
 * @param[in]	UARTx	
 * @param[in]	inter-frame delay time : 1 bit time  unit (0~7)
 * @return  HAL Satus
 **********************************************************************/
HAL_Status_Type HAL_UART_IFDelayConfig(UART_Type *UARTx, uint8_t waitval)
{
	if (waitval<8){ 
		UARTx->IDTR=waitval;
	}
	return HAL_OK;
}

/*********************************************************************//**
 * @brief		Determines best dividers to get a target clock rate
 * @param[in]	UARTx	Pointer to selected UART peripheral, should be:
 * 					- UART0	:UART0 peripheral
 * 					- UART1	:UART1 peripheral
 * @param[in]	baudrate Desired UART baud rate.
 * @return 		 HAL Satus
 **********************************************************************/
void HAL_UART_set_divisors(UART_Type *UARTx, uint32_t baudrate)
{
	uint32_t numerator; 
	uint32_t denominator; 
	uint32_t bdr, bfr;
	uint32_t fd; 

	//------------------------------------------------------------------------------
	// numerator & denominator 
	// 
	// bdr = (UartBaseClock) / (16 * baudrate) 
	//------------------------------------------------------------------------------
	numerator = UartBaseClock; 
	denominator = 16 * baudrate; 

	bdr = numerator / denominator; 

	//------------------------------------------------------------------------------
	//	fraction = numerator - bdr x denominator
	//
	//	bfr = INT (fraction * 256) = ((f x d ) << 8) / d 
	//------------------------------------------------------------------------------

	fd = numerator - (bdr * denominator); 
	bfr = (fd * 256) / denominator; 
	UARTx->BDR= (uint16_t)(bdr&0xffff);
	UARTx->BFR= (uint8_t)(bfr&0xff);
}


/* Private Functions --------------------------------------------------------- */
//******************************************************************************
// Private Function
//******************************************************************************

UARTBUFFER	UART_Buffer[2];
volatile uint8_t     uart_mode = USART_POLLING;  // 1: Interrupt, 0: Polling (default)

UARTBUFFER *uart_get_buffer(UART_Type *UARTn)
{
	UARTBUFFER	*ub;

	if (UARTn == UART0) {
        if (uart_mode) {
            ub = &UART_Buffer[0];
        } else {
            ub = NULL;
        }
	} else if (UARTn == UART1) {
        if (uart_mode) {
            ub = &UART_Buffer[1];
        } else {
            ub = NULL;
        }
	} else {
		ub = NULL;
	}

	return ub;
}

void	uart_set_buffer(UART_Type *UARTn, uint8_t *buffer, uint32_t rxqsize, uint32_t txqsize)
{
	UARTBUFFER	*ub;

	ub = uart_get_buffer(UARTn);

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

void	uflush(UART_Type* UARTn)
{
	UARTBUFFER	*ub;

	ub = uart_get_buffer(UARTn);

	if (ub != NULL) {
		ub->status=0;
		ub->rxhead=ub->rxtail=ub->rxsize=0;
		ub->txhead=ub->txtail=ub->txsize=0;
	}
}


void uputc(UART_Type* UARTn, uint8_t Data)
{

	UARTBUFFER	*ub;

	ub = uart_get_buffer(UARTn);

	if (ub != NULL) {

		if(ub->txqsize>0) {
			if (ub->txsize == 0 && (UARTn->LSR & UART_LSR_THRE)!=0) {
				UARTn->THR = Data;
			}
			else {
				while(ub->txsize==ub->txqsize) ;
				__disable_irq();
				ub->buffer[ub->rxqsize+ub->txtail]=Data;
				if(++ub->txtail>=ub->txqsize)
					ub->txtail=0;
				ub->txsize++;
				UARTn->IER |= UART_IER_THREIE_Msk;	//_b.THREIE = 1;	//uawrite(port,IA_IER,uaread(port,IA_IER)|IER_TIE);
				__enable_irq();
			}
		}
		else {
			while ((UARTn->LSR & (UART_LSR_THRE | UART_LSR_TEMT)) != (UART_LSR_THRE | UART_LSR_TEMT));
			UARTn->THR = Data;
		}
	} else {
		while ((UARTn->LSR&(UART_LSR_THRE|UART_LSR_TEMT)) != (UART_LSR_THRE|UART_LSR_TEMT));
		UARTn->THR = Data;
	}
}

int32_t	uwrite(UART_Type *UARTn,void *p,int size)
{
	int32_t i;
	uint8_t	*dp=p;

	for (i = 0; i < size; i++) {
		uputc(UARTn,*dp++);
	}
	return size;
}

int32_t	uread(UART_Type *UARTn,void *p,int size)
{
	int32_t i;
	uint8_t	*dp=p;
	for (i = 0; i < size; i++) {
		*dp++=ugetc(UARTn);
	}
	return i;
}

int32_t	uputs(UART_Type *UARTn,char *s)
{
	int32_t	i;
	int	ch;
	for(i=0;;i++) {
		ch=*s++;
		if(ch==0)
			break;
		uputc(UARTn, ch);
	}
	return i;
}

int32_t uprintf(UART_Type *UARTn, const char* format, ...)
{
	int32_t ret;
	va_list ap;

	// TODO: rewrite it to no longer use newlib, it is way too heavy
	static char buf[OS_INTEGER_UART_PRINTF_TMP_ARRAY_SIZE];

	va_start (ap, format);
    
    
	// Print to the local buffer
	ret = vsnprintf (buf, sizeof(buf), format, ap);
	if (ret > 0)
    {
      // Transfer the buffer to the device
      uwrite(UARTn, buf, (size_t)ret);
    }

	va_end (ap);
	return ret;
}

int32_t	ugetq(UARTBUFFER *ub)
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

int32_t	ucheck(UART_Type* UARTn)
{
	UARTBUFFER *ub;

	ub = uart_get_buffer(UARTn);
	if (ub != NULL) {
		return (ub->rxsize);
	}
	return UARTn->LSR & UART_LSR_RDR;
}

int32_t	ugetc(UART_Type* UARTn)
{
	UARTBUFFER *ub;

	ub = uart_get_buffer(UARTn);
	if (ub != NULL) {
		int	ch;
		while ((ch = ugetq(ub)) < 0);
		return ch;
	}

	while ((UARTn->LSR & UART_LSR_RDR) == 0);
	return UARTn->RBR;
}

int32_t	ugetchar(UART_Type* UARTn)
{
	int	ch = -1;

	if (ucheck(UARTn))
		ch = ugetc(UARTn);

	return ch;
}

int32_t	ugets(UART_Type* UARTn, char *strin, int32_t maxsize)
{
	int i;

	for (i = 0; i < maxsize-1; i++) {
		int32_t	ch;

		ch = ugetc(UARTn);
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
void UART_Handler(UART_Type *UARTn)
{
	int			ch;
	uint32_t	iir,lsr;
	UARTBUFFER	*ub;

	ub = uart_get_buffer(UARTn);
	for (;;) {
		iir = (UARTn->IIR & UART_IIR_BITMASK);                  /*!< Get UART Interrupt status */

		if(iir == UART_IIR_INTSTAT_PEND) {
			break;
        }

		switch (iir) {
       case UART_IIR_INTID_RDA:                                /*!< Receive Data Interrupt (0x04)*/ 
			ch = UARTn->RBR;
			if (ub->rxsize < ub->rxqsize) {
				ub->rxsize++;
				ub->buffer[ub->rxtail] = ch;
				if (++ub->rxtail >= ub->rxqsize)
					ub->rxtail = 0;
			}
			break;

        case UART_IIR_INTID_THRE:                               /*!< Transmit Hold Register Empty Interrupt (0x02) */ 
			if (ub->txsize==0) {
				UARTn->IER |= UART_IER_THREIE_Msk;
			} else if (ub->txqsize > 0) {
				ch = ub->buffer[ub->rxqsize + ub->txhead];
				if (++ub->txhead >= ub->txqsize)
					ub->txhead = 0;
				UARTn->THR = ch;
				if (--(ub->txsize) <= 0)
					break;
			}
			break;

        case UART_IIR_INTID_RLS:                                 /*!< Receive Line Status Interrupt (0x06) */
			lsr = (UARTn->LSR & UART_LSR_BITMASK);               /*!< Get Line Status */
			if (lsr & UART_LSR_BI)                               /*!< Break Condition (0x10) */
				ub->status |= UART_LSR_BI;
			if (lsr & UART_LSR_FE)                               /*!< Framing Error (0x08) */
				ub->status |= UART_LSR_FE;
			if (lsr & UART_LSR_PE)                               /*!< Parity Error (0x04) */
				ub->status |= UART_LSR_PE;
			if (lsr & UART_LSR_OE)                               /*!< Overrun Error (0x02) */
				ub->status |= UART_LSR_OE;
			break;
		}
	}
}

//void	UART0_Handler(void)
//{
//	UART_Handler(UART0);
//}

//void	UART1_Handler(void)
//{
//	UART_Handler(UART1);
//}

