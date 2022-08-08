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

#define _USE_HW_LED
#define      HW_LED_MAX_CH          6

#define _USE_HW_UART
#define      HW_UART_MAX_CH         2

#define _USE_HW_LOG
#define      HW_LOG_CH              _DEF_UART1



#endif /* SRC_HW_HW_DEF_H_ */
