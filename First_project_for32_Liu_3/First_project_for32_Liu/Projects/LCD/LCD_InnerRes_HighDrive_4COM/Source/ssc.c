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
#include "ssc.h"

/**
 * @brief 系统时钟配置函数（核心！给单片机设置工作频率）
 * @note  最终目标：把内部时钟(RCH)通过PLL倍频，作为系统主时钟
 */
void system_clock_config(void)
{
    std_flash_set_latency(FLASH_LATENCY_2CLK);

    std_rcc_rch_enable();
    while(!std_rcc_get_rch_ready());

    std_rcc_pll_config(RCC_PLLSRC_RCH, RCC_PLLM_MUL12, RCC_PLLN_DIV2, RCC_PLL_DIV2);
    std_rcc_pll_enable();
    
    while(!std_rcc_get_pll_ready());

    std_rcc_pll_output_enable();    


    std_rcc_set_sysclk_source(RCC_SYSCLK_SRC_PLLCLK);
    
    while(std_rcc_get_sysclk_source() != RCC_SYSCLK_SRC_STATUS_PLLCLK);

    std_rcc_set_ahbdiv(RCC_HCLK_DIV1);
    std_rcc_set_apb1div(RCC_PCLK1_DIV1);
    std_rcc_set_apb2div(RCC_PCLK2_DIV1);

    SystemCoreClockUpdate();
}

