/************************************************************************************************/
/**
* @file               lptim_bsp.c
* @author             MCU Ecosystem Development Team
* @brief              LPTIM BSP驱动函数，实现LPTIM外部脉冲计数等功能
*
*
**************************************************************************************************
* @attention
* Copyright (c) CEC Huada Electronic Design Co.,Ltd. All rights reserved.
*
**************************************************************************************************
*/

/*------------------------------------------includes--------------------------------------------*/
#include "lptim_bsp.h"

/*--------------------------------------------define--------------------------------------------*/
#define PERIOD      (uint32_t)1000

/*-------------------------------------------functions------------------------------------------*/
/**
* @brief  LPTIM1中断服务程序
* @retval 无
*/
void LPTIM1_IRQHandler(void)
{
    if ((std_lptim_get_interrupt_status(LPTIM1, LPTIM_INTERRUPT_ARRM)) && (std_lptim_get_flag(LPTIM1, LPTIM_FLAG_ARRM)) )
    {
        /* 清除ARRM状态标志 */
        std_lptim_clear_flag(LPTIM1, LPTIM_CLEAR_ARRM);
        
    }
}

/**
* @brief  LPTIM1使能自动重载匹配中断后启动计数
* @retval 无
*/
void bsp_lptim_counter_start(void)
{   
    /* 使能自动重载匹配中断 */
    std_lptim_interrupt_enable(LPTIM1, LPTIM_INTERRUPT_ARRM);
    
    /* 使能LPTIM */
    std_lptim_enable(LPTIM1);
    
    /* 设置自动重载值 */
    std_lptim_set_auto_reload(LPTIM1, PERIOD);
    
    /* 启动LPTIM计数 */
    std_lptim_start_counter(LPTIM1, LPTIM_COUNT_CONTINUOUS);
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
