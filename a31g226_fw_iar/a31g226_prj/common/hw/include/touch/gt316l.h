/*
 * gt316l.h
 *
 *  Created on: 2022. 8. 17.
 *      Author: sunta
 */

#ifndef SRC_COMMON_HW_INCLUDE_TOUCH_GT316L_H_
#define SRC_COMMON_HW_INCLUDE_TOUCH_GT316L_H_

#include "hw_def.h"

#ifdef _USE_HW_GT316L

#include "touch.h"

#define GT316L_CHIP_ADDR1           0xB0
#define GT316L_CHIP_ADDR2           0xB2

#define GT316L_MAX_TOUCHES          5

#define GT316L_TOUCH_OUT            0x02      // 0x02 ~ 0x03
#define GT316L_IO_DIR1              0x04
#define GT316L_IO_DIR2              0x05
#define GT316L_CONTROL1             0x06
#define GT316L_EX_CONTROL           0x07
#define GT316L_SENSITIVITY_CONTROL  0x10   // 0x10 ~ 0x1F  16개
#define GT316L_PWM_CONTROL          0x20      // 0x20 ~ 0x2f 16개
#define GT316L_PWM_ALLBLOCK         0x2B      // 0x20 ~ 0x2f 16개
#define GT316L_PWM_GAS              0x2C      // 0x20 ~ 0x2f 16개
#define GT316L_PWM_GOOUT            0x2D      // 0x20 ~ 0x2f 16개
#define GT316L_PWM_AISLE            0x2E      // 0x20 ~ 0x2f 16개
#define GT316L_PWM_EV               0x2F      // 0x20 ~ 0x2f 16개

#define GT316L_PWM_EX1_CONTROL      0x30     // 0x30 ~ 0x33  4개
#define GT316L_PWM_EX2_CONTROL      0x31     // 0x30 ~ 0x33  4개
#define GT316L_PWM_EX3_CONTROL      0x32     // 0x30 ~ 0x33  4개


bool gt316lInitDriver(touch_driver_t *p_driver);

#endif /* SRC_COMMON_HW_INCLUDE_TOUCH_GT316L_H_ */
