/*
 * relay.c
 *
 *  Created on: 2022. 8. 18.
 *      Author: sunta
 */

#include "relay.h"

#ifdef _USE_HW_RELAY


bool relayInit(void)
{
  bool ret = true;

  HAL_GPIO_ConfigOutput((PORT_Type *)PA, 2, PCU_MODE_INPUT);
  HAL_GPIO_ConfigPullup((PORT_Type *)PA, 2, PCU_PUPD_DISABLE);

  HAL_GPIO_EXTI_ClearPin((PORT_Type *)PA, HAL_GPIO_EXTI_GetState((PORT_Type *)PA));

  HAL_GPIO_EXTI_Config((PORT_Type *)PA, 2, PCU_INTERRUPT_MODE_EDGE, PCU_INTERRUPT_CTRL_EDGE_FALLING);


  NVIC_EnableIRQ(GPIOAB_IRQn);
  NVIC_SetPriority(GPIOAB_IRQn, 5);

  HAL_GPIO_ConfigOutput((PORT_Type *)PC, 12, PCU_MODE_INPUT);
  HAL_GPIO_ConfigPullup((PORT_Type *)PC, 12, PCU_PUPD_DISABLE);

  HAL_GPIO_EXTI_ClearPin((PORT_Type *)PC, HAL_GPIO_EXTI_GetState((PORT_Type *)PC));

  HAL_GPIO_EXTI_Config((PORT_Type *)PC, 12, PCU_INTERRUPT_MODE_EDGE, PCU_INTERRUPT_CTRL_EDGE_FALLING);


  NVIC_EnableIRQ(GPIOCD_IRQn);
  NVIC_SetPriority(GPIOCD_IRQn, 5);




  return ret;
}

#endif
