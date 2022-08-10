/*
 * hw.c
 *
 *  Created on: 2022. 7. 31.
 *      Author: sunta
 */



#include "hw.h"


extern uint32_t _flash_tag_addr;
extern uint32_t __isr_vector_addr;


__attribute__((section(".tag"))) firm_tag_t firm_tag =
    {
        .magic_number = 0xAAAA5555,

        //-- fw info
        //
        .addr_tag = (uint32_t)&_flash_tag_addr,
        .addr_fw  = (uint32_t)&__isr_vector_addr,
        .size_tag = 1024,


        //-- tag info
        //
    };


__attribute__((section(".version"))) firm_version_t firm_ver =
    {
        _DEF_FIRMWATRE_VERSION,
        _DEF_BOARD_NAME
    };



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

  flashInit();
}
