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
#include <stdio.h>


/*-------------------------------------------define---------------------------------------------*/
#define LED_GPIO_PORT                     GPIOB
#define LED_RED_PIN                          GPIO_PIN_14
    
#define LED1_ON()                         std_gpio_reset_pin(LED_GPIO_PORT, LED_RED_PIN)    
#define LED1_OFF()                        std_gpio_set_pin(LED_GPIO_PORT, LED_RED_PIN)

#define BUTTON_USER_GPIO                  GPIOC
#define BUTTON_USER_PIN                   GPIO_PIN_13

#define BUTTON_USER_EXTI_PORT             EXTI_GPIOC
#define BUTTON_USER_EXTI_LINE             EXTI_LINE_GPIO_PIN13

    
/*-------------------------------------------functions------------------------------------------*/
void system_clock_config(void);
void rtc_clock_config(void);
void exti_init(void);
void led_init(void);    

void gpio_init(void);
void uart_init(void);



#ifdef __cplusplus
}
#endif

#endif /* COMMON_H */

