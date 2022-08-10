/**********************************************************************
* @file		debug_frmwrk.c
* @brief	Contains some utilities that used for debugging through UART
* @version	2.0
* @date: 	2020-01-08
* @author	BC Lee
*
* Copyright(C) 2017-2020, ABOV Semiconductor. All rights reserved.
**********************************************************************/

#ifndef _DEBUG_FRMWRK_
#define _DEBUG_FRMWRK_

/* Includes ------------------------------------------------------------------- */
#include "debug_frmwrk.h"
#include "A31G22x_hal_pcu.h"
#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
/* Debug framework */

/*********************************************************************//**
 * @brief		Initialize Debug frame work through initializing UART port
 * @param[in]	None
 * @return		None
 **********************************************************************/

#ifdef      DEBUG_INTERRUPT
#undef      DEBUG_POLLING
#define	DEBUG_TX_SIZE	64
#define	DEBUG_RX_SIZE	64
static uint8_t	_debug_buffer[DEBUG_RX_SIZE + DEBUG_TX_SIZE];
#else
#define     DEBUG_POLLING
#undef      DEBUG_INTERRUPT
#define	DEBUG_TX_SIZE	0
#define	DEBUG_RX_SIZE	0
uint8_t	*_debug_buffer = NULL;
#endif

#if (DEBUG_PORT_NUMBER == 0 || DEBUG_PORT_NUMBER == 1)
void 	debug_frmwrk_init_uart(void)
{
	UART_CFG_Type UARTConfigStruct;

#if (DEBUG_PORT_NUMBER == 0)
	/*
	 * Initialize UART0 pin connect
	 */
	HAL_GPIO_ConfigOutput(PB, 5, PCU_MODE_ALT_FUNC);
	HAL_GPIO_ConfigFunction(PB, 5, PCU_ALT_FUNCTION_1);
	HAL_GPIO_ConfigPullup(PB, 5, ENPU);

	HAL_GPIO_ConfigOutput(PB, 4, PCU_MODE_ALT_FUNC);
	HAL_GPIO_ConfigFunction(PB, 4, PCU_ALT_FUNCTION_1);

#elif (DEBUG_PORT_NUMBER == 1)
	/*
	 * Initialize UART1 pin connect
	 */
	HAL_GPIO_ConfigOutput(PB, 7, PCU_MODE_ALT_FUNC);
	HAL_GPIO_ConfigFunction(PB, 7, PCU_ALT_FUNCTION_1);
	HAL_GPIO_ConfigPullup(PB, 7, PCU_PUPD_DISABLE);
	
	HAL_GPIO_ConfigOutput(PB, 6, PCU_MODE_ALT_FUNC);
	HAL_GPIO_ConfigFunction(PB, 6, PCU_ALT_FUNCTION_1);
	
#endif

	/* Initialize UART Configuration parameter structure to default state:
	 * Baudrate = 38400bps
	 * 8 data bit
	 * 1 Stop bit
	 * None parity
	 */
	HAL_UART_ConfigStructInit(&UARTConfigStruct);
	UARTConfigStruct.Baud_rate = 38400;
    
	UARTConfigStruct.RxTxBuffer = (uint8_t *)_debug_buffer;
	UARTConfigStruct.RxBufferSize = DEBUG_RX_SIZE;
	UARTConfigStruct.TxBufferSize = DEBUG_TX_SIZE;

	// Initialize DEBUG_UART_PORT peripheral with given to corresponding parameter
	HAL_UART_Init((UART_Type*)DEBUG_PORT, &UARTConfigStruct);
}
#endif

#if (DEBUG_PORT_NUMBER == 10 || DEBUG_PORT_NUMBER == 11 || DEBUG_PORT_NUMBER == 12 || DEBUG_PORT_NUMBER == 13)
void 	debug_frmwrk_init_usart(void)
{
	USART_CFG_Type UST_InitStruct;

#if (DEBUG_PORT_NUMBER == 10)
	/*
	 * Initialize USART10 pin connect
	 */
    HAL_GPIO_ConfigOutput(PB, 1, PCU_MODE_ALT_FUNC);
    HAL_GPIO_ConfigFunction(PB, 1, PCU_ALT_FUNCTION_1);
    HAL_GPIO_ConfigPullup(PB, 1, PCU_PUPD_PULL_UP);

    // Initialize Txd port connect
    HAL_GPIO_ConfigOutput(PB, 0, PCU_MODE_ALT_FUNC);
    HAL_GPIO_ConfigFunction(PB, 0, PCU_ALT_FUNCTION_1);
//    HAL_GPIO_ConfigPullup(PB, 0, PCU_PUPD_PULL_UP);    

#elif (DEBUG_PORT_NUMBER == 11)
	/*
	 * Initialize USART11 pin connect
	 */
        // Initialize Rxd port connect
        HAL_GPIO_ConfigOutput(PD, 3, PCU_MODE_ALT_FUNC);
        HAL_GPIO_ConfigFunction(PD, 3, PCU_ALT_FUNCTION_1);
        HAL_GPIO_ConfigPullup(PD, 3, PCU_PUPD_PULL_UP);

        // Initialize Txd port connect
        HAL_GPIO_ConfigOutput(PD, 2, PCU_MODE_ALT_FUNC);
        HAL_GPIO_ConfigFunction(PD, 2, PCU_ALT_FUNCTION_1);


#elif (DEBUG_PORT_NUMBER == 12)
	/*
	 * Initialize USART12 pin connect
	 */
        // Initialize Rxd port connect
        HAL_GPIO_ConfigOutput(PE, 13, PCU_MODE_ALT_FUNC);
        HAL_GPIO_ConfigFunction(PE, 13, PCU_ALT_FUNCTION_1);
        HAL_GPIO_ConfigPullup(PE, 13, PCU_PUPD_PULL_UP);

        // Initialize Txd port connect
        HAL_GPIO_ConfigOutput(PE, 12, PCU_MODE_ALT_FUNC);
        HAL_GPIO_ConfigFunction(PE, 12, PCU_ALT_FUNCTION_1);
//        HAL_GPIO_ConfigPullup(PE, 12, PCU_PUPD_PULL_UP);

#elif (DEBUG_PORT_NUMBER == 13)
	/*
	 * Initialize USART13 pin connect
	 */
        // Initialize Rxd port connect
        HAL_GPIO_ConfigOutput(PE, 9, PCU_MODE_ALT_FUNC);
        HAL_GPIO_ConfigFunction(PE, 9, PCU_ALT_FUNCTION_1);
        HAL_GPIO_ConfigPullup(PE, 9, PCU_PUPD_PULL_UP);

        // Initialize Txd port connect
        HAL_GPIO_ConfigOutput(PE, 8, PCU_MODE_ALT_FUNC);
        HAL_GPIO_ConfigFunction(PE, 8, PCU_ALT_FUNCTION_1);
//        HAL_GPIO_ConfigPullup(PE, 8, PCU_PUPD_PULL_UP);

#endif

	/* Initialize UART Configuration parameter structure to default state:
	 * Baudrate = 38400bps
	 * 8 data bit
	 * 1 Stop bit
	 * None parity
	 */
	HAL_USART_UART_Mode_Config(&UST_InitStruct);
	UST_InitStruct.Baud_rate = 38400;
	UST_InitStruct.RxTxBuffer = _debug_buffer;
	UST_InitStruct.RxBufferSize = DEBUG_RX_SIZE;
	UST_InitStruct.TxBufferSize = DEBUG_TX_SIZE;

	// Initialize DEBUG_UART_PORT peripheral with given to corresponding parameter
	HAL_USART_Init(DEBUG_PORT, &UST_InitStruct);
}
#endif

void 	debug_frmwrk_init(void)
{
#if (DEBUG_PORT_NUMBER == 0 || DEBUG_PORT_NUMBER == 1)
    
#ifdef DEBUG_POLLING
    uart_mode = 0;
#endif
#ifdef DEBUG_INTERRUPT
    uart_mode = 1;
#endif

  	debug_frmwrk_init_uart();

    
#elif (DEBUG_PORT_NUMBER == 10 || DEBUG_PORT_NUMBER == 11 || DEBUG_PORT_NUMBER == 12 || DEBUG_PORT_NUMBER == 13)

#ifdef DEBUG_POLLING
    usart_mode = 0;
#endif
#ifdef DEBUG_INTERRUPT
    usart_mode = 1;
#endif
    
	debug_frmwrk_init_usart();
#endif
}


void	_DBG(const char *msg)
{
#if (DEBUG_PORT_NUMBER == 0 || DEBUG_PORT_NUMBER == 1)
	uputs(DEBUG_PORT, (void*)msg);
#elif (DEBUG_PORT_NUMBER == 10 || DEBUG_PORT_NUMBER == 11 || DEBUG_PORT_NUMBER == 12 || DEBUG_PORT_NUMBER == 13)
	usputs(DEBUG_PORT, (void*)msg);
#endif
}

#if (DEBUG_PORT_NUMBER == 0 || DEBUG_PORT_NUMBER == 1)
int32_t	cgetc(void)		{	return ugetc(DEBUG_PORT);		}
int32_t	cgetchar(void)	{	return ugetchar(DEBUG_PORT);	}
int32_t	cgets(char *strin, int32_t maxsize)	{	return ugets(DEBUG_PORT, strin, maxsize);	}
void	cputc(char ch)	{	uputc(DEBUG_PORT, ch);	}
int32_t	cputs(char *strin)	{	return uputs(DEBUG_PORT, strin);	}
int32_t	cwrite(void *p,int size)	{	return uwrite(DEBUG_PORT, p, size);	}
int32_t	cread(void *p,int size)		{	return uread(DEBUG_PORT, p, size);		}
void	cflush(void)	{	uflush(DEBUG_PORT);		}

#elif (DEBUG_PORT_NUMBER == 10 || DEBUG_PORT_NUMBER == 11 || DEBUG_PORT_NUMBER == 12 || DEBUG_PORT_NUMBER == 13)
int32_t	cgetc(void)		{	return usgetc(DEBUG_PORT);		}
int32_t	cgetchar(void)	{	return usgetchar(DEBUG_PORT);	}
int32_t	cgets(char *strin, int32_t maxsize)	{	return usgets(DEBUG_PORT, strin, maxsize);	}
void	cputc(char ch)	{	usputc(DEBUG_PORT, (uint8_t)ch);	}
int32_t	cputs(char *strin)	{	return usputs(DEBUG_PORT, strin);	}
int32_t	cwrite(void *p,int size)	{	return uswrite(DEBUG_PORT, p, size);	}
int32_t	cread(void *p,int size)		{	return usread(DEBUG_PORT, p, size);		}
void	cflush(void)	{	usflush(DEBUG_PORT);		}

#endif

#define	OS_INTEGER_DEBUG_PRINTF_TMP_ARRAY_SIZE	128
static char buf[OS_INTEGER_DEBUG_PRINTF_TMP_ARRAY_SIZE];


int32_t cprintf(const char* format, ...)
{
  int ret;
  va_list ap;

  va_start (ap, format);

  // Print to the local buffer
  ret = vsnprintf (buf, sizeof(buf), format, ap);
  if (ret > 0)
    {
      // Transfer the buffer to the device
      cwrite(buf, (size_t)ret);
    }

  va_end (ap);
  return ret;
}

/**************************************************************************
 *
 *	Debug
 *		type: 	0=Executable, 1=Byte, 2=Short, 4=Int32(Long), (bit-16 = 1) = string
 *				-1=end-of-table.
 *		name: command name string
 *		variable:	variable ptr or function ptr to execute.
 *		arguement:	function arguement when the variable is a function.
 *
 **************************************************************************/
static	int32_t	_cmd_line_size;
static	char	_cmd_line[64];

extern	DEBUG_VARIABLES dbg_variables[];

DEBUG_VARIABLES	*dbg_find_table(char *name)
{
	DEBUG_VARIABLES	*p;
	int	i;

	for(i = 0; i < 255; i++) {
		p = &dbg_variables[i];
		if (p->type < 0) {
			break;
		}
		if (strcmp(name, p->name) == 0) {
			return p;
		}
	}
	return NULL;
}

int32_t	dbg_print_value(char *name)
{
	DEBUG_VARIABLES	*p;

	p = dbg_find_table(name);
	if (p != NULL) {
		if (p->type < 0) {
			;
		} else {
			switch (p->type) {
			default:	cputs(" Not supported format.");
			case 0:	//executable
					cputs(" - Not available.");
					break;
			case 1:	//byte
					cprintf(" %s = %d, 0x%02X ('%c')", name, *((int8_t*)p->variable), *((uint8_t*)p->variable), *((uint8_t*)p->variable) );
					break;
			case 2:	//byte
					cprintf(" %s = %d (0x%04X)", name, *((int16_t*)p->variable),*((uint16_t*)p->variable) );
					break;
			case 4:	//byte
					cprintf(" %s = %d (0x%08X)", name, *((int32_t*)p->variable),*((uint32_t*)p->variable) );
					break;
			}
		}
	} else {
		cputs(" - command not found.");
	}
	//cputs("\r\n");
	return 0;
}

uint32_t	datol(char *a)
{
	int			i;
	uint32_t	value;

	i = 0;
	value = 0;
	if (a[0] == '0' && a[1] == 'x') {
		for (i = 2; i < 10; i++) {
			uint32_t	t;
			t = a[i];
			if (t >= '0' && t <= '9') {
				value <<= 4;
				value += t - '0';
			} else if (t >= 'a' && t <='f') {
				value <<= 4;
				value += t - 'a' + 10;
			} else if (t >= 'A' && t <='F') {
				value <<= 4;
				value += t - 'A' + 10;
			} else break;
		}
	} else {
		int sign = 0;
		if (a[0] == '-') {
			sign = 1;
		}
		for (i = sign; i < 16; i++) {
			uint32_t	t;
			t = a[i];
			if (t >= '0' && t <= '9') {
				value *= 10;
				value += t - '0';
			} else break;
		}
		if (sign)
			value = (uint32_t)(-(int32_t)value);
	}
	return value;
}

int32_t	dbg_set_value(char *name, char *pvalue)
{
	DEBUG_VARIABLES	*p;

	p = dbg_find_table(name);
	if (p != NULL) {
		if (p->type < 0) {
			;
		} else {
			switch (p->type) {
			default:	cputs(" Not supported format.");
			case 0:	//executable
					cputs(" - Not available.");
					break;
			case 1:	//byte
					*((uint8_t*)p->variable) = datol(pvalue);
					break;
			case 2:	//short
					*((uint16_t*)p->variable) = datol(pvalue);
					break;
			case 4:	//long
					*((uint32_t*)p->variable) = datol(pvalue);
					break;
			}
		}
	} else {
		cputs(" - command not found.");
	}
	//cputs("\r\n");
	return 0;
}

int32_t	dbg_run_value(char *name, char *cmd)
{
	int32_t	result = 0;
	DEBUG_VARIABLES	*p;

	p = dbg_find_table(name);
	if (p != NULL) {
		if (p->type < 0) {
			;
		} else if (p->type == 0) {
			int32_t	(*exec)(int32_t);
			*(void**)&exec = p->variable;
			(*exec)(p->arguement);
		} else {
			cputs(" - Not available.");
		}
	} else if (name[0] == 0) {
		;
	} else {
		extern	int cmd_run(char *cmd);
		//cputs(" - command not found.");
		cmd_run(cmd);
	}
	//cputs("\r\n");
	return result;
}

int32_t	dbg_execute(char *cmd)
{
	char	ch;
	int		size;
	char	varname[16];
	char	*p = cmd;

	while (*p == ' ') p++;

	if (p[0] == 0) {
		return 0;
	}

	cputs("\r\n");

	size = 0;
	while (1) {
		ch = *p++;
		if (ch == ' ') {
			break;
		}
		if (ch == '?' || ch == '=' || ch == '\r' || ch == '\n' || ch == 0) {
			break;
		}
		varname[size++] = ch;
	}
	varname[size++] = 0;

	if (ch == ' ') {
		while ((ch = *p++) == ' ');
	}

	if (ch == '?') {
		dbg_print_value(varname);
	} else if (ch == '=') {
		while (*p == ' ') p++;
		dbg_set_value(varname, p);
	} else {
		dbg_run_value(varname, cmd);
	}

	return 1;
}

void	dbg_init(void)
{
	_cmd_line_size = 0;
	memset((void*)_cmd_line,0,sizeof(_cmd_line));
	cputs("\r\n*");
}

int32_t	dbg_console_check(void)
{
	int result = 0;
	int	ch;

	ch = cgetchar();
	if (ch >= 0) {
		if (ch == 8) {
			if (_cmd_line_size > 0) {
				cputc(ch);
				_cmd_line_size--;
			}
		} else if (ch == '\n') {

		} else if (ch == '\r') {
			//cputs("\n");
			_cmd_line[_cmd_line_size++] = 0;
			result = dbg_execute(_cmd_line);
			dbg_init();
		} else {
			cputc(ch);
			_cmd_line[_cmd_line_size++] = ch;
		}
	}
	return result;
}



#endif /* _DEBUG_FRMWRK_ */

/* --------------------------------- End Of File ------------------------------ */
