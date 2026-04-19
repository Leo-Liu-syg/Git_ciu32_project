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
    /* 2Hz周期中断处理流程 */
    if(std_rtc_get_wut_interrupt_enable(RTC_WUT_INTERRUPT_2HZ)
    && std_rtc_get_interrupt_flag(RTC_INTERRUPT_FLAG_2HZ))
    {
        /* 清除2Hz周期中断标志 */
        std_rtc_clear_flag(RTC_CLEAR_2HZ);
        g_wut_2hz_flag = 1;
    }
    
    /* 秒周期中断处理流程 */
    if(std_rtc_get_wut_interrupt_enable(RTC_WUT_INTERRUPT_SEC)
    && std_rtc_get_interrupt_flag(RTC_INTERRUPT_FLAG_SEC))
    {
        /* 清除秒周期中断标志 */
        std_rtc_clear_flag(RTC_CLEAR_SEC);
        g_wut_sec_flag = 1;
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
* @brief RTC唤醒定时器配置
* @retval 无
*/
void bsp_rtc_wakeup_config(void)
{
    /* 关闭RTC寄存器写保护 */
    std_rtc_write_protection_disable();
    
    /* 使能秒周期、2Hz周期定时中断 */
    std_rtc_wut_interrupt_enable(RTC_WUT_INTERRUPT_SEC | RTC_WUT_INTERRUPT_2HZ);
        
    /* 使能RTC寄存器写保护 */
    std_rtc_write_protection_enable();
    
    /* 配置中断优先级 */
    NVIC_SetPriority(RTC_TAMP_IRQn, NVIC_PRIO_0); 
    /* 使能中断 */
    NVIC_EnableIRQ(RTC_TAMP_IRQn);    
}

/**
* @brief  获取日历日期与时间
* @param  rtcdate 输出日期信息
* @param  rtctime 输出时间信息
* @retval 无
*/
void bsp_rtc_get_calendar(uint8_t *rtcdate, uint8_t *rtctime)
{
    uint32_t i, read_ok = 0;
    uint32_t temp_time1 = 0, temp_time2 = 0; 
    uint32_t temp_date1 = 0, temp_date2 = 0; 

    /* 读取日历 */
    for(i = 0; i < TIMES_TO_GET_TR; i++)
    {
        /* 读一次日历时间 */
        temp_time1 = std_rtc_time_get_time();
        temp_date1 = std_rtc_date_get_date();
        
        /* 再读一次日历时间 */        
        temp_time2 = std_rtc_time_get_time();
        temp_date2 = std_rtc_date_get_date();
        
        /* 连续两次读取结果比对 */
        if((temp_time1 == temp_time2) && (temp_date1 == temp_date2))
        {
            /* 两者一致，表示读取成功 */
            read_ok = 1;
            break;
        }
    }
    
    /* 时间读取正确 */
    if(read_ok)
    {
        /* 获取时间 */
        rtctime[0] = (temp_time1 >> RTC_OFFSET_HOUR) & 0xFF;
        rtctime[1] = (temp_time1 >> RTC_OFFSET_MINUTE) & 0xFF;
        rtctime[2] = temp_time1 & 0xFF;

        /* 获取日期 */
        rtcdate[0] = (temp_date1 >> RTC_OFFSET_YEAR) & 0xFF;
        rtcdate[1] = (temp_date1 >> RTC_OFFSET_MONTH) & 0xFF;
        rtcdate[2] = temp_date1 & 0xFF;
    }
    
    /* 时间读取不正确 */
    else
    {
        while(1)
        {
            /* 时间读取不正确处理代码 */
        }
    }
}

/**
* @brief  使能PMU低功耗配置
* @retval 无
*/
void bsp_pmu_lowpower_config(void)
{
    std_rcc_apb1_clk_enable(RCC_PERIPH_CLK_PMU);
    
    /* 使能超低功耗配置 */
    std_pmu_ultra_lowpower_enable();
}
