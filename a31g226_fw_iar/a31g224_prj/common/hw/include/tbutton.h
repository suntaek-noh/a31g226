/*
 * tbutton.h
 *
 *  Created on: 2022. 8. 23.
 *      Author: sunta
 */

#ifndef SRC_COMMON_HW_INCLUDE_TBUTTON_H_
#define SRC_COMMON_HW_INCLUDE_TBUTTON_H_

#ifdef __cplusplus
 extern "C" {
#endif


#include "hw_def.h"

#ifdef _USE_HW_TOUCH

 typedef struct tbutton_driver_t_
 {
   bool     (*init)(void);
   uint8_t  (*getTocuhedStatus)(uint8_t ic);
   bool     (*getTouchedData)(uint8_t ic, uint8_t *value);
   bool     (*setTouchedData)(uint8_t ic, uint8_t value);
   //void     (*resetTouch)(uint8_t ic);
 } tbutton_driver_t;

 typedef struct
 {
   uint8_t ic;
   uint8_t ch;
   uint8_t state;
   uint8_t state_pre;
   uint8_t state_next;

   uint32_t pressed_time;
   uint32_t repeat_start_time;
   uint32_t repeat_pressed_time;
   uint32_t pre_time;

   uint8_t event_flag;
   uint8_t state_flag;
   uint8_t touch_count;
 }tbutton_obj_t;

 enum TbuttonEvt
 {
   TOUCH_EVT_PRESSED  = (1<<0),
   TOUCH_EVT_RELEASED = (1<<1),
   TOUCH_EVT_CLICKED  = (1<<2),
   TOUCH_EVT_REPEATED = (1<<3),
 };

 enum TbuttonState
 {
   TOUCH_STATE_PRESSED  = (1<<0),
   TOUCH_STATE_RELEASED = (1<<1),
   TOUCH_STATE_REPEATED = (1<<3),
 };

 bool tbuttonInit(void);
 uint8_t tbuttonGetTouchStatus(uint8_t ic);
 bool tbuttonGetTouchedData(uint8_t ic, uint8_t *p_data);
 bool tbuttonIoOut(uint8_t ch, uint8_t value);

 void tbuttonObjCreate(tbutton_obj_t *p_obj, uint8_t ic, uint8_t ch, uint32_t pressed_time, uint32_t repeat_start_time, uint32_t repeat_pressed_time);
 bool tbuttonObjUpdate(tbutton_obj_t *p_obj);
 uint8_t tbuttonObjGetEvent(tbutton_obj_t *p_obj);
 void tbuttonObjClearEvent(tbutton_obj_t *p_obj);
 uint8_t tbuttonObjGetState(tbutton_obj_t *p_obj);

 void tbuttonISR(void);
 uint16_t tbuttonGetValue(void);

#endif


#ifdef __cplusplus
}
#endif



#endif /* SRC_COMMON_HW_INCLUDE_TBUTTON_H_ */
