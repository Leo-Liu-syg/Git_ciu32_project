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

/*-------------------------------------------includes-------------------------------------------*/
#include "ciu32l051_std.h"

/*-------------------------------------------define---------------------------------------------*/
/* LED1定义 */
#define LED1_GPIO_PORT            GPIOB
#define LED1_PIN                  GPIO_PIN_14
    
#define LED1_ON()                 std_gpio_reset_pin(LED1_GPIO_PORT, LED1_PIN)
#define LED1_OFF()                std_gpio_set_pin(LED1_GPIO_PORT, LED1_PIN)
#define LED1_TOGGLE()             std_gpio_toggle_pin(LED1_GPIO_PORT, LED1_PIN)

/*-------------------------------------------functions------------------------------------------*/
void system_clock_config(void);
void vbgr_init(void);
void comp1_init(void);
void gpio_init(void);
void exti_init(void);
void nvic_init(void);

#ifdef __cplusplus
}
#endif

#endif /* COMMON_H */

