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
* @brief  GPIO初始化配置
* @retval 无
*/
void gpio_init(void)
{
    std_gpio_init_t lptim_gpio_config = {0};
    
    std_rcc_gpio_clk_enable(RCC_PERIPH_CLK_GPIOC);
    
    /* 初始化LPTIM1_OUT GPIO PC0 */
    lptim_gpio_config.pin = GPIO_PIN_0;
    lptim_gpio_config.mode = GPIO_MODE_ALTERNATE;
    lptim_gpio_config.pull = GPIO_NOPULL;
    lptim_gpio_config.output_type = GPIO_OUTPUT_PUSHPULL;
    lptim_gpio_config.alternate = GPIO_AF5_LPTIM1;
    std_gpio_init(GPIOC, &lptim_gpio_config);
}


/**
* @brief  LPTIM1初始化配置
* @retval 无
*/
void lptim1_init(void)
{
    std_lptim_init_t lptim_config = {0};
    
    /* 使能LPTIM1时钟 */
    std_rcc_apb1_clk_enable(RCC_PERIPH_CLK_LPTIM1);
    
    /* LPTIM1初始化配置 */
    lptim_config.clock_source = LPTIM_CLK_SRC_INT;
    lptim_config.prescaler = LPTIM_PRESCALER_DIV1;
    lptim_config.count_source = LPTIM_COUNTER_CLK_INT;
    lptim_config.out_polarity = LPTIM_OUTPUT_POLARITY_REGULAR; 
    std_lptim_init(LPTIM1, &lptim_config);
    
    /* 使能软件触发计数 */
    std_lptim_set_software_trig(LPTIM1);
}



