/************************************************************************************************/
/**
* @file               lptim_bsp.c
* @author             MCU Ecosystem Development Team
* @brief              LPTIM BSP驱动函数，实现LPTIM超时唤醒低功耗等功能
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
#define PERIOD          (uint32_t)(65535)
#define TIMEOUT         (uint32_t)(PERIOD >> 1)

/*-------------------------------------------functions------------------------------------------*/
/**
* @brief  LPTIM1使能超时功能
* @retval 无
*/
void bsp_lptim_timeout_enable(void)
{
    /* 使能超时功能 */
    std_lptim_timeout_enable(LPTIM1);
    
    /* 使能LPTIM */
    std_lptim_enable(LPTIM1);
    
    /* 加载周期值至ARR寄存器 */
    std_lptim_set_auto_reload(LPTIM1, PERIOD);
    
    /* 加载比较值至CMP寄存器 */
    std_lptim_set_compare(LPTIM1, TIMEOUT);
    
    /* 计数器启动连续计数 */
    std_lptim_start_counter(LPTIM1, LPTIM_COUNT_CONTINUOUS);
    
}

/**
* @brief  LPTIM1禁止超时功能
* @retval 无
*/
void bsp_lptim_timeout_disable(void)
{
    /* 禁止LPTIM */
    std_lptim_disable(LPTIM1);
    
    /* 禁止超时功能 */
    std_lptim_timeout_disable(LPTIM1);
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
