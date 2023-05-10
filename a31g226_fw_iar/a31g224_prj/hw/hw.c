/*
 * hw.c
 *
 *  Created on: 2022. 7. 31.
 *      Author: sunta
 */



#include "hw.h"



void hwInit(void)
{
  __disable_irq();

  bspInit();

  uartInit();
  cliInit();
  logInit();
  gpioInit();

  buzzerInit();

  pwmInit();
  irRemoteInit();

  flashInit();
  dataFlashInit();
  
  lcdInit();

  uartOpen(_DEF_UART4, 115200);

  logPrintf("[ Firmware Begin... ]\r\n");
  logPrintf("Booting..Name \t\t: %s\r\n", _DEF_BOARD_NAME);
  logPrintf("Booting..Ver  \t\t: %s\r\n", _DEF_FIRMWATRE_VERSION);

  logPrintf("SystemCoreClock\t\t: %dMhz\r\n", SystemCoreClock/1000000);
  logPrintf("SystemPeriClock\t\t: %dMhz\r\n", SystemPeriClock/1000000);

  i2cInit();
  //tbuttonInit();
  //relayInit();


}
