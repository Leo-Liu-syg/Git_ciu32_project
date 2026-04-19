/************************************************************************************************/
/**
* @file               common.h
* @author             MCU Ecosystem Development Team
* @brief              COMMON头文件。
*
*
**************************************************************************************************
* @attention
* Copyright (c) CEC Huada Electronic Design Co.,Ltd. All rights reserved.
*
**************************************************************************************************
*/

/* 避免头文件重复引用 */
#ifndef COMMON_H
#define COMMON_H

#ifdef __cplusplus
extern "C" {
#endif

/*------------------------------------------includes--------------------------------------------*/
#include "ciu32l051_std.h"
/*-------------------------------------------define---------------------------------------------*/
/* 智能卡IO相关定义 */
#define SC_USART_PORT                 GPIOA
#define SC_USART_SCIO_PIN             GPIO_PIN_11
#define SC_USART_CLK_PIN              GPIO_PIN_10
    
#define SC_GPIO_PORT                  GPIOB
#define SC_GPIO_RST_PIN               GPIO_PIN_10
#define SC_GPIO_DETECT_PIN            GPIO_PIN_11
#define SC_DETECT_EXTI_PORT           EXTI_GPIOB
#define SC_DETECT_EXTI_PIN            EXTI_LINE_GPIO_PIN11  

#define SC_GPIO_POWER_CTRL_PORT       GPIOD
#define SC_GPIO_POWER_CTRL_PIN        GPIO_PIN_2    
    

/* 智能卡配置相关定义 */    
#define SC_CLK_PRESCALER_DIV_12       (0x00000006U)
#define SC_CLKOUT_FREQUECY            (4000000U)
#define SC_BAUDRATE                   ((uint32_t)(SC_CLKOUT_FREQUECY/372))
#define SC_GUARDTIME_DEFAULT          (2U)
#define SC_RETRY_COUNT                (4U)
/*-------------------------------------------functions------------------------------------------*/
void system_clock_config(void);
void gpio_init(void);
void smartcard_io_init(void);
void usart_smartcard_init(void);
void exti_init(void);

#ifdef __cplusplus
}
#endif

#endif /* COMMON_H */

