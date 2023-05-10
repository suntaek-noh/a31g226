/***************************************************************************//**
* @file     A31G22x_hal_pwr.h
* @brief    Contains all macro definitions and function prototypes support
*           for power control driver on A31G22x
* @author   AE Team, ABOV Semiconductor Co., Ltd.
* @version  V1.00
* @date     28. DEC. 2020
*
* Copyright(C) 2020, ABOV Semiconductor All rights reserved.
********************************************************************************
* DISCLAIMER
*
* THE PRESENT FIRMWARE WHICH IS FOR GUIDANCE ONLY AIMS AT PROVIDING CUSTOMERS
* WITH CODING INFORMATION REGARDING THEIR PRODUCTS IN ORDER FOR THEM TO SAVE
* TIME. AS A RESULT, ABOV SEMICONDUCTOR DISCLAIMS ALL LIABILITIES FROM ANY
* DIRECT, INDIRECT OR CONSEQUENTIAL DAMAGES WITH RESPECT TO ANY CLAIMS ARISING
* FROM THE CONTENT OF SUCH FIRMWARE AND/OR THE USE MADE BY CUSTOMERS OF THE
* CODING INFORMATION CONTAINED HEREIN IN CONNECTION WITH THEIR PRODUCTS.
*
*******************************************************************************/

#ifndef _A31G22x_HAL_PWR_H_
#define _A31G22x_HAL_PWR_H_

#ifdef __cplusplus
extern "C" {
#endif


/*******************************************************************************
* Included File
*******************************************************************************/
#include "A31G22x.h"
#include "A31G22x_hal_aa_types.h"


/*******************************************************************************
* Public Macro
*******************************************************************************/
#define SCULV_LVICR_LVIEN_DISABLE                   (0x00UL << 7)
#define SCULV_LVICR_LVIEN_ENABLE                    (0x01UL << 7)

#define SCULV_LVICR_LVINTEN_DISABLE                 (0x00UL << 5)
#define SCULV_LVICR_LVINTEN_ENABLE                  (0x01UL << 5)

#define SCULV_LVICR_LVIFLAG                         (0x01UL << SCULV_LVICR_LVIFLAG_Pos)

#define SCULV_LVRCR_LVREN_ENABLE                    (0x00UL << SCULV_LVRCR_LVREN_Pos)
#define SCULV_LVRCR_LVREN_DISABLE                   (0x55UL << SCULV_LVRCR_LVREN_Pos)

#define SCULV_LVRCNFIG_WTIDKY                       (0x72A5UL << SCULV_LVRCNFIG_WTIDKY_Pos)
#define SCULV_LVRCNFIG_LVRENM_ENABLE                (0x00UL << SCULV_LVRCNFIG_LVRENM_Pos)
#define SCULV_LVRCNFIG_LVRENM_DISABLE               (0xAAUL << SCULV_LVRCNFIG_LVRENM_Pos)

/*******************************************************************************
* Public Typedef
*******************************************************************************/
/**
 * @brief  LVI voltage threshold enumerated definition
 */
typedef enum {
    LVI_VOLTAGE_156 = 0x00UL, /*!< 1.56V */
    LVI_VOLTAGE_170 = 0x01UL, /*!< 1.70V */
    LVI_VOLTAGE_179 = 0x02UL, /*!< 1.79V */
    LVI_VOLTAGE_191 = 0x03UL, /*!< 1.91V */
    LVI_VOLTAGE_200 = 0x04UL, /*!< 2.00V */
    LVI_VOLTAGE_213 = 0x05UL, /*!< 2.13V */
    LVI_VOLTAGE_231 = 0x06UL, /*!< 2.31V */
    LVI_VOLTAGE_248 = 0x07UL, /*!< 2.48V */
    LVI_VOLTAGE_268 = 0x08UL, /*!< 2.68V */
    LVI_VOLTAGE_305 = 0x09UL, /*!< 3.05V */
    LVI_VOLTAGE_319 = 0x0AUL, /*!< 3.19V */
    LVI_VOLTAGE_360 = 0x0BUL, /*!< 3.60V */
    LVI_VOLTAGE_374 = 0x0CUL, /*!< 3.74V */
    LVI_VOLTAGE_404 = 0x0DUL, /*!< 4.04V */
    LVI_VOLTAGE_422 = 0x0EUL, /*!< 4.22V */
    LVI_VOLTAGE_450 = 0x0FUL  /*!< 4.50V */
} LVI_VOLTAGE_Type;

/**
 * @brief  LVR voltage threshold enumerated definition
 */
typedef enum {
    LVR_VOLTAGE_450 = 0x00UL, /*!< 4.50V */
    LVR_VOLTAGE_422 = 0x01UL, /*!< 4.22V */
    LVR_VOLTAGE_404 = 0x02UL, /*!< 4.04V */
    LVR_VOLTAGE_374 = 0x03UL, /*!< 3.74V */
    LVR_VOLTAGE_360 = 0x04UL, /*!< 3.60V */
    LVR_VOLTAGE_319 = 0x05UL, /*!< 3.19V */
    LVR_VOLTAGE_305 = 0x06UL, /*!< 3.05V */
    LVR_VOLTAGE_268 = 0x07UL, /*!< 2.68V */
    LVR_VOLTAGE_248 = 0x08UL, /*!< 2.48V */
    LVR_VOLTAGE_231 = 0x09UL, /*!< 2.31V */
    LVR_VOLTAGE_213 = 0x0AUL, /*!< 2.13V */
    LVR_VOLTAGE_200 = 0x0BUL, /*!< 2.00V */
    LVR_VOLTAGE_191 = 0x0CUL, /*!< 1.91V */
    LVR_VOLTAGE_179 = 0x0DUL, /*!< 1.79V */
    LVR_VOLTAGE_170 = 0x0EUL, /*!< 1.70V */
    LVR_VOLTAGE_156 = 0x0FUL  /*!< 1.56V */
} LVR_VOLTAGE_Type;

/**
 * @brief  LVI Enable/Disable function enumerated definition
 */
typedef enum {
    LVI_DISABLE,
    LVI_ENABLE
} LVI_EN_Type;
/**
 * @brief  LVI Interrupt Enable/Disable function enumerated definition
 */
typedef enum {
    LVI_INTR_DISABLE,
    LVI_INTR_ENABLE
} LVI_INTR_Type;

/**
 * @brief  LVR Enable/Disable function enumerated definition
 */
typedef enum {
    LVR_ENABLE = 0x00,
    LVR_DISABLE = 0x55,
} LVR_EN_Type;
/**
 * @brief  LVR Control block enumerated definition
 */
typedef enum {
    LVR_CONFIG_MASTER = 0x00,
    LVR_CONFIG_LVREN = 0xAA
} LVR_CONFIG_Type;

/**
 * @brief  LVI configuration structure definition
 */
typedef struct {
    /* LVICR */
    LVI_EN_Type EnableIndicator;
    LVI_INTR_Type EnableInterrupt;
    uint32_t LVI_Voltage;
} LVI_CFG_Type;

/**
 * @brief  LVR configuration structure definition
 */
typedef struct {
    /* LVRCNFIG */
    LVR_EN_Type EnableReset;
    LVR_CONFIG_Type EnableOperation;
    uint32_t LVR_Voltage;
} LVR_CFG_Type;

/*******************************************************************************
* Exported Public Function
*******************************************************************************/
void HAL_LVI_Init(LVI_CFG_Type *pConfig);
FlagStatus HAL_LVI_GetInterruptStatus(void);
void HAL_LVI_ClearInterruptStatus(void);
void HAL_LVR_Init(LVR_CFG_Type *pConfig);

void HAL_PWR_EnterSleep(void);
void HAL_PWR_EnterDeepSleep(void);
void HAL_PWR_AlwaysOnLSIForDeepSleep(uint8_t LSI, uint8_t BGR, uint8_t VDC);
void HAL_PWR_AlwaysOnLSEForDeepSleep(uint8_t LSE, uint8_t BGR, uint8_t VDC);

#ifdef __cplusplus
}
#endif

#endif /* _A31G22x_PWR_H_ */
/* --------------------------------- End Of File ------------------------------ */
