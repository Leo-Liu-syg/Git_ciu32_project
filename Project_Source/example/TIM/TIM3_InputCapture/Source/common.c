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


/*--------------------------------------------define--------------------------------------------*/
#define TIM_ARR_VALUE        (0xFFFFU)


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
* @brief  GPIO初始化
* @retval 无
*/
void gpio_init(void)
{
    std_gpio_init_t tim3_gpio_init = {0};
    
    /* GPIOB时钟使能 */
    std_rcc_gpio_clk_enable(RCC_PERIPH_CLK_GPIOB);
    
    /* TIM3 GPIO 配置
    PB5     ------> TIM3_CH2 */
    tim3_gpio_init.pin = GPIO_PIN_5;
    tim3_gpio_init.mode = GPIO_MODE_ALTERNATE;
    tim3_gpio_init.pull = GPIO_PULLDOWN;
    tim3_gpio_init.alternate = GPIO_AF2_TIM3;
    std_gpio_init(GPIOB, &tim3_gpio_init); 
}


/**
* @brief  TIM3初始化
* @retval 无
*/
void tim3_init(void)
{
    std_tim_basic_init_t basic_init_struct = {0};
    std_tim_input_capture_init_t input_capture_struct = {0};
    
    /* TIM3时钟使能 */
    std_rcc_apb1_clk_enable(RCC_PERIPH_CLK_TIM3);
    
    /* TIM3基本定时器配置 */
    basic_init_struct.counter_mode = TIM_COUNTER_MODE_UP;
    basic_init_struct.period = TIM_ARR_VALUE;
    basic_init_struct.clock_div = TIM_CLOCK_DTS_DIV1;
    basic_init_struct.auto_reload_preload = TIM_AUTORELOAD_PRE_DISABLE;
    std_tim_init(TIM3, &basic_init_struct);
    
    /* 配置TI2FP2映射到IC2上，且上升沿有效 */
    input_capture_struct.input_capture_pol = TIM_INPUT_POL_RISING;
    input_capture_struct.input_capture_sel = TIM_CCM1_CC1S_DIRECTTI;
    input_capture_struct.input_capture_prescaler = TIM_INPUT_CAPTURE_PSC_DIV1;
    std_tim_input_capture_init(TIM3, &input_capture_struct, TIM_CHANNEL_2);
}

/**
* @brief  NVIC初始化
* @retval 无
*/
void nvic_init(void)
{
    /* NVIC初始化 */
    NVIC_SetPriority(TIM3_IRQn, NVIC_PRIO_0);
    NVIC_EnableIRQ(TIM3_IRQn);
}


