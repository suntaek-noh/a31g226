/*
 * hw.h
 *
 *  Created on: 2022. 7. 31.
 *      Author: sunta
 */

#ifndef SRC_HW_HW_H_
#define SRC_HW_HW_H_

#include "hw_def.h"


#include "led.h"
#include "uart.h"
#include "log.h"
#include "button.h"
#include "flash.h"
#include "buzzer.h"
#include "cli.h"
#include "pwm.h"
#include "gpio.h"
#include "ir_remote.h"
#include "lcd.h"
#include "data_flash.h"
#include "i2c.h"
#include "relay.h"
#include "touch.h"



void hwInit(void);


#endif /* SRC_HW_HW_H_ */
