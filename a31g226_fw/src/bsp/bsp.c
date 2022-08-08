/*
 * bsp.c
 *
 *  Created on: 2022. 7. 31.
 *      Author: sunta
 */

#include "bsp.h"


static volatile uint32_t systick_counter = 0;
extern uint32_t __isr_vector_addr;

extern void SystemClock_Config (void);


static void Port_Init(void);

void SysTick_Handler(void)
{
  systick_counter++;
}

void bspInit(void)
{

  Port_Init();


  SCB->VTOR = (uint32_t)&__isr_vector_addr;
  //SystemInit();
  /* Configure the system clock to HSE 8 MHz */
  SystemClock_Config();       //48MHz

  // Enable Interrupt for Systick Timer
  NVIC_SetPriority (SysTick_IRQn, 1);
  // 1msec interrupt
  SysTick_Config(SystemCoreClock/1000);     //1ms interrupt

  /* Enable IRQ Interrupts */
    __enable_irq();
}

void delay(uint32_t ms)
{
  uint32_t pre_time = systick_counter;

  while(systick_counter - pre_time < ms);
}

uint32_t millis(void)
{
  return systick_counter;
}

void Port_Init(void)
{
    // Peripheral Enable Register 1  0:Disable 1:Enable
    SCU->PER1 |= 0x00UL
            | (0x01UL << SCU_PER1_GPIOF_Pos)   // GPIO F
            | (0x01UL << SCU_PER1_GPIOE_Pos)   // GPIO E
            | (0x01UL << SCU_PER1_GPIOD_Pos)   // GPIO D
            | (0x01UL << SCU_PER1_GPIOC_Pos)   // GPIO C
            | (0x01UL << SCU_PER1_GPIOB_Pos)   // GPIO B
            | (0x01UL << SCU_PER1_GPIOA_Pos);  // GPIO A

    // Peripheral Clock Enable Register 1 0:Disable 1:Enable
    SCU->PCER1 |= 0x00UL
            | (0x01UL << SCU_PCER1_GPIOF_Pos)   // GPIO F
            | (0x01UL << SCU_PCER1_GPIOE_Pos)   // GPIO E
            | (0x01UL << SCU_PCER1_GPIOD_Pos)   // GPIO D
            | (0x01UL << SCU_PCER1_GPIOC_Pos)   // GPIO C
            | (0x01UL << SCU_PCER1_GPIOB_Pos)   // GPIO B
            | (0x01UL << SCU_PCER1_GPIOA_Pos);  // GPIO A

    if (!HAL_GPIO_GetPortEnStatus()) {
        PORT_ACCESS_EN();  // enable writing permittion of ALL PCU register
    } else {
        ;
    }

    //--------------------------------------------------------------
    //    PORT INIT
    //        PA PB PC PD PE PF
    //--------------------------------------------------------------
    // PORT - A
    PA->MOD = 0x00UL  // 0 : Input Mode 1 : Output Mode 2 : Alternative function mode
            | (0x01UL << PORT_MOD_MODE11_Pos)  // P11
            | (0x01UL << PORT_MOD_MODE10_Pos)  // P10
            | (0x01UL << PORT_MOD_MODE9_Pos)   // P9
            | (0x01UL << PORT_MOD_MODE8_Pos)   // P8
            | (0x01UL << PORT_MOD_MODE7_Pos)   // P7
            | (0x01UL << PORT_MOD_MODE6_Pos)   // P6
            | (0x01UL << PORT_MOD_MODE5_Pos)   // P5
            | (0x01UL << PORT_MOD_MODE4_Pos)   // P4
            | (0x01UL << PORT_MOD_MODE3_Pos)   // P3
            | (0x01UL << PORT_MOD_MODE2_Pos)   // P2
            | (0x01UL << PORT_MOD_MODE1_Pos)   // P1
            | (0x01UL << PORT_MOD_MODE0_Pos);  // P0

    PA->TYP = 0x00UL  // 0 : Push-pull Output 1 : Open-drain Output
            | (0x00UL << PORT_TYP_TYP11_Pos)  // P11
            | (0x00UL << PORT_TYP_TYP10_Pos)  // P10
            | (0x00UL << PORT_TYP_TYP9_Pos)   // P9
            | (0x00UL << PORT_TYP_TYP8_Pos)   // P8
            | (0x00UL << PORT_TYP_TYP7_Pos)   // P7
            | (0x00UL << PORT_TYP_TYP6_Pos)   // P6
            | (0x00UL << PORT_TYP_TYP5_Pos)   // P5
            | (0x00UL << PORT_TYP_TYP4_Pos)   // P4
            | (0x00UL << PORT_TYP_TYP3_Pos)   // P3
            | (0x00UL << PORT_TYP_TYP2_Pos)   // P2
            | (0x00UL << PORT_TYP_TYP1_Pos)   // P1
            | (0x00UL << PORT_TYP_TYP0_Pos);  // P0

    PA->AFSR1 = 0x00UL
            | (0x00UL << PORT_AFSR1_AFSR7_Pos)   // P7  - 0 :      1 :          2 :            3 : AN7/CREF0      4 :
            | (0x00UL << PORT_AFSR1_AFSR6_Pos)   // P6  - 0 :      1 : T11O     2 : T11C       3 : AN6/CREF1/DAO  4 :
            | (0x00UL << PORT_AFSR1_AFSR5_Pos)   // P5  - 0 :      1 : T12O     2 : T12C       3 : AN5/CP1A       4 :
            | (0x00UL << PORT_AFSR1_AFSR4_Pos)   // P4  - 0 :      1 :          2 :            3 : AN4/CP1B       4 :
            | (0x00UL << PORT_AFSR1_AFSR3_Pos)   // P3  - 0 :      1 :          2 :            3 : AN3/CP1C       4 :
            | (0x00UL << PORT_AFSR1_AFSR2_Pos)   // P2  - 0 :      1 : EC12     2 :            3 : AN2/CP0        4 :
            | (0x00UL << PORT_AFSR1_AFSR1_Pos)   // P1  - 0 :      1 : SCL1     2 :            3 : AN1            4 :
            | (0x00UL << PORT_AFSR1_AFSR0_Pos);  // P0  - 0 :      1 : SDA1     2 :            3 : AN0            4 :

    PA->AFSR2 = 0x00UL
            | (0x00UL << PORT_AFSR2_AFSR11_Pos)  // P11 - 0 :      1 :          2 :            3 : AN14           4 :
            | (0x00UL << PORT_AFSR2_AFSR10_Pos)  // P10 - 0 :      1 :          2 :            3 : AN13           4 :
            | (0x00UL << PORT_AFSR2_AFSR9_Pos)   // P9  - 0 :      1 :          2 :            3 : AN12           4 :
            | (0x00UL << PORT_AFSR2_AFSR8_Pos);  // P8  - 0 :      1 :          2 :            3 : AN11           4 :

    PA->PUPD = 0x00UL  // 0 : Disable Pull-up/down 1 : Enable Pull-up 2 : Enable Pull-down
            | (0x00UL << PORT_PUPD_PUPD11_Pos)  // P11
            | (0x00UL << PORT_PUPD_PUPD10_Pos)  // P10
            | (0x00UL << PORT_PUPD_PUPD9_Pos)   // P9
            | (0x00UL << PORT_PUPD_PUPD8_Pos)   // P8
            | (0x00UL << PORT_PUPD_PUPD7_Pos)   // P7
            | (0x00UL << PORT_PUPD_PUPD6_Pos)   // P6
            | (0x00UL << PORT_PUPD_PUPD5_Pos)   // P5
            | (0x00UL << PORT_PUPD_PUPD4_Pos)   // P4
            | (0x00UL << PORT_PUPD_PUPD3_Pos)   // P3
            | (0x00UL << PORT_PUPD_PUPD2_Pos)   // P2
            | (0x00UL << PORT_PUPD_PUPD1_Pos)   // P1
            | (0x00UL << PORT_PUPD_PUPD0_Pos);  // P0

    PA->OUTDR = 0x00UL  // 0 : Output Low 1 : Output High
            | (0x00UL << PORT_OUTDR_OUTDR11_Pos)  // P11
            | (0x00UL << PORT_OUTDR_OUTDR10_Pos)  // P10
            | (0x00UL << PORT_OUTDR_OUTDR9_Pos)   // P9
            | (0x00UL << PORT_OUTDR_OUTDR8_Pos)   // P8
            | (0x00UL << PORT_OUTDR_OUTDR7_Pos)   // P7
            | (0x00UL << PORT_OUTDR_OUTDR6_Pos)   // P6
            | (0x00UL << PORT_OUTDR_OUTDR5_Pos)   // P5
            | (0x00UL << PORT_OUTDR_OUTDR4_Pos)   // P4
            | (0x00UL << PORT_OUTDR_OUTDR3_Pos)   // P3
            | (0x00UL << PORT_OUTDR_OUTDR2_Pos)   // P2
            | (0x00UL << PORT_OUTDR_OUTDR1_Pos)   // P1
            | (0x00UL << PORT_OUTDR_OUTDR0_Pos);  // P0


    // PORT - B
    PB->MOD = 0x00UL  // 0 : Input Mode 1 : Output Mode 2 : Alternative function mode
            | (0x01UL << PORT_MOD_MODE15_Pos)  // P15
            | (0x01UL << PORT_MOD_MODE14_Pos)  // P14
            | (0x01UL << PORT_MOD_MODE13_Pos)  // P13
            | (0x01UL << PORT_MOD_MODE12_Pos)  // P12
            | (0x01UL << PORT_MOD_MODE11_Pos)  // P11
            | (0x01UL << PORT_MOD_MODE10_Pos)  // P10
            | (0x01UL << PORT_MOD_MODE9_Pos)   // P9
            | (0x01UL << PORT_MOD_MODE8_Pos)   // P8
            | (0x01UL << PORT_MOD_MODE7_Pos)   // P7
            | (0x01UL << PORT_MOD_MODE6_Pos)   // P6
            | (0x02UL << PORT_MOD_MODE5_Pos)   // P5  - Alternative function mode (SWDIO)
            | (0x02UL << PORT_MOD_MODE4_Pos)   // P4  - Alternative function mode (SWCLK)
            | (0x01UL << PORT_MOD_MODE3_Pos)   // P3
            | (0x01UL << PORT_MOD_MODE2_Pos)   // P2
            | (0x01UL << PORT_MOD_MODE1_Pos)   // P1
            | (0x01UL << PORT_MOD_MODE0_Pos);  // P0

    PB->TYP = 0x00UL  // 0 : Push-pull Output 1 : Open-drain Output
            | (0x00UL << PORT_TYP_TYP15_Pos)  // P15
            | (0x00UL << PORT_TYP_TYP14_Pos)  // P14
            | (0x00UL << PORT_TYP_TYP13_Pos)  // P13
            | (0x00UL << PORT_TYP_TYP12_Pos)  // P12
            | (0x00UL << PORT_TYP_TYP11_Pos)  // P11
            | (0x00UL << PORT_TYP_TYP10_Pos)  // P10
            | (0x00UL << PORT_TYP_TYP9_Pos)   // P9
            | (0x00UL << PORT_TYP_TYP8_Pos)   // P8
            | (0x00UL << PORT_TYP_TYP7_Pos)   // P7
            | (0x00UL << PORT_TYP_TYP6_Pos)   // P6
            | (0x00UL << PORT_TYP_TYP5_Pos)   // P5
            | (0x00UL << PORT_TYP_TYP4_Pos)   // P4
            | (0x00UL << PORT_TYP_TYP3_Pos)   // P3
            | (0x00UL << PORT_TYP_TYP2_Pos)   // P2
            | (0x00UL << PORT_TYP_TYP1_Pos)   // P1
            | (0x00UL << PORT_TYP_TYP0_Pos);  // P0

    PB->AFSR1 = 0x00UL
            | (0x00UL << PORT_AFSR1_AFSR7_Pos)   // P7  - 0 :                1 : RXD1           2 :                3 : AN16           4 :
            | (0x00UL << PORT_AFSR1_AFSR6_Pos)   // P6  - 0 :                1 : TXD1           2 : EC11           3 : AN15           4 :
            | (0x02UL << PORT_AFSR1_AFSR5_Pos)   // P5  - 0 :                1 : RXD0           2 : SWDIO          3 :                4 :
            | (0x02UL << PORT_AFSR1_AFSR4_Pos)   // P4  - 0 :                1 : TXD0           2 : SWCLK          3 :                4 :
            | (0x00UL << PORT_AFSR1_AFSR3_Pos)   // P3  - 0 :                1 : BOOT           2 : SS10/SS20      3 :                4 :
            | (0x00UL << PORT_AFSR1_AFSR2_Pos)   // P2  - 0 :                1 :                2 : SCK10/SCK20    3 : AN10           4 :
            | (0x00UL << PORT_AFSR1_AFSR1_Pos)   // P1  - 0 :                1 : RXD10          2 : MISO10/MISO20  3 : AN9            4 :
            | (0x00UL << PORT_AFSR1_AFSR0_Pos);  // P0  - 0 :                1 : TXD10          2 : MOSI10/MOSI20  3 : AN8            4 :

    PB->AFSR2 = 0x00UL
            | (0x00UL << PORT_AFSR2_AFSR15_Pos)  // P15 - 0 :                1 :                2 :                3 :                4 :
            | (0x00UL << PORT_AFSR2_AFSR14_Pos)  // P14 - 0 :                1 :                2 :                3 :                4 :
            | (0x00UL << PORT_AFSR2_AFSR13_Pos)  // P13 - 0 :                1 :                2 :                3 :                4 :
            | (0x00UL << PORT_AFSR2_AFSR12_Pos)  // P12 - 0 :                1 :                2 :                3 :                4 :
            | (0x00UL << PORT_AFSR2_AFSR11_Pos)  // P11 - 0 :                1 : T15C           2 : EC16           3 : T15O           4 :
            | (0x00UL << PORT_AFSR2_AFSR10_Pos)  // P10 - 0 :                1 : T16C           2 : EC15           3 : T16O           4 :
            | (0x00UL << PORT_AFSR2_AFSR9_Pos)   // P9  - 0 :                1 : T16O           2 : T16C           3 : EC15           4 :
            | (0x00UL << PORT_AFSR2_AFSR8_Pos);  // P8  - 0 :                1 : T15O           2 : T15C           3 : EC16           4 :

    PB->PUPD = 0x00UL  // 0 : Disable Pull-up/down 1 : Enable Pull-up 2 : Enable Pull-down
            | (0x00UL << PORT_PUPD_PUPD15_Pos)  // P15
            | (0x00UL << PORT_PUPD_PUPD14_Pos)  // P14
            | (0x00UL << PORT_PUPD_PUPD13_Pos)  // P13
            | (0x00UL << PORT_PUPD_PUPD12_Pos)  // P12
            | (0x00UL << PORT_PUPD_PUPD11_Pos)  // P11
            | (0x00UL << PORT_PUPD_PUPD10_Pos)  // P10
            | (0x00UL << PORT_PUPD_PUPD9_Pos)   // P9
            | (0x00UL << PORT_PUPD_PUPD8_Pos)   // P8
            | (0x00UL << PORT_PUPD_PUPD7_Pos)   // P7
            | (0x00UL << PORT_PUPD_PUPD6_Pos)   // P6
            | (0x00UL << PORT_PUPD_PUPD5_Pos)   // P5
            | (0x00UL << PORT_PUPD_PUPD4_Pos)   // P4
            | (0x00UL << PORT_PUPD_PUPD3_Pos)   // P3
            | (0x00UL << PORT_PUPD_PUPD2_Pos)   // P2
            | (0x00UL << PORT_PUPD_PUPD1_Pos)   // P1
            | (0x00UL << PORT_PUPD_PUPD0_Pos);  // P0

    PB->OUTDR = 0x00UL  // 0 : Output Low 1 : Output High
            | (0x00UL << PORT_OUTDR_OUTDR15_Pos)  // P15
            | (0x00UL << PORT_OUTDR_OUTDR14_Pos)  // P14
            | (0x00UL << PORT_OUTDR_OUTDR13_Pos)  // P13
            | (0x00UL << PORT_OUTDR_OUTDR12_Pos)  // P12
            | (0x00UL << PORT_OUTDR_OUTDR11_Pos)  // P11
            | (0x00UL << PORT_OUTDR_OUTDR10_Pos)  // P10
            | (0x00UL << PORT_OUTDR_OUTDR9_Pos)   // P9
            | (0x00UL << PORT_OUTDR_OUTDR8_Pos)   // P8
            | (0x00UL << PORT_OUTDR_OUTDR7_Pos)   // P7
            | (0x00UL << PORT_OUTDR_OUTDR6_Pos)   // P6
            | (0x00UL << PORT_OUTDR_OUTDR5_Pos)   // P5
            | (0x00UL << PORT_OUTDR_OUTDR4_Pos)   // P4
            | (0x00UL << PORT_OUTDR_OUTDR3_Pos)   // P3
            | (0x00UL << PORT_OUTDR_OUTDR2_Pos)   // P2
            | (0x00UL << PORT_OUTDR_OUTDR1_Pos)   // P1
            | (0x00UL << PORT_OUTDR_OUTDR0_Pos);  // P0


    // PORT - C
    PC->MOD = 0x00UL  // 0 : Input Mode 1 : Output Mode 2 : Alternative function mode
            | (0x01UL << PORT_MOD_MODE12_Pos)  // P12
            | (0x01UL << PORT_MOD_MODE11_Pos)  // P11
            | (0x01UL << PORT_MOD_MODE10_Pos)  // P10
            | (0x01UL << PORT_MOD_MODE9_Pos)   // P9
            | (0x01UL << PORT_MOD_MODE8_Pos)   // P8
            | (0x01UL << PORT_MOD_MODE7_Pos)   // P7
            | (0x01UL << PORT_MOD_MODE6_Pos)   // P6
            | (0x01UL << PORT_MOD_MODE5_Pos)   // P5
            | (0x01UL << PORT_MOD_MODE4_Pos)   // P4
            | (0x01UL << PORT_MOD_MODE3_Pos)   // P3
            | (0x01UL << PORT_MOD_MODE2_Pos)   // P2
            | (0x01UL << PORT_MOD_MODE1_Pos)   // P1
            | (0x01UL << PORT_MOD_MODE0_Pos);  // P0

    PC->TYP = 0x00UL  // 0 : Push-pull Output 1 : Open-drain Output
            | (0x00UL << PORT_TYP_TYP12_Pos)  // P12
            | (0x00UL << PORT_TYP_TYP11_Pos)  // P11
            | (0x00UL << PORT_TYP_TYP10_Pos)  // P10
            | (0x00UL << PORT_TYP_TYP9_Pos)   // P9
            | (0x00UL << PORT_TYP_TYP8_Pos)   // P8
            | (0x00UL << PORT_TYP_TYP7_Pos)   // P7
            | (0x00UL << PORT_TYP_TYP6_Pos)   // P6
            | (0x00UL << PORT_TYP_TYP5_Pos)   // P5
            | (0x00UL << PORT_TYP_TYP4_Pos)   // P4
            | (0x00UL << PORT_TYP_TYP3_Pos)   // P3
            | (0x00UL << PORT_TYP_TYP2_Pos)   // P2
            | (0x00UL << PORT_TYP_TYP1_Pos)   // P1
            | (0x00UL << PORT_TYP_TYP0_Pos);  // P0

    PC->AFSR1 = 0x00UL
            | (0x00UL << PORT_AFSR1_AFSR7_Pos)   // P7  - 0 :                1 :                2 :                3 :                4 :
            | (0x00UL << PORT_AFSR1_AFSR6_Pos)   // P6  - 0 :                1 : SCL2           2 :                3 :                4 :
            | (0x00UL << PORT_AFSR1_AFSR5_Pos)   // P5  - 0 :                1 : SDA2           2 :                3 :                4 :
            | (0x00UL << PORT_AFSR1_AFSR4_Pos)   // P4  - 0 :                1 :                2 : SCK20          3 :                4 :
            | (0x00UL << PORT_AFSR1_AFSR3_Pos)   // P3  - 0 :                1 : EC21           2 : MISO20         3 :                4 :
            | (0x00UL << PORT_AFSR1_AFSR2_Pos)   // P2  - 0 :                1 : EC20           2 : MOSI20         3 :                4 :
            | (0x00UL << PORT_AFSR1_AFSR1_Pos)   // P1  - 0 :                1 : T21O           2 : T21C           3 :                4 :
            | (0x00UL << PORT_AFSR1_AFSR0_Pos);  // P0  - 0 :                1 : T20O           2 : T20C           3 : AN17           4 :

    PC->AFSR2 = 0x00UL
            | (0x00UL << PORT_AFSR2_AFSR12_Pos)  // P12 - 0 :                1 : EC11           2 :                3 :                4 :
            | (0x00UL << PORT_AFSR2_AFSR11_Pos)  // P11 - 0 :                1 : EC10           2 :                3 :                4 :
            | (0x00UL << PORT_AFSR2_AFSR10_Pos)  // P10 - 0 :                1 :                2 :                3 :                4 :
            | (0x00UL << PORT_AFSR2_AFSR9_Pos)   // P9  - 0 :                1 :                2 :                3 :                4 :
            | (0x00UL << PORT_AFSR2_AFSR8_Pos);  // P8  - 0 :                1 :                2 :                3 :                4 :

    PC->PUPD = 0x00UL  // 0 : Disable Pull-up/down 1 : Enable Pull-up 2 : Enable Pull-down
            | (0x00UL << PORT_PUPD_PUPD12_Pos)  // P12
            | (0x00UL << PORT_PUPD_PUPD11_Pos)  // P11
            | (0x00UL << PORT_PUPD_PUPD10_Pos)  // P10
            | (0x00UL << PORT_PUPD_PUPD9_Pos)   // P9
            | (0x00UL << PORT_PUPD_PUPD8_Pos)   // P8
            | (0x00UL << PORT_PUPD_PUPD7_Pos)   // P7
            | (0x00UL << PORT_PUPD_PUPD6_Pos)   // P6
            | (0x00UL << PORT_PUPD_PUPD5_Pos)   // P5
            | (0x00UL << PORT_PUPD_PUPD4_Pos)   // P4
            | (0x00UL << PORT_PUPD_PUPD3_Pos)   // P3
            | (0x00UL << PORT_PUPD_PUPD2_Pos)   // P2
            | (0x00UL << PORT_PUPD_PUPD1_Pos)   // P1
            | (0x00UL << PORT_PUPD_PUPD0_Pos);  // P0

    PC->OUTDR = 0x00UL  // 0 : Output Low 1 : Output High
            | (0x00UL << PORT_OUTDR_OUTDR12_Pos)  // P12
            | (0x00UL << PORT_OUTDR_OUTDR11_Pos)  // P11
            | (0x00UL << PORT_OUTDR_OUTDR10_Pos)  // P10
            | (0x00UL << PORT_OUTDR_OUTDR9_Pos)   // P9
            | (0x00UL << PORT_OUTDR_OUTDR8_Pos)   // P8
            | (0x00UL << PORT_OUTDR_OUTDR7_Pos)   // P7
            | (0x00UL << PORT_OUTDR_OUTDR6_Pos)   // P6
            | (0x00UL << PORT_OUTDR_OUTDR5_Pos)   // P5
            | (0x00UL << PORT_OUTDR_OUTDR4_Pos)   // P4
            | (0x00UL << PORT_OUTDR_OUTDR3_Pos)   // P3
            | (0x00UL << PORT_OUTDR_OUTDR2_Pos)   // P2
            | (0x00UL << PORT_OUTDR_OUTDR1_Pos)   // P1
            | (0x00UL << PORT_OUTDR_OUTDR0_Pos);  // P0


    // PORT - D
    PD->MOD = 0x00UL  // 0 : Input Mode 1 : Output Mode 2 : Alternative function mode
            | (0x01UL << PORT_MOD_MODE5_Pos)   // P5
            | (0x01UL << PORT_MOD_MODE4_Pos)   // P4
            | (0x01UL << PORT_MOD_MODE3_Pos)   // P3
            | (0x01UL << PORT_MOD_MODE2_Pos)   // P2
            | (0x01UL << PORT_MOD_MODE1_Pos)   // P1
            | (0x01UL << PORT_MOD_MODE0_Pos);  // P0

    PD->TYP = 0x00UL  // 0 : Push-pull Output 1 : Open-drain Output
            | (0x00UL << PORT_TYP_TYP5_Pos)   // P5
            | (0x00UL << PORT_TYP_TYP4_Pos)   // P4
            | (0x00UL << PORT_TYP_TYP3_Pos)   // P3
            | (0x00UL << PORT_TYP_TYP2_Pos)   // P2
            | (0x00UL << PORT_TYP_TYP1_Pos)   // P1
            | (0x00UL << PORT_TYP_TYP0_Pos);  // P0

    PD->AFSR1 = 0x00UL
            | (0x00UL << PORT_AFSR1_AFSR5_Pos)   // P5  - 0 :        1 :                2 : SS11/SS21      3 :                4 : ICOM6
            | (0x00UL << PORT_AFSR1_AFSR4_Pos)   // P4  - 0 :        1 : BLNK           2 : SCK11/SCK21    3 :                4 : ICOM7
            | (0x00UL << PORT_AFSR1_AFSR3_Pos)   // P3  - 0 :        1 : RXD11          2 : MISO11/MISO21  3 :                4 : ICOM8
            | (0x00UL << PORT_AFSR1_AFSR2_Pos)   // P2  - 0 :        1 : TXD11          2 : MOSI11/MOSI21  3 :                4 : ICOM9
            | (0x00UL << PORT_AFSR1_AFSR1_Pos)   // P1  - 0 :        1 : SDA0           2 : EC10           3 :                4 :
            | (0x00UL << PORT_AFSR1_AFSR0_Pos);  // P0  - 0 :        1 : SCL0           2 : SS20           3 :                4 :

    PD->PUPD = 0x00UL  // 0 : Disable Pull-up/down 1 : Enable Pull-up 2 : Enable Pull-down
            | (0x00UL << PORT_PUPD_PUPD5_Pos)   // P5
            | (0x00UL << PORT_PUPD_PUPD4_Pos)   // P4
            | (0x00UL << PORT_PUPD_PUPD3_Pos)   // P3
            | (0x00UL << PORT_PUPD_PUPD2_Pos)   // P2
            | (0x00UL << PORT_PUPD_PUPD1_Pos)   // P1
            | (0x00UL << PORT_PUPD_PUPD0_Pos);  // P0

    PD->OUTDR = 0x00UL  // 0 : Output Low 1 : Output High
            | (0x00UL << PORT_OUTDR_OUTDR5_Pos)   // P5
            | (0x00UL << PORT_OUTDR_OUTDR4_Pos)   // P4
            | (0x00UL << PORT_OUTDR_OUTDR3_Pos)   // P3
            | (0x00UL << PORT_OUTDR_OUTDR2_Pos)   // P2
            | (0x00UL << PORT_OUTDR_OUTDR1_Pos)   // P1
            | (0x00UL << PORT_OUTDR_OUTDR0_Pos);  // P0


    // PORT - E
    PE->MOD = 0x00UL  // 0 : Input Mode 1 : Output Mode 2 : Alternative function mode
            | (0x01UL << PORT_MOD_MODE15_Pos)  // P15
            | (0x01UL << PORT_MOD_MODE14_Pos)  // P14
            | (0x01UL << PORT_MOD_MODE13_Pos)  // P13
            | (0x01UL << PORT_MOD_MODE12_Pos)  // P12
            | (0x01UL << PORT_MOD_MODE11_Pos)  // P11
            | (0x01UL << PORT_MOD_MODE10_Pos)  // P10
            | (0x01UL << PORT_MOD_MODE9_Pos)   // P9
            | (0x01UL << PORT_MOD_MODE8_Pos)   // P8
            | (0x01UL << PORT_MOD_MODE7_Pos)   // P7
            | (0x01UL << PORT_MOD_MODE6_Pos)   // P6
            | (0x01UL << PORT_MOD_MODE5_Pos)   // P5
            | (0x01UL << PORT_MOD_MODE4_Pos)   // P4
            | (0x01UL << PORT_MOD_MODE3_Pos)   // P3
            | (0x01UL << PORT_MOD_MODE2_Pos)   // P2
            | (0x01UL << PORT_MOD_MODE1_Pos)   // P1
            | (0x01UL << PORT_MOD_MODE0_Pos);  // P0

    PE->TYP = 0x00UL  // 0 : Push-pull Output 1 : Open-drain Output
            | (0x00UL << PORT_TYP_TYP15_Pos)   // P15
            | (0x00UL << PORT_TYP_TYP14_Pos)   // P14
            | (0x00UL << PORT_TYP_TYP13_Pos)   // P13
            | (0x00UL << PORT_TYP_TYP12_Pos)   // P12
            | (0x00UL << PORT_TYP_TYP11_Pos)   // P11
            | (0x00UL << PORT_TYP_TYP10_Pos)   // P10
            | (0x00UL << PORT_TYP_TYP9_Pos)    // P9
            | (0x00UL << PORT_TYP_TYP8_Pos)    // P8
            | (0x00UL << PORT_TYP_TYP7_Pos)    // P7
            | (0x00UL << PORT_TYP_TYP6_Pos)    // P6
            | (0x00UL << PORT_TYP_TYP5_Pos)    // P5
            | (0x00UL << PORT_TYP_TYP4_Pos)    // P4
            | (0x00UL << PORT_TYP_TYP3_Pos)    // P3
            | (0x00UL << PORT_TYP_TYP2_Pos)    // P2
            | (0x00UL << PORT_TYP_TYP1_Pos)    // P1
            | (0x00UL << PORT_TYP_TYP0_Pos);   // P0

    PE->AFSR1 = 0x00UL
            | (0x00UL << PORT_AFSR1_AFSR7_Pos)   // P7 - 0 :      1 : T11O           2 : T11C           3 :                4 : ICOM5
            | (0x00UL << PORT_AFSR1_AFSR6_Pos)   // P6 - 0 :      1 : T10O           2 : T10C           3 :                4 : ICOM4
            | (0x02UL << PORT_AFSR1_AFSR5_Pos)   // P5 - 0 :      1 : PWM30CB        2 : MOSI21         3 :                4 : ICOM3
            | (0x02UL << PORT_AFSR1_AFSR4_Pos)   // P4 - 0 :      1 : PWM30CA        2 : MISO21         3 :                4 : ICOM2
            | (0x00UL << PORT_AFSR1_AFSR3_Pos)   // P3 - 0 :      1 : PWM30BB        2 : SCK21          3 :                4 : ICOM1
            | (0x00UL << PORT_AFSR1_AFSR2_Pos)   // P2 - 0 :      1 : PWM30BA        2 : SS21           3 :                4 : ICOM0
            | (0x00UL << PORT_AFSR1_AFSR1_Pos)   // P1 - 0 :      1 : PWM30AB        2 :                3 :                4 :
            | (0x00UL << PORT_AFSR1_AFSR0_Pos);  // P0 - 0 :      1 : PWM30AA        2 : SS11           3 :                4 :

    PE->AFSR2 = 0x00UL
            | (0x00UL << PORT_AFSR2_AFSR15_Pos)  // P15 - 0 :     1 :                2 : SS12           3 :                4 :
            | (0x00UL << PORT_AFSR2_AFSR14_Pos)  // P14 - 0 :     1 :                2 : SCK12          3 :                4 :
            | (0x00UL << PORT_AFSR2_AFSR13_Pos)  // P13 - 0 :     1 : RXD12          2 : MISO12         3 :                4 :
            | (0x00UL << PORT_AFSR2_AFSR12_Pos)  // P12 - 0 :     1 : TXD12          2 : MOSI12         3 :                4 :
            | (0x00UL << PORT_AFSR2_AFSR11_Pos)  // P11 - 0 :     1 :                2 : SS13           3 :                4 :
            | (0x00UL << PORT_AFSR2_AFSR10_Pos)  // P10 - 0 :     1 :                2 : SCK13          3 :                4 :
            | (0x00UL << PORT_AFSR2_AFSR9_Pos)   // P9  - 0 :     1 : RXD13          2 : MISO13         3 :                4 :
            | (0x00UL << PORT_AFSR2_AFSR8_Pos);  // P8  - 0 :     1 : TXD13          2 : MOSI13         3 :                4 :

    PE->PUPD = 0x00UL  // 0 : Disable Pull-up/down 1 : Enable Pull-up 2 : Enable Pull-down
            | (0x00UL << PORT_PUPD_PUPD15_Pos)  // P15
            | (0x00UL << PORT_PUPD_PUPD14_Pos)  // P14
            | (0x00UL << PORT_PUPD_PUPD13_Pos)  // P13
            | (0x00UL << PORT_PUPD_PUPD12_Pos)  // P12
            | (0x00UL << PORT_PUPD_PUPD11_Pos)  // P11
            | (0x00UL << PORT_PUPD_PUPD10_Pos)  // P10
            | (0x00UL << PORT_PUPD_PUPD9_Pos)   // P9
            | (0x00UL << PORT_PUPD_PUPD8_Pos)   // P8
            | (0x00UL << PORT_PUPD_PUPD7_Pos)   // P7
            | (0x00UL << PORT_PUPD_PUPD6_Pos)   // P6
            | (0x00UL << PORT_PUPD_PUPD5_Pos)   // P5
            | (0x00UL << PORT_PUPD_PUPD4_Pos)   // P4
            | (0x00UL << PORT_PUPD_PUPD3_Pos)   // P3
            | (0x00UL << PORT_PUPD_PUPD2_Pos)   // P2
            | (0x00UL << PORT_PUPD_PUPD1_Pos)   // P1
            | (0x00UL << PORT_PUPD_PUPD0_Pos);  // P0

    PE->OUTDR = 0x00UL  // 0 : Output Low 1 : Output High
            | (0x00UL << PORT_OUTDR_OUTDR15_Pos)  // P15
            | (0x00UL << PORT_OUTDR_OUTDR14_Pos)  // P14
            | (0x00UL << PORT_OUTDR_OUTDR13_Pos)  // P13
            | (0x00UL << PORT_OUTDR_OUTDR12_Pos)  // P12
            | (0x00UL << PORT_OUTDR_OUTDR11_Pos)  // P11
            | (0x00UL << PORT_OUTDR_OUTDR10_Pos)  // P10
            | (0x00UL << PORT_OUTDR_OUTDR9_Pos)   // P9
            | (0x00UL << PORT_OUTDR_OUTDR8_Pos)   // P8
            | (0x00UL << PORT_OUTDR_OUTDR7_Pos)   // P7
            | (0x00UL << PORT_OUTDR_OUTDR6_Pos)   // P6
            | (0x00UL << PORT_OUTDR_OUTDR5_Pos)   // P5
            | (0x00UL << PORT_OUTDR_OUTDR4_Pos)   // P4
            | (0x00UL << PORT_OUTDR_OUTDR3_Pos)   // P3
            | (0x00UL << PORT_OUTDR_OUTDR2_Pos)   // P2
            | (0x00UL << PORT_OUTDR_OUTDR1_Pos)   // P1
            | (0x00UL << PORT_OUTDR_OUTDR0_Pos);  // P0


    // PORT - F
    PF->MOD = 0x00UL  // 0 : Input Mode 1 : Output Mode 2 : Alternative function mode
            | (0x01UL << PORT_MOD_MODE11_Pos)  // P11
            | (0x01UL << PORT_MOD_MODE10_Pos)  // P10
            | (0x01UL << PORT_MOD_MODE9_Pos)   // P9
            | (0x01UL << PORT_MOD_MODE8_Pos)   // P8
            | (0x01UL << PORT_MOD_MODE7_Pos)   // P7
            | (0x01UL << PORT_MOD_MODE6_Pos)   // P6
            | (0x01UL << PORT_MOD_MODE5_Pos)   // P5
            | (0x01UL << PORT_MOD_MODE4_Pos)   // P4
            | (0x01UL << PORT_MOD_MODE3_Pos)   // P3
            | (0x01UL << PORT_MOD_MODE2_Pos)   // P2
            | (0x01UL << PORT_MOD_MODE1_Pos)   // P1
            | (0x01UL << PORT_MOD_MODE0_Pos);  // P0

    PF->TYP = 0x00UL  // 0 : Push-pull Output 1 : Open-drain Output
            | (0x00UL << PORT_TYP_TYP11_Pos)  // P11
            | (0x00UL << PORT_TYP_TYP10_Pos)  // P10
            | (0x00UL << PORT_TYP_TYP9_Pos)   // P9
            | (0x00UL << PORT_TYP_TYP8_Pos)   // P8
            | (0x00UL << PORT_TYP_TYP7_Pos)   // P7
            | (0x00UL << PORT_TYP_TYP6_Pos)   // P6
            | (0x00UL << PORT_TYP_TYP5_Pos)   // P5
            | (0x00UL << PORT_TYP_TYP4_Pos)   // P4
            | (0x00UL << PORT_TYP_TYP3_Pos)   // P3
            | (0x00UL << PORT_TYP_TYP2_Pos)   // P2
            | (0x00UL << PORT_TYP_TYP1_Pos)   // P1
            | (0x00UL << PORT_TYP_TYP0_Pos);  // P0

    PF->AFSR1 = 0x00UL
            | (0x00UL << PORT_AFSR1_AFSR7_Pos)   // P7  - 0 :            1 : T30C           2 : SDA0           3 :                4 :
            | (0x00UL << PORT_AFSR1_AFSR6_Pos)   // P6  - 0 :            1 : EC30           2 : SCL0           3 :                4 :
            | (0x02UL << PORT_AFSR1_AFSR5_Pos)   // P5  - 0 :            1 : BLNK           2 :                3 :                4 :
            | (0x02UL << PORT_AFSR1_AFSR4_Pos)   // P4  - 0 :            1 : CLKO           2 :                3 :                4 :
            | (0x00UL << PORT_AFSR1_AFSR3_Pos)   // P3  - 0 :            1 : RXD1           2 :                3 : SXOUT          4 :
            | (0x00UL << PORT_AFSR1_AFSR2_Pos)   // P2  - 0 :            1 : TXD1           2 :                3 : SXIN           4 :
            | (0x00UL << PORT_AFSR1_AFSR1_Pos)   // P1  - 0 :            1 : SDA1           2 :                3 : XIN            4 :
            | (0x00UL << PORT_AFSR1_AFSR0_Pos);  // P0  - 0 :            1 : SCL1           2 :                3 : XOUT           4 :

    PF->AFSR2 = 0x00UL
            | (0x00UL << PORT_AFSR2_AFSR11_Pos)  // P11 - 0 :            1 : T14O           2 : T14C           3 :                4 :
            | (0x00UL << PORT_AFSR2_AFSR10_Pos)  // P10 - 0 :            1 : T13O           2 : T13C           3 :                4 :
            | (0x00UL << PORT_AFSR2_AFSR9_Pos)   // P9  - 0 :            1 : EC14           2 :                3 :                4 :
            | (0x00UL << PORT_AFSR2_AFSR8_Pos);  // P8  - 0 :            1 : EC13           2 :                3 :                4 :

    PF->PUPD = 0x00UL  // 0 : Disable Pull-up/down 1 : Enable Pull-up 2 : Enable Pull-down
            | (0x00UL << PORT_PUPD_PUPD11_Pos)  // P11
            | (0x00UL << PORT_PUPD_PUPD10_Pos)  // P10
            | (0x00UL << PORT_PUPD_PUPD9_Pos)   // P9
            | (0x00UL << PORT_PUPD_PUPD8_Pos)   // P8
            | (0x00UL << PORT_PUPD_PUPD7_Pos)   // P7
            | (0x00UL << PORT_PUPD_PUPD6_Pos)   // P6
            | (0x00UL << PORT_PUPD_PUPD5_Pos)   // P5
            | (0x00UL << PORT_PUPD_PUPD4_Pos)   // P4
            | (0x00UL << PORT_PUPD_PUPD3_Pos)   // P3
            | (0x00UL << PORT_PUPD_PUPD2_Pos)   // P2
            | (0x00UL << PORT_PUPD_PUPD1_Pos)   // P1
            | (0x00UL << PORT_PUPD_PUPD0_Pos);  // P0

    PF->OUTDR = 0x00UL  // 0 : Output Low 1 : Output High
            | (0x00UL << PORT_OUTDR_OUTDR11_Pos)  // P11
            | (0x00UL << PORT_OUTDR_OUTDR10_Pos)  // P10
            | (0x00UL << PORT_OUTDR_OUTDR9_Pos)   // P9
            | (0x00UL << PORT_OUTDR_OUTDR8_Pos)   // P8
            | (0x00UL << PORT_OUTDR_OUTDR7_Pos)   // P7
            | (0x00UL << PORT_OUTDR_OUTDR6_Pos)   // P6
            | (0x00UL << PORT_OUTDR_OUTDR5_Pos)   // P5
            | (0x00UL << PORT_OUTDR_OUTDR4_Pos)   // P4
            | (0x00UL << PORT_OUTDR_OUTDR3_Pos)   // P3
            | (0x00UL << PORT_OUTDR_OUTDR2_Pos)   // P2
            | (0x00UL << PORT_OUTDR_OUTDR1_Pos)   // P1
            | (0x00UL << PORT_OUTDR_OUTDR0_Pos);  // P0

}
