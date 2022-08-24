/*
 * uart.c
 *
 *  Created on: 2022. 8. 1.
 *      Author: sunta
 */



#include "uart.h"
#include "qbuffer.h"


#ifdef _USE_HW_UART
//#define _USE_UART2

#define UART_RX_BUF_LENGTH      256


typedef enum
{
  UART_HW_TYPE_MCU,
  UART_HW_TYPE_USB,
} UartHwType_t;


typedef struct
{
  bool            is_open;
  uint32_t        baud;
  UartHwType_t    type;

  uint8_t         rx_buf[UART_RX_BUF_LENGTH];
  qbuffer_t       qbuffer;

  UART_Type       *p_huart;
  USART_Type      *p_husart;
  UART_CFG_Type   uart_init;
  USART_CFG_Type  usart_init;
} uart_tbl_t;


static uart_tbl_t uart_tbl[UART_MAX_CH];

bool uartInit(void)
{
  for (int i=0; i<UART_MAX_CH; i++)
  {
    uart_tbl[i].is_open = false;
    uart_tbl[i].baud = 115200;
  }

  return true;
}

bool uartOpen(uint8_t ch, uint32_t baud)
{
  bool ret = false;
  //HAL_Status_Type status;

  switch(ch)
  {
    case _DEF_UART1:
      uart_tbl[ch].type    = UART_HW_TYPE_MCU;
      uart_tbl[ch].baud    = baud;

      uart_tbl[ch].p_husart = USART11;

      uart_tbl[ch].usart_init.Mode               = USART_UART_MODE;
      uart_tbl[ch].usart_init.Baud_rate          = baud;
      uart_tbl[ch].usart_init.Databits           = UART_DATABIT_8;
      uart_tbl[ch].usart_init.Parity             = UART_PARITY_NONE;
      uart_tbl[ch].usart_init.Stopbits           = UART_STOPBIT_1;
      uart_tbl[ch].usart_init.RxTxBuffer         = NULL;
      uart_tbl[ch].usart_init.RxBufferSize       = 0;
      uart_tbl[ch].usart_init.TxBufferSize       = 0;

      qbufferCreate(&uart_tbl[ch].qbuffer, uart_tbl[ch].rx_buf, UART_RX_BUF_LENGTH);

      // Initialize Rxd port connect
      HAL_GPIO_ConfigOutput(PD, 3, PCU_MODE_ALT_FUNC);
      HAL_GPIO_ConfigFunction(PD, 3, PCU_ALT_FUNCTION_1);
      HAL_GPIO_ConfigPullup(PD, 3, PCU_PUPD_PULL_UP);

      // Initialize Txd port connect
      HAL_GPIO_ConfigOutput(PD, 2, PCU_MODE_ALT_FUNC);
      HAL_GPIO_ConfigFunction(PD, 2, PCU_ALT_FUNCTION_1);

      HAL_USART_Init(uart_tbl[ch].p_husart, &uart_tbl[ch].usart_init);

      uart_tbl[ch].p_husart->CR1 |= USART_IER_RXCINT_EN;

      NVIC_SetPriority(USART11_IRQn, 5);
      NVIC_EnableIRQ(USART11_IRQn);

      uart_tbl[ch].is_open = true;

      ret = true;
      break;

#ifdef _USE_UART2
    case _DEF_UART1:
      uart_tbl[ch].type    = UART_HW_TYPE_MCU;
      uart_tbl[ch].baud    = baud;

      uart_tbl[ch].p_huart = UART0;

      uart_tbl[ch].uart_init.Baud_rate          = baud;
      uart_tbl[ch].uart_init.Databits           = UART_DATABIT_8;
      uart_tbl[ch].uart_init.Parity             = UART_PARITY_NONE;
      uart_tbl[ch].uart_init.Stopbits           = UART_STOPBIT_1;
      uart_tbl[ch].uart_init.RxTxBuffer         = NULL;
      uart_tbl[ch].uart_init.RxBufferSize       = 0;
      uart_tbl[ch].uart_init.TxBufferSize       = 0;

      qbufferCreate(&uart_tbl[ch].qbuffer, uart_tbl[ch].rx_buf, UART_RX_BUF_LENGTH);

      HAL_GPIO_ConfigOutput(PB, 5, PCU_MODE_ALT_FUNC);
      HAL_GPIO_ConfigFunction(PB, 5, PCU_ALT_FUNCTION_1);
      HAL_GPIO_ConfigPullup(PB, 5, PCU_PUPD_PULL_UP);

      HAL_GPIO_ConfigOutput(PB, 4, PCU_MODE_ALT_FUNC);
      HAL_GPIO_ConfigFunction(PB, 4, PCU_ALT_FUNCTION_1);

      status = HAL_UART_Init(uart_tbl[ch].p_huart, &uart_tbl[ch].uart_init);

      uart_tbl[ch].p_huart->IER = UART_IER_DREINT_EN;

      NVIC_SetPriority(UART0_IRQn, 5);
      NVIC_EnableIRQ(UART0_IRQn);

      uart_tbl[ch].is_open = true;

      ret = true;
      break;
#endif
  }

  return ret;
}

bool uartClose(uint8_t ch)
{
  return true;
}

uint32_t uartAvailable(uint8_t ch)
{
  uint32_t ret = 0;

  switch(ch)
  {
    case _DEF_UART1:
      ret = qbufferAvailable(&uart_tbl[ch].qbuffer);
      break;

#ifdef _USE_UART2
    case _DEF_UART2:
      ret = qbufferAvailable(&uart_tbl[ch].qbuffer);
      break;
#endif
  }

  return ret;
}

bool uartFlush(uint8_t ch)
{
  uint32_t pre_time;

  pre_time = millis();
  while(uartAvailable(ch))
  {
    if (millis()-pre_time >= 10)
    {
      break;
    }
    uartRead(ch);
  }

  return true;
}

uint8_t uartRead(uint8_t ch)
{
  uint8_t ret = 0;

  switch(ch)
  {
    case _DEF_UART1:
      qbufferRead(&uart_tbl[ch].qbuffer, &ret, 1);
      break;

#ifdef _USE_UART2
    case _DEF_UART2:
      qbufferRead(&uart_tbl[ch].qbuffer, &ret, 1);
      break;
#endif
  }

  return ret;
}

uint32_t uartWrite(uint8_t ch, uint8_t *p_data, uint32_t length)
{
  uint32_t ret = 0;

  switch(ch)
  {
    case _DEF_UART1:
      ret = uswrite(uart_tbl[ch].p_husart, (void *)p_data, (int)length);
      break;

#ifdef _USE_UART2
    case _DEF_UART2:
      ret = uwrite(uart_tbl[ch].p_huart, (void *)p_data, (int)length);
      break;
#endif
  }

  return ret;
}

uint32_t uartPrintf(uint8_t ch, const char *fmt, ...)
{
  char buf[256];
  va_list args;
  int len;
  uint32_t ret;

  va_start(args, fmt);
  len = vsnprintf(buf, 256, fmt, args);

  ret = uartWrite(ch, (uint8_t *)buf, len);

  va_end(args);


  return ret;
}

uint32_t uartGetBaud(uint8_t ch)
{
  uint32_t ret = 0;

  switch(ch)
  {
    case _DEF_UART1:
      ret = uart_tbl[ch].baud;
      break;

#ifdef _USE_UART2
    case _DEF_UART2:
      ret = uart_tbl[ch].baud;
      break;
#endif
  }

  return ret;
}

#ifdef _USE_UART2
void UART0_IRQHandler(void)
{
  return;
}
#endif

void USART11_IRQHandler(void)
{
  uart_tbl_t *p_usart = &uart_tbl[_DEF_UART1];
  uint32_t status_reg;
  uint32_t isr_id;

  status_reg = p_usart->p_husart->ST;
  isr_id = status_reg & 0xE0;

  if(isr_id & USART_ST_RXC_Msk)
  {
    uint8_t read_data;
    read_data = p_usart->p_husart->DR;

    qbufferWrite(&p_usart->qbuffer, &read_data, 1);

  }
  return;
}

#endif
