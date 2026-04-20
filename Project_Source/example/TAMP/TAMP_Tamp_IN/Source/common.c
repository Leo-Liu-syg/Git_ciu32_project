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
* @brief  TAMP初始化
* @retval 无
*/
void tamp_init(void)
{
    /* 使能PMU时钟，开启VCORE_AON写使能 */
    std_rcc_apb1_clk_enable(RCC_PERIPH_CLK_PMU);
    std_pmu_vaon_write_enable();

    /* RTC APB时钟使能 */
    std_rcc_apb1_clk_enable(RCC_PERIPH_CLK_RTC);
    
    /* 使能RCL时钟 */
    std_rcc_rcl_enable();
    while(!std_rcc_get_rcl_ready());
    
    /* 选择RCL作为RTC时钟源
        
       注意: 选择RTC时钟源后，以下情况可重新配置RTC时钟源：
             - VCORE_AON域复位
    */
    std_rcc_set_rtcclk_source(RCC_RTC_ASYNC_CLK_SRC_RCL);
    
    /* RTC外设时钟使能 */
    std_rcc_rtc_enable();
        
    /* 关闭RTC寄存器写保护 */
    std_rtc_write_protection_disable();

    /* 设置TAMP_IN引脚输入 */
    std_rtc_output_disable();
    
    /* RTC寄存器写保护使能 */
    std_rtc_write_protection_enable();
}

