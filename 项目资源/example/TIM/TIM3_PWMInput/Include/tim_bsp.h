/************************************************************************************************/
/**
* @file               tim_bsp.h
* @author             MCU Ecosystem Development Team
* @brief              TIM BSP头文件
*                           
*
**************************************************************************************************
* @attention
* Copyright (c) CEC Huada Electronic Design Co.,Ltd. All rights reserved.
*
**************************************************************************************************
*/


/* 避免头文件重复引用 */
#ifndef TIM_BSP_H
#define TIM_BSP_H

#ifdef __cplusplus
extern "C" {
#endif

/*------------------------------------------includes--------------------------------------------*/
#include "ciu32l051_std.h"


/*-------------------------------------------functions------------------------------------------*/
void TIM3_IRQHandler(void);
void bsp_tim3_capture_start(void);
void bsp_rcc_mco_config(void);

#ifdef __cplusplus
}
#endif

#endif /* TIM_BSP_H */
