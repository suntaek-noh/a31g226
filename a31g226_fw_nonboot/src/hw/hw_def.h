/*
 * hw_def.h
 *
 *  Created on: 2022. 7. 31.
 *      Author: sunta
 */

#ifndef SRC_HW_HW_DEF_H_
#define SRC_HW_HW_DEF_H_

#include "bsp.h"


#define _DEF_FIRMWATRE_VERSION    "V220808R1"
#define _DEF_BOARD_NAME           "A31G226ML2N"

#define _USE_HW_BUZZER
#define _USE_HW_FLASH
#define _USE_HW_DATA_FLASH
#define _USE_HW_IR_REMOTE

#define _USE_HW_GPIO
#define      HW_GPIO_MAX_CH         13

#define _USE_HW_LED
#define      HW_LED_MAX_CH          6

#define _USE_HW_UART
#define      HW_UART_MAX_CH         2

#define _USE_HW_I2C
#define      HW_I2C_MAX_CH          1



#define _USE_HW_LOG
#define      HW_LOG_CH              _DEF_UART1

#define _USE_HW_BUTTON
#define      HW_BUTTON_MAX_CH       1
#define      HW_BUTTON_OBJ_USE      1

#define _USE_HW_CLI
#define      HW_CLI_CMD_LIST_MAX    16
#define      HW_CLI_CMD_NAME_MAX    16
#define      HW_CLI_LINE_HIS_MAX    4
#define      HW_CLI_LINE_BUF_MAX    64


#define FLASH_ADDR_TAG              0x0008000
#define FLASH_ADDR_FW               0x0008400
#define FLASH_ADDR_FW_VER           0x0008800

#define FLASH_ADDR_START            0x0008000
#define FLASH_ADDR_END              (FLASH_ADDR_START + (256-32)*1024)

#define _USE_HW_PWM
#define      HW_PWM_MAX_CH          3

#define _USE_HW_SEG_LCD
#define      HW_LCD_SEG_MAX_CH      11
#define      HW_LCD_COM_MAX_CH      4

#define _USE_HW_RELAY

#define _USE_HW_TOUCH
#define      HW_TBUTTON_MAX_IC      2
#define      HW_MAX_TOUCH_IC1_CH    10
#define      HW_MAX_TOUCH_IC2_CH    2
#define      HW_TOUCH_OBJ_USE       1
#define _USE_HW_GT216L



//#define _USE_HW_GT216L









#endif /* SRC_HW_HW_DEF_H_ */
