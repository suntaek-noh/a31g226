/*
 * bsp.h
 *
 *  Created on: 2022. 7. 31.
 *      Author: sunta
 */

#ifndef SRC_BSP_BSP_H_
#define SRC_BSP_BSP_H_

#include "def.h"

#include "A31G22x_libcfg.h"
#include "A31G22x_hal_pcu.h"
#include "A31G22x_hal_scu.h"
#include "A31G22x_hal_cfmc.h"
//#include "debug_frmwrk.h"
//#include "debug_cmd.h"
//#include "debug.h"
#include "A31G22x_hal_assert.h"
#include "A31G22x_it.h"

#include "A31G22x_hal_uartn.h"
#include "A31G22x_hal_usart1n.h"

#include "A31G22x_hal_cfmc_SelfWriteErase.h"
#include "A31G22x_hal_cfmc.h"
#include "A31G22x_hal_timer1n.h"
#include "A31G22x_hal_timer2n.h"
#include "A31G22x_hal_lcd.h"
#include "A31G22x_hal_dfmc.h"
#include "A31G22x_hal_i2c.h"




void bspInit(void);
void delay(uint32_t ms);
uint32_t millis(void);

void logPrintf(const char *fmt, ...);

#endif /* SRC_BSP_BSP_H_ */
