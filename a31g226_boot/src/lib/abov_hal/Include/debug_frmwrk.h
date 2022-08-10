/**********************************************************************
* @file		debug_frmwrk.h
* @brief	Contains some utilities that used for debugging through UART or USART in UART mode
* @version	1.1
* @date: 	2019-12-20
* @author	BC Lee
*
* Copyright(C) 2019, ABOV Semiconductor. All rights reserved.
**********************************************************************/

#ifndef _DEBUG_FRMWRK_H_
#define _DEBUG_FRMWRK_H_

/* Includes ------------------------------------------------------------------- */
#include "A31G22x_hal_uartn.h"
#include "A31G22x_hal_usart1n.h"

//#define     DEBUG_POLLING
#define     DEBUG_INTERRUPT

#define DEBUG_PORT_NUMBER	10

#if (DEBUG_PORT_NUMBER == 0)
#define DEBUG_PORT	UART0
#elif (DEBUG_PORT_NUMBER == 1)
#define DEBUG_PORT	UART1
#elif (DEBUG_PORT_NUMBER == 10)
#define DEBUG_PORT	USART10
#elif (DEBUG_PORT_NUMBER == 11)
#define DEBUG_PORT	USART11
#elif (DEBUG_PORT_NUMBER == 12)
#define DEBUG_PORT	USART12
#elif (DEBUG_PORT_NUMBER == 13)
#define DEBUG_PORT	USART13
#else
#define	DEBUG_PORT	(NULL)
#endif

extern uint8_t uart_mode;
extern uint8_t usart_mode;

int32_t	cgetc(void);
int32_t	cgetchar(void);
int32_t	cgets(char *strin, int32_t maxsize);
void	cputc(char ch);
int32_t	cputs(char *strin);
void	cflush(void);
int32_t	cwrite(void *p,int size);
int32_t	cread(void *p,int size);
int32_t	cprintf(const  char *format, ...);

void 	debug_frmwrk_init(void);
void	dbg_init(void);
int32_t	dbg_console_check(void);
void	_DBG(const char *msg);



typedef struct {
	int32_t		type;
	char		*name;
	void		*variable;
	int32_t		arguement;
} DEBUG_VARIABLES;


#endif /* _DEBUG_FRMWRK_H_ */

/* --------------------------------- End Of File ------------------------------ */
