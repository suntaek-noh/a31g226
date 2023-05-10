/*
 * usb.h
 *
 *  Created on: Jan 30, 2022
 *      Author: suntaek.noh
 */

#ifndef SRC_COMMON_HW_INCLUDE_USB_H_
#define SRC_COMMON_HW_INCLUDE_USB_H_

#include "hw_def.h"

#ifdef _USE_HW_USB

typedef enum
{
  USB_NON_MODE,
  USB_CDC_MODE,
  USB_MSC_MODE
} UsbMode;




bool usbInit(void);
bool usbBegin(UsbMode usb_mode);
void usbDeInit(void);

UsbMode usbGetMode(void);

#endif


#endif /* SRC_COMMON_HW_INCLUDE_USB_H_ */
