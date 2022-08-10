/*
 * ap.c
 *
 *  Created on: 2022. 7. 31.
 *      Author: sunta
 */


#include "ap.h"
#include "boot/boot.h"



cmd_t cmd;          //명렁어 처리하기 위한 구조체



void apInit(void)
{
  cmdInit(&cmd);
  cmdOpen(&cmd, _DEF_UART1, 115200);

}

void apMain(void)
{
  uint32_t pre_time;

  pre_time = millis();

  while(1)
  {
    if(millis() - pre_time >= 100)
    {
      pre_time = millis();
      ledToggle(_DEF_LED1);
    }

#if 1
    if (cmdReceivePacket(&cmd) == true)
    {
      bootProcessCmd(&cmd);
    }
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
        flashErase(0x8000, 256);
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
        flashWrite(0x8000, w_data, 256);
        logPrintf("OK\n");
      }

      if(rx_data == 's')
      {

        uint8_t r_data[256];

        logPrintf("show...\n");

        flashRead(0x8000, r_data, 256);

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
}
