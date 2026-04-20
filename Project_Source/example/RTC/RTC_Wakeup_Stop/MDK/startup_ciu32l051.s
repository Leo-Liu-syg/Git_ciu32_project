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

;/*
;//-------- <<< Use Configuration Wizard in Context Menu >>> ------------------
;*/


; <h> Stack Configuration
;   <o> Stack Size (in Bytes) <0x0-0xFFFFFFFF:8>
; </h>

Stack_Size      EQU     0x00000400

                AREA    STACK, NOINIT, READWRITE, ALIGN=3
Stack_Mem       SPACE   Stack_Size
__initial_sp


; <h> Heap Configuration
;   <o>  Heap Size (in Bytes) <0x0-0xFFFFFFFF:8>
; </h>

Heap_Size       EQU     0x00000000
                                
                AREA    HEAP, NOINIT, READWRITE, ALIGN=3
__heap_base
Heap_Mem        SPACE   Heap_Size
__heap_limit


                PRESERVE8
                THUMB


; Vector Table Mapped to Address 0 at Reset
                AREA    RESET, DATA, READONLY
                EXPORT  __vector_table
                EXPORT  __vectors_End
                EXPORT  __vectors_Size

__vector_table  DCD     __initial_sp                   ; Top of Stack
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

__vectors_End

__vectors_Size  EQU  __vectors_End - __vector_table

                AREA    |.text|, CODE, READONLY

; Reset Handler
Reset_Handler    PROC
                 EXPORT  Reset_Handler                 [WEAK]
                 IMPORT  SystemInit 
                 IMPORT  __main                  
                     
                 LDR     R0, =SystemInit
                 BLX     R0
                 LDR     R0, =__main
                 BX      R0
                 ENDP

; Dummy Exception Handlers (infinite loops which can be modified)

NMI_Handler     PROC
                EXPORT  NMI_Handler                    [WEAK]
                B       .
                ENDP
HardFault_Handler\
                PROC
                EXPORT  HardFault_Handler              [WEAK]
                B       .
                ENDP
SVC_Handler     PROC
                EXPORT  SVC_Handler                    [WEAK]
                B       .
                ENDP
PendSV_Handler  PROC
                EXPORT  PendSV_Handler                 [WEAK]
                B       .
                ENDP
SysTick_Handler PROC
                EXPORT  SysTick_Handler                [WEAK]
                B       .
                ENDP

Default_Handler PROC

                EXPORT  WWDG_IRQHandler          [WEAK]
                EXPORT  PVD_IRQHandler           [WEAK]
                EXPORT  RTC_TAMP_IRQHandler      [WEAK]
                EXPORT  FLASH_IRQHandler         [WEAK]
                EXPORT  RCC_IRQHandler           [WEAK]
                EXPORT  EXTI0_1_IRQHandler       [WEAK]
                EXPORT  EXTI2_3_IRQHandler       [WEAK]
                EXPORT  EXTI4_15_IRQHandler      [WEAK]
                EXPORT  LCD_IRQHandler           [WEAK]
                EXPORT  DMA_Channel0_IRQHandler  [WEAK]
                EXPORT  DMA_Channel1_IRQHandler  [WEAK]
                EXPORT  ADC_COMP_IRQHandler      [WEAK]
                EXPORT  TIM3_IRQHandler          [WEAK]
                EXPORT  TIM4_IRQHandler          [WEAK]
                EXPORT  TIM5_IRQHandler          [WEAK]   
                EXPORT  TIM8_IRQHandler          [WEAK]
                EXPORT  AES_IRQHandler           [WEAK]                    
                EXPORT  LPTIM1_IRQHandler        [WEAK]
                EXPORT  I2C1_IRQHandler          [WEAK]
                EXPORT  SPI1_IRQHandler          [WEAK]
                EXPORT  SPI2_IRQHandler          [WEAK]   
                EXPORT  USART1_IRQHandler        [WEAK]
                EXPORT  UART2_IRQHandler         [WEAK]
                EXPORT  UART3_4_IRQHandler       [WEAK]
                EXPORT  LPUART1_IRQHandler       [WEAK]
                EXPORT  TRNG_IRQHandler          [WEAK]
                EXPORT  LPTIM2_IRQHandler        [WEAK]
                EXPORT  LPUART2_IRQHandler       [WEAK]                           

WWDG_IRQHandler        
PVD_IRQHandler         
RTC_TAMP_IRQHandler    
FLASH_IRQHandler       
RCC_IRQHandler         
EXTI0_1_IRQHandler     
EXTI2_3_IRQHandler     
EXTI4_15_IRQHandler    
LCD_IRQHandler         
DMA_Channel0_IRQHandler
DMA_Channel1_IRQHandler
ADC_COMP_IRQHandler    
TIM3_IRQHandler        
TIM4_IRQHandler        
TIM5_IRQHandler        
TIM8_IRQHandler
AES_IRQHandler        
LPTIM1_IRQHandler      
I2C1_IRQHandler        
SPI1_IRQHandler        
SPI2_IRQHandler        
USART1_IRQHandler      
UART2_IRQHandler       
UART3_4_IRQHandler     
LPUART1_IRQHandler     
TRNG_IRQHandler        
LPTIM2_IRQHandler      
LPUART2_IRQHandler     

                B       .

                ENDP

                ALIGN


; User Initial Stack & Heap

                IF      :DEF:__MICROLIB

                EXPORT  __initial_sp
                EXPORT  __heap_base
                EXPORT  __heap_limit

                ELSE

                IMPORT  __use_two_region_memory
                EXPORT  __user_initial_stackheap

__user_initial_stackheap PROC
                LDR     R0, =  Heap_Mem
                LDR     R1, =(Stack_Mem + Stack_Size)
                LDR     R2, = (Heap_Mem +  Heap_Size)
                LDR     R3, = Stack_Mem
                BX      LR
                ENDP

                ALIGN

                ENDIF


                END
