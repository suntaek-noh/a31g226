/**
  ******************************************************************************
  * @file    A31G22x_hal_assert.h
  * @brief   A31G22x HAL assert file.
  * @author  ABOV AE Team  
  ******************************************************************************
  * @attention
  * &copy; Copyright (c) 2020 ABOV Semiconductor Co., Ltd. All rights reserved.
  ******************************************************************************
  */


/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __A31G22X_HAL_ASSERT_H
#define __A31G22X_HAL_ASSERT_H

#ifdef __cplusplus
 extern "C" {
#endif


/* Exported types ------------------------------------------------------------*/
/* Exported constants --------------------------------------------------------*/
/* Includes ------------------------------------------------------------------*/
/* Exported macro ------------------------------------------------------------*/

#ifdef  USE_FULL_ASSERT
/*******************************************************************************************
* @brief                The CHECK_PARAM macro is used for function's parameters check.
*                               It is used only if the library is compiled in DEBUG mode.
* @param	    		expr - If expr is false, it calls check_failed() function
*                       which reports the name of the source file and the source
*                       line number of the call that failed.
*                    - If expr is true, it returns no value.
* @return               None
*******************************************************************************************/
#define CHECK_PARAM(expr) ((expr) ? (void)0 : check_failed((uint8_t *)__FILE__, __LINE__))
#else
#define CHECK_PARAM(expr) ((void)0U)
#endif 

/****************************************************************************************
* Public Typedef
****************************************************************************************/
 
 
/*******************************************************************************
* Exported Public Function
*******************************************************************************/
#ifdef  USE_FULL_ASSERT
void check_failed(uint8_t *file, uint32_t line);
#endif

#ifdef __cplusplus
}
#endif

#endif /* __A31G22X_HAL_ASSERT_H */


/* --------------------------------- End Of File ------------------------------ */
