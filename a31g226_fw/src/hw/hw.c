/*
 * hw.c
 *
 *  Created on: 2022. 7. 31.
 *      Author: sunta
 */



#include "hw.h"


void hwInit(void)
{
  bspInit();

  logInit();
  ledInit();
  buttonInit();
  uartInit();

  uartOpen(_DEF_UART1, 115200);

  logPrintf("[ Firmware Begin... ]\r\n");
  logPrintf("Booting..Name \t\t: %s\r\n", _DEF_BOARD_NAME);
  logPrintf("Booting..Ver  \t\t: %s\r\n", _DEF_FIRMWATRE_VERSION);

}
