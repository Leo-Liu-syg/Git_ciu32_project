/************************************************************************************************/
/**
* @file               pmu_bsp.h
* @author             MCU Ecosystem Development Team
* @brief              PMU BSP头文件。
*                           
*
**************************************************************************************************
* @attention
* Copyright (c) CEC Huada Electronic Design Co.,Ltd. All rights reserved.
*
**************************************************************************************************
*/


/* 避免头文件重复引用 */
#ifndef PMU_BSP_H
#define PMU_BSP_H

#ifdef __cplusplus
extern "C" {
#endif

/*------------------------------------------includes--------------------------------------------*/
#include "ciu32l051_std.h"
    

/*-------------------------------------------define---------------------------------------------*/
#define TIMES_TO_GET_TR              (0x03)              /* 两次读时间不一致后再次循环次数 */

    
/*-------------------------------------------functions------------------------------------------*/
void EXTI4_15_IRQHandler(void);
void bsp_pmu_lowpower_config(void);

void bsp_rtc_show_time(uint8_t *showtime);
    

#ifdef __cplusplus
}
#endif

#endif /* PMU_BSP_H */

