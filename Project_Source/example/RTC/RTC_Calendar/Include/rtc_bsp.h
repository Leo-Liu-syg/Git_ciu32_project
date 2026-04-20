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

/*-------------------------------------------define---------------------------------------------*/
#define TIMES_TO_GET_TR  (0x03)       /* 两次读日历不一致后再次循环次数 */

/*------------------------------------------functions-------------------------------------------*/
void bsp_rtc_datetime_config(void);
void bsp_rtc_get_calendar(uint8_t *rtcdate, uint8_t *rtctime);
void bsp_rtc_output_config(void);

#ifdef __cplusplus
}
#endif

#endif /* RTC_BSP_H */

