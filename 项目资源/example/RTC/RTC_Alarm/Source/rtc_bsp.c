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
* @brief  RTC中断服务函数
* @retval 无
*/
void RTC_TAMP_IRQHandler(void)
{
    /* 闹钟中断处理流程 */
    if(std_rtc_get_alarm_interrupt_enable() 
    && std_rtc_get_interrupt_flag(RTC_INTERRUPT_FLAG_ALARM))
    {
        /* 清除闹钟标志 */
        std_rtc_clear_flag(RTC_CLEAR_ALARM);
        g_alarm_flag = 1;
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
    
    /* RTC日期（BCD码）初始化 */
    rtc_date.weekday = RTC_WEEKDAY_TUESDAY;
    rtc_date.month = RTC_MONTH_NOVEMBER;
    rtc_date.day = 0x11;
    rtc_date.year = 0x23;

    status = std_rtc_date_init(&rtc_date);
    while (status != STD_OK)
    {
        /* RTC日期初始化失败处理代码 */
    }

    /* RTC时间（BCD码）初始化 */
    rtc_time.hours = 0x10;
    rtc_time.minutes = 0x05;
    rtc_time.seconds = 0x55;

    status = std_rtc_time_init(&rtc_time);
    while (status != STD_OK)
    {
        /* RTC时间初始化失败处理代码 */
    }
}

/**
* @brief  RTC闹钟配置
* @retval 无
*/
void bsp_rtc_alarm_config(void)
{
    std_rtc_alarm_t rtc_alarm = {0};
    
    /* 禁止闹钟 */
    std_rtc_write_protection_disable();
    std_rtc_alarm_disable();
    std_rtc_write_protection_enable();
        
    /* 闹钟初始化 */
    rtc_alarm.time.hours   = std_rtc_convert_bin2bcd(10);
    rtc_alarm.time.minutes = std_rtc_convert_bin2bcd(6);
    rtc_alarm.time.seconds = std_rtc_convert_bin2bcd(00);
    rtc_alarm.time_mask    = RTC_ALARM_MASK_NONE;
    
    std_rtc_alarm_init(&rtc_alarm);

    /* 使能闹钟，开启闹钟中断 */
    std_rtc_write_protection_disable();
    std_rtc_alarm_interrupt_enable();
    std_rtc_alarm_enable();
    std_rtc_write_protection_enable();
    
    /* 配置中断优先级 */
    NVIC_SetPriority(RTC_TAMP_IRQn, NVIC_PRIO_0); 
    /* 使能中断 */
    NVIC_EnableIRQ(RTC_TAMP_IRQn);
}

