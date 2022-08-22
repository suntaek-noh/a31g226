/*
 * gt216l.h
 *
 *  Created on: 2022. 8. 17.
 *      Author: sunta
 */

#ifndef SRC_COMMON_HW_INCLUDE_TOUCH_GT216L_H_
#define SRC_COMMON_HW_INCLUDE_TOUCH_GT216L_H_


#include "hw_def.h"

#ifdef _USE_HW_GT216L

#include "touch.h"


#define GT216L_CHIP1_ADDR                 0xB0
#define GT216L_CHIP2_ADDR                 0xB2

#define GT216L_1_MAX_TOUCHES          8
#define GT216L_2_MAX_TOUCHES          2


#define GT216L_TOUCH_OUT            0x02      // 0x02 ~ 0x03
#define GT21L_IO_DIR1               0x04
#define GT21L_IO_DIR2               0x05
#define GT216L_CONTROL1             0x06
#define GT216L_EX_CONTROL           0x07
#define GT216L_SENSITIVITY_CONTROL  0x10   // 0x10 ~ 0x1F  16개
#define GT216L_PWM_CONTROL          0x20      // 0x20 ~ 0x2f 16개
#define GT216L_PWM_ALLBLOCK         0x2B      // 0x20 ~ 0x2f 16개
#define GT216L_PWM_GAS              0x2C      // 0x20 ~ 0x2f 16개
#define GT216L_PWM_GOOUT            0x2D      // 0x20 ~ 0x2f 16개
#define GT216L_PWM_AISLE            0x2E      // 0x20 ~ 0x2f 16개
#define GT216L_PWM_EV               0x2F      // 0x20 ~ 0x2f 16개

#define GT216L_PWM_EX1_CONTROL      0x30     // 0x30 ~ 0x33  4개
#define GT216L_PWM_EX2_CONTROL      0x31     // 0x30 ~ 0x33  4개
#define GT216L_PWM_EX3_CONTROL      0x32     // 0x30 ~ 0x33  4개



bool gt216lInitDriver(touch_driver_t *p_driver);



#endif



#endif /* SRC_COMMON_HW_INCLUDE_TOUCH_GT216L_H_ */
