/************************************************************************************************/
/**
* @file               tamp_bsp.h
* @author             MCU Ecosystem Development Team
* @brief              TAMP BSP头文件
*                           
*
*************************************************************************************************
* @attention
* Copyright (c) CEC Huada Electronic Design Co.,Ltd. All rights reserved.
*
*************************************************************************************************/

/* 避免头文件重复引用 */
#ifndef TAMP_BSP_H
#define TAMP_BSP_H

#ifdef __cplusplus
extern "C" {
#endif

/*------------------------------------------includes--------------------------------------------*/
#include "ciu32l051_std.h"

/*--------------------------------------------variables-----------------------------------------*/
extern __IO uint8_t g_tampin_flag;

/*------------------------------------------functions-------------------------------------------*/
void RTC_TAMP_IRQHandler(void);
void bsp_tamp_tampin_config(void);

#ifdef __cplusplus
}
#endif

#endif /* TAMP_BSP_H */

