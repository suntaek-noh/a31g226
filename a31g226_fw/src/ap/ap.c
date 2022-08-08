/*
 * ap.c
 *
 *  Created on: 2022. 7. 31.
 *      Author: sunta
 */


#include "ap.h"


void apInit(void)
{

}

void apMain(void)
{
  uint32_t pre_time;

  pre_time = millis();

  while(1)
  {
    if(millis() - pre_time >= 500)
    {
      pre_time = millis();
      ledToggle(_DEF_LED1);
      ledToggle(_DEF_LED2);
      ledToggle(_DEF_LED3);
      ledToggle(_DEF_LED4);
      ledToggle(_DEF_LED5);
    }

    if(uartAvailable(_DEF_UART1) > 0)
    {
      uartPrintf(_DEF_UART1, "rx : 0x%X\n", uartRead(_DEF_UART1));
    }
  }
}
