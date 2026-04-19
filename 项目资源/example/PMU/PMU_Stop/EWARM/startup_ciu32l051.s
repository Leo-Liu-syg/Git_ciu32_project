;/***********************************************************************************************/
;/**
;* @file                startup_ciu32l051.s
;* @author              MCU Ecosystem Development Team
;* @brief               CMSIS Cortex-M0+ Core Device Startup File for CIU32L051. 
;*                                
;*
;************************************************************************************************* 
;* @attention
;* Copyright (c) CEC Huada Electronic Design Co.,Ltd. All rights reserved.
;*
;*************************************************************************************************
;*/

                MODULE  ?cstartup
                
                ;; Forward declaration of sections.
                SECTION CSTACK:DATA:NOROOT(3)
                
                SECTION .intvec:CODE:NOROOT(2)
                
                EXTERN  __iar_program_start
                EXTERN  SystemInit
                PUBLIC  __vector_table
                
                DATA
__vector_table
                DCD     sfe(CSTACK)
                DCD     Reset_Handler                  ; Reset Handler
                
                DCD     NMI_Handler                    ; NMI Handler
                DCD     HardFault_Handler              ; Hard Fault Handler
                DCD     0                              ; Reserved
                DCD     0                              ; Reserved
                DCD     0                              ; Reserved
                DCD     0                              ; Reserved
                DCD     0                              ; Reserved
                DCD     0                              ; Reserved
                DCD     0                              ; Reserved
                DCD     SVC_Handler                    ; SVCall Handler
                DCD     0                              ; Reserved
                DCD     0                              ; Reserved
                DCD     PendSV_Handler                 ; PendSV Handler
                DCD     SysTick_Handler                ; SysTick Handler

                ; Interrupts
                DCD     WWDG_IRQHandler                       ; Window Watchdog
                DCD     PVD_IRQHandler                        ; PVD 
                DCD     RTC_TAMP_IRQHandler                   ; RTC & TAMP 
                DCD     FLASH_IRQHandler                      ; FLASH
                DCD     RCC_IRQHandler                        ; RCC
                DCD     EXTI0_1_IRQHandler                    ; EXTI Line 0 & 1
                DCD     EXTI2_3_IRQHandler                    ; EXTI Line 2 & 3
                DCD     EXTI4_15_IRQHandler                   ; EXTI Line 4 to 15
                DCD     LCD_IRQHandler                        ; LCD
                DCD     DMA_Channel0_IRQHandler               ; DMA Channel 0
                DCD     DMA_Channel1_IRQHandler               ; DMA Channel 1
                DCD     0                                     ; Reserved
                DCD     ADC_COMP_IRQHandler                   ; ADC & COMP1/2 
                DCD     0                                     ; Reserved
                DCD     0                                     ; Reserved
                DCD     TIM3_IRQHandler                       ; TIM3
                DCD     TIM4_IRQHandler                       ; TIM4
                DCD     TIM5_IRQHandler                       ; TIM5
                DCD     TIM8_IRQHandler                       ; TIM8
                DCD     AES_IRQHandler                        ; AES
                DCD     LPTIM1_IRQHandler                     ; LPTIM1      
                DCD     I2C1_IRQHandler                       ; I2C1
                DCD     0                                     ; Reserved
                DCD     SPI1_IRQHandler                       ; SPI1
                DCD     SPI2_IRQHandler                       ; SPI2
                DCD     USART1_IRQHandler                     ; USART1
                DCD     UART2_IRQHandler                      ; UART2
                DCD     UART3_4_IRQHandler                    ; UART3 & UART4
                DCD     LPUART1_IRQHandler                    ; LPUART1
                DCD     TRNG_IRQHandler                       ; TRNG
                DCD     LPTIM2_IRQHandler                     ; LPTIM2      
                DCD     LPUART2_IRQHandler                    ; LPUART2


                THUMB
                
                PUBWEAK Reset_Handler
                SECTION .text:CODE:NOROOT:REORDER(2)
Reset_Handler
                LDR     R0, =SystemInit
                BLX     R0
                LDR     R0, =__iar_program_start
                BX      R0
                
                PUBWEAK NMI_Handler
                SECTION .text:CODE:NOROOT:REORDER(1)
NMI_Handler
                B NMI_Handler
                
                PUBWEAK HardFault_Handler
                SECTION .text:CODE:NOROOT:REORDER(1)
HardFault_Handler
                B HardFault_Handler
                
                PUBWEAK SVC_Handler
                SECTION .text:CODE:NOROOT:REORDER(1)
SVC_Handler
                B SVC_Handler
                
                PUBWEAK PendSV_Handler
                SECTION .text:CODE:NOROOT:REORDER(1)
PendSV_Handler
                B PendSV_Handler
                
                PUBWEAK SysTick_Handler
                SECTION .text:CODE:NOROOT:REORDER(1)
SysTick_Handler
                B SysTick_Handler
                
                PUBWEAK WWDG_IRQHandler
                SECTION .text:CODE:NOROOT:REORDER(1)
WWDG_IRQHandler
                B WWDG_IRQHandler
                
                PUBWEAK PVD_IRQHandler
                SECTION .text:CODE:NOROOT:REORDER(1)
PVD_IRQHandler
                B PVD_IRQHandler
                
                PUBWEAK RTC_TAMP_IRQHandler
                SECTION .text:CODE:NOROOT:REORDER(1)
RTC_TAMP_IRQHandler
                B RTC_TAMP_IRQHandler
                
                PUBWEAK FLASH_IRQHandler
                SECTION .text:CODE:NOROOT:REORDER(1)
FLASH_IRQHandler
                B FLASH_IRQHandler
                
                PUBWEAK RCC_IRQHandler
                SECTION .text:CODE:NOROOT:REORDER(1)
RCC_IRQHandler
                B RCC_IRQHandler
                
                PUBWEAK EXTI0_1_IRQHandler
                SECTION .text:CODE:NOROOT:REORDER(1)
EXTI0_1_IRQHandler
                B EXTI0_1_IRQHandler
                
                PUBWEAK EXTI2_3_IRQHandler
                SECTION .text:CODE:NOROOT:REORDER(1)
EXTI2_3_IRQHandler
                B EXTI2_3_IRQHandler
                
                PUBWEAK EXTI4_15_IRQHandler
                SECTION .text:CODE:NOROOT:REORDER(1)
EXTI4_15_IRQHandler
                B EXTI4_15_IRQHandler
                
                PUBWEAK LCD_IRQHandler
                SECTION .text:CODE:NOROOT:REORDER(1)
LCD_IRQHandler
                B LCD_IRQHandler
                
                PUBWEAK DMA_Channel0_IRQHandler
                SECTION .text:CODE:NOROOT:REORDER(1)
DMA_Channel0_IRQHandler
                B DMA_Channel0_IRQHandler
                
                PUBWEAK DMA_Channel1_IRQHandler
                SECTION .text:CODE:NOROOT:REORDER(1)
DMA_Channel1_IRQHandler
                B DMA_Channel1_IRQHandler
                              
                PUBWEAK ADC_COMP_IRQHandler
                SECTION .text:CODE:NOROOT:REORDER(1)
ADC_COMP_IRQHandler
                B ADC_COMP_IRQHandler
                
                PUBWEAK TIM3_IRQHandler
                SECTION .text:CODE:NOROOT:REORDER(1)
TIM3_IRQHandler
                B TIM3_IRQHandler
                
                PUBWEAK TIM4_IRQHandler
                SECTION .text:CODE:NOROOT:REORDER(1)
TIM4_IRQHandler
                B TIM4_IRQHandler
                
                PUBWEAK TIM5_IRQHandler
                SECTION .text:CODE:NOROOT:REORDER(1)
TIM5_IRQHandler
                B TIM5_IRQHandler
                
                PUBWEAK TIM8_IRQHandler
                SECTION .text:CODE:NOROOT:REORDER(1)
TIM8_IRQHandler
                B TIM8_IRQHandler
                
                PUBWEAK AES_IRQHandler
                SECTION .text:CODE:NOROOT:REORDER(1)
AES_IRQHandler
                B AES_IRQHandler
                
                PUBWEAK LPTIM1_IRQHandler
                SECTION .text:CODE:NOROOT:REORDER(1)
LPTIM1_IRQHandler
                B LPTIM1_IRQHandler
                
                PUBWEAK I2C1_IRQHandler
                SECTION .text:CODE:NOROOT:REORDER(1)
I2C1_IRQHandler
                B I2C1_IRQHandler
                                
                PUBWEAK SPI1_IRQHandler
                SECTION .text:CODE:NOROOT:REORDER(1)
SPI1_IRQHandler
                B SPI1_IRQHandler
                
                PUBWEAK SPI2_IRQHandler
                SECTION .text:CODE:NOROOT:REORDER(1)
SPI2_IRQHandler
                B SPI2_IRQHandler
                
                PUBWEAK USART1_IRQHandler
                SECTION .text:CODE:NOROOT:REORDER(1)
USART1_IRQHandler
                B USART1_IRQHandler
                
                PUBWEAK UART2_IRQHandler
                SECTION .text:CODE:NOROOT:REORDER(1)
UART2_IRQHandler
                B UART2_IRQHandler
                
                PUBWEAK UART3_4_IRQHandler
                SECTION .text:CODE:NOROOT:REORDER(1)
UART3_4_IRQHandler
                B UART3_4_IRQHandler
                
                PUBWEAK LPUART1_IRQHandler
                SECTION .text:CODE:NOROOT:REORDER(1)
LPUART1_IRQHandler
                B LPUART1_IRQHandler
                
                PUBWEAK TRNG_IRQHandler
                SECTION .text:CODE:NOROOT:REORDER(1)
TRNG_IRQHandler
                B TRNG_IRQHandler
                
                PUBWEAK LPTIM2_IRQHandler
                SECTION .text:CODE:NOROOT:REORDER(1)
LPTIM2_IRQHandler
                B LPTIM2_IRQHandler                
                
                PUBWEAK LPUART2_IRQHandler
                SECTION .text:CODE:NOROOT:REORDER(1)
LPUART2_IRQHandler
                B LPUART2_IRQHandler
                
                END
