/*
 * main.c
 *
 *  Created on: 2022. 7. 31.
 *      Author: sunta
 */

#include "main.h"


int main(void)
{
  hwInit();
  apInit();

  apMain();

  while(1)
  {

  }

  return 0;
}
