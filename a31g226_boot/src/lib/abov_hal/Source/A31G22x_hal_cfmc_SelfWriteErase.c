/***************************************************************************//**
* @file     A31G22x_hal_cfmc_SelfWriteErase.c
* @brief    Contains all functions support for CFMC (Code Flash Main Controller) dirver
*           on A31G22x
* @author   AE Team, ABOV Semiconductor Co., Ltd.
* @version  V1.00
* @date     08. FEB. 2021
*
* Copyright(C) 2021, ABOV Semiconductor All rights reserved.
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
#ifdef _CFMC

#include "A31G22x_hal_cfmc_SelfWriteErase.h"

/*< The usage of code flash memory */
/*< 1. Self Erase / Program Code Flash code execute on the code flash area. */
/*< 2. Interrupt should be disabled during WRITING or ERASING the code flash. */

/*******************************************************************************
* Private Pre-processor Definition & Macro
*******************************************************************************/


/*******************************************************************************
* Private Typedef
*******************************************************************************/


/*******************************************************************************
* Private Variable
*******************************************************************************/


/*******************************************************************************
* Private Function Prototype
*******************************************************************************/


/*******************************************************************************
* Public Function
*******************************************************************************/

/***************************************************************************//**
* @Name       HAL_CFMC_SelfWrite
* @brief      The data is programmed in the address specified in word (4 bytes) units
*             in the code flash memory. When executing the program, it must be performed in the unused flash area.
* @param[in]  addr: code flash memory address of the area where data is to be written.
* @param[in]  size is data buffer length. It must be word(4-bytes) unit.
* @param[in]  *buf is a pointer of the data buffer to be written to the code flash memory.
* @return     - 0 : Success
*             - 1 : Error
*******************************************************************************/
int HAL_CFMC_SelfWrite (unsigned long addr, unsigned long size, unsigned char *buf)
{
	int	i;
	unsigned long *ptr;

	//---------------------------------------------------------------------------------
	// check address and size alignment
	//---------------------------------------------------------------------------------
	if((addr & 0x03) || (size & 0x03)) {
		return (1);
	}

	ptr = (unsigned long*)buf;

    CFMC->MR = 0x66;
    CFMC->MR = 0x99;
    CFMC->WPROT = 0x00000000;   // Remove FLASH write protection
    CFMC->ABWPROT = (0xAABUL << 20) | (0x0000UL << 0); // Remove Active bank write protection
    CFMC->NBWPROT = (0x55BUL << 20) | (0x0000UL << 0); // Remove Non-active bank write protection
    CFMC->MR = 0x00;
    CFMC->MR = 0x00;

	// flash mode entry
	CFMC->MR = 0x5A;CFMC->MR = 0xA5;

	CFMC->CR |= (CFMC_CR_SELFPGM_Msk | CFMC_CR_PGM_Msk);
	__NOP();__NOP();__NOP();__NOP();__NOP();

	for(i = 0 ; i < size ; i += 4) {
		*(volatile unsigned long *)(addr) = *ptr;
		addr+=4;
		ptr++;
	}
	__NOP();__NOP();__NOP();__NOP();

	CFMC->CR &= ~(CFMC_CR_PGM_Msk);

	CFMC->CR = 0x0;
    CFMC->MR = 0x0;

	return 0;
}

/***************************************************************************//**
* @Name       HAL_CFMC_SelfErase
* @brief      The data is erased in the address specified in word (4 bytes) units
*             in the code flash memory. When executing the program, it must be performed in the unused flash area.
* @param[in]  addr: code flash memory address of the area where data is to be written.
* @param[in]  size is Erase length in word (4 bytes) unit.
* @return     - 0 : Success
*             - 1 : Error
*******************************************************************************/
int HAL_CFMC_SelfErase (unsigned long addr, unsigned long size)
{
	int i;

    CFMC->MR = 0x66;
    CFMC->MR = 0x99;
    CFMC->WPROT = 0x00000000;   // Remove FLASH write protection
    CFMC->ABWPROT = (0xAABUL << 20) | (0x0000UL << 0); // Remove Active bank write protection
    CFMC->NBWPROT = (0x55BUL << 20) | (0x0000UL << 0); // Remove Non-active bank write protection
    CFMC->MR = 0x00;
    CFMC->MR = 0x00;

// flash mode entry
	CFMC->MR = 0x5A;CFMC->MR = 0xA5;

	CFMC->CR |= (CFMC_CR_SELFPGM_Msk | CFMC_CR_ERS_Msk);
	__NOP();__NOP();__NOP();__NOP();__NOP();

	for(i = 0 ; i < size ; i += 4) {
        *(unsigned long *)(addr) = 0xFFFFFFFF;
		addr+=4;
	}

	__NOP();__NOP();__NOP();__NOP();

	CFMC->CR &= ~(CFMC_CR_ERS_Msk);

	CFMC->CR = 0x0;
    CFMC->MR = 0x0;

	return (0);
}

#endif /* _CFMC */
/* --------------------------------- End Of File ------------------------------ */
