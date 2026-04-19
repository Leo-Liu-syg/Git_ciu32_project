/************************************************************************************************/
/**
* @file               lptim_bsp.h
* @author             MCU Ecosystem Development Team
* @brief              LPTIM BSP头文件。
*
*
**************************************************************************************************
* @attention
* Copyright (c) CEC Huada Electronic Design Co.,Ltd. All rights reserved.
*
**************************************************************************************************
*/

/* 避免头文件重复引用 */
#ifndef LPTIM_BSP_H
#define LPTIM_BSP_H

#ifdef __cplusplus
extern "C" {
#endif

/*------------------------------------------includes--------------------------------------------*/
#include "ciu32l051_std.h"
#include "common.h" 

/*-------------------------------------------functions------------------------------------------*/
void LPTIM1_IRQHandler(void);
void bsp_lptim_encoder_start(void);

#ifdef __cplusplus
}
#endif

#endif /* LPTIM_BSP_H */

