/************************************************************************************************/
/**
* @file               exti_bsp.h
* @author             MCU Ecosystem Development Team
* @brief              EXTI BSP头文件
*                           
*
**************************************************************************************************
* @attention
* Copyright (c) CEC Huada Electronic Design Co.,Ltd. All rights reserved.
*
**************************************************************************************************
*/

/* 避免头文件重复引用 */
#ifndef EXTI_BSP_H
#define EXTI_BSP_H

#ifdef __cplusplus
extern "C" {
#endif

/*------------------------------------------includes--------------------------------------------*/
#include "ciu32l051_std.h"

/*------------------------------------------variables-------------------------------------------*/
extern uint32_t g_exti_gpio_flag;

/*------------------------------------------functions-------------------------------------------*/
void EXTI4_15_IRQHandler(void);

#ifdef __cplusplus
}
#endif

#endif /* EXTI_BSP_H */

