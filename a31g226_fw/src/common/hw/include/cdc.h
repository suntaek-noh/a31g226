/*
 * cdc.h
 *
 *  Created on: Jan 30, 2022
 *      Author: suntaek.noh
 */

#ifndef SRC_COMMON_HW_INCLUDE_CDC_H_
#define SRC_COMMON_HW_INCLUDE_CDC_H_

#include "hw_def.h"

#ifdef  _USE_HW_CDC

bool cdcInit(void);
bool cdcIsInit(void);
uint32_t cdcAvailable(void);     // 현대 수신된 data가 몇개인지 확인하는 함수
uint8_t cdcRead(void);
void cdcDataIn(uint8_t rx_data);
uint32_t cdcWrite(uint8_t *p_data, uint32_t length);     // data를 usb를 통해 송신 하는 함수
uint32_t cdcGetBaud(uint8_t ch);


#endif

#endif /* SRC_COMMON_HW_INCLUDE_CDC_H_ */
