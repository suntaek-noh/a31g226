/***************************************************************************//**
* @file     A31G22x_hal_cfmc.c
* @brief    Contains all functions support for CFMC (Code Flash Main Controller) dirver
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
#ifdef _CFMC

#include "A31G22x_hal_cfmc.h"

/*< The usage of code flash memory */
/*< 1. Code Flash code should execute on the SRAM to PROGRAM or ERASE the code flash. */
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
* @Name       HAL_CFMC_SET_OTP
* @brief      Set Option area Access bits
* @param      otp : the variable to access option area
*              - 1: Enable option area access
*              - 0: Disable option area access
* @return     None
*******************************************************************************/
void HAL_CFMC_SET_OTP(unsigned long otp)
{
	CFMC->MR = 0x5A;
	CFMC->MR = 0xA5;
	if(otp==0) {
		CFMC->MR = 0;
	}
	else {
		CFMC->MR = (otp<<28);// | BV_OTPE;
		FMCR_IFEN_SET;
		FMCR_4KEN_CLR;
		//FMCR_1KEN_CLR;
		//FMCR_BBLOCK_SET;
	}
}

/***************************************************************************//**
* @Name       HAL_CFMC_Init
* @brief      Initialization of code flash memory
* @return     None
*******************************************************************************/
int HAL_CFMC_Init(unsigned long addr, unsigned long clk, unsigned long func)
{
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
            |(0x8<<4)   // 32MHz
            |(0x8<<0);

	SCU->SCCR = (0x570AUL<<16)
            |(0x02);   // HSIOSC enable

	return (0);
}


/***************************************************************************//**
* @Name       HAL_CFMC_UnInit
* @brief      Uninitialization of code flash memory
* @return     None
*******************************************************************************/
int HAL_CFMC_UnInit(unsigned long func)
{
	return (0);
}

/***************************************************************************//**
* @Name       HAL_CFMC_WAIT
* @brief      Wait until Busy Flag is set.
* @return     None
*******************************************************************************/
void HAL_CFMC_WAIT(void)
{
	while(1) {
		// wait until MR_BUSYE is "0"
		if (FMMR_BUSY == 0)
			break;
	}
}

/***************************************************************************//**
* @Name       HAL_CFMC_FLASH_ALL_ENABLE
* @brief      Disable write protection to enable write/erase all region of code flash memory
* @return     None
*******************************************************************************/
void HAL_CFMC_FLASH_ALL_ENABLE(void)
{
	CFMC->MR = 0x66;
	CFMC->MR = 0x99;
	CFMC->WPROT = 0x00000000;	// Remove FLASH write protection
	CFMC->ABWPROT = (0xAABUL << 20) | (0x0000UL << 0); // Remove Active bank write protection
	CFMC->NBWPROT = (0x55BUL << 20) | (0x0000UL << 0); // Remove Non-active bank write protection
	CFMC->MR = 0x00;
	CFMC->MR = 0x00;
}

void HAL_CFMC_FLASH_NAB_ENABLE(void)
{
    uint32_t Reg32;

    CFMC->MR = 0x66;
    CFMC->MR = 0x99;

    Reg32 = SCUCC->CHIPID;

    if (Reg32 == 0x4D31A00B) {
        // if A31G224
        CFMC->WPROT = 0x0000000F;   // Remove FLASH write protection of Non Active Area, protect Active area
        CFMC->NBWPROT = (0x55BUL << 20) | (0x0000UL << 0); // Remove Non-active bank write protection
    } else {
        // if A31G226
        CFMC->WPROT = 0x000000FF;   // Remove FLASH write protection of Non Active Area, protect Active area
        CFMC->NBWPROT = (0x55BUL << 20) | (0x0000UL << 0); // Remove Non-active bank write protection
    }

    CFMC->ABWPROT = (0xAABUL << 20) | (0xFFFFUL << 0); // Protect Active bank write protection

    CFMC->MR = 0x00;
    CFMC->MR = 0x00;
}

/***************************************************************************//**
* @Name       HAL_CFMC_EraseChip
* @brief      Chip (Mass) erase - removed all data to '0xFF' of code flash memory
* @return     0: Success
*******************************************************************************/
int HAL_CFMC_EraseChip(void)
{
#if 0					//Erase Lock
	HAL_CFMC_SET_OTP(0x08);
	HAL_CFMC_EraseSector(0);
	HAL_CFMC_SET_OTP(0x00);
#endif

	HAL_CFMC_FLASH_ALL_ENABLE();

	CFMC->MR = 0x81;CFMC->MR = 0x28;
	CFMC->CFG= FMCFG_ENTRY;
	CFMC->MR = 0x00;CFMC->MR = 0x00;

	CFMC->MR = 0x5A;CFMC->MR = 0xA5;

	FMCR_PMOD_SET;
	FMCR_MAS_SET;
	FMCR_ERS_SET;
	FMCR_WADCK_SET;
	FMCR_HVEN_SET;

	HAL_CFMC_WAIT();					// Chip Erase

	CFMC->CR = 0x0;
	CFMC->MR = 0x0;

	return (0); 									   // Success
}

/***************************************************************************//**
* @Name       HAL_CFMC_EraseSector
* @brief      Erase 512Bytes Sector Erase of code flash memory area
* @param[in]  sel is OTP_TYPE variable, It should be
*             - OPTION_NOT_USE: No access option area
*             - OPTION_USERDATA0: Enable the access of User Data 0 Option area
*             - OPTION_USERDATA1: Enable the access of User Data 1 Option area
*             - OPTION_USERDATA2: Enable the access of User Data 2 Option area
*             - OPTION_READPROT: Enable the access of Read Protection Option area
*             - OPTION_BANKSWAP: Enable the access of Memory Bank Swap Option area
* @param[in]  addr is sector address to erase
* @return     - 0 : Success
*             - 1 : Error
*******************************************************************************/
int HAL_CFMC_EraseSector(int sel, unsigned long addr)
{
	HAL_CFMC_FLASH_ALL_ENABLE();

	CFMC->AR = addr>>2;

	CFMC->MR = 0x81;CFMC->MR = 0x28;
	CFMC->CFG= FMCFG_ENTRY;
	CFMC->MR = 0x00;CFMC->MR = 0x00;

// flash mode entry
	CFMC->MR = 0x5A;CFMC->MR = 0xA5;

	FMCR_PMOD_SET;
	FMCR_ERS_SET;
	if (!HAL_CFMC_OtpAccess((OTP_TYPE)sel, ENABLE)) {
		return (1);
	}
	FMCR_WADCK_SET;
	FMCR_HVEN_SET;

	HAL_CFMC_WAIT();

	if (!HAL_CFMC_OtpAccess((OTP_TYPE)sel, DISABLE)) {
		return (1);
	}

	CFMC->CR = 0x0;
	CFMC->MR = 0x0;

	return (0);
}

/***************************************************************************//**
* @Name       HAL_CFMC_EraseSector1k
* @brief      Erase 1KB Sector Erase of code flash memory area
* @param[in]  sel is OTP_TYPE variable, It should be
*             - OPTION_NOT_USE: No access option area
*             - OPTION_USERDATA0: Enable the access of User Data 0 Option area
*             - OPTION_USERDATA1: Enable the access of User Data 1 Option area
*             - OPTION_USERDATA2: Enable the access of User Data 2 Option area
*             - OPTION_READPROT: Enable the access of Read Protection Option area
*             - OPTION_BANKSWAP: Enable the access of Memory Bank Swap Option area
* @param[in]  addr is sector address to erase
* @return     - 0 : Success
*             - 1 : Error
*******************************************************************************/
int HAL_CFMC_EraseSector1k (int sel, unsigned long addr)
{
	HAL_CFMC_FLASH_ALL_ENABLE();

	CFMC->AR = addr>>2;

	CFMC->MR = 0x81;CFMC->MR = 0x28;
	CFMC->CFG= FMCFG_ENTRY;
	CFMC->MR = 0x00;CFMC->MR = 0x00;

// flash mode entry
	CFMC->MR = 0x5A;CFMC->MR = 0xA5;

	FMCR_PMOD_SET;
	FMCR_ERS_SET;
	FMCR_1KEN_SET; // 1KB Erase Enable
	if (!HAL_CFMC_OtpAccess((OTP_TYPE)sel, ENABLE)) {
		return (1);
	}
	FMCR_WADCK_SET;
	FMCR_HVEN_SET;

	HAL_CFMC_WAIT();

	if (!HAL_CFMC_OtpAccess((OTP_TYPE)sel, DISABLE)) {
		return (1);
	}

	CFMC->CR = 0x0;
	CFMC->MR = 0x0;

	return (0);
}

/***************************************************************************//**
* @Name       HAL_CFMC_EraseSector1k
* @brief      Erase 4KB Sector Erase of code flash memory area
* @param[in]  sel is OTP_TYPE variable, It should be
*             - OPTION_NOT_USE: No access option area
*             - OPTION_USERDATA0: Enable the access of User Data 0 Option area
*             - OPTION_USERDATA1: Enable the access of User Data 1 Option area
*             - OPTION_USERDATA2: Enable the access of User Data 2 Option area
*             - OPTION_READPROT: Enable the access of Read Protection Option area
*             - OPTION_BANKSWAP: Enable the access of Memory Bank Swap Option area
* @param[in]  addr is sector address to erase
* @return     - 0 : Success
*             - 1 : Error
*******************************************************************************/
int HAL_CFMC_EraseSector4k (int sel, unsigned long addr)
{
	HAL_CFMC_FLASH_ALL_ENABLE();

	CFMC->AR = addr>>2;

	CFMC->MR = 0x81;CFMC->MR = 0x28;
	CFMC->CFG= FMCFG_ENTRY;
	CFMC->MR = 0x00;CFMC->MR = 0x00;

// flash mode entry
	CFMC->MR = 0x5A;CFMC->MR = 0xA5;

	FMCR_PMOD_SET;
	FMCR_ERS_SET;
	FMCR_4KEN_SET;
	if (!HAL_CFMC_OtpAccess((OTP_TYPE)sel, ENABLE)) {
		return (1);
	}
	FMCR_WADCK_SET;
	FMCR_HVEN_SET;

	HAL_CFMC_WAIT();

	if (!HAL_CFMC_OtpAccess((OTP_TYPE)sel, DISABLE)) {
		return (1);
	}
	CFMC->CR = 0x0;
	CFMC->MR = 0x0;

	return (0);
}

/***************************************************************************//**
* @Name       HAL_CFMC_ProgramPage
* @brief      Program 512 Bytes Page(Sector) Program of code flash memory area
* @param[in]  sel is OTP_TYPE variable, It should be
*             - OPTION_NOT_USE: No access option area
*             - OPTION_USERDATA0: Enable the access of User Data 0 Option area
*             - OPTION_USERDATA1: Enable the access of User Data 1 Option area
*             - OPTION_USERDATA2: Enable the access of User Data 2 Option area
*             - OPTION_READPROT: Enable the access of Read Protection Option area
*             - OPTION_BANKSWAP: Enable the access of Memory Bank Swap Option area
* @param[in]  addr is sector address to write to code flash memory
* @param[in]  size is data buffer size to write to code flash memory (512 Bytes)
* @param[in]  *buf is data buffer pointer to be wrote data to code flash memory
* @return     - 0 : Success
*             - 1 : Error
*******************************************************************************/
int HAL_CFMC_ProgramPage (int sel, unsigned long addr, unsigned long size, unsigned char *buf)
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

	HAL_CFMC_FLASH_ALL_ENABLE();

	CFMC->MR = 0x81; CFMC->MR = 0x28;
	CFMC->CFG= FMCFG_ENTRY;
	CFMC->MR = 0x00; CFMC->MR = 0x00;

	// flash mode entry
	CFMC->MR = 0x5A; CFMC->MR = 0xA5;

	FMCR_PMOD_SET;
	if (!HAL_CFMC_OtpAccess((OTP_TYPE)sel, ENABLE)) {
		return (1);
	}

	CFMC->AR = (addr >> 2);
	FMCR_PGM_SET;

	for(i = 0 ; i < size ; i += 4) {
		CFMC->DR = *ptr++;
		FMCR_WADCK_SET;
		FMCR_HVEN_SET;

		HAL_CFMC_WAIT();

		FMCR_HVEN_CLR;
		CFMC->BUSY = 0x80; // WRBUSY flag clear
	}

//	FMCR_PGM_CLR;
	if (!HAL_CFMC_OtpAccess((OTP_TYPE)sel, DISABLE)) {
		return (1);
	}

	CFMC->CR = 0x0;
	CFMC->MR = 0x0;

	return 0;
}

/***************************************************************************//**
* @Name       HAL_CFMC_Verify
* @brief      vefify that designated code flash memory and data buffer are matched.
* @param[in]  addr is the sector address written to code flash memory.
* @param[in]  size is data buffer length
* @param[in]  * buf is a pointer to the data buffer written to the code flash memory.
* @return     - 0 : Success
*             - 1 : Error
*******************************************************************************/
unsigned long HAL_CFMC_Verify (unsigned long addr, unsigned long size, unsigned char *buf)
{
	unsigned long	i;
	unsigned long	*src;
	unsigned long	*dst;
	unsigned long	result;

	//---------------------------------------------------------------------------------
	// check address and size alignment
	//---------------------------------------------------------------------------------
	if((addr & 0x03) || (size & 0x03)) {
		return (1);
	}

	//---------------------------------------------------------------------------------
	// Verify
	//---------------------------------------------------------------------------------
	result = addr + size;

	CFMC->MR = 0x81;CFMC->MR = 0x28;
	CFMC->CFG= FMCFG_ENTRY;
	CFMC->MR = 0x00;CFMC->MR = 0x00;

	CFMC->MR = 0x5A;
	CFMC->MR = 0xA5;

//	FMCR_PVER_SET;					//PGM-level Verify

	src = (unsigned long *)buf;
	dst = (unsigned long *)addr;
	for( i=0; i<size; i+=4) {
		if( *src++ != *dst++ ) {
			result = addr + i;
			break;
		}
	}
//	FMCR_PVER_CLR;
//	FMCR_EVER_SET;					//ERASE-level Verify

	src = (unsigned long *)buf;
	dst = (unsigned long *)addr;
	for( i=0; i<size; i+=4) {
		if( *src++ != *dst++ ) {
			result = addr + i;
			break;
		}
	}
//	FMCR_EVER_CLR;

	CFMC->CR = 0x00000000;

	CFMC->MR = 0x81;CFMC->MR = 0x28;
	CFMC->CFG = FMCFG_EXIT;
	CFMC->MR = 0x00;CFMC->MR = 0x00;

	return (result);
}

/***************************************************************************//**
* @Name       HAL_CFMC_OtpAccess
* @brief      Enable the access to specific optional area of code flash memory
* @param[in]  mode is an OTP_TYPE enumerated value indicating the type
*             of the code flash memory option area. It should be,
*             - OPTION_NOT_USE: No access option area
*             - OPTION_USERDATA0: Enable the access of User Data 0 Option area
*             - OPTION_USERDATA1: Enable the access of User Data 1 Option area
*             - OPTION_USERDATA2: Enable the access of User Data 2 Option area
*             - OPTION_READPROT: Enable the access of Read Protection Option area
*             - OPTION_BANKSWAP: Enable the access of Memory Bank Swap Option area
* @param[in]  en is FunctionalState enumerated value, should be
*             - ENABLE: Enable the access to specific option area
*             - DISABLE: Disable the access to specific option area
* @return     - 0 : SUCCESS
*             - 1 : ERROR
*******************************************************************************/
Status HAL_CFMC_OtpAccess(OTP_TYPE mode, FunctionalState en) {
	if ((mode == OPTION_USERDATA0 || mode == OPTION_USERDATA1 || mode == OPTION_USERDATA2)) {
		if (en == ENABLE) {
			FMCR_IFEN_SET;
		} else {
			FMCR_IFEN_CLR;
		}
		return (SUCCESS);
	} else if (mode == OPTION_READPROT) {
		if (en == ENABLE) {
			FMCR_LOCKSEL_SET;
		} else {
			FMCR_LOCKSEL_CLR;
		}
		return (SUCCESS);
	} else if (mode == OPTION_BANKSWAP) {
		if (en == ENABLE) {
			FMCR_BANKSEL_SET;
		} else {
			FMCR_BANKSEL_CLR;
		}
		return (SUCCESS);
	} else if (mode == OPTION_NOT_USE) {
		return (SUCCESS);
	} else {
		return (ERROR);
	}
}

/***************************************************************************//**
* @Name       HAL_CFMC_SetflashAccesstiming
* @brief      Configure flash access timing (wait-time), It is recommanded to be set below 20MHz (50ns)
* @param[in]  SystemCoreClock uses global variable value that is updated when the system core clock is set.
* @param[in]  The waiting time sets the access time of the code flash memory
*             - 0 to 6: 0-wait to 6-wait
* @param[in]  autoAdjust is a function that automatically corrects
*             when the 'WaitTime' inserted by the user exceeds the recommended value. It should be,
*             - ENABLE: Enable auto adjust function of waittime
*             - DISABLE: Disable auto adjust function of waittime
* @return     None
*******************************************************************************/
void HAL_CFMC_SetflashAccesstiming(uint32_t SystemCoreClock, uint16_t WaitTime, FunctionalState autoAdjust)
{
	static uint32_t FlashAccessTiming = 0;
    Bool Valid = FALSE;

	// wait setting order 1. default wait setting -> 2. clock change -> 3. adjust wait setting/// flash memory controller
	CFMC->MR = 0x81;       // after changing 0x81 -> 0x28 in MR reg, flash access timing will be able to be set.
	CFMC->MR = 0x28;       // enter flash access timing changing mode

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

	CFMC->CFG = (0x7858 << CFMC_CFG_WTIDKY_Pos) | (WaitTime << CFMC_CFG_WAIT_Pos);
	                              // flash access time cannot overflow 20MHz.
	                              // ex) if MCLK=48MHz,
	                              //       48/(0+1) = 48 (can't set no wait)
	                              //       48/(1+1) = 24 (1 wait is not ok)
	                              //       48/(2+1) = 16 (2 wait is ok)
	                              // so, 2 wait is possible.
	CFMC->MR = 0;	      // exit flash access timing --> normal mode
}

/***************************************************************************//**
* @Name       HAL_CFMC_GetFlashAccessTiming
* @brief      Returns current the access timing (Hz) of code flash memory.
* @return     FlashAccessTiming (Hz)
*******************************************************************************/
uint32_t HAL_CFMC_GetFlashAccessTiming(void)
{
	static uint32_t FlashAccessTiming;
	static uint16_t WaitTime;

	WaitTime = HAL_CFMC_GetFlashWaitValue();
	FlashAccessTiming = SystemCoreClock/(uint32_t)(WaitTime + 1); // Get Flash Access Timing Value, (WaitTime+1) cycle

	return FlashAccessTiming;
}

/***************************************************************************//**
* @Name       HAL_CFMC_GetFlashWaitValue
* @brief      Returns current the wait time value of code flash memory.
* @return     Wait time
*******************************************************************************/
uint16_t HAL_CFMC_GetFlashWaitValue(void)
{
	return (((CFMC->CFG) & CFMC_CFG_WAIT_Msk) >> CFMC_CFG_WAIT_Pos); // Get Flash wait value
}

/***************************************************************************//**
* @Name       HAL_CFMC_GetBankSwapStatus
* @brief      Returns current Memory Bank Swap Status of code flash memory
* @return     Memory Bank Swap Status
*******************************************************************************/
uint32_t HAL_CFMC_GetBankSwapStatus(void) {
	return (CFMC->BSR);
}

/***************************************************************************//**
* @Name       HAL_CFMC_GetReadProtectionStatus
* @brief      Returns current Read Protection Status of code flash memory
* @return     CFMC_READPROTECT_Type enumeration values. It should be,
*             - READ_PROTECT_UNLOCK: Does NOT apply Read Protection
*             - READ_PROTECT_LV1: Read Protection Level-1
*             - READ_PROTECT_LV2: Read Protection Level-2
*             - READ_PROTECT_ERR: Error Value
*******************************************************************************/
CFMC_READPROTECT_Type HAL_CFMC_GetReadProtectionStatus(void) {
    int Reg32;

    Reg32 = ((CFMC->RPROT & (0x3 << CFMC_RPROT_LVL1_EN_Pos)) >> CFMC_RPROT_LVL1_EN_Pos);

    if (Reg32 == READ_PROTECT_UNLOCK) {
        return READ_PROTECT_UNLOCK;
    } else if (Reg32 == READ_PROTECT_LV1) {
        return READ_PROTECT_LV1;
    } else if (Reg32 == READ_PROTECT_LV2) {
        return READ_PROTECT_LV2;
    } else {
        return READ_PROTECT_ERR;
    }
}

/***************************************************************************//**
* @Name       HAL_CFMC_SetPassword
* @brief      Set a password to be used in read protection mode.
* @param[in]  passwd: 4-byte(32-bit) value.
* @return     None
*******************************************************************************/
void HAL_CFMC_SetPassword(uint32_t passwd) {
    CFMC->PWPRST = passwd;
    CFMC->PWPRST = passwd;
}

/***************************************************************************//**
* @Name       HAL_CFMC_InsertPassword
* @brief      Enter a password.
* @param[in]  passwd: 4-byte(32-bit) value. Enter a 4-byte password. To release read protection,
*             it must match the set password. Otherwise, you will need to restart the system and enter your password again.
* @return     None
*******************************************************************************/
void HAL_CFMC_InsertPassword(uint32_t passwd) {
    CFMC->PWIN = passwd;
    CFMC->PWIN = passwd;
}

/***************************************************************************//**
* @Name       HAL_CFMC_CheckPasswordStatus
* @brief      Checks whether the entered password matches.
* @return     TRUE: password is matched. (Read protection is temporarily released.)
*             FALSE: unmatched password (It needs to restart system and enter password again.)
*******************************************************************************/
Bool HAL_CFMC_CheckPasswordStatus(void) {
    int Reg32;

    Reg32 = CFMC->RPROT;

    if ((Reg32 & CFMC_RPROT_PWMATCH_Msk) == CFMC_RPROT_PWMATCH_Msk) {
        return TRUE;
    } else {
        return FALSE;
    }
}

#endif /* _CFMC */
/* --------------------------------- End Of File ------------------------------ */
