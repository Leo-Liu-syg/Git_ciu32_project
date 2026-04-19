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
#include <stdio.h>
#include "ciu32l051_std.h"
    
/*-------------------------------------------define---------------------------------------------*/
#define LED_RED_GPIO_PORT          GPIOB
#define LED_RED_PIN                GPIO_PIN_14
    
#define LED1_ON()               std_gpio_reset_pin (LED_RED_GPIO_PORT, LED_RED_PIN)
#define LED1_OFF()              std_gpio_set_pin   (LED_RED_GPIO_PORT, LED_RED_PIN)
#define LED_RED_FLIP()           std_gpio_toggle_pin(LED_RED_GPIO_PORT, LED_RED_PIN)

/*-------------------------------------------functions------------------------------------------*/
void system_clock_config(void); 
void rtc_clock_config(void);
void led_init(void);
void gpio_init(void);
void uart_init(void);

#ifdef __cplusplus
}
#endif

#endif /* COMMON_H */

