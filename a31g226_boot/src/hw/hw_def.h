/*
 * hw_def.h
 *
 *  Created on: 2022. 7. 31.
 *      Author: sunta
 */

#ifndef SRC_HW_HW_DEF_H_
#define SRC_HW_HW_DEF_H_

#include "bsp.h"


#define _DEF_FIRMWATRE_VERSION    "V220809R1"
#define _DEF_BOARD_NAME           "A31G226ML2N_BOOT"

#define _USE_HW_FLASH

#define _USE_HW_LED
#define      HW_LED_MAX_CH          1

#define _USE_HW_UART
#define      HW_UART_MAX_CH         2

#define _USE_HW_LOG
#define      HW_LOG_CH              _DEF_UART1

#define _USE_HW_BUTTON
#define      HW_BUTTON_MAX_CH       1
#define      HW_BUTTON_OBJ_USE      0

#define _USE_HW_CMD
#define      HW_CMD_MAX_DATA_LENGTH   1024        //1k

#define FLASH_ADDR_TAG              0x00008000
#define FLASH_ADDR_FW               0x00008400
#define FLASH_ADDR_FW_VER           0x00008800

#define FLASH_ADDR_START            0x00008000
#define FLASH_ADDR_END              (FLASH_ADDR_START + (256-32)*1024)


#endif /* SRC_HW_HW_DEF_H_ */
