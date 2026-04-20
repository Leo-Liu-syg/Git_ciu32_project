/************************************************************************************************/
/**
* @file               pmu_bsp.c
* @author             MCU Ecosystem Development Team
* @brief              PMU BSP驱动函数，实现PMU+RTC配置，并进入Stop的功能配置。
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
* @brief  GPIO_PIN_4到GPIO_PIN_15中断服务函数
* @retval 无
*/
void EXTI4_15_IRQHandler(void)
{
    /* 检测EXTI Line 中断 */
    if (std_exti_get_pending_status(EXTI_LINE_GPIO_PIN13))
    {
        std_exti_clear_pending(EXTI_LINE_GPIO_PIN13);
    }
    
    /* 用户根据EXTI的中断事件，结合当前的状态实现中断处理函数 */    
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

/**
* @brief  显示当前时间
* @param  showtime 输出时间信息
* @retval 无
*/
void bsp_rtc_show_time(uint8_t *showtime)
{
    uint32_t i;
    uint32_t read_ok = 0;
    uint32_t tmp1 =0 , tmp2 = 0;

    /* 读取时间 */
    for(i = 0; i < TIMES_TO_GET_TR; i++)
    {    
        /* 连续读两次时间信息 */
        tmp1 = std_rtc_time_get_time();
        tmp2 = std_rtc_time_get_time();
        
        /* 两次结果比对 */
        if(tmp1 == tmp2)
        {
            /* 数据一致，表示读取成功 */
            read_ok = 1;
            break;
        }
    }
    
    if(read_ok)
    {
        /* 获取时间 */        
        showtime[0] = (uint8_t)((tmp1 >> RTC_OFFSET_HOUR) & 0xFF);
        showtime[1] = (uint8_t)((tmp1 >> RTC_OFFSET_MINUTE) & 0xFF);
        showtime[2] = (uint8_t)(tmp1 & 0xFF);
    }
    else
    {
        while(1)
        {
            /* 时间读取不正确处理代码 */
        }
    }
}
