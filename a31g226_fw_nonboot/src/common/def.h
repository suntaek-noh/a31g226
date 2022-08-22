/*
 * def.h
 *
 *  Created on: Apr 13, 2022
 *      Author: suntaek.noh
 */

#ifndef SRC_COMMON_DEF_H_
#define SRC_COMMON_DEF_H_

#include <stdio.h>
#include <stdint.h>
#include <stdbool.h>
#include <stdarg.h>
#include <string.h>
#include <stdlib.h>


#define _DEF_UART1            0
#define _DEF_UART2            1
#define _DEF_UART3            2
#define _DEF_UART4            3


#define _DEF_LED1             0
#define _DEF_LED2             1
#define _DEF_LED3             2
#define _DEF_LED4             3
#define _DEF_LED5             4
#define _DEF_LED6             5
#define _DEF_LED7             6
#define _DEF_LED8             7

#define _DEF_BUTTON1          0
#define _DEF_BUTTON2          1
#define _DEF_BUTTON3          2
#define _DEF_BUTTON4          3

#define _DEF_SPI1             0
#define _DEF_SPI2             1
#define _DEF_SPI3             2
#define _DEF_SPI4             3

#define _DEF_I2C1             0
#define _DEF_I2C2             1
#define _DEF_I2C3             2
#define _DEF_I2C4             3


#define _DEF_PWM1             0
#define _DEF_PWM2             1
#define _DEF_PWM3             2
#define _DEF_PWM4             3

#define _DEF_INPUT                  0
#define _DEF_INPUT_PULLUP           1
#define _DEF_INPUT_PULLDOWN         2
#define _DEF_OUTPUT                 3
#define _DEF_OUTPUT_PULLUP          4
#define _DEF_OUTPUT_PULLDOWN        5
#define _DEF_INPUT_INTRRUPT         6
#define _DEF_INPUT_INTRRUPT_PULLUP  7


#define _DEF_LOW              0
#define _DEF_HIGH             1



#define constrain(amt,low,high) ((amt)<(low)?(low):((amt)>(high)?(high):(amt)))

#ifndef CMAX
#define CMAX(a,b) (((a) > (b)) ? (a) : (b))
#define CMIN(a,b) (((a) < (b)) ? (a) : (b))
#endif


#ifndef CMAP
#define CMAP(value, in_min, in_max, out_min, out_max) ((value - in_min) * (out_max - out_min) / (in_max - in_min) + out_min)
#endif

#define _BIT(n) (1<<(n))

#define FLASH_MAGIC_NUMBER      0x5555AAAA

typedef struct
{
  uint8_t version[32];
  uint8_t name[32];
} firm_version_t;


typedef struct
{
  uint32_t magic_number;

  //-- fw info
  //
  uint32_t addr_tag;
  uint32_t addr_fw;
  uint32_t size_tag;


  //-- tag info
  //
  uint32_t tag_flash_type;
  uint32_t tag_flash_start;
  uint32_t tag_flash_end;
  uint32_t tag_flash_length;
  uint32_t tag_flash_crc;
  uint32_t tag_length;
  uint8_t  tag_date_str[32];
  uint8_t  tag_time_str[32];

} firm_tag_t;


#endif /* SRC_COMMON_DEF_H_ */
