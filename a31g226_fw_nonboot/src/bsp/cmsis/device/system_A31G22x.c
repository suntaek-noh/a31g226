/**************************************************************************//**
 * @file     system_A31G22x.c
 * @brief    CMSIS Cortex-M0+ Device Peripheral Access Layer Source File for
 *           Device A31G22x
 * @version  V1.00
 * @date     22 July 2020
 * @author   ABOV AE Team
 ******************************************************************************/
/* Copyright (c) 2012 ARM LIMITED

   All rights reserved.
   Redistribution and use in source and binary forms, with or without
   modification, are permitted provided that the following conditions are met:
   - Redistributions of source code must retain the above copyright
     notice, this list of conditions and the following disclaimer.
   - Redistributions in binary form must reproduce the above copyright
     notice, this list of conditions and the following disclaimer in the
     documentation and/or other materials provided with the distribution.
   - Neither the name of ARM nor the names of its contributors may be used
     to endorse or promote products derived from this software without
     specific prior written permission.
   *
   THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
   AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
   IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
   ARE DISCLAIMED. IN NO EVENT SHALL COPYRIGHT HOLDERS AND CONTRIBUTORS BE
   LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
   CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
   SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
   INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
   CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
   ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
   POSSIBILITY OF SUCH DAMAGE.
   ---------------------------------------------------------------------------*/


#include <stdint.h>
#include "A31G22x.h"
#include "system_A31G22x.h"


/*----------------------------------------------------------------------------
  DEFINES
 *----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------
  Define clocks
 *----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------
  Clock Variable definitions
 *----------------------------------------------------------------------------*/
 uint32_t SystemCoreClock;     /*!< System Clock Frequency (Core Clock)  HLCK */
 uint32_t SystemPeriClock;     /*!< System Clock Frequency (Peri Clock)  PCLK */

/*----------------------------------------------------------------------------
  Clock functions
 *----------------------------------------------------------------------------*/

void SystemInit (void)
{
	static int i;

    __disable_irq();

    // disable WDT - default ON so you must turn off
    WDT->CR = 0
        |(0x5A69 << 16)
        |(0x25 << 10)
        |(0x1A << 4);

    SCU->CSCR = (SCU->CSCR & 0x000F0FF) | 0xA5070880U;  // Enables LSI oscillator and disables HSE, LSE
    SCU->SCCR = 0x570A0002U;  // Select HSI (32MHz)
    for(i = 0 ; i < 0x100; i++);      // Stabilization of HSI (Min. 100 us)

    SystemCoreClock = HSI_CLOCK;  // System Code clock is set LSI clock (Global variable have to be called in main routine.)
    SystemPeriClock = HSI_CLOCK;  // System Peripheral clock is set LSI clock (Global variable have to be called in main routine.)

    // flash memory controller
    CFMC->MR = 0x81;       // after changing 0x81 -> 0x28 in MR reg, flash access timing will be able to be set.
    CFMC->MR = 0x28;       // enter flash access timing changing mode
    CFMC->CFG = (0x7858 << CFMC_CFG_WTIDKY_Pos) | (1 << CFMC_CFG_WAIT_Pos);     // 1-wait: 32MHz/(1+1) = 16MHz Access Freq.
    CFMC->MR = 0;	      // exit flash access timing --> normal mode
}
