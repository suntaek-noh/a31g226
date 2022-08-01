/***************************************************************************//**
* @file     A31G22x_hal_dfmc.h
* @brief    Contains all macro definitions and function prototypes support
*           for DFMC(Data Flash Main Controller) driver on A31G22x
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

#ifndef _A31G22x_HAL_DFMC_H_
#define _A31G22x_HAL_DFMC_H_

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
#define	DFMCFG_ENTRY			(0x78580300UL)
#define	DFMCFG_EXIT				(0x78580300UL)

#define	DBV_LOCKSEL				(0x01UL << 24)
#define	DBV_BYTEPGM				(0x01UL << 23) // Byte-PGM Data Flash Only
#define	DBV_WPGMEN				(0x01UL << 16) // Word-PGM Data Flash Only
#define	DBV_IFEN				(0x01UL << 12)
#define	DBV_BBLOCK				(0x01UL << 8)
#define	DBV_MAS					(0x01UL << 7)
#define	DBV_4KEN				(0x01UL << 6)
#define	DBV_1KEN				(0x01UL << 5)
#define	DBV_PMOD				(0x01UL << 4)
#define	DBV_WADCK				(0x01UL << 3)
#define	DBV_PGM					(0x01UL << 2)
#define	DBV_ERS					(0x01UL << 1)
#define	DBV_HVEN					(0x01UL << 0)

#define	DFMMR_BUSY				(DFMC->BUSY & 0x01)

#define	DFMCR_LOCKSEL_SET		(DFMC->CR |=  DBV_LOCKSEL)
#define	DFMCR_LOCKSEL_CLR		(DFMC->CR &=~ DBV_LOCKSEL)
#define	DFMCR_BYTEPGM_SET		(DFMC->CR |=  DBV_BYTEPGM)
#define	DFMCR_BYTEPGM_CLR		(DFMC->CR &=~ DBV_BYTEPGM)
#define	DFMCR_WORDPGM_SET		(DFMC->CR |=  DBV_WPGMEN)
#define	DFMCR_WORDPGM_CLR		(DFMC->CR &=~ DBV_WPGMEN)
#define	DFMCR_IFEN_SET			(DFMC->CR |=  DBV_IFEN)
#define	DFMCR_IFEN_CLR			(DFMC->CR &=~ DBV_IFEN)
#define	DFMCR_BBLOCK_SET		(DFMC->CR |=  DBV_BBLOCK)
#define	DFMCR_BBLOCK_CLR		(DFMC->CR &=~ DBV_BBLOCK)
#define	DFMCR_MAS_SET			(DFMC->CR |=  DBV_MAS)
#define	DFMCR_MAS_CLR			(DFMC->CR &=~ DBV_MAS)
#define	DFMCR_4KEN_SET			(DFMC->CR |=  DBV_4KEN)
#define	DFMCR_4KEN_CLR			(DFMC->CR &=~ DBV_4KEN)
#define	DFMCR_1KEN_SET			(DFMC->CR |=  DBV_1KEN)
#define	DFMCR_1KEN_CLR			(DFMC->CR &=~ DBV_1KEN)
#define	DFMCR_PMOD_SET			(DFMC->CR |=  DBV_PMOD)
#define	DFMCR_PMOD_CLR			(DFMC->CR &=~ DBV_PMOD)
#define	DFMCR_WADCK_SET			(DFMC->CR |=  DBV_WADCK)
#define	DFMCR_WADCK_CLR			(DFMC->CR &=~ DBV_WADCK)
#define	DFMCR_PGM_SET			(DFMC->CR |=  DBV_PGM)
#define	DFMCR_PGM_CLR			(DFMC->CR &=~ DBV_PGM)
#define	DFMCR_ERS_SET			(DFMC->CR |=  DBV_ERS)
#define	DFMCR_ERS_CLR			(DFMC->CR &=~ DBV_ERS)
#define	DFMCR_HVEN_SET			(DFMC->CR |=  DBV_HVEN)
#define	DFMCR_HVEN_CLR			(DFMC->CR &=~ DBV_HVEN)

#define DFMC_OPTION_READRPOT_ADDR  (0x0E010000UL)
#define DFMC_OPTION_USERDATA0_ADDR   (0x0E016000UL)
#define DFMC_OPTION_USERDATA1_ADDR   (0x0E016200UL)
#define DFMC_OPTION_USERDATA2_ADDR   (0x0E016400UL)

#define DFMC_WAIT_MAX_INDEX		(0x06UL)

typedef enum {
	DATA_OPTION_NOT_USE = 0,
	DATA_OPTION_READPROT,
    DATA_OPTION,
} DOTP_TYPE;

/*******************************************************************************
* Public Typedef
*******************************************************************************/

/*******************************************************************************
* Exported Public Function
*******************************************************************************/
int HAL_DFMC_EraseChip(void);
int HAL_DFMC_EraseSector(int sel, unsigned long addr);
int HAL_DFMC_EraseSector1k(int sel, unsigned long addr);
int HAL_DFMC_EraseSector4k(int sel, unsigned long addr);

int HAL_DFMC_ByteProgram(int sel, unsigned long addr, unsigned long size, unsigned char *buf);  // 1byte write program
int HAL_DFMC_WordProgram(int sel, unsigned long addr, unsigned long size, unsigned char *buf);  // 4bytes write (1 word) program

unsigned long HAL_DFMC_Verify(unsigned long addr, unsigned long size, unsigned char *buf);

Status HAL_DFMC_OptionAccess(DOTP_TYPE mode, FunctionalState en);

void HAL_DFMC_SetflashAccesstiming(uint32_t SystemCoreClock, uint16_t WaitTime, FunctionalState autoAdjust);
uint32_t HAL_DFMC_GetFlashAccessTiming(void);
uint16_t HAL_DFMC_GetFlashWaitValue(void);

int HAL_DFMC_Init (unsigned long addr, unsigned long clk, unsigned long func);
int HAL_DFMC_UnInit (unsigned long func);
void HAL_DFMC_WAIT(void);
void HAL_DFMC_SetWriteProtection(FunctionalState wprot, int segmentNum);
void HAL_DFMC_FLASH_ALL_ENABLE(void);

void HAL_DFMC_SetPassword(uint32_t passwd);
void HAL_DFMC_InsertPassword(uint32_t passwd);
Bool HAL_DFMC_CheckPasswordStatus(void);

#ifdef __cplusplus
}
#endif

#endif /* _A31G22x_DFMC_H_ */
/* --------------------------------- End Of File ------------------------------ */
