
  .syntax unified
  .cpu cortex-m0
  .fpu softvfp
  .thumb

.global g_pfnVectors
.global Default_Handler

/* start address for the initialization values of the .data section.
defined in linker script */
.word _sidata
/* start address for the .data section. defined in linker script */
.word _sdata
/* end address for the .data section. defined in linker script */
.word _edata
/* start address for the .bss section. defined in linker script */
.word _sbss
/* end address for the .bss section. defined in linker script */
.word _ebss

  .section .text.Reset_Handler
  .weak Reset_Handler
  .type Reset_Handler, %function
Reset_Handler:
  ldr   r0, =_estack
  mov   sp, r0          /* set stack pointer */

/* Copy the data segment initializers from flash to SRAM */
  ldr r0, =_sdata
  ldr r1, =_edata
  ldr r2, =_sidata
  movs r3, #0
  b LoopCopyDataInit

CopyDataInit:
  ldr r4, [r2, r3]
  str r4, [r0, r3]
  adds r3, r3, #4

LoopCopyDataInit:
  adds r4, r0, r3
  cmp r4, r1
  bcc CopyDataInit

/* Zero fill the bss segment. */
  ldr r2, =_sbss
  ldr r4, =_ebss
  movs r3, #0
  b LoopFillZerobss

FillZerobss:
  str  r3, [r2]
  adds r2, r2, #4

LoopFillZerobss:
  cmp r2, r4
  bcc FillZerobss

/* Call the clock system intitialization function.*/
  //bl  SystemInit
/* Call static constructors */
  bl __libc_init_array
/* Call the application's entry point.*/
  bl main

LoopForever:
    b LoopForever


.size Reset_Handler, .-Reset_Handler

/**
 * @brief  This is the code that gets called when the processor receives an
 *         unexpected interrupt.  This simply enters an infinite loop, preserving
 *         the system state for examination by a debugger.
 *
 * @param  None
 * @retval : None
*/
    .section .text.Default_Handler,"ax",%progbits
Default_Handler:
Infinite_Loop:
  b Infinite_Loop
  .size Default_Handler, .-Default_Handler
/******************************************************************************
*
* The minimal vector table for a Cortex M0.  Note that the proper constructs
* must be placed on this to ensure that it ends up at physical address
* 0x0000.0000.
*
******************************************************************************/
   .section .isr_vector,"a",%progbits
  .type g_pfnVectors, %object
  .size g_pfnVectors, .-g_pfnVectors


g_pfnVectors:
  .word  _estack
  .word  Reset_Handler
  .word  NMI_Handler
  .word  HardFault_Handler
  .word  0
  .word  0
  .word  0
  .word  0
  .word  0
  .word  0
  .word  0
  .word  SVC_Handler
  .word  0
  .word  0
  .word  PendSV_Handler
  .word  SysTick_Handler

  .word  LVI_IRQHandler                   /* Window WatchDog              */
  .word  SYSCLKFAIL_IRQHandler             /* PVD and VDDIO2 through EXTI Line detect */
  .word  WDT_IRQHandler                    /* RTC through the EXTI line    */
  .word  GPIOAB_IRQHandler                  /* FLASH                        */
  .word  GPIOCD_IRQHandler                /* RCC and CRS                  */
  .word  GPIOE_IRQHandler                /* EXTI Line 0 and 1            */
  .word  GPIOF_IRQHandler                /* EXTI Line 2 and 3            */
  .word  TIMER10_IRQHandler               /* EXTI Line 4 to 15            */
  .word  TIMER11_IRQHandler                    /* TSC                          */
  .word  TIMER12_IRQHandler               /* DMA1 Channel 1               */
  .word  I2C0_IRQHandler  /* DMA1 Channel 2 and 3 & DMA2 Channel 1 and 2 */
  .word  USART10_IRQHandler  /* DMA1 Channel 4 to 7 & DMA2 Channel 3 to 5 */
  .word  WT_IRQHandler              /* ADC1, COMP1 and COMP2         */
  .word  TIMER30_IRQHandler    /* TIM1 Break, Update, Trigger and Commutation */
  .word  I2C1_IRQHandler                /* TIM1 Capture Compare         */
  .word  TIMER20_IRQHandler                   /* TIM2                         */
  .word  TIMER21_IRQHandler                   /* TIM3                         */
  .word  USART11_IRQHandler               /* TIM6 and DAC                 */
  .word  ADC_IRQHandler                   /* TIM7                         */
  .word  UART0_IRQHandler                  /* TIM14                        */
  .word  UART1_IRQHandler                  /* TIM15                        */
  .word  TIMER13_IRQHandler                  /* TIM16                        */
  .word  TIMER14_IRQHandler                  /* TIM17                        */
  .word  TIMER15_IRQHandler                   /* I2C1                         */
  .word  TIMER16_IRQHandler                   /* I2C2                         */
  .word  I2C2_SPI20_IRQHandler                   /* SPI1                         */
  .word  USART12_13_SPI21_IRQHandler                   /* SPI2                         */
  .word  DAC_IRQHandler                 /* USART1                       */
  .word  TEMP_SENSOR_IRQHandler                 /* USART2                       */
  .word  0               /* USART3, USART4, USART5, USART6, USART7, USART8 */
  .word  0                /* CEC and CAN                  */
  .word  CMP_CRC_IRQHandler                /* CEC and CAN                  */

/*******************************************************************************
*
* Provide weak aliases for each Exception handler to the Default_Handler.
* As they are weak aliases, any function with the same name will override
* this definition.
*
*******************************************************************************/

  .weak      NMI_Handler
  .thumb_set NMI_Handler,Default_Handler

  .weak      HardFault_Handler
  .thumb_set HardFault_Handler,Default_Handler

  .weak      SVC_Handler
  .thumb_set SVC_Handler,Default_Handler

  .weak      PendSV_Handler
  .thumb_set PendSV_Handler,Default_Handler

  .weak      SysTick_Handler
  .thumb_set SysTick_Handler,Default_Handler

  .weak      LVI_IRQHandler
  .thumb_set LVI_IRQHandler,Default_Handler

  .weak      SYSCLKFAIL_IRQHandler
  .thumb_set SYSCLKFAIL_IRQHandler,Default_Handler

  .weak      WDT_IRQHandler
  .thumb_set WDT_IRQHandler,Default_Handler

  .weak      GPIOAB_IRQHandler
  .thumb_set GPIOAB_IRQHandler,Default_Handler

  .weak      GPIOCD_IRQHandler
  .thumb_set GPIOCD_IRQHandler,Default_Handler

  .weak      GPIOE_IRQHandler
  .thumb_set GPIOE_IRQHandler,Default_Handler

  .weak      GPIOF_IRQHandler
  .thumb_set GPIOF_IRQHandler,Default_Handler

  .weak      TIMER10_IRQHandler
  .thumb_set TIMER10_IRQHandler,Default_Handler

  .weak      TIMER11_IRQHandler
  .thumb_set TIMER11_IRQHandler,Default_Handler

  .weak      TIMER12_IRQHandler
  .thumb_set TIMER12_IRQHandler,Default_Handler

  .weak      I2C0_IRQHandler
  .thumb_set I2C0_IRQHandler,Default_Handler

  .weak      USART10_IRQHandler
  .thumb_set USART10_IRQHandler,Default_Handler

  .weak      WT_IRQHandler
  .thumb_set WT_IRQHandler,Default_Handler

  .weak      TIMER30_IRQHandler
  .thumb_set TIMER30_IRQHandler,Default_Handler

  .weak      I2C1_IRQHandler
  .thumb_set I2C1_IRQHandler,Default_Handler

  .weak      TIMER20_IRQHandler
  .thumb_set TIMER20_IRQHandler,Default_Handler

  .weak      TIMER21_IRQHandler
  .thumb_set TIMER21_IRQHandler,Default_Handler

  .weak      USART11_IRQHandler
  .thumb_set USART11_IRQHandler,Default_Handler

  .weak      ADC_IRQHandler
  .thumb_set ADC_IRQHandler,Default_Handler

  .weak      UART0_IRQHandler
  .thumb_set UART0_IRQHandler,Default_Handler

  .weak      UART1_IRQHandler
  .thumb_set UART1_IRQHandler,Default_Handler

  .weak      TIMER13_IRQHandler
  .thumb_set TIMER13_IRQHandler,Default_Handler

  .weak      TIMER14_IRQHandler
  .thumb_set TIMER14_IRQHandler,Default_Handler

  .weak      TIMER15_IRQHandler
  .thumb_set TIMER15_IRQHandler,Default_Handler

  .weak      TIMER16_IRQHandler
  .thumb_set TIMER16_IRQHandler,Default_Handler

  .weak      I2C2_SPI20_IRQHandler
  .thumb_set I2C2_SPI20_IRQHandler,Default_Handler

  .weak      USART12_13_SPI21_IRQHandler
  .thumb_set USART12_13_SPI21_IRQHandler,Default_Handler

  .weak      DAC_IRQHandler
  .thumb_set DAC_IRQHandler,Default_Handler

  .weak      TEMP_SENSOR_IRQHandler
  .thumb_set TEMP_SENSOR_IRQHandler,Default_Handler

  .weak      CMP_CRC_IRQHandler
  .thumb_set CMP_CRC_IRQHandler,Default_Handler
