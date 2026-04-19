/************************************************************************************************/
/**
* @file               lpuart_bsp.h
* @author             MCU Ecosystem Development Team
* @brief              LPUART BSP头文件。
*                           
*                     
**************************************************************************************************
* @attention
* Copyright (c) CEC Huada Electronic Design Co.,Ltd. All rights reserved.
*
**************************************************************************************************
*/

/* 避免头文件重复引用 */
#ifndef LPUART_BSP_H
#define LPUART_BSP_H

#ifdef __cplusplus
extern "C" {
#endif

/*------------------------------------------includes--------------------------------------------*/
#include "ciu32l051_std.h"
    
    
/*-------------------------------------------functions------------------------------------------*/
void LPUART1_IRQHandler(void);    
void bsp_lpuart_wakeup_config(void);
void bsp_pmu_lowpower_config(void);


#ifdef __cplusplus
}
#endif

#endif /* LPUART_BSP_H */

