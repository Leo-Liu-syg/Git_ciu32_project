/************************************************************************************************/
/**
* @file               pmu_bsp.c
* @author             MCU Ecosystem Development Team
* @brief              PMU BSP驱动函数，实现RTC_OUT1输出功能配置。
*
*
**************************************************************************************************
* @attention
* Copyright (c) CEC Huada Electronic Design Co.,Ltd. All rights reserved.
*
**************************************************************************************************
*/

/*------------------------------------------includes--------------------------------------------*/
#include "pmu_bsp.h"

/*-------------------------------------------functions------------------------------------------*/
/**
* @brief  配置RTC时钟输出信号
* @retval 无
*/
void bsp_rtc_out_config(void)
{
    /* 关闭RTC寄存器写保护 */
    std_rtc_write_protection_disable();

    /* 选择RTC时钟输出信号 */
    std_rtc_out_config(RTC_OUTPUT_APRE_256HZ);
    
    /* 使能RTC OUT信号输出 */
    std_rtc_output_enable();
    
    /* RTC寄存器写保护使能 */
    std_rtc_write_protection_enable();
}

