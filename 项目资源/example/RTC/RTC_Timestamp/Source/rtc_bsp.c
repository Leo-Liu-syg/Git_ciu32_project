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
#include <stdio.h>
#include "rtc_bsp.h"

/*------------------------------------------functions-------------------------------------------*/
/**
* @brief TAMP中断服务函数
* @retval 无
*/
void RTC_TAMP_IRQHandler(void)
{
    /* TAMP IN中断处理流程 */
    if(std_tamp_get_interrupt_enable(TAMP_INTERRUPT_TAMP_IN) 
    && std_tamp_get_interrupt_flag(TAMP_INTERRUPT_FLAG_TAMP_IN))
    {
        /* 清除外部入侵检测（外部引脚）的标志 */
        std_tamp_clear_flag(TAMP_CLEAR_FLAG_TAMP_IN);
        
        g_timestamp_flag = 1;
    }
}

/**
* @brief  RTC日期时间配置
* @retval 无
*/
void bsp_rtc_datetime_config(void)
{
    std_rtc_time_t rtc_time = {0};
    std_rtc_date_t rtc_date = {0};
    std_status_t status = STD_ERR;
    
    /* RTC日期（BCD码）初始化  */
    rtc_date.weekday = RTC_WEEKDAY_TUESDAY;
    rtc_date.month = RTC_MONTH_OCTOBER;
    rtc_date.day = 0x24;
    rtc_date.year = 0x23;

    status = std_rtc_date_init(&rtc_date);
    while (status != STD_OK)
    {
        /* RTC日期初始化失败处理代码 */
    }

    /* RTC时间（BCD码）初始化 */
    rtc_time.hours = 0x10;
    rtc_time.minutes = 0x05;
    rtc_time.seconds = 0x05;

    status = std_rtc_time_init(&rtc_time);
    while (status != STD_OK)
    {
        /* RTC时间初始化失败处理代码 */
    }
}

/**
* @brief  外部引脚入侵检测TAMP IN配置
* @retval 无
*/
void bsp_rtc_tamp_in_config(void)
{
    /* 关闭RTC寄存器写保护 */
    std_rtc_write_protection_disable();

    /* 关闭RTC_OUT信号输出 */
    std_rtc_output_disable();
    
    /* RTC寄存器写保护使能 */
    std_rtc_write_protection_enable();

    /* 禁止外部入侵检测 */
    std_tamp_disable(TAMP_SOURCE_TAMP_IN);
    
    /* 设置入侵检测触发方式 */
    std_tamp_set_trigger(TAMP_TRIGGER_FALLING_EDGE);
        
    /* 设置入侵检测引脚数字滤波参数 */
    std_tamp_set_filter(TAMP_FILTER_DISABLE);
        
    /* 使能入侵检测引脚上拉电阻 */
    std_tamp_pullup_enable();
    
    /* 使能外部入侵检测中断 */
    std_tamp_interrupt_enable(TAMP_INTERRUPT_TAMP_IN);
    
    /* 使能外部入侵检测 */
    std_tamp_enable(TAMP_SOURCE_TAMP_IN);
    
    /* 配置中断优先级 */
    NVIC_SetPriority(RTC_TAMP_IRQn, NVIC_PRIO_0); 
    /* 使能中断 */
    NVIC_EnableIRQ(RTC_TAMP_IRQn); 
}

/**
* @brief  获取时间戳日期与时间
* @param  timestamp_date 输出时间戳日期信息
* @param  timestamp_time 输出时间戳时间信息
* @retval 无
*/
void bsp_rtc_get_timestamp(uint8_t *timestamp_date, uint8_t *timestamp_time)
{
    /* 等待时间戳标志TSF置1 */
    while(!std_rtc_get_flag(RTC_FLAG_TIMESTAMP));
    
    /* 读取时间戳时间 */
    timestamp_time[0] = std_rtc_timestamp_get_hour();
    timestamp_time[1] = std_rtc_timestamp_get_minute();
    timestamp_time[2] = std_rtc_timestamp_get_second();

    /* 读取时间戳日期 */
    timestamp_date[0] = std_rtc_timestamp_get_day();
    timestamp_date[1] = std_rtc_timestamp_get_month();
    
    /* 清除时间戳标志 */
    std_rtc_clear_flag(RTC_CLEAR_TIMESTAMP);
}
