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
/* LED1定义 */
#define LED1_GPIO_PORT            GPIOB
#define LED1_PIN                  GPIO_PIN_14
    
#define LED1_ON()                 std_gpio_reset_pin(LED1_GPIO_PORT, LED1_PIN)
#define LED1_OFF()                std_gpio_set_pin(LED1_GPIO_PORT, LED1_PIN)
#define LED1_TOGGLE()             std_gpio_toggle_pin(LED1_GPIO_PORT, LED1_PIN)

/* 6BIT DAC输出电压：不分压 */
#define SYSCFG_6BIT_DAC_VOLTAGE_DIV_NONE      (63U)

/*------------------------------------------functions-------------------------------------------*/
void system_clock_config(void);
void vbgr_init(void);
void syscfg_6bit_dac_init(void);
void comp_init(void);
void gpio_init(void);

#ifdef __cplusplus
}
#endif

#endif /* COMMON_H */

