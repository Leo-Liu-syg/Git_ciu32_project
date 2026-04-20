/************************************************************************************************/
/**
* @file               rtc_bsp.c
* @author             MCU Ecosystem Development Team
* @brief              RTC BSP驱动函数，实现RTC功能配置。
*
*
**************************************************************************************************
* @attention
* Copyright (c) CEC Huada Electronic Design Co.,Ltd. All rights reserved.
*
**************************************************************************************************
*/

/*------------------------------------------includes--------------------------------------------*/
#include "rtc_bsp.h"

/*------------------------------------------functions-------------------------------------------*/
/**
* @brief RTC中断服务函数
* @retval 无
*/
void RTC_TAMP_IRQHandler(void)
{
    /* 校准周期中断处理流程 */
    if(std_rtc_get_calib_interrupt_enable() 
    && std_rtc_get_interrupt_flag(RTC_INTERRUPT_FLAG_CALIBRATION))
    {
        /* 清除校准周期标志 */
        std_rtc_clear_flag(RTC_CLEAR_CALIBRATION);
        g_calib_flag = 1;
    }
}

/**
* @brief  RTC_OUT输出信号配置
* @retval 无
*/
void bsp_rtc_output_config(void)
{
    /* 配置RTC_OUT输出1Hz */
    std_rtc_out_config(RTC_OUTPUT_SPRE_1HZ);
    
    /* 配置RTC_OUT输出使能 */
    std_rtc_output_enable();
}

/**
* @brief  配置RTC时钟数字平滑校准参数
* @param  calib_plus_pulses 插入时钟脉冲控制
*             @arg RTC_SMOOTH_CALIB_PLUS_PULSES_RESET  
*             @arg RTC_SMOOTH_CALIB_PLUS_PULSES_SET   
* @param  calib_minus_pulses 屏蔽时钟脉冲个数
* @retval 无
*/
void bsp_rtc_calib_config(uint32_t calib_plus_pulses, uint32_t calib_minus_pulses)
{
    /* 开启校准周期中断 */ 
    std_rtc_calib_interrupt_enable();

    /* 配置数字平滑校准参数 */
    while(std_rtc_cal_get_status());
    std_rtc_cal_config(calib_plus_pulses, calib_minus_pulses);
    
    /* 配置中断优先级 */
    NVIC_SetPriority(RTC_TAMP_IRQn, NVIC_PRIO_0); 
    /* 使能中断 */
    NVIC_EnableIRQ(RTC_TAMP_IRQn);
}
