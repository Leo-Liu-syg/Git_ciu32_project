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

/*--------------------------------------------define--------------------------------------------*/
/* LED1定义 */
#define LED_RED_GPIO_PORT                     GPIOB
#define LED_RED_PIN                           GPIO_PIN_14
#define LED_RED_FLIP()                      std_gpio_toggle_pin(LED_RED_GPIO_PORT, LED_RED_PIN)

/*-------------------------------------------functions------------------------------------------*/

void system_clock_config(void);
void iwdg_init(void);
void led_init(void);


#ifdef __cplusplus
}
#endif

#endif /* COMMON_H */

