/************************************************************************************************/
/**
* @file               common.c
* @author             MCU Ecosystem Development Team
* @brief              通用函数或本外设相关的配置实现函数。
*
*
**************************************************************************************************
* @attention
* Copyright (c) CEC Huada Electronic Design Co.,Ltd. All rights reserved.
*
**************************************************************************************************
*/

/*------------------------------------------includes--------------------------------------------*/
#include "common.h"

/*-------------------------------------------functions------------------------------------------*/
/**
* @brief  系统时钟配置
* @retval 无
*/
void system_clock_config(void)
{
    /* 设置Flash读访问等待时间 */
    std_flash_set_latency(FLASH_LATENCY_2CLK);

    /* 使能RCH */
    std_rcc_rch_enable();
    while(!std_rcc_get_rch_ready());
    
    /* 配置PLL相关参数，并使能 */
    std_rcc_pll_config(RCC_PLLSRC_RCH, RCC_PLLM_MUL12, RCC_PLLN_DIV2, RCC_PLL_DIV2);
    std_rcc_pll_enable();
    while(!std_rcc_get_pll_ready());
    
    std_rcc_pll_output_enable();

    /* 设置系统时钟源为PLL */
    std_rcc_set_sysclk_source(RCC_SYSCLK_SRC_PLLCLK);
    while(std_rcc_get_sysclk_source() != RCC_SYSCLK_SRC_STATUS_PLLCLK);

    /* 设置AHB分频因子 */
    std_rcc_set_ahbdiv(RCC_HCLK_DIV1);
    
    /* 设置APB1、APB2分频因子 */
    std_rcc_set_apb1div(RCC_PCLK1_DIV1);
    std_rcc_set_apb2div(RCC_PCLK2_DIV1);
    
    SystemCoreClockUpdate();
}

/**
* @brief  LPTIM2初始化配置
* @retval 无
*/
void lptim2_init(void)
{
    /* 使能LXTAL时钟并等待时钟稳定 */
    std_rcc_apb1_clk_enable(RCC_PERIPH_CLK_PMU);
    std_pmu_vaon_write_enable();
    std_rcc_lxtal_drive_mode_config(RCC_LXTAL_DRIVE_MODE_ENHANCE);
    std_rcc_lxtal_drive_config(RCC_LXTAL_DRIVE_LEVEL2);
    std_rcc_lxtal_enable(RCC_LXTAL_ON);
    while(!std_rcc_get_lxtal_ready());
    
    /* 选择LPTIM2的时钟源为LXTAL */
    std_rcc_set_lptim2clk_source(RCC_LPTIM2_ASYNC_CLK_SRC_LXTAL);
    
    /* 使能LPTIM2时钟 */
    std_rcc_apb1_clk_enable(RCC_PERIPH_CLK_LPTIM2);
    
    /* 设置LPTIM2预分频器 */
    std_lptim_set_prescaler(LPTIM2, LPTIM_PRESCALER_DIV1);
    
    /* 使能软件触发计数 */
    std_lptim_set_software_trig(LPTIM2);
}

/**
* @brief  NVIC初始化
* @retval 无
*/
void nvic_init(void)
{
    NVIC_SetPriority(LPTIM2_IRQn, NVIC_PRIO_0);
    NVIC_EnableIRQ(LPTIM2_IRQn);
}

