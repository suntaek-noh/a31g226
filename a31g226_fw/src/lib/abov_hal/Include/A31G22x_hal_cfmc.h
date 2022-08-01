/***************************************************************************//**
* @file     A31G22x_hal_cfmc.h
* @brief    Contains all macro definitions and function prototypes support
*           for CFMC(Code Flash Main Controller) driver on A31G22x
* @author   ABOV AE Team, ABOV Semiconductor Co., Ltd.
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

#ifndef _A31G22x_HAL_CFMC_H_
#define _A31G22x_HAL_CFMC_H_

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
#define	FMCFG_ENTRY				(0x78580300UL)
#define	FMCFG_EXIT				(0x78580300UL)

#define BV_BANKSEL				(0x01UL << 25)
#define	BV_LOCKSEL				(0x01UL << 24)
#define	BV_SELFPGM				(0x01UL << 23)
#define	BV_IFEN					(0x01UL << 12)
#define	BV_BBLOCK				(0x01UL << 8)
#define	BV_MAS					(0x01UL << 7)
#define	BV_4KEN					(0x01UL << 6)
#define	BV_1KEN					(0x01UL << 5)
#define	BV_PMOD					(0x01UL << 4)
#define	BV_WADCK				(0x01UL << 3)
#define	BV_PGM					(0x01UL << 2)
#define	BV_ERS					(0x01UL << 1)
#define	BV_HVEN					(0x01UL << 0)

#define	FMMR_BUSY				(CFMC->BUSY & 0x01)

#define FMCR_BANKSEL_SET		(CFMC->CR |=  BV_BANKSEL)
#define FMCR_BANKSEL_CLR		(CFMC->CR &=~ BV_BANKSEL)
#define	FMCR_LOCKSEL_SET		(CFMC->CR |=  BV_LOCKSEL)
#define	FMCR_LOCKSEL_CLR		(CFMC->CR &=~ BV_LOCKSEL)
#define	FMCR_SELFPGM_SET		(CFMC->CR |=  BV_SELFPGM)
#define	FMCR_SELFPGM_CLR		(CFMC->CR &=~ BV_SELFPGM)
#define	FMCR_IFEN_SET			(CFMC->CR |=  BV_IFEN)
#define	FMCR_IFEN_CLR			(CFMC->CR &=~ BV_IFEN)
#define	FMCR_BBLOCK_SET			(CFMC->CR |=  BV_BBLOCK)
#define	FMCR_BBLOCK_CLR			(CFMC->CR &=~ BV_BBLOCK)
#define	FMCR_MAS_SET			(CFMC->CR |=  BV_MAS)
#define	FMCR_MAS_CLR			(CFMC->CR &=~ BV_MAS)
#define	FMCR_4KEN_SET			(CFMC->CR |=  BV_4KEN)
#define	FMCR_4KEN_CLR			(CFMC->CR &=~ BV_4KEN)
#define	FMCR_1KEN_SET			(CFMC->CR |=  BV_1KEN)
#define	FMCR_1KEN_CLR			(CFMC->CR &=~ BV_1KEN)
#define	FMCR_PMOD_SET			(CFMC->CR |=  BV_PMOD)
#define	FMCR_PMOD_CLR			(CFMC->CR &=~ BV_PMOD)
#define	FMCR_WADCK_SET			(CFMC->CR |=  BV_WADCK)
#define	FMCR_WADCK_CLR			(CFMC->CR &=~ BV_WADCK)
#define	FMCR_PGM_SET			(CFMC->CR |=  BV_PGM)
#define	FMCR_PGM_CLR			(CFMC->CR &=~ BV_PGM)
#define	FMCR_ERS_SET			(CFMC->CR |=  BV_ERS)
#define	FMCR_ERS_CLR			(CFMC->CR &=~ BV_ERS)
#define	FMCR_HVEN_SET			(CFMC->CR |=  BV_HVEN)
#define	FMCR_HVEN_CLR			(CFMC->CR &=~ BV_HVEN)

#define CFMC_OPTION_USERDATA0_ADDR (0x0F000000UL)
#define CFMC_OPTION_USERDATA1_ADDR (0x0F000200UL)
#define CFMC_OPTION_USERDATA2_ADDR (0x0F000400UL)
#define CFMC_OPTION_READRPOT_ADDR  (0x0F010000UL)
#define CFMC_OPTION_BANKSWAP_ADDR  (0x0F010800UL)

#define CFMC_OPTION_MIRROR_OFFSET  (0x30000000UL)

#define CFMC_HWID_REG_ADDR  (0x40000184UL)
#define CFMC_HWID_MASK      (0xFFFFFFFFUL)
#define CFMC_SIZE_REG_ADDR  (0x40000188UL)
#define CFMC_SIZE_MASK      (0xFFUL << 0)

#define CFMC_WAIT_MAX_INDEX (0x06UL)

#define CFMC_ABWPROT_KEY   (0xAAB << CFMC_ABWPROT_AB_KEY_Pos)
#define CFMC_NBWPROT_KEY   (0x55B << CFMC_NBWPROT_NB_KEY_Pos)

#define CFMC_READPROT_STATUS_MSK      (3UL << CFMC_)
#define CFMC_READPROT_STATUS_POS      (16)

/*******************************************************************************
* Public Typedef
*******************************************************************************/
typedef enum {
	OPTION_NOT_USE = 0,
	OPTION_USERDATA0,
	OPTION_USERDATA1,
	OPTION_USERDATA2,
	OPTION_READPROT,
	OPTION_BANKSWAP
} OTP_TYPE;

typedef enum {
    READ_PROTECT_UNLOCK = 0,
    READ_PROTECT_LV1 = 1,
    READ_PROTECT_LV2 = 2,
    READ_PROTECT_LV1PW = 3,
    READ_PROTECT_LV2PW = 4,
    READ_PROTECT_ERR = -1,
} CFMC_READPROTECT_Type;

/*******************************************************************************
* Exported Public Function
*******************************************************************************/

int HAL_CFMC_EraseChip (void);
int HAL_CFMC_EraseSector (int sel, unsigned long addr);
int HAL_CFMC_ProgramPage (int sel,unsigned long addr, unsigned long size, unsigned char *buf);
unsigned long HAL_CFMC_Verify (unsigned long addr, unsigned long size, unsigned char *buf);

int HAL_CFMC_EraseSector1k (int sel, unsigned long addr);
int HAL_CFMC_EraseSector4k (int sel, unsigned long addr);

Status HAL_CFMC_OtpAccess(OTP_TYPE mode, FunctionalState en);
void HAL_CFMC_SetflashAccesstiming(uint32_t SystemCoreClock, uint16_t WaitTime, FunctionalState autoAdjust);
uint32_t HAL_CFMC_GetFlashAccessTiming(void);
uint16_t HAL_CFMC_GetFlashWaitValue(void);
int HAL_CFMC_EraseChipBBlock (void);

void HAL_CFMC_SET_OTP(unsigned long otp);
int HAL_CFMC_Init (unsigned long addr, unsigned long clk, unsigned long func);
int HAL_CFMC_UnInit (unsigned long func);
void HAL_CFMC_WAIT(void);
void HAL_CFMC_FLASH_ALL_ENABLE(void);

uint32_t HAL_CFMC_GetBankSwapStatus(void);

CFMC_READPROTECT_Type HAL_CFMC_GetReadProtectionStatus(void);
void HAL_CFMC_SetPassword(uint32_t passwd);
void HAL_CFMC_InsertPassword(uint32_t passwd);
Bool HAL_CFMC_CheckPasswordStatus(void);

#ifdef __cplusplus
}
#endif

#endif /* _A31G22x_DFMC_H_ */
/* --------------------------------- End Of File ------------------------------ */
