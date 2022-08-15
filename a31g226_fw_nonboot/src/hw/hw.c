/*
 * hw.c
 *
 *  Created on: 2022. 7. 31.
 *      Author: sunta
 */



#include "hw.h"

__attribute__((section(".version"))) firm_version_t firm_ver =
    {
        _DEF_FIRMWATRE_VERSION,
        _DEF_BOARD_NAME
    };



void hwInit(void)
{
  bspInit();

  cliInit();
  gpioInit();
  logInit();
  //ledInit();
  buttonInit();
  uartInit();

  uartOpen(_DEF_UART1, 115200);

  logPrintf("[ Firmware Begin... ]\r\n");
  logPrintf("Booting..Name \t\t: %s\r\n", _DEF_BOARD_NAME);
  logPrintf("Booting..Ver  \t\t: %s\r\n", _DEF_FIRMWATRE_VERSION);

  logPrintf("SystemCoreClock\t\t: %dMhz\r\n", SystemCoreClock/1000000);
  logPrintf("SystemPeriClock\t\t: %dMhz\r\n", SystemPeriClock/1000000);

  buzzerInit();
  pwmInit();
  irRemoteInit();

  flashInit();
  dataFlashInit();

  lcdInit();
}
