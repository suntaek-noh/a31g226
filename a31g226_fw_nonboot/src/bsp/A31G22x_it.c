/**********************************************************************
* @file		A31G22x_it.c
* @brief	Contains all macro definitions and function prototypes
* 			support for PCU firmware library
* @version	1.00
* @date		
* @author	ABOV AE team
*
* Copyright(C) 2020, ABOV Semiconductor
* All rights reserved.
*
**********************************************************************/
#include "A31G22x_it.h"

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private function prototypes -----------------------------------------------*/
/* Private variables ---------------------------------------------------------*/


/******************************************************************************/
/*            Cortex M3 Processor Exceptions Handlers                         */
/******************************************************************************/
void NMI_Handler(void) {

}

void HardFault_Handler(void) {

  /* Go to infinite loop when Hard Fault exception occurs */
  while (1)
  {
  }
}

void SVC_Handler(void) {

}

void PendSV_Handler(void) {

}

/* SYSTEM */
void LVI_IRQHandler(void) {

}

void SYSCLKFAIL_IRQHandler(void) {

}

/* GPIO */
void GPIOAB_IRQHandler(void) {

}

void GPIOCD_IRQHandler(void) {

}

void GPIOE_IRQHandler(void) {

}

void GPIOF_IRQHandler(void) {

}

/* TIMERS */
void TIMER10_IRQHandler(void) {

}

void TIMER11_IRQHandler(void) {

}

void TIMER12_IRQHandler(void) {

}

void TIMER13_IRQHandler(void) {

}

void TIMER14_IRQHandler(void) {

}

void TIMER16_IRQHandler(void) {

}

void TIMER20_IRQHandler(void) {

}

void TIMER21_IRQHandler(void) {

}

void TIMER30_IRQHandler(void) {

}

void WDT_IRQHandler(void) {

}

void WT_IRQHandler(void) {

}

/* SERIAL INTERFACES */
void I2C0_IRQHandler(void) {

}

void I2C1_IRQHandler(void) {

}

void I2C2_SPI20_IRQHandler(void) {

}

//void USART10_IRQHandler(void) {
    //USART_Handler(USART10);
//}

void USART11_IRQHandler(void) {
    //USART_Handler(USART11);
}

#if defined(USART12) || defined(UART13)
void USART12_13_SPI21_IRQHandler(void) {
#ifdef USART12
	//USART_Handler(USART12);
#endif
#ifdef USART13
	//USART_Handler(USART13);
#endif    
}
#endif

//void UART0_IRQHandler(void) {
    //UART_Handler(UART0);
//}

void UART1_IRQHandler(void) {
    //UART_Handler(UART1);
}

/* ANALOG FUNCTIONS */
void ADC_IRQHandler(void) {

}

void DAC_IRQHandler(void) {

}

void TEMP_SENSOR_IRQHandler(void) {

}

void CMP_CRC_IRQHandler(void) {

}

/******************************************************************************/
/*                 A31G22x Peripherals Interrupt Handlers                     */
/*  Add here the Interrupt Handler for the used peripheral(s) (PPP), for the  */
/*  available peripheral interrupt handler's name please refer to the startup */
/*  file (startup_A31G22x.s).                                                 */
/******************************************************************************/
