/***************************************************************************//**
* @file     A31G22x_hal_spi.h
* @brief    Contains all macro definitions and function prototypes support
*           for SPI driver on A31G22x
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


/* Includes ------------------------------------------------------------------- */
#include "A31G22x.h"
#include "A31G22x_hal_aa_types.h"


#ifdef __cplusplus
extern "C" {
#endif

/* Public Macros -------------------------------------------------------------- */
/* --------------------- BIT DEFINITIONS -------------------------------------- */
/**********************************************************************
 * Macro defines for CR register
 **********************************************************************/
/** SPI data size select */
#define SPI_DS_8BITS                ((uint32_t)(0))         /**< SPI Transmission 8-bit Data length  */
#define SPI_DS_9BITS                ((uint32_t)(1))         /**< SPI Transmission 9-bit Data length  */
#define SPI_DS_16BITS               ((uint32_t)(2))         /**< SPI Transmission 16-bit Data length  */
#define SPI_DS_17BITS               ((uint32_t)(3))         /**< SPI Transmission 17-bit Data length  */
#define SPI_DS_BITMASK              ((uint32_t)(0x3))       /**< SPI Transmission Data length Mask  */

/** Clock phase control bit */
#define SPI_CPHA_LO                 ((uint32_t)(0<<3))      /**< SPI clock phase is Active-Low  */
#define SPI_CPHA_HI                 ((uint32_t)(1<<3))      /**< SPI clock phase is Active-High  */

#define SPI_CPOL_LO                 ((uint32_t)(0<<2))      /**< SPI clock Polarity is Active-Low  */
#define SPI_CPOL_HI                 ((uint32_t)(1<<2))      /**< SPI clock Polarity is Active-High  */

/** SPI MSB/LSB Transmit select bit */
#define SPI_LSB_FIRST               ((uint32_t)(0))         /**< SPI data transmission with LSB first */
#define SPI_MSB_FIRST               ((uint32_t)(1))         /**< SPI data transmission with LSB first */

/** SPI master mode enable */
#define SPI_SLAVE_MODE              ((uint32_t)(0))        /**< SPI channel is SLAVE */
#define SPI_MASTER_MODE             ((uint32_t)(1))        /**< SPI channel is MASTER */

/** SPI SS output signal control */
#define SPI_SSOUT_LO                ((uint32_t)(0<<12))    /**< SS output signal is Active-Low */
#define SPI_SSOUT_HI                ((uint32_t)(1<<12))    /**< SS output Signal is Active-High */

/** SPI SS mode */
#define SPI_SSMOD_AUTO              ((uint32_t)(0<<13))    /**< SS output signal is NOT set by SSOUT. (Default) */
#define SPI_SSMOD_MANUAL            ((uint32_t)(1<<13))    /**< SS output signal is set by SSOUT. (Recommended more than 1MHz SCLK) */

/** SPI loop back enable */
#define SPI_LBM_EN                  ((uint32_t)(1<<11))    /**< Loop back mode in Master Mode */

/** SPI interrupt enable */
#define SPI_INTCFG_TXDIE            ((uint32_t)(1<<18))    /**< DMA TX done interrupt enable */
#define SPI_INTCFG_RXDIE            ((uint32_t)(1<<17))    /**< DMA RX done interrupt enable */
#define SPI_INTCFG_SSCIE            ((uint32_t)(1<<16))    /**< SS edge change interrupt enable */
#define SPI_INTCFG_TXIE             ((uint32_t)(1<<15))    /**< TX interrupt enable */
#define SPI_INTCFG_RXIE             ((uint32_t)(1<<14))    /**< RX interrupt enable */

/** SPI Delay Mask enable */
#define SPI_LR_DELAY_MASK           ((uint32_t)0xFF)       /**< SPI delay length Mask */


/*********************************************************************//**
* SPI Status defines
*************************************************************************/

#define SPI_STAT_RXBUF_READY        ((uint32_t)(1<<0))     /**< SPI status RX buffer ready bit */
#define SPI_STAT_TXBUF_EMPTY        ((uint32_t)(1<<1))     /**< SPI status TX buffer empty bit */

#define SPI_STAT_IDLE               ((uint32_t)(1<<2))     /**< SPI status TX/RX IDLE bit */
#define SPI_STAT_TXUNDERRUN_ERR     ((uint32_t)(1<<3))     /**< SPI status TX underrun error bit */
#define SPI_STAT_RXOVERRUN_ERR      ((uint32_t)(1<<4))     /**< SPI status RX overrun error bit */

#define SPI_STAT_SS_ACT             ((uint32_t)(1<<5))     /**< SPI status SS active bit */
#define SPI_STAT_SS_DET             ((uint32_t)(1<<6))     /**< SPI status SS detect bit */

#define SPI_STAT_SBUSY              ((uint32_t)(1<<7))     /**< SPI status Sbusy Operation Flag */
#define SPI_STAT_RXDMA_DONE         ((uint32_t)(1<<8))     /**< SPI status RX DMA done bit */
#define SPI_STAT_TXDMA_DONE         ((uint32_t)(1<<9))     /**< SPI status TX DMA done bit */


/**
 * @brief SPI Channel Re-mapping type enumeration
 */
enum {
    SS_AUTO = 0,
    SS_MANUAL
};

enum {
    SS_OUT_LO = 0,
    SS_OUT_HI
};

/* Public Types --------------------------------------------------------------- */
/** @brief SPI configuration structure */
typedef struct {
    uint32_t Databit;         /**< Databit number, should be
                                        - SPI_DS_8BITS :0
                                        - SPI_DS_9BITS :1
                                        - SPI_DS_16BITS :2
                                        - SPI_DS_17BITS :3 */
    uint32_t CPHA;            /**< Clock phase, should be:
                                        - SSP_CPHA_FIRST: first clock edge
                                        - SSP_CPHA_SECOND: second clock edge */
    uint32_t CPOL;            /**< Clock polarity, should be:
                                        - SSP_CPOL_HI: high level
                                        - SSP_CPOL_LO: low level */
    uint8_t DataDir;          /**< SPI mode, should be:
                                        - SPI_LSB_FIRST
                                        - SPI_MSB_FIRST */
    uint32_t Mode;            /**< SPI mode, should be:
                                        - SPI_MASTER_MODE: Master mode
                                        - SPI_SLAVE_MODE: Slave mode */
    uint32_t BaudRate;        /**< PCLK / (BaudRate+1)  BaudRate>=2 (0x0002~0xFFFF */
} SPI_CFG_Type;

/**
 * @brief SPI Channel Re-mapping type enumeration
 */
typedef enum {
    REMAP_USART10,          /**< SPI20 Channel re-mapped USART10 channel */
    REMAP_USART11           /**< SPI21 Channel re-mapped USART11 channel */
} SPI_REMAP_Type;


/* Public Functions ----------------------------------------------------------- */
void HAL_SPI_Init(SPI_Type *SPIx, SPI_CFG_Type *SPI_ConfigStruct);
void HAL_SPI_DeInit(SPI_Type* SPIx);
void HAL_SPI_DelayConfig(SPI_Type* SPIx, uint8_t StartDelay, uint8_t BurstDelay, uint8_t StopDelay);

void HAL_SPI_Cmd(SPI_Type* SPIx, FunctionalState NewState);
void HAL_SPI_LoopBackCmd(SPI_Type* SPIx, FunctionalState NewState);
void HAL_SPI_SSOutputCmd(SPI_Type* SPIx, uint32_t State);
void HAL_SPI_SSOutput(SPI_Type* SPIx, uint32_t State);

void HAL_SPI_SendData(SPI_Type* SPIx, uint32_t Data);
uint32_t HAL_SPI_ReceiveData(SPI_Type* SPIx);

uint32_t HAL_SPI_GetStatus(SPI_Type* SPIx);
void HAL_SPI_ClearPending(SPI_Type* SPIx, uint32_t IntType);

void HAL_SPI_IntConfig(SPI_Type *SPIx, uint32_t IntType, FunctionalState NewState);
uint32_t HAL_SPI_GetControl(SPI_Type* SPIx);
void HAL_SPI_PortInit(SPI_Type* SPIx);
void HAL_SPI_PortRemap(SPI_REMAP_Type Remap, FunctionalState ENABLE);

#ifdef __cplusplus
}
#endif

/* --------------------------------- End Of File ------------------------------ */
