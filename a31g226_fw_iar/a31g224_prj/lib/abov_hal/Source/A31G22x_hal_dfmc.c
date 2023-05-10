/***************************************************************************//**
* @file     A31G22x_hal_dfmc.c
* @brief    Contains all functions support for DFMC (Data Flash Memory Controller) dirver
*           on A31G22x
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
#ifdef _DFMC

#include "A31G22x_hal_dfmc.h"
/*******************************************************************************
* Private Pre-processor Definition & Macro
*******************************************************************************/
extern uint32_t SystemCoreClock;

/***************************************************************************//**
* @Name       HAL_DFMC_Init
* @brief      Initialization of data flash memory
* @return     None
*******************************************************************************/
int HAL_DFMC_Init (unsigned long addr, unsigned long clk, unsigned long func) {
	__disable_irq();
	WDT->CR = (0x5A69 << 16)
            |(0x25<<10)
            |(0x1A<<4)
            |0;

	//---------------------------------------------------------------------------------
	// 1. ALL clock enable
	//---------------------------------------------------------------------------------
	SCU->CSCR = (0xA507UL<<16)
            |(0x8<<12)
            |(0x8<<8)
            |(0x8<<4)   // 32MHz IRC (HSI)
            |(0x8<<0);

	SCU->SCCR = (0x570AUL<<16)
            |(0x02);   // HSIOSC enable

    //  Flash Wait
	DFMC->MR = 0x81; DFMC->MR = 0x28;
	DFMC->CFG= DFMCFG_ENTRY;
	DFMC->MR = 0x00;

	return (0);
}


/***************************************************************************//**
* @Name       HAL_DFMC_UnInit
* @brief      Uninitialization of data flash memory
* @return     None
*******************************************************************************/
int HAL_DFMC_UnInit (unsigned long func) {

    //  Flash Wait
	DFMC->MR = 0x81; DFMC->MR = 0x28;
	DFMC->CFG= DFMCFG_EXIT;
	DFMC->MR = 0x00;

	return (0);
}

/***************************************************************************//**
* @Name       HAL_DFMC_WAIT
* @brief      Wait until Busy Flag is set.
* @return     None
*******************************************************************************/
void HAL_DFMC_WAIT(void) {
	while(1) {
		// wait until MR_BUSYE is "0"
		if (DFMMR_BUSY==0)
			break;
	}
}

/***************************************************************************//**
* @Name       HAL_DFMC_FLASH_ALL_ENABLE
* @brief      Disable write protection to enable write/erase all region of data flash memory
* @return     None
*******************************************************************************/
void HAL_DFMC_FLASH_ALL_ENABLE(void) {
	DFMC->MR = 0x66;
	DFMC->MR = 0x99;

	DFMC->WPROT = 0x00000000;	// Remove FLASH write protection

	DFMC->MR = 0x00;
}

/***************************************************************************//**
* @Name       HAL_DFMC_EraseChip
* @brief      Chip (Mass) erase - removed all data to '0xFF' of data flash memory
* @return     0: Success
*******************************************************************************/
int HAL_DFMC_EraseChip (void)
{
	HAL_DFMC_FLASH_ALL_ENABLE();

	DFMC->MR = 0x5A;
    DFMC->MR = 0xA5;

	DFMCR_PMOD_SET;
	DFMCR_MAS_SET;
	DFMCR_ERS_SET;
	DFMCR_WADCK_SET;
	DFMCR_HVEN_SET;

	HAL_DFMC_WAIT();					// Chip Erase

	DFMC->CR = 0x0;
	DFMC->MR = 0x0;

	return (0); 						 // Success
}


/***************************************************************************//**
* @Name       HAL_DFMC_EraseSector
* @brief      Erase 512Bytes Sector Erase of data flash memory area
* @param[in]  sel is DOTP_TYPE enumeration variable, It should be
*             - DATA_OPTION_NOT_USE: No access option area
*             - DATA_OPTION_READPROT: Enable the access of Read Protection Option area
*             - DATA_OPTION: Enable the access of User Data Option area
* @param[in]  addr is sector address to erase
* @return     - 0 : Success
*             - 1 : Error
*******************************************************************************/
int HAL_DFMC_EraseSector (int sel, unsigned long addr) {
	HAL_DFMC_FLASH_ALL_ENABLE();

	DFMC->AR = addr;

// flash mode entry
	DFMC->MR = 0x5A;DFMC->MR = 0xA5;

	DFMCR_PMOD_SET;
	DFMCR_ERS_SET;
	if (!HAL_DFMC_OptionAccess((DOTP_TYPE)sel, ENABLE)) {
		return (1);
	}
	DFMCR_WADCK_SET;
	DFMCR_HVEN_SET;

	HAL_DFMC_WAIT();

	if (!HAL_DFMC_OptionAccess((DOTP_TYPE)sel, DISABLE)) {
		return (1);
	}

	DFMC->CR = 0x0;
	DFMC->MR = 0x0;

	return (0);
}

/***************************************************************************//**
* @Name       HAL_DFMC_EraseSector1k
* @brief      Erase 1KB Sector Erase of data flash memory area
* @param[in]  sel is DOTP_TYPE enumeration variable, It should be
*             - DATA_OPTION_NOT_USE: No access option area
*             - DATA_OPTION_READPROT: Enable the access of Read Protection Option area
*             - DATA_OPTION: Enable the access of User Data Option area
* @param[in]  addr is sector address to erase
* @return     - 0 : Success
*             - 1 : Error
*******************************************************************************/
int HAL_DFMC_EraseSector1k (int sel, unsigned long addr) {
	HAL_DFMC_FLASH_ALL_ENABLE();

//	DFMC->AR = addr>>2;
	DFMC->AR = addr;

// flash mode entry
	DFMC->MR = 0x5A;DFMC->MR = 0xA5;

	DFMCR_PMOD_SET;
	DFMCR_ERS_SET;
	DFMCR_1KEN_SET; // 1KB Erase Enable
	if (!HAL_DFMC_OptionAccess((DOTP_TYPE)sel, ENABLE)) {
		return (1);
	}
	DFMCR_WADCK_SET;
	DFMCR_HVEN_SET;

	HAL_DFMC_WAIT();

	if (!HAL_DFMC_OptionAccess((DOTP_TYPE)sel, DISABLE)) {
		return (1);
	}

	DFMC->CR = 0x0;
	DFMC->MR = 0x0;

	return (0);
}

/***************************************************************************//**
* @Name       HAL_DFMC_EraseSector4k
* @brief      Erase 4KB Sector Erase of data flash memory area
* @param[in]  sel is DOTP_TYPE enumeration variable, It should be
*             - DATA_OPTION_NOT_USE: No access option area
*             - DATA_OPTION_READPROT: Enable the access of Read Protection Option area
*             - DATA_OPTION: Enable the access of User Data Option area
* @param[in]  addr is sector address to erase
* @return     - 0 : Success
*             - 1 : Error
*******************************************************************************/
int HAL_DFMC_EraseSector4k (int sel, unsigned long addr) {
	HAL_DFMC_FLASH_ALL_ENABLE();

	DFMC->AR = addr>>2;

// flash mode entry
	DFMC->MR = 0x5A;DFMC->MR = 0xA5;

	DFMCR_PMOD_SET;
	DFMCR_ERS_SET;
	DFMCR_4KEN_SET;
	if (!HAL_DFMC_OptionAccess((DOTP_TYPE)sel, ENABLE)) {
		return (1);
	}
	DFMCR_WADCK_SET;
	DFMCR_HVEN_SET;

	HAL_DFMC_WAIT();

	if (!HAL_DFMC_OptionAccess((DOTP_TYPE)sel, DISABLE)) {
		return (1);
	}

	DFMC->CR = 0x0;
	DFMC->MR = 0x0;

	return (0);
}


/***************************************************************************//**
* @Name       HAL_DFMC_ProgramPage
* @brief      Program 512 Bytes Page(Sector) Program of data flash memory area
* @param[in]  sel is DOTP_TYPE enumeration variable, It should be
*             - DATA_OPTION_NOT_USE: No access option area
*             - DATA_OPTION_READPROT: Enable the access of Read Protection Option area
*             - DATA_OPTION: Enable the access of User Data Option area
* @param[in]  addr is sector address to write to data flash memory
* @param[in]  size is data buffer size to write to data flash memory (512 Bytes)
* @param[in]  *buf is data buffer pointer to be wrote data to code flash memory
* @return     - 0 : Success
*             - 1 : Error
*******************************************************************************/
int HAL_DFMC_ProgramPage (int sel,unsigned long addr, unsigned long size, unsigned char *buf) {
	int	i;
	unsigned long *ptr;

	//---------------------------------------------------------------------------------
	// check address and size alignment
	//---------------------------------------------------------------------------------
	if((addr & 0x03) || (size & 0x03)) {
		return (1);
	}

	ptr = (unsigned long*)buf;

	HAL_DFMC_FLASH_ALL_ENABLE();

	DFMC->MR = 0x81; DFMC->MR = 0x28;
	DFMC->CFG= DFMCFG_ENTRY;
	DFMC->MR = 0x00; DFMC->MR = 0x00;

	// flash mode entry
	DFMC->MR = 0x5A; DFMC->MR = 0xA5;

	DFMCR_PMOD_SET;
	if (!HAL_DFMC_OptionAccess((DOTP_TYPE)sel, ENABLE)) {
		return (1);
	}

	DFMC->AR = (addr >> 2);
	DFMCR_PGM_SET;

	for(i = 0 ; i < size ; i += 4) {
		DFMC->DR = *ptr++;
		DFMCR_WADCK_SET;
		DFMCR_HVEN_SET;

		HAL_DFMC_WAIT();

		DFMCR_HVEN_CLR;
		DFMC->BUSY = 0x80; // WRBUSY flag clear
	}

	if (!HAL_DFMC_OptionAccess((DOTP_TYPE)sel, DISABLE)) {
		return (1);
	}

	DFMC->CR = 0x0;
	DFMC->MR = 0x0;

	return 0;
}

/***************************************************************************//**
* @Name       HAL_DFMC_ByteProgram
* @brief      The data is programmed in the address specified in a byte unit in the data flash memory.
* @param[in]  sel is DOTP_TYPE enumeration variable, It should be
*             - DATA_OPTION_NOT_USE: No access option area
*             - DATA_OPTION_READPROT: Enable the access of Read Protection Option area
*             - DATA_OPTION: Enable the access of User Data Option area
* @param[in]  addr: data flash memory address of the area where data is to be written.
* @param[in]  size is data buffer length. It must be byte unit.
* @param[in]  *buf is a pointer of the data buffer to be written to the data flash memory.
* @return     - 0 : Success
*             - 1 : Error
*******************************************************************************/
int HAL_DFMC_ByteProgram(int sel, unsigned long addr, unsigned long size, unsigned char *buf) {
	int	i;
	unsigned char *ptr;

	//---------------------------------------------------------------------------------
	// check address
	//---------------------------------------------------------------------------------
    addr &= 0x00007FFF;     // Address Masking (0x0~0x7FFF, 32KB)
    if (addr >= 0x8000) {
        return (1);
    }

	ptr = (unsigned char *)buf;

	HAL_DFMC_FLASH_ALL_ENABLE();

	// flash mode entry
	DFMC->MR = 0x5A; DFMC->MR = 0xA5;

	DFMC->AR = (addr);              // Set Address

	DFMCR_PMOD_SET;
	if (!HAL_DFMC_OptionAccess((DOTP_TYPE)sel, ENABLE)) {
		return (1);
	}
	DFMCR_PGM_SET;
    DFMCR_BYTEPGM_SET;

	for(i = 0 ; i < size ; i++) {

		DFMC->DR = ptr[i]&0xFF;		// update data value
		DFMCR_WADCK_SET; 			// This Bit is enabled, DFMC->AR+1
		DFMCR_HVEN_SET;

		HAL_DFMC_WAIT();
		DFMCR_HVEN_CLR;
		DFMCR_WADCK_CLR;
	}

	DFMC->CR = 0x0;
	DFMC->MR = 0x0;

	return (0);
}

/***************************************************************************//**
* @Name       HAL_DFMC_WordProgram
* @brief      The data is programmed in the address specified in word (4 Bytes) unit in the data flash memory.
* @param[in]  sel is DOTP_TYPE enumeration variable, It should be
*             - DATA_OPTION_NOT_USE: No access option area
*             - DATA_OPTION_READPROT: Enable the access of Read Protection Option area
*             - DATA_OPTION: Enable the access of User Data Option area
* @param[in]  addr: data flash memory address of the area where data is to be written.
* @param[in]  size is data buffer length. It must be word (4 Bytes) unit.
* @param[in]  *buf is a pointer of the data buffer to be written to the data flash memory.
* @return     - 0 : Success
*             - 1 : Error
*******************************************************************************/
int HAL_DFMC_WordProgram(int sel, unsigned long addr, unsigned long size, unsigned char *buf) {
	int	i;
	unsigned long *ptr;

	//---------------------------------------------------------------------------------
	// check address
	//---------------------------------------------------------------------------------
	if (addr & 0x03) {
		return (1);
	}

	ptr = (unsigned long *)buf; // Points buf value

	HAL_DFMC_FLASH_ALL_ENABLE();


	// flash mode entry
	DFMC->MR = 0x5A; DFMC->MR = 0xA5;

	DFMC->AR = (addr);	// Update Addr to write

	DFMCR_PMOD_SET;		// PMOD Set
	DFMCR_WORDPGM_SET;	// Word-PGM Set

	if (!HAL_DFMC_OptionAccess((DOTP_TYPE)sel, ENABLE)) {
		return (1);
	}

	DFMCR_PGM_SET;

    // Word Program
	for(i = 0 ; i < size ; i+=4) {

		DFMC->DR = *ptr++;			// DATA Update

		DFMCR_WADCK_SET; 			// WADCK Set
		DFMCR_HVEN_SET;				// HVEN Set

		HAL_DFMC_WAIT();
		DFMCR_HVEN_CLR;				// HVEN Clear
		DFMCR_WADCK_CLR;			// WADCK Clear
	}

	DFMC->CR = 0x0;
	DFMC->MR = 0x0;

	return (0);
}

/***************************************************************************//**
* @Name       HAL_DFMC_Verify
* @brief      vefify that designated data flash memory and data buffer are matched.
* @param[in]  addr is the sector address written to data flash memory.
* @param[in]  size is data buffer length
* @param[in]  *buf is a pointer to the data buffer written to the data flash memory.
* @return     - 0 : Success
*             - 1 : Error
*******************************************************************************/
unsigned long HAL_DFMC_Verify (unsigned long addr, unsigned long size, unsigned char *buf) {
	unsigned long	i;
	unsigned long	*src;
	unsigned long	*dst;
	unsigned long	result;

	//---------------------------------------------------------------------------------
	// Verify
	//---------------------------------------------------------------------------------
	result = addr + size;

	DFMC->MR = 0x5A;
	DFMC->MR = 0xA5;

	src = (unsigned long *)buf;
	dst = (unsigned long *)addr;
	for( i=0; i<size; i+=4) {
		if( *src++ != *dst++ ) {
			result = addr + i;
			break;
		}
	}

	src = (unsigned long *)buf;
	dst = (unsigned long *)addr;
	for( i=0; i<size; i+=4) {
		if( *src++ != *dst++ ) {
			result = addr + i;
			break;
		}
	}

	return (result);
}

/***************************************************************************//**
* @Name       HAL_DFMC_OptionAccess
* @brief      Enable the access to specific optional area of data flash memory
* @param[in]  sel is DOTP_TYPE enumeration variable, It should be
*             - DATA_OPTION_NOT_USE: No access option area
*             - DATA_OPTION_READPROT: Enable the access of Read Protection Option area
*             - DATA_OPTION: Enable the access of User Data Option area
* @param[in]  en is FunctionalState enumerated value, should be
*             - ENABLE: Enable the access to specific option area
*             - DISABLE: Disable the access to specific option area
* @return     - 0 : SUCCESS
*             - 1 : ERROR
*******************************************************************************/
Status HAL_DFMC_OptionAccess(DOTP_TYPE mode, FunctionalState en) {
	if (mode == DATA_OPTION_READPROT) {
		if (en == ENABLE) {
			DFMCR_LOCKSEL_SET;
		} else {
			DFMCR_LOCKSEL_CLR;
		}
		return (SUCCESS);
	} else if (mode == DATA_OPTION) {
        if (en == ENABLE) {
            DFMCR_IFEN_SET;
        } else {
            DFMCR_IFEN_CLR;
        }
		return (SUCCESS);
	} else if (mode == DATA_OPTION_NOT_USE) {
		return (SUCCESS);
	} else {
		return (ERROR);
	}
}

/***************************************************************************//**
* @Name       HAL_DFMC_SetflashAccesstiming
* @brief      Configure flash access timing (wait-time), It is recommanded to be set below 20MHz (50ns)
* @param[in]  SystemCoreClock uses global variable value that is updated when the system core clock is set.
* @param[in]  The waiting time sets the access time of the data flash memory
*             - 0 to 6: 0-wait to 6-wait
* @param[in]  autoAdjust is a function that automatically corrects
*             when the 'WaitTime' inserted by the user exceeds the recommended value. It should be,
*             - ENABLE: Enable auto adjust function of waittime
*             - DISABLE: Disable auto adjust function of waittime
* @return     None
*******************************************************************************/
void HAL_DFMC_SetflashAccesstiming(uint32_t SystemCoreClock, uint16_t WaitTime, FunctionalState autoAdjust)
{
	static uint32_t FlashAccessTiming = 0;
	Bool Valid = FALSE;

	// wait setting order 1. default wait setting -> 2. clock change -> 3. adjust wait setting/// flash memory controller
	DFMC->MR = 0x81;       // after changing 0x81 -> 0x28 in MR reg, flash access timing will be able to be set.
	DFMC->MR = 0x28;       // enter flash access timing changing mode

    if (autoAdjust == ENABLE) {
        Valid = FALSE;

        while(Valid == FALSE) {
            FlashAccessTiming = SystemCoreClock/(uint32_t)(WaitTime + 1);
            if (FlashAccessTiming > 20000000) {
                Valid = FALSE;
                WaitTime++;
            } else {
                Valid = TRUE;
                break;
            }
        }
    }

	DFMC->CFG = (0x7858 << DFMC_CFG_WTIDKY_Pos) | (WaitTime << DFMC_CFG_WAIT_Pos);
	                              // flash access time cannot overflow 20MHz.
	                              // ex) if MCLK=48MHz,
	                              //       48/(0+1) = 48 (can't set no wait)
	                              //       48/(1+1) = 24 (1 wait is not ok)
	                              //       48/(2+1) = 16 (2 wait is ok)
	                              // so, 2 wait is possible.
	DFMC->MR = 0;	      // exit flash access timing --> normal mode
}

/***************************************************************************//**
* @Name       HAL_DFMC_GetFlashAccessTiming
* @brief      Returns current the access timing (Hz) of data flash memory.
* @return     FlashAccessTiming (Hz)
*******************************************************************************/
uint32_t HAL_DFMC_GetFlashAccessTiming(void)
{
	static uint32_t FlashAccessTiming;
	static uint16_t WaitTime;

	WaitTime = HAL_DFMC_GetFlashWaitValue();
	FlashAccessTiming = SystemCoreClock/(uint32_t)(WaitTime + 1); // Get Flash Access Timing Value, (WaitTime+1) cycle

	return FlashAccessTiming;
}

/***************************************************************************//**
* @Name       HAL_DFMC_GetFlashWaitValue
* @brief      Returns current the wait time value of data flash memory.
* @return     Wait time
*******************************************************************************/
uint16_t HAL_DFMC_GetFlashWaitValue(void)
{
	return (((DFMC->CFG) & DFMC_CFG_WAIT_Msk) >> DFMC_CFG_WAIT_Pos); // Get Flash wait value
}

/***************************************************************************//**
* @Name       HAL_DFMC_SetWriteProtection
* @brief      Enable or Disable write protection (1KB unit) of data flash memory
* @param[in]  wprot: write protection selection of data flash memory, It should be,
*             - ENABLE: Enable write protection
*             - DISABLE: Disable write protection
* @param[in]  segmentNum: The segment number of data flash memory to be set write protection
*             - 0 to 31: 0-segment to 31-segment
* @return     None
*******************************************************************************/
void HAL_DFMC_SetWriteProtection(FunctionalState wprot, int segmentNum) {
    // PROT Mode
	DFMC->MR = 0x66;
	DFMC->MR = 0x99;

    if (wprot == ENABLE) {
        DFMC->WPROT |= (1 << segmentNum);	// Remove FLASH write protection
    } else {
        DFMC->WPROT &= ~(1 << segmentNum);	// Remove FLASH write protection
    }

	DFMC->MR = 0x00;
}

/***************************************************************************//**
* @Name       HAL_DFMC_SetPassword
* @brief      Set a password to be used in read protection mode.
* @param[in]  passwd: 4-byte(32-bit) value.
* @return     None
*******************************************************************************/
void HAL_DFMC_SetPassword(uint32_t passwd) {
    CFMC->PWPRST = passwd;
    CFMC->PWPRST = passwd;
}

/***************************************************************************//**
* @Name       HAL_DFMC_InsertPassword
* @brief      Enter a password.
* @param[in]  passwd: 4-byte(32-bit) value. Enter a 4-byte password. To release read protection,
*             it must match the set password. Otherwise, you will need to restart the system and enter your password again.
* @return     None
*******************************************************************************/
void HAL_DFMC_InsertPassword(uint32_t passwd) {
    CFMC->PWIN = passwd;
    CFMC->PWIN = passwd;
}

/***************************************************************************//**
* @Name       HAL_DFMC_CheckPasswordStatus
* @brief      Checks whether the entered password matches.
* @return     TRUE: password is matched. (Read protection is temporarily released.)
*             FALSE: unmatched password (It needs to restart system and enter password again.)
*******************************************************************************/
Bool HAL_DFMC_CheckPasswordStatus(void) {
    int Reg32;

    Reg32 = CFMC->RPROT;

    if ((Reg32 & CFMC_RPROT_PWMATCH_Msk) == CFMC_RPROT_PWMATCH_Msk) {
        return TRUE;
    } else {
        return FALSE;
    }
}

#endif /* _DFMC */
/* --------------------------------- End Of File ------------------------------ */
