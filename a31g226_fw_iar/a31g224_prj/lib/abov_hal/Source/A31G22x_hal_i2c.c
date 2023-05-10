/***************************************************************************//**
* @file     A31G22x_hal_i2c.c
* @brief    Contains all functions support for I2C dirver on A31G22x
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

/*******************************************************************************
* Included File
*******************************************************************************/
#include "A31G22x_libcfg.h"
#ifdef _I2C

#include "A31G22x_hal_scu.h"
#include "A31G22x_hal_i2c.h"

/*******************************************************************************
* Private Pre-processor Definition & Macro
*******************************************************************************/
#define BLOCKING_TIMEOUT (0x000FFFFFUL)

/**
 * @brief I2C device configuration structure type
 */
#if defined(__ARMCC_VERSION)
#pragma anon_unions
#endif
typedef struct {
    union {
        I2C_M_SETUP_Type    txrx_setup_master;         /* Transmission setup */
        I2C_S_SETUP_Type    txrx_setup_slave;          /* Transmission setup */
    };
    int32_t        dir;                                /* Current direction phase, 0: write, 1: read */
} I2C_CFG_T;


/* Private Variables ---------------------------------------------------------- */
/**
 * @brief II2C driver data for I2C0, I2C1, I2C2
 */
static I2C_CFG_T i2cdat[3];

static Bool I2C_MasterComplete[3];
static Bool I2C_SlaveComplete[3];

/********************************************************************//**
* @brief        Convert from I2C peripheral to number
* @param[in]    I2Cx    I2C peripheral selected, should be:
*                     - I2C0      :I2C0 peripheral
*                     - I2C1      :I2C1 peripheral
*                     - I2C2      :I2C2 peripheral
* @return       I2C number or error code, could be:
*                     -  0        :I2C0
*                     -  1        :I2C1
*                     -  2        :I2C2
*                     - -1        :Error
*********************************************************************/
static int32_t HAL_I2C_getNum(I2C_Type *I2Cx) {
    if (I2Cx == I2C0) {
        return (0);
    } else if (I2Cx == I2C1) {
        return (1);
    } else if (I2Cx == I2C2) {
        return (2);
    }

    return (-1);
}

/* Public Functions ----------------------------------------------------------- */
/*********************************************************************
* @brief        Initializes the I2Cx peripheral with specified parameter.
* @param[in]    I2Cx    I2C peripheral selected, should be:
*                     - I2C0    :I2C0 peripheral
*                     - I2C1    :I2C1 peripheral
*                     - I2C2    :I2C2 peripheral
* @param[in]    clockrate Target clock rate value to initialized I2C
*                 peripheral(Hz)
* @return       None
*********************************************************************/
void HAL_I2C_Init(I2C_Type *I2Cx, uint32_t clockrate) {
    if (I2Cx == I2C0) {
        /* Set up clock for I2C0 module */
        SCU->PER2 &= ~(1 << 4);
        SCU->PCER2 &= ~(1 << 4);

        SCU->PER2 |= (1 << 4);
        SCU->PCER2  |= (1 << 4);
    } else if (I2Cx == I2C1) {
        /* Set up clock for I2C1 module */
        SCU->PER2 &= ~(1 << 5);
        SCU->PCER2 &= ~(1 << 5);

        SCU->PER2 |= (1 << 5);
        SCU->PCER2 |= (1 << 5);
    } else if (I2Cx == I2C2) {
        /* Set up clock for I2C2 module */
        SCU->PER2 &= ~(1 << 6);
        SCU->PCER2 &= ~(1 << 6);

        SCU->PER2 |= (1 << 6);
        SCU->PCER2 |= (1 << 6);
    }

    I2Cx->CR = (1<<7);  // I2C Block Active
    I2Cx->CR |= (1<<5);  // INTEN

    I2Cx->SCLR = ((SystemPeriClock / clockrate)/4 -1)/2;
    I2Cx->SCHR = ((SystemPeriClock / clockrate)/4 -1)/2;
    I2Cx->SDHR = 1;

    I2Cx->CR |= (1 << 3);    // ACKEN
}


/**********************************************************************
 * @brief        De-initializes the I2C peripheral registers to their
 *               default reset values.
 * @param[in]    I2Cx    I2C peripheral selected, should be:
 *                     - I2C0    :I2C0 peripheral
 *                     - I2C1    :I2C1 peripheral
 *                     - I2C2    :I2C2 peripheral
 * @return       None
 **********************************************************************/
void HAL_I2C_DeInit(I2C_Type* I2Cx) {
    /* Disable I2C control */
    if (I2Cx == I2C0) {
        /* Set up clock for I2C0 module */
        SCU->PER2 &= ~(1 << 4);
        SCU->PCER2 &= ~(1 << 4);
    } else if (I2Cx == I2C1) {
        /* Set up clock for I2C1 module */
        SCU->PER2 &= ~(1 << 5);
        SCU->PCER2 &= ~(1 << 5);
    } else if (I2Cx == I2C2) {
        /* Set up clock for I2C2 module */
        SCU->PER2 &= ~(1 << 6);
        SCU->PCER2 &= ~(1 << 6);
    }
}

/*********************************************************************//**
* @brief        Enable/Disable interrupt for I2C peripheral
* @param[in]    I2Cx    I2C peripheral selected, should be:
*                       - I2C0    :I2C0 peripheral
*                       - I2C1    :I2C1 peripheral
*                       - I2C2    :I2C2 peripheral
* @param[in]    NewState    New State of I2C peripheral interrupt in NVIC core
*               should be:
*                       - ENABLE: enable interrupt for this I2C peripheral
*                       - DISABLE: disable interrupt for this I2C peripheral
* @return       None
**********************************************************************/
void HAL_I2C_IntCmd(I2C_Type *I2Cx, Bool NewState) {
    if (NewState) {
        if (I2Cx == I2C0) {
            NVIC_ClearPendingIRQ(I2C0_IRQn);
            NVIC_EnableIRQ(I2C0_IRQn);
        } else if (I2Cx == I2C1) {
            NVIC_ClearPendingIRQ(I2C1_IRQn);
            NVIC_EnableIRQ(I2C1_IRQn);
        } else if (I2Cx == I2C2) {
            NVIC_ClearPendingIRQ(I2C2_SPI20_IRQn);
            NVIC_EnableIRQ(I2C2_SPI20_IRQn);
        }
    } else {
        if (I2Cx == I2C0) {
            NVIC_DisableIRQ(I2C0_IRQn);
        } else if (I2Cx == I2C1) {
            NVIC_DisableIRQ(I2C1_IRQn);
        } else if (I2Cx == I2C2) {
            NVIC_DisableIRQ(I2C2_SPI20_IRQn);
        }
    }
}

/*********************************************************************//**
* @brief        General Master Interrupt handler for I2C peripheral
* @param[in]    I2Cx    I2C peripheral selected, should be:
*                       - I2C0    :I2C0 peripheral
*                       - I2C1    :I2C1 peripheral
*                       - I2C2    :I2C2 peripheral
* @return        None
**********************************************************************/
void HAL_I2C_Interrupt_MasterHandler(I2C_Type  *I2Cx) {
    int32_t tmp;
    I2C_M_SETUP_Type *txrx_setup;
    uint32_t status;

    tmp = HAL_I2C_getNum(I2Cx);
    txrx_setup = (I2C_M_SETUP_Type *) &(i2cdat[tmp].txrx_setup_master);

    status = I2Cx->ST;
    switch (status) {
        case 0x87: // transmit mode - addr ACK
            if (txrx_setup->tx_count < txrx_setup->tx_length) {
                I2Cx->DR = txrx_setup->tx_data[txrx_setup->tx_count];
                txrx_setup->tx_count++;
            } else {
                I2Cx->CR = 0
                | (1 << 7)        // Enable I2C Block
                | (1 << 5)     // Interrupt Enable
                | (1 << 1);    // STOP
            }
            break;

        case 0x47: // transmit mode - data ACK
            if (txrx_setup->tx_count < txrx_setup->tx_length) {
                I2Cx->DR = txrx_setup->tx_data[txrx_setup->tx_count];
                txrx_setup->tx_count++;
            } else {
                if (txrx_setup->rx_count < txrx_setup->rx_length) {
                    I2Cx->DR = ((txrx_setup->sl_addr7bit << 1) | 0x01);  // 0:write, 1:read
                    I2Cx->CR |= (1 << 0);   // reSTART
                } else {
                    I2Cx->CR = 0
                    | (1 << 7)              // Enable I2C Block
                    | (1 << 5)              // Interrupt Enable
                    | (1 << 1);             // STOP
                }
            }
            break;

        case 0x85: // receive mode - addr ACK
            if (txrx_setup->rx_count < txrx_setup->rx_length) {
                if ((txrx_setup->rx_length > 1) && (txrx_setup->rx_count < (txrx_setup->rx_length - 1))) {
                    __NOP();
                } else {
                    I2Cx->CR = 0     // disable ACKEN
                    | (1 << 7)       // Enable I2C Block
                    | (1 << 5);      // Interrupt Enable
                }
            } else {
                I2Cx->CR = 0
                | (1 << 7)     // Enable I2C Block
                | (1 << 5)     // Interrupt Enable
                | (1 << 1);    // STOP
            }
            break;

        case 0x45: // receive mode - data ACK
            if (txrx_setup->rx_count < txrx_setup->rx_length) {
                txrx_setup->rx_data[txrx_setup->rx_count] = I2Cx->DR;
                txrx_setup->rx_count++;

                if ((txrx_setup->rx_length > 1) && (txrx_setup->rx_count < (txrx_setup->rx_length - 1))) {
                    __NOP();
                } else {
                    I2Cx->CR = 0     // disable ACKEN
                    | (1 << 7)       // Enable I2C Block
                    | (1 << 5);      // Interrupt Enable
                }
            }
            break;

        case 0x44: // receive mode - data NOACK
            if (txrx_setup->rx_count < txrx_setup->rx_length) {
                txrx_setup->rx_data[txrx_setup->rx_count] = I2Cx->DR;
                txrx_setup->rx_count++;

                I2Cx->CR = 0
                | (1 << 7)     // Enable I2C Block
                | (1 << 5)     // Interrupt Enable
                | (1 << 1);    // STOP
            }
            break;

        case 0x20:             // receive mode
        case 0x22:             // transmit mode - stop receive
            I2Cx->CR = 0
            | (1 << 7)         // Enable I2C Block
            | (1 << 5)         // Interrupt Enable
            | (1 << 3);        // ACKEN
            goto s_int_end;

        default:
            if (status & 0x08) {}  // mastership lost

            break;
    }

    I2Cx->ST = 0xff;  // flag clear and SCL go to HIGH
    return;

s_int_end:

    I2Cx->ST = 0xff;  // flag clear and SCL go to HIGH
    // Disable interrupt
    HAL_I2C_IntCmd(I2Cx, FALSE);
    I2C_MasterComplete[tmp] = TRUE;
}

/*********************************************************************//**
* @brief        General Slave Interrupt handler for I2C peripheral
* @param[in]    I2Cx    I2C peripheral selected, should be:
*                       - I2C0    :I2C0 peripheral
*                       - I2C1    :I2C1 peripheral
*                       - I2C2    :I2C2 peripheral
* @return       None
**********************************************************************/
void HAL_I2C_Interrupt_SlaveHandler(I2C_Type *I2Cx) {
    int32_t tmp;
    I2C_S_SETUP_Type *txrx_setup;
    uint32_t status;

    tmp = HAL_I2C_getNum(I2Cx);
    txrx_setup = (I2C_S_SETUP_Type *)&i2cdat[tmp].txrx_setup_slave;

    status = I2Cx->ST;
    switch (status) {
        case 0x15: // receive mode - slave select + ACK
        case 0x45: // receive mode - data ACK
            if ((txrx_setup->rx_count < txrx_setup->rx_length) && (txrx_setup->rx_data != NULL)) {
                txrx_setup->rx_data[txrx_setup->rx_count] = I2Cx->DR;
                txrx_setup->rx_count++;
            }
            break;

        case 0x20: // receive mode
        case 0x22: // transmit mode - stop receive
            goto s_int_end;

        case 0x17: // transmit mode - slave select + ACK
        case 0x46: // transmit mode - data NOACK
        case 0x47: // transmit mode - data ACK
            if ((txrx_setup->tx_count < txrx_setup->tx_length) && (txrx_setup->tx_data != NULL)) {
                I2Cx->DR = txrx_setup->tx_data[txrx_setup->tx_count];
                txrx_setup->tx_count++;
            }
            break;
    }

    I2Cx->ST = 0xff; // flag clear and SCL go to HIGH
    return;

s_int_end:

    I2Cx->ST = 0xff; // flag clear and SCL go to HIGH
    // Disable interrupt
    HAL_I2C_IntCmd(I2Cx, FALSE);
    I2C_SlaveComplete[tmp] = TRUE;
}

/**********************************************************************
* @brief         Wait and return status in master mode
* @param[in]     I2Cx    I2C peripheral selected, should be:
*                        - I2C0    :I2C0 peripheral
*                        - I2C1    :I2C1 peripheral
*                        - I2C2    :I2C2 peripheral
* @return        Return status
**********************************************************************/
int32_t HAL_I2C_MWait(I2C_Type *I2Cx) {
    uint32_t tmp;
    int32_t ret = 0;

    I2Cx->ST = 0xFF;
    while (1) {
        if((I2Cx->CR & 0x10) != 0)           // touch ic를 제거시 여기서 먹통됨.
          break;

        //logPrintf("MWait error\r\n");
    }

    tmp = I2Cx->ST;
    switch (tmp) {
        // Transmitter mode
        case 0x87: ret = TRANS_MODE; break;
        // Receive mode
        case 0x85: ret = RECEIVE_MODE; break;
        // Transed Data
        case 0x47: ret = TRANS_DATA; break;
        // Received Data
        case 0x44:
        case 0x45: ret = RECEIVE_DATA; break;
        default:
            if (tmp&0x08) {
                ret = LOST_BUS;        // lost
            } else if (tmp&0x20) {
                ret = STOP_DECT;  // stop
            } else {
                ret = -1;
            }
        break;
    }

    return ret;
}

/**********************************************************************
* @brief         Transmit and Receive data in master mode
* @param[in]     I2Cx    I2C peripheral selected, should be:
*                        - I2C0    :I2C0 peripheral
*                        - I2C1    :I2C1 peripheral
*                        - I2C2    :I2C2 peripheral
* @param[in]     TransferCfg        Pointer to a I2C_M_SETUP_Type structure that
*                                   contains specified information about the
*                                   configuration for master transfer.
* @param[in]     Opt                a I2C_TRANSFER_OPT_Type type that selected for
*                                   interrupt or polling mode.
* @return        SUCCESS or ERROR
**********************************************************************/
Status HAL_I2C_MasterTransferData(I2C_Type* I2Cx, I2C_M_SETUP_Type *TransferCfg, I2C_TRANSFER_OPT_Type Opt) {
    int32_t tmp;
    uint32_t exitflag;
    int32_t Ret;

    // Reset I2C setup value to default state
    TransferCfg->tx_count = 0;
    TransferCfg->rx_count = 0;

    while (I2Cx->ST & 0x04) {}    // busy check //

    if (Opt == I2C_TRANSFER_POLLING) {
        /* First Start condition -------------------------------------------------------------- */
        // Reset I2C setup value to default state
        TransferCfg->tx_count = 0;
        TransferCfg->rx_count = 0;

        if (TransferCfg->tx_count < TransferCfg->tx_length) {

            I2Cx->DR = (TransferCfg->sl_addr7bit << 1); //write

            // Start command
            I2Cx->CR |= (1 << 0); // START
            Ret = HAL_I2C_MWait(I2Cx);

            if ((Ret != TRANS_MODE)) {
                I2Cx->CR |= (1 << 1); // STOP
                HAL_I2C_MWait(I2Cx);
                I2Cx->ST = 0xFF;
                I2Cx->CR = 0
                | (1 << 7)    // I2C Block Enable
                | (1 << 5) // Interrupt Enable
                | (1 << 3); // ACKEN

                return ERROR;
            }

            exitflag = 1;
            while (exitflag) {
              if (TransferCfg->tx_count < TransferCfg->tx_length) {
                    I2Cx->DR = TransferCfg->tx_data[TransferCfg->tx_count];
                    TransferCfg->tx_count++;
                    Ret = HAL_I2C_MWait(I2Cx);

                    if ((Ret != TRANS_DATA)) {
                        I2Cx->CR |= (1 << 1);    // STOP
                        HAL_I2C_MWait(I2Cx);
                        I2Cx->ST = 0xFF;
                        I2Cx->CR = 0
                        | (1 << 7)            // I2C Block Enable
                        | (1 << 5)            // Interrupt Enable
                        | (1 << 3);           // ACKEN
                        return ERROR;
                    }
                } else {
                    if (TransferCfg->rx_count >= TransferCfg->rx_length) {
                        I2Cx->CR |= (1 << 1);    // STOP
                        HAL_I2C_MWait(I2Cx);
                        I2Cx->ST = 0xFF; //?
                        I2Cx->CR = 0
                        | (1 << 7)            // I2C Block Enable
                        | (1 << 5)           // Interrupt Enable
                        | (1 << 3); // ACKEN
                        return SUCCESS;
                    } else {
                        exitflag = 0;
                    }
                }
            }
        }

        if (TransferCfg->rx_count < TransferCfg->rx_length) {
            I2Cx->DR = ((TransferCfg->sl_addr7bit << 1) | 0x01);
            I2Cx->CR |= (1 << 0);     // START
            Ret = HAL_I2C_MWait(I2Cx);

            if ((Ret != RECEIVE_MODE)) {
                I2Cx->CR |= (1 << 1);   // STOP
                HAL_I2C_MWait(I2Cx);
                I2Cx->ST = 0xFF;
                I2Cx->CR = 0
                | (1 << 7)              // I2C Block Enable
                | (1 << 5)              // Interrupt Enable
                | (1 << 3);             // ACKEN
                return ERROR;
            }

            exitflag = 1;
            while (exitflag) {
                if ((TransferCfg->rx_length > 1) && (TransferCfg->rx_count < (TransferCfg->rx_length - 1))) {
                    Ret = HAL_I2C_MWait(I2Cx);

                    if ((Ret != RECEIVE_DATA)) {
                        I2Cx->CR |= (1 << 1);    // STOP
                        HAL_I2C_MWait(I2Cx);
                        I2Cx->ST = 0xFF;
                        I2Cx->CR = 0
                        | (1 << 7)            // I2C Block Enable
                        | (1 << 5)           // Interrupt Enable
                        | (1 << 3); // ACKEN
                        return ERROR;
                    }
                } else {  // the next byte is the last byte, send NACK instead.
                    I2Cx->CR &= ((1 << 7) | (1 << 5));    // clear ACKEN
                    Ret = HAL_I2C_MWait(I2Cx);

                    if ((Ret != RECEIVE_DATA)) {
                        I2Cx->CR |= (1<<1);    // STOP
                        HAL_I2C_MWait(I2Cx);
                        I2Cx->ST = 0xFF;
                        I2Cx->CR = 0
                        | (1 << 7)            // I2C Block Enable
                        | (1 << 5)           // Interrupt Enable
                        | (1 << 3); // ACKEN
                        return ERROR;
                    }
                }

                TransferCfg->rx_data[TransferCfg->rx_count] = I2Cx->DR;
                TransferCfg->rx_count++;

                if (TransferCfg->rx_count == TransferCfg->rx_length) {
                    exitflag = 0;
                }
            }

            I2Cx->CR |= (1 << 1);    // STOP
            HAL_I2C_MWait(I2Cx);
            I2Cx->ST = 0xFF;
            I2Cx->CR = 0
            | (1 << 7)            // I2C Block Enable
            | (1 << 5)           // Interrupt Enable
            | (1 << 3); // ACKEN
            return SUCCESS;
        }
    } else if (Opt == I2C_TRANSFER_INTERRUPT) {
        tmp = HAL_I2C_getNum(I2Cx);
        I2C_MasterComplete[tmp] = FALSE;
        // Setup tx_rx data, callback and interrupt handler
        i2cdat[tmp].txrx_setup_master = *TransferCfg;

        // Set direction phase, write first
        i2cdat[tmp].dir = 0;

        /* First Start condition -------------------------------------------------------------- */
        HAL_I2C_IntCmd(I2Cx, TRUE);
        if (TransferCfg->tx_count < TransferCfg->tx_length) {
            I2Cx->DR = (TransferCfg->sl_addr7bit << 1); //write
        } else if (TransferCfg->rx_count < TransferCfg->rx_length) {
            I2Cx->DR = ((TransferCfg->sl_addr7bit << 1) | 0x01);
        }

        // Start command
        I2Cx->CR |= (1 << 0);    // START

        return (SUCCESS);
    }

    return ERROR;
}

/**********************************************************************
* @brief        Wait and return status in slave mode
* @param[in]    I2Cx    I2C peripheral selected, should be:
*                       - I2C0    :I2C0 peripheral
*                       - I2C1    :I2C1 peripheral
*                       - I2C2    :I2C2 peripheral
* @return        Return status
**********************************************************************/
int32_t HAL_I2C_SWait(I2C_Type *I2Cx) {
    uint32_t tmp;
    int32_t ret = 0;

    I2Cx->ST = 0xFF;
    while (1) {
        if ((I2Cx->CR & 0x10) != 0) break;
    }

    tmp = I2Cx->ST;

    switch (tmp) {
        // Receive mode
        case 0x15:
        case 0x95: ret = RECEIVE_MODE; break;
        // Transmitter mode
        case 0x17:
        case 0x97: ret = TRANS_MODE; break;
        // Received Data
        case 0x45: ret = RECEIVE_DATA; break;
        // Transed Data
        case 0x47: ret = TRANS_DATA; break;
        default:
            if (tmp & 0x08) {
                ret = LOST_BUS;        // lost
            } else if (tmp & 0x20) {
                ret = STOP_DECT;        // stop
            } else {
                ret = -1;
            }
        break;
    }

    return ret;
}

/*********************************************************************//**
* @brief        Receive and Transmit data in slave mode
* @param[in]    I2Cx    I2C peripheral selected, should be:
*                       - I2C0    :I2C0 peripheral
*                       - I2C1    :I2C1 peripheral
*                       - I2C2    :I2C2 peripheral
* @param[in]    TransferCfg       Pointer to a I2C_S_SETUP_Type structure that
*                                 contains specified information about the
*                                 configuration for master transfer.
* @param[in]    Opt               I2C_TRANSFER_OPT_Type type that selected for
*                                 interrupt or polling mode.
* @return       SUCCESS or ERROR
**********************************************************************/
Status HAL_I2C_SlaveTransferData(I2C_Type* I2Cx, I2C_S_SETUP_Type *TransferCfg, I2C_TRANSFER_OPT_Type Opt) {
    int32_t tmp;
    int32_t Ret;

    // Reset I2C setup value to default state
    TransferCfg->tx_count = 0;
    TransferCfg->rx_count = 0;

    // Polling option
    if (Opt == I2C_TRANSFER_POLLING) {
        while (1) {
            Ret = HAL_I2C_SWait(I2Cx);    // Start
            switch (Ret) {
                case RECEIVE_MODE:
                case RECEIVE_DATA:
                    if ((TransferCfg->rx_count < TransferCfg->rx_length) && (TransferCfg->rx_data != NULL)) {
                        TransferCfg->rx_data[TransferCfg->rx_count] = I2Cx->DR;
                        TransferCfg->rx_count++;
                    }
                    break;
                case TRANS_MODE:
                case TRANS_DATA:
                    if ((TransferCfg->tx_count < TransferCfg->tx_length) && (TransferCfg->tx_data != NULL)) {
                        I2Cx->DR = TransferCfg->tx_data[TransferCfg->tx_count];
                        TransferCfg->tx_count++;
                    }
                    break;
                case STOP_DECT:
                    goto s_end_stage;
                case 0: break;
                default:
                    goto s_error;
            }
        }

s_end_stage:
        I2Cx->ST = 0xFF;
        return SUCCESS;

s_error:
        I2Cx->ST = 0xFF;
        return ERROR;
    } else if (Opt == I2C_TRANSFER_INTERRUPT) {
        tmp = HAL_I2C_getNum(I2Cx);
        I2C_SlaveComplete[tmp] = FALSE;
        // Setup tx_rx data, callback and interrupt handler
        i2cdat[tmp].txrx_setup_slave = *TransferCfg;

        // Set direction phase, read first
        i2cdat[tmp].dir = 1;

        HAL_I2C_IntCmd(I2Cx, TRUE);

        return (SUCCESS);
    }

    return ERROR;
}

/********************************************************************//**
* @brief        Transmit an array of bytes in Master mode
* @param[in]    I2Cx    I2C peripheral selected, should be:
*                       - I2C0    :I2C0 peripheral
*                       - I2C1    :I2C1 peripheral
*                       - I2C2    :I2C2 peripheral
* @param[in]    TransferCfg   Pointer to a I2C_M_SETUP_Type structure that
*                                 contains specified information about the
*                                 configuration for master transfer.
* @param[in]    Opt               a I2C_TRANSFER_OPT_Type type that selected for
*                                 interrupt or polling mode.
* @return       SUCCESS or ERROR
*********************************************************************/
Status    HAL_I2C_MasterTransmitData(I2C_Type* I2Cx, I2C_M_SETUP_Type *TransferCfg, I2C_TRANSFER_OPT_Type Opt) {
    TransferCfg->rx_data = NULL;
    TransferCfg->rx_length = 0;
    TransferCfg->tx_count = 0;
    TransferCfg->rx_count = 0;

    return HAL_I2C_MasterTransferData(I2Cx, TransferCfg, Opt);
}

/********************************************************************//**
* @brief        Receive an array of bytes in Master mode
* @param[in]    I2Cx    I2C peripheral selected, should be:
*                       - I2C0    :I2C0 peripheral
*                       - I2C1    :I2C1 peripheral
*                       - I2C2    :I2C2 peripheral
* @param[in]    TransferCfg   Pointer to a I2C_M_SETUP_Type structure that
*                                 contains specified information about the
*                                 configuration for master transfer.
* @param[in]    Opt               a I2C_TRANSFER_OPT_Type type that selected for
*                                 interrupt or polling mode.
* @return       SUCCESS or ERROR
*********************************************************************/
Status HAL_I2C_MasterReceiveData(I2C_Type* I2Cx, I2C_M_SETUP_Type *TransferCfg, I2C_TRANSFER_OPT_Type Opt) {
    TransferCfg->tx_data = NULL;
    TransferCfg->tx_length = 0;
    TransferCfg->tx_count = 0;
    TransferCfg->rx_count = 0;

    return HAL_I2C_MasterTransferData (I2Cx, TransferCfg, Opt);
}

/********************************************************************//**
* @brief        Receive an array of bytes in Slave mode
* @param[in]    I2Cx    I2C peripheral selected, should be:
*                       - I2C0    :I2C0 peripheral
*                       - I2C1    :I2C1 peripheral
*                       - I2C2    :I2C2 peripheral
* @param[in]    TransferCfg   Pointer to a I2C_S_SETUP_Type structure that
*                                 contains specified information about the
*                                 configuration for slave transfer.
* @param[in]    Opt               a I2C_TRANSFER_OPT_Type type that selected for
*                                 interrupt or polling mode.
* @return       SUCCESS or ERROR
*********************************************************************/
Status HAL_I2C_SlaveReceiveData(I2C_Type* I2Cx, I2C_S_SETUP_Type *TransferCfg, I2C_TRANSFER_OPT_Type Opt) {
    TransferCfg->tx_data = NULL;
    TransferCfg->tx_length = 0;
    TransferCfg->tx_count = 0;
    TransferCfg->rx_count = 0;

    return HAL_I2C_SlaveTransferData(I2Cx, TransferCfg, Opt);
}

/*********************************************************************//**
* @brief        Set Own slave address in I2C peripheral corresponding to
*               parameter specified in OwnSlaveAddrConfigStruct.
* @param[in]    I2Cx    I2C peripheral selected, should be:
*                     - I2C0    :I2C0 peripheral
*                     - I2C1    :I2C1 peripheral
*                     - I2C2    :I2C2 peripheral
* @param[in]    SlaveAddr_7bit : own slave address
* @param[in]    GeneralCallState :
*                     - ENABLE
*                     - DISABLE
* @return        None
**********************************************************************/
void HAL_I2C_SetOwnSlaveAddr0(I2C_Type *I2Cx, uint8_t SlaveAddr_7bit, uint8_t GeneralCallState) {
    I2Cx->SAR1 = (((uint32_t)(SlaveAddr_7bit << 1)) | ((GeneralCallState == ENABLE) ? 0x01 : 0x00)) & I2C_I2ADR_BITMASK;
}

void HAL_I2C_SetOwnSlaveAddr1(I2C_Type *I2Cx, uint8_t SlaveAddr_7bit, uint8_t GeneralCallState) {
    I2Cx->SAR2 = (((uint32_t)(SlaveAddr_7bit << 1)) | ((GeneralCallState == ENABLE) ? 0x01 : 0x00)) & I2C_I2ADR_BITMASK;
}


/*********************************************************************//**
* @brief        Get status of Master Transfer
* @param[in]    I2Cx    I2C peripheral selected, should be:
*                     - I2C0    :I2C0 peripheral
*                     - I2C1    :I2C1 peripheral
*                     - I2C2    :I2C2 peripheral
* @return       Master transfer status, could be:
*                     - TRUE        :master transfer completed
*                     - FALSE     :master transfer have not completed yet
**********************************************************************/
uint32_t HAL_I2C_MasterTransferComplete(I2C_Type *I2Cx) {
    uint32_t retval, tmp;

    tmp = HAL_I2C_getNum(I2Cx);
    retval = I2C_MasterComplete[tmp];
    I2C_MasterComplete[tmp] = FALSE;
    return retval;
}

/*********************************************************************//**
* @brief         Get status of Slave Transfer
* @param[in]     I2Cx    I2C peripheral selected, should be:
*                        - I2C0    :I2C0 peripheral
*                        - I2C1    :I2C1 peripheral
*                        - I2C2    :I2C2 peripheral
* @return        Complete status, could be: TRUE/FALSE
**********************************************************************/
uint32_t HAL_I2C_SlaveTransferComplete(I2C_Type *I2Cx) {
    uint32_t retval, tmp;

    tmp = HAL_I2C_getNum(I2Cx);
    retval = I2C_SlaveComplete[tmp];
    I2C_SlaveComplete[tmp] = FALSE;
    return retval;
}

/*********************************************************************//**
* @brief        Set SCL Low Time-out Function
* @param[in]    I2Cx  I2C peripheral selected, should be:
*                     - I2C0    :I2C0 peripheral
*                     - I2C1    :I2C1 peripheral
*                     - I2C2    :I2C2 peripheral
* @param[in]    Enable : Enable or Disable SCL Low Time-out
*                     - ENABLE
*                     - DISABLE
* @param[in]    Interrupt : Enable or Disable SCL Low Time-out Interrupt
*                     - ENABLE
*                     - DISABLE
* @param[in]    SclPeriod : Setting
*                     - Range
* @return       None
**********************************************************************/
void HAL_I2C_SetSclLowTimeout(I2C_Type *I2Cx, FunctionalState Enable, FunctionalState Interrupt, uint32_t SclPeriod) {
    uint32_t Reg32;

    Reg32 = I2Cx->SLTCR;
    Reg32 &= ~(I2C_SLTCR_SLTEN | I2C_SLTCR_SLTINT);

    if (Enable == ENABLE) {
        Reg32 |= I2C_SLTCR_SLTEN; // Enable SCL Time-out
    } else {
        Reg32 &= ~(I2C_SLTCR_SLTEN); // Disable SCL Time-out
    }

    if (Interrupt == ENABLE) {
        Reg32 |= I2C_SLTCR_SLTINT; // Enable Interrupt
    } else {
        Reg32 &= ~(I2C_SLTCR_SLTINT); // Disable Interrupt
    }

    I2Cx->SLTCR = Reg32; // Update SCL Timeout Function state and Interrupt

    I2Cx->SLTPDR = (SclPeriod & I2C_SLTPDR_PDATA_Msk);    // SCL Time-out Period
}

/*********************************************************************//**
* @brief         Set SCL Low Time-out Function
* @param[in]     I2Cx    I2C peripheral selected, should be:
*                        - I2C0    :I2C0 peripheral
*                        - I2C1    :I2C1 peripheral
*                        - I2C2    :I2C2 peripheral
* @return         None
**********************************************************************/
void HAL_I2C_ClearSclLowTimeoutFlag(I2C_Type *I2Cx) {
    I2Cx->ST |= I2C_ST_SLT_FLAG;
}

/*********************************************************************//**
* @brief         I2C Bus Manual Control
* @param[in]     I2Cx    I2C peripheral selected, should be:
*                        - I2C0    :I2C0 peripheral
*                        - I2C1    :I2C1 peripheral
*                        - I2C2    :I2C2 peripheral
* @param[in]     I2CBus  I2C Bus Type
*                        - I2C_SCL    : SCL Bus
*                        - I2C_SDA    : SDA Bus
* @param[in]     output  I2C Bus direction
*                        - I2C_OUTPUT_LOW               : Low
*                        - I2C_OUTPUT_HIGH_OPENDRAIN    : High (Open Drain)
* @param[in]     ManualControl  Functional State
*                        - Enable
*                        - Disable
* @return         None
**********************************************************************/
void HAL_I2C_SetManualBus(I2C_Type *I2Cx, I2C_BUS_Type I2CBus, I2C_BUS_OUTPUT_Type output, FunctionalState ManualControl) {
    uint32_t Reg32;

    Reg32 = (I2Cx->MBCR) & I2C_MBCR_CONTROL_MASK;

    // I2C Bus Manual Control Enable/Disable
    if (ManualControl == ENABLE) {
        if (I2CBus == I2C_SCL) {
            Reg32 |= I2C_MBCR_SCL_MANUAL;
        }
        if (I2CBus == I2C_SDA) {
            Reg32 |= I2C_MBCR_SDA_MANUAL;
        }
    } else {
        if (I2CBus == I2C_SCL) {
            Reg32 &= ~(I2C_MBCR_SCL_MANUAL);
        }
        if (I2CBus == I2C_SDA) {
            Reg32 &= ~(I2C_MBCR_SDA_MANUAL);
        }
    }
    // I2C Bus Output Control
    if (output == I2C_OUTPUT_HIGH_OPENDRAIN) {
        if (I2CBus == I2C_SCL) {
            Reg32 |= I2C_MBCR_SCL_OUTPUT;
        }
        if (I2CBus == I2C_SDA) {
            Reg32 |= I2C_MBCR_SDA_OUTPUT;
        }
    } else if (output == I2C_OUTPUT_LOW) {
        if (I2CBus == I2C_SCL) {
            Reg32 &= ~(I2C_MBCR_SCL_OUTPUT);
        }
        if (I2CBus == I2C_SDA) {
            Reg32 &= ~(I2C_MBCR_SDA_OUTPUT);
        }
    }

    I2Cx->MBCR = Reg32;
}

/*********************************************************************//**
* @brief         Set SCL Low Time-out Function
* @param[in]     I2Cx    I2C peripheral selected, should be:
*                        - I2C0    :I2C0 peripheral
*                        - I2C1    :I2C1 peripheral
*                        - I2C2    :I2C2 peripheral
* @param[in]     I2C_BUS_TYPE    I2C Bus type, should be:
*                        - I2C_SCL
*                        - I2C_SDA
* @return        I2C_BUS_STATUS_Type, should be:
*                        - I2C_STATUS_OPENDRAIN
*                        - I2C_STATUS_LOW
**********************************************************************/
I2C_BUS_STATUS_Type HAL_I2C_GetBusStatus(I2C_Type *I2Cx, I2C_BUS_Type I2CBus) {
    uint32_t Reg32;
    Bool state;

    Reg32 = (I2Cx->MBCR) & I2C_MBCR_STATUS_MASK;

    if (I2CBus == I2C_SCL) {
        if ((Reg32 & I2C_MBCR_STATUS_SCL) == I2C_MBCR_STATUS_SCL) {
            state = TRUE;
        } else {
            state = FALSE;
        }
    } else if (I2CBus == I2C_SDA) {
        if ((Reg32 & I2C_MBCR_STATUS_SDA) == I2C_MBCR_STATUS_SDA) {
            state = TRUE;
        } else {
            state = FALSE;
        }
    }

    if (state)        return I2C_STATUS_OPENDRAIN;
    else             return I2C_STATUS_LOW;
}

#endif /* _I2C */
/* --------------------------------- End Of File ------------------------------ */














