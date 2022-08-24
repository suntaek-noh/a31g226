/*
 * lcd.h
 *
 *  Created on: 2022. 8. 14.
 *      Author: sunta
 */

#ifndef SRC_COMMON_HW_INCLUDE_LCD_H_
#define SRC_COMMON_HW_INCLUDE_LCD_H_

#include "hw_def.h"

#ifdef _USE_HW_SEG_LCD

#define LCD_COM_MAX_CH        HW_LCD_COM_MAX_CH
#define LCD_SEG_MAX_CH        HW_LCD_SEG_MAX_CH



bool lcdInit(void);
bool lcdIsInit(void);
void lcdReset(void);

uint8_t lcdGetBackLight(void);
void    lcdSetBackLight(uint8_t value);


void lcdClearBuffer(void);
void lcdAllDisp(void);
void lcd7SegDis(uint8_t pos, uint8_t data);




#endif




#endif /* SRC_COMMON_HW_INCLUDE_LCD_H_ */
