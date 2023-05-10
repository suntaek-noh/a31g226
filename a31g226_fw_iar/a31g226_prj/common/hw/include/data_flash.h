/*
 * data_flash.h
 *
 *  Created on: 2022. 8. 15.
 *      Author: sunta
 */

#ifndef SRC_COMMON_HW_INCLUDE_DATA_FLASH_H_
#define SRC_COMMON_HW_INCLUDE_DATA_FLASH_H_

#include "hw_def.h"

#ifdef _USE_HW_DATA_FLASH

bool dataFlashInit(void);
bool dataFlashErase(uint32_t addr, uint32_t length);
bool dataFlashWrite(uint32_t addr, uint8_t *p_data, uint32_t length);
bool dataFlashRead(uint32_t addr, uint8_t *p_data, uint32_t length);

#endif

#endif /* SRC_COMMON_HW_INCLUDE_DATA_FLASH_H_ */
