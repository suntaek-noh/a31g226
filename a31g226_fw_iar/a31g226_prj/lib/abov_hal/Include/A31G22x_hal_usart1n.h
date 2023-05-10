/**********************************************************************//**
* @file		A31G22x_hal_usart1n.h
* @brief    Contains all macro definitions and function prototypes
*           support for USART firmware library on A31G22x
* @author   AE Team, ABOV Semiconductor Co., Ltd.
* @version  V1.00
* @date     28. DEC. 2020
*
* Copyright(C) 2020, ABOV Semiconductor All rights reserved.
********************************************************************************
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


#ifndef _USART1n_H_
#define _USART1n_H_

//------------------------------- Includes ----------------------------
#include <stdio.h>
#include <stdarg.h>
#include "A31G22x.h"
#include "A31G22x_hal_aa_types.h"

#ifdef __cplusplus
extern "C"
{
#endif

/** USART time-out definitions in case of using Read() and Write function
 * with Blocking Flag mode
 */

#define USART_BLOCKING_TIMEOUT			(0xFFFFFFFFUL)

#define USART_POLLING       (0)
#define USART_INTERRUPT     (1)

/* Private Macros ------------------------------------------------------------- */

/* --------------------- BIT DEFINITIONS -------------------------------------- */
/**********************************************************************//**
 * Macro defines for Macro defines for USART interrupt enable register
 **********************************************************************/
#define USART_IER_WAKEINT_EN		((uint16_t)(1<<2)) 	/*!< WAKE Interrupt enable*/
#define USART_IER_RXCINT_EN		((uint16_t)(1<<3)) 	/*!< RXC Interrupt enable*/
#define USART_IER_TXCINT_EN		((uint16_t)(1<<4)) 	/*!< TXC interrupt enable*/
#define USART_IER_DR_EN		    ((uint16_t)(1<<5)) 	/*!< DR interrupt enable */
#define USART_IER_BITMASK		    ((uint16_t)(0x3C))  /*!< USART interrupt enable register bit mask */

/**********************************************************************//**
 * Macro defines for Macro defines for USART interrupt status register
 **********************************************************************/
#define USART_ST_DMATXF           (0x01UL << 9)         /*!<Interrupt identification: DMA Tx Complete */
#define USART_ST_DMARXF           (0x01UL << 8)         /*!<Interrupt identification: DMA Rx Complete */
#define USART_ST_DRE              ((uint16_t)(1<<7)) 	/*!<Interrupt identification: Tx Buffer Busy*/
#define USART_ST_TXC              ((uint16_t)(1<<6)) 	/*!<Interrupt identification: Tx Complete*/
#define USART_ST_RXC              ((uint16_t)(1<<5)) 	/*!<Interrupt identification: Rx Complete*/
//#define USART_ST_WAKE           ((uint16_t)(1<<4)) 	/*!<Interrupt identification: Wake*/            // A31G226 doesn't support.
#define USART_ST_RTOF             (0x01UL << 3) /*!<Interrupt identification: Receive Time-out */
#define USART_ST_DOR              ((uint16_t)(1<<2))	/*!<Interrupt identification: Data OverRun */
#define USART_ST_FE               ((uint16_t)(1<<1))	/*!<Interrupt identification: Frame Error */
#define USART_ST_PE               ((uint16_t)(1<<0))	/*!<Interrupt identification: Parity Error */
#define USART_ST_BITMASK		  ((uint16_t)(0xFFF))	/*!<USART interrupt identification register bit mask */

#define USART_CR2_USTRX8	      ((uint16_t)(1<<0))        /*!< */
#define USART_CR2_USTTX8          ((uint16_t)(1<<1))        /*!< */
#define USART_CR2_USTSB           ((uint16_t)(1<<2))        /*!< */
#define USART_CR2_FXCH            ((uint16_t)(1<<3))        /*!< */
#define USART_CR2_USTSSEN         ((uint16_t)(1<<4))        /*!< */
#define USART_CR2_DISSCK          ((uint16_t)(1<<5))        /*!< */
#define USART_CR2_LOOPS           ((uint16_t)(1<<6))        /*!< */
#define USART_CR2_MASTER          ((uint16_t)(1<<7))        /*!< */
#define USART_CR2_DBLS            ((uint16_t)(1<<8))        /*!< */
#define USART_CR2_USTEN           ((uint16_t)(1<<9))        /*!< */
#define USART_CR2_BFREN             (0x01UL << 10)
#define USART_CR2_RTOEN             (0x01UL << 11)
#define USART_CR2_RTOIE             (0x01UL << 12)
#define USART_CR2_DMARXIE           (0x01UL << 13)
#define USART_CR2_DMATXIE           (0x01UL << 14)
#define USART_CR2_BITMASK         ((uint16_t)(0x7FF))       /*!< */

#define USART_RTO_RTOMASK           (0xFFFFFF)

#define USART_IP_INDEX_MAX          (4)

/***********************************************************************//**
 * @brief USART enumeration
**********************************************************************/
/**
 * @brief USART Databit type definitions
 */

typedef enum {
	USART_UART_MODE		= 0,     		/*!< UART mode */
	USART_SYNC_MODE,		     			/*!< Syncronous Mode mode*/
	USART_SPI_MODE        = 3    			/*!< USART 9 bit data mode */
} USART_OPMODE_Type;

typedef enum {
	USART_SPI_LSB		= 0,     		/*!< SPI LSB First */
	USART_SPI_MSB,		     		/*!< SPI MSB First */
} USART_SPI_ORDER_Type;

typedef enum {
	USART_SPI_TX_RISING		= 0,     	/*!< Txd Change : Rising / Rxd Change : Falling */
	USART_SPI_TX_FALLING,		     		/*!< Txd Change : Falling / Rxd Change : Rising */
} USART_ACK_Type;

typedef enum {
	USART_SPI_TX_LEADEDGE_SAMPLE		= 0,     	/*!< Leading edge : Sample / Trailing edge : Setup */
	USART_SPI_TX_LEADEDGE_SETUP,		     		/*!< Leading edge : Setup / Trailing edge : Sample */
} USART_EDGE_Type;



typedef enum {
	USART_DATABIT_5		= 0,     		/*!< USART 5 bit data mode */
	USART_DATABIT_6,		     			/*!< USART 6 bit data mode */
	USART_DATABIT_7,		     			/*!< USART 7 bit data mode */
	USART_DATABIT_8,		     			/*!< USART 8 bit data mode */
	USART_DATABIT_9       = 7    			/*!< USART 9 bit data mode */
} USART_DATABIT_Type;

/**
 * @brief USART Stop bit type definitions
 */
typedef enum {
	USART_STOPBIT_1		= (0),   					/*!< USART 1 Stop Bits Select */
	USART_STOPBIT_2		 							/*!< USART 2 Stop Bits Select */
} USART_STOPBIT_Type;

/**
 * @brief USART Parity type definitions
 */
typedef enum {
	USART_PARITY_NONE 	= 0,					/*!< No parity */
	USART_PARITY_EVEN     = 2,    				/*!< Even parity */
	USART_PARITY_ODD	 	= 3 					/*!< Odd parity */
} USART_PARITY_Type;

/*********************************************************************//**
* @brief USART Interrupt Type definitions
**********************************************************************/
/**
 * @brief USART Data Control type definition
 */
typedef enum {
	USART_CONTROL_USTRX8	= 0,
	USART_CONTROL_USTTX8,
	USART_CONTROL_USTSB,
	USART_CONTROL_FXCH,
	USART_CONTROL_USTSSEN,
	USART_CONTROL_DISSCK,
	USART_CONTROL_LOOPS,
	USART_CONTROL_MASTER,
	USART_CONTROL_DBLS,
	USART_CONTROL_USTEN
} USART_CONTROL_Type;

typedef enum {
    USART_STATUS_PE = 0,
    USART_STATUS_FE = 1,
    USART_STATUS_DOR = 2,
    USART_STATUS_RTOF = 3 ,
//    USART_STATUS_WAKE = 4,        // A31G226 doesn't support.
    USART_STATUS_RXC = 5,
    USART_STATUS_TXC = 6,
    USART_STATUS_DRE = 7,
    USART_STATUS_DMARXF = 8,
    USART_STATUS_DMATXF = 9,
    USART_STATUS_MASK = 0x3FFUL
} USART_STATUS_Type;


typedef enum {
//	USART_INTCFG_WAKE = 0,	/*!< Wake-up Interrupt enable*/     // A31G226 doesn't support.
	USART_INTCFG_RXC,		    /*!< Receive Complete Interrupt enable*/
	USART_INTCFG_TXC,		    /*!< Transmit Complete line status interrupt enable*/
	USART_INTCFG_DR		    /*!< Data Register Empty interrupt */
} USART_INT_Type;



typedef struct {
  uint32_t Baud_rate;   		/**< USART baud rate */
  USART_OPMODE_Type Mode;
  USART_SPI_ORDER_Type Order;
  USART_ACK_Type ACK;
  USART_EDGE_Type Edge;
  USART_DATABIT_Type Databits;   /**< Number of data bits, should be:
							   - USART_DATABIT_5: USART 5 bit data mode
							   - USART_DATABIT_6: USART 6 bit data mode
							   - USART_DATABIT_7: USART 7 bit data mode
							   - USART_DATABIT_8: USART 8 bit data mode
                               - USART_DATABIT_9: USART 9 bit data mode
							   */
  USART_PARITY_Type Parity;    	/**< Parity selection, should be:
							   - USART_PARITY_NONE: No parity
							   - USART_PARITY_ODD: Odd parity
							   - USART_PARITY_EVEN: Even parity
							   - USART_PARITY_SP_1: Forced "1" stick parity
							   - USART_PARITY_SP_0: Forced "0" stick parity
							   */
  USART_STOPBIT_Type Stopbits;   /**< Number of stop bits, should be:
							   - USART_STOPBIT_1: USART 1 Stop Bits Select
							   - USART_STOPBIT_2: USART 2 Stop Bits Select
							   */
  uint8_t	*RxTxBuffer;
  uint16_t	RxBufferSize;
  uint16_t	TxBufferSize;

} USART_CFG_Type;


HAL_Status_Type HAL_USART_Init(USART_Type *USTx, USART_CFG_Type *USART_ConfigStruct);
HAL_Status_Type HAL_USART_DeInit(USART_Type* USTx);
HAL_Status_Type HAL_USART_UART_Mode_Config(USART_CFG_Type *HAL_USART_InitStruct);
HAL_Status_Type HAL_USART_USRT_Mode_Config(USART_CFG_Type *HAL_USART_InitStruct);
HAL_Status_Type HAL_USART_SPI_Mode_Config(USART_CFG_Type *HAL_USART_InitStruct);
HAL_Status_Type HAL_USART_TransmitByte(USART_Type* USTx, uint8_t Data);
uint8_t HAL_USART_ReceiveByte(USART_Type* USTx);

uint32_t HAL_USART_Transmit(USART_Type *USTx, uint8_t *txbuf, uint32_t buflen);
uint32_t HAL_USART_Receive(USART_Type *USTx, uint8_t *rxbuf, uint32_t buflen);

HAL_Status_Type HAL_USART_ConfigInterrupt(USART_Type *USTx, USART_INT_Type USTIntCfg, FunctionalState NewState);
uint8_t HAL_USART_GetStatus(USART_Type* USTx);
HAL_Status_Type HAL_USART_ClearStatus(USART_Type* USTx, USART_STATUS_Type Status);
FlagStatus HAL_USART_CheckBusy(USART_Type *USTx);
HAL_Status_Type HAL_USART_DataControlConfig(USART_Type *USTx, USART_CONTROL_Type Mode, FunctionalState NewState);
HAL_Status_Type HAL_USART_Enable(USART_Type* USTx, FunctionalState state);

void HAL_USART_set_divisors(USART_Type *USARTx, uint32_t baudrate);

//void HAL_USART_SPI_Configuration(const USART_SPI_PORT_Type * USTx, USART_SPI_OPERATION_Type Type, USART_CFG_Type *UsartConfig);
void HAL_USART_SPI_SS_control(USART_Type * , char ch);
void HAL_USART_ReceiveTimeOut(USART_Type* USTx, FunctionalState rtoen, FunctionalState Intr, uint32_t TimeOut);
void HAL_USART_DmaConfig(USART_Type* USTx, uint32_t DmaInterrupType, FunctionalState en);

/* USART Send/Receive NEW functions ----------------------------------------------*/
typedef struct {
	volatile uint16_t	rxhead,rxtail,rxsize;
	volatile uint16_t	txhead,txtail,txsize;
	uint16_t	rxqsize,txqsize;
	uint8_t		status;
	uint8_t		*buffer;
} USARTBUFFER;

void	usart_set_buffer(USART_Type *USART1n, uint8_t *buffer, uint32_t rxqsize, uint32_t txqsize);
USARTBUFFER *usart_get_buffer(USART_Type *USART1n);

void 	usputc(USART_Type* USARTn, uint8_t Data);
int32_t	uswrite(USART_Type *USARTn,void *p,int size);
int32_t	usread(USART_Type *USARTn,void *p,int size);
int32_t	usputs(USART_Type *USARTn,char *s);

#define OS_INTEGER_USART_PRINTF_TMP_ARRAY_SIZE	128

int32_t	usprintf(USART_Type *USARTn, const char* format, ...);

int32_t	usgetq(USARTBUFFER *ub);
int32_t	uscheck(USART_Type* USARTn);
int32_t	usgetc(USART_Type* USARTn);
int32_t	usgetchar(USART_Type* USARTn);
int32_t	usgets(USART_Type* USARTn, char *strin, int32_t maxsize);

void	usflush(USART_Type* USARTn);

void USART_Handler(USART_Type *USARTn);
//void USART11_Handler(void)
//void USART12_13_Handler(void);

#ifdef __cplusplus
}
#endif


#endif /*  */

