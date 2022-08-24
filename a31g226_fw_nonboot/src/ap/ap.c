/*
 * ap.c
 *
 *  Created on: 2022. 7. 31.
 *      Author: sunta
 */


#include "ap.h"


void apInit(void)
{
  cliOpen(_DEF_UART1, 115200);
}

void apMain(void)
{
  uint32_t pre_time;

  //uint8_t data  = 0;
  uint8_t count = 0;
  //uint8_t bit_count = 0;

  uint8_t step  = 0;

  lcdClearBuffer();

  pre_time = millis();

  while(1)
  {
    if(millis() - pre_time >= 500)
    {
      pre_time = millis();

      if(step == 0)
      {
        if(count < 10)
        {
          lcd7SegDis(1, count);
          lcd7SegDis(2, count);
          lcd7SegDis(3, count);
          lcd7SegDis(4, count);
          count++;
        }
        else
        {
          count = 0;
          step = 0;

          lcdClearBuffer();
        }
      }

    }

   cliMain();
  }




#if 0
  while(1)
  {
    if(millis() - pre_time >= 1000)
    {
      pre_time = millis();

      //lcdAllDisp();
      //ledToggle(_DEF_LED1);
      //ledToggle(_DEF_LED2);
      //ledToggle(_DEF_LED3);
      //ledToggle(_DEF_LED4);
      //ledToggle(_DEF_LED5);

      //touchGetTouchedData(touch_data);


      //logPrintf("touch value : 0x%x\r\n", tbuttonGetValue());
    }

    //tbuttonISR();
#endif





#if 0
    if(uartAvailable(_DEF_UART1) > 0)
    {
      uint8_t rx_data;


      rx_data = uartRead(_DEF_UART1);

      if(rx_data == 'm')
      {
        logPrintf("m menu\n");
        logPrintf("e erase\n");
        logPrintf("w write\n");
        logPrintf("s show\n");
      }

      if(rx_data == 'e')
      {
        logPrintf("erase...\n");
        flashErase(0x00000000, 256);
        logPrintf("OK\n");
      }

      if(rx_data == 'w')
      {
        uint8_t w_data[256];

        for(int i=0; i<256; i++)
        {
          w_data[i] = i;
        }

        logPrintf("write...\n");
        flashWrite(0x00000000, w_data, 256);
        logPrintf("OK\n");
      }

      if(rx_data == 's')
      {

        uint8_t r_data[256];

        logPrintf("show...\n");

        flashRead(0, r_data, 256);

        for(int i=0; i<256; i++)
        {
          logPrintf("%d : %d\n", i, r_data[i]);
        }

        logPrintf("OK\n");
      }



    }
#endif

#if 0
    if(buttonGetPressed(_DEF_BUTTON1) == true)
    {
      uartPrintf(_DEF_UART1, "pressed\n");
    }
    else
    {
      uartPrintf(_DEF_UART1, "released\n");
    }

    delay(100);
#endif
#if 0
    if(uartAvailable(_DEF_UART1) > 0)
    {
      uartPrintf(_DEF_UART1, "rx : 0x%X\n", uartRead(_DEF_UART1));
    }
#endif




}
