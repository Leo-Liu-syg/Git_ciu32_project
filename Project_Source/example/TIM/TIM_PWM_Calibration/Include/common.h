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
#define TIM3_ARR_VALUE                (0xFFFF)
#define TIM5_ARR_VALUE                (0xFFFF)

#define TIM3_PSC_VALUE                (0x00)
#define TIM5_PSC_VALUE                (0x01)
    
    
/*-------------------------------------------functions------------------------------------------*/
void system_clock_config(void);
void tim3_init(void);
void tim5_init(void);
void gpio_init(void);  
void nvic_init(void);    


#ifdef __cplusplus
}
#endif

#endif /* COMMON_H */

