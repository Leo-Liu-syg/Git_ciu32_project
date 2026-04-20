/************************************************************************************************/
/**
* @file               rtc_bsp.h
* @author             MCU Ecosystem Development Team
* @brief              RTC BSP头文件
*                           
*
**************************************************************************************************
* @attention
* Copyright (c) CEC Huada Electronic Design Co.,Ltd. All rights reserved.
*
**************************************************************************************************
*/

/* 避免头文件重复引用 */
#ifndef RTC_BSP_H
#define RTC_BSP_H

#ifdef __cplusplus
extern "C" {
#endif

/*------------------------------------------includes--------------------------------------------*/
#include "ciu32l051_std.h"

/*----------------------------------------- variables------------------------------------------*/
extern __IO uint8_t g_calib_flag;
    
/*------------------------------------------functions-------------------------------------------*/
void RTC_TAMP_IRQHandler(void);
void bsp_rtc_output_config(void);
void bsp_rtc_calib_config(uint32_t calib_plus_pulses, uint32_t calib_minus_pulses);
    
#ifdef __cplusplus
}
#endif

#endif /* RTC_BSP_H */

