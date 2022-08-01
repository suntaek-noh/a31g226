/*
 * ap.c
 *
 *  Created on: 2022. 7. 31.
 *      Author: sunta
 */


#include "ap.h"


void apInit(void)
{

}

void apMain(void)
{
  while(1)
  {
    ledToggle(_DEF_LED1);
    //ledOn(_DEF_LED1);
    delay(500);
    //ledOff(_DEF_LED1);
    //delay(500);
  }
}
