/*
 * touch.h
 *
 *  Created on: 2022. 8. 18.
 *      Author: sunta
 */

#ifndef SRC_COMMON_HW_INCLUDE_TOUCH_H_
#define SRC_COMMON_HW_INCLUDE_TOUCH_H_


#ifdef __cplusplus
 extern "C" {
#endif


#include "hw_def.h"

#ifdef _USE_HW_TOUCH


 typedef struct touch_driver_t_
 {
   bool  (*init)(void);
   bool  (*getTouchedData)(uint16_t *p_data);
   bool  (*setTouchedData)(uint8_t ch, uint8_t value);
 } touch_driver_t;

 bool touchInit(void);
 bool touchGetTouchedData(uint16_t *p_data);
 bool touchIoOut(uint8_t ch, uint8_t value);

#endif


#ifdef __cplusplus
}
#endif


#endif /* SRC_COMMON_HW_INCLUDE_TOUCH_H_ */
