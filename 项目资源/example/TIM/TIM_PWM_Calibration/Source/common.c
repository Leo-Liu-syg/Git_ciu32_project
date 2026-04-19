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
* @brief  TIM3初始化
* @retval 无
*/
void tim3_init(void)
{
    std_tim_basic_init_t basic_init_struct = {0};
    std_tim_input_capture_init_t input_capture_struct = {0};
       
    /* 要更新LXTAL配置，必须解除备份域的写保护 */
    std_rcc_apb1_clk_enable(RCC_PERIPH_CLK_PMU);
    std_pmu_vaon_write_enable();
    
    /* 配置LXTAL驱动参数 */
    std_rcc_lxtal_drive_mode_config(RCC_LXTAL_DRIVE_MODE_ENHANCE);
    std_rcc_lxtal_drive_config(RCC_LXTAL_DRIVE_LEVEL2);
    
    /* 使能LXTAL，并等待LXTAL时钟稳定 */
    std_rcc_lxtal_enable(RCC_LXTAL_ON);
    while(!std_rcc_get_lxtal_ready());    
    
    /* 配置TIM3计数模式为时钟模式2 */
    std_rcc_apb1_clk_enable(RCC_PERIPH_CLK_TIM3);
    std_tim_set_etr_source(TIM3, TIM_TIM3_ETR_LXTAL);
    std_tim_etr_config(TIM3, TIM_ETR_POL_HIGH, TIM_ETR_PSC_DIV1, 0x00);
    std_tim_clock_source_config(TIM3, TIM_CLKSRC_ETR_MODE2);
    
    /* TIM3基本定时器配置 */
    basic_init_struct.counter_mode = TIM_COUNTER_MODE_UP;
    basic_init_struct.period = TIM3_ARR_VALUE;
    basic_init_struct.clock_div = TIM_CLOCK_DTS_DIV1;
    basic_init_struct.prescaler = TIM3_PSC_VALUE;
    basic_init_struct.auto_reload_preload = TIM_AUTORELOAD_PRE_DISABLE;
    std_tim_init(TIM3, &basic_init_struct);
    
    /* 配置TRC(ITR0)映射到IC1上，且上升沿有效 */
    std_tim_trig_source_config(TIM3, TIM_TRIG_SOURCE_ITR0);
    input_capture_struct.input_capture_pol = TIM_INPUT_POL_RISING;
    input_capture_struct.input_capture_sel = TIM_INPUT_CAPTURE_SEL_TRC;
    input_capture_struct.input_capture_prescaler = TIM_INPUT_CAPTURE_PSC_DIV1;
    input_capture_struct.input_capture_filter = 0x00;
    std_tim_input_capture_init(TIM3, &input_capture_struct, TIM_CHANNEL_1);
}

/**
* @brief  TIM5初始化
* @retval 无
*/
void tim5_init(void)
{
    std_tim_basic_init_t basic_init_struct = {0};
    std_tim_output_compare_init_t oc_config_struct = {0};
    
    /* TIM5时钟使能 */
    std_rcc_apb1_clk_enable(RCC_PERIPH_CLK_TIM5);
    
    /* TIM5基本定时器配置 */
    basic_init_struct.counter_mode = TIM_COUNTER_MODE_UP;
    basic_init_struct.period = TIM5_ARR_VALUE;
    basic_init_struct.clock_div = TIM_CLOCK_DTS_DIV1;
    basic_init_struct.prescaler = TIM5_PSC_VALUE;
    basic_init_struct.auto_reload_preload = TIM_AUTORELOAD_PRE_DISABLE;
    std_tim_init(TIM5, &basic_init_struct);
    
    /* 配置通道1输出模式为PWM1模式 */
    oc_config_struct.output_compare_mode = TIM_OUTPUT_MODE_PWM1;
    oc_config_struct.pulse = (TIM5_ARR_VALUE >> 1);
    oc_config_struct.output_pol = TIM_OUTPUT_POL_HIGH;
    oc_config_struct.output_preloadccx = TIM_OUTPUT_PRECC_DISABLE;
    oc_config_struct.output_fast_mode = TIM_OUTPUT_FAST_DISABLE;
    std_tim_output_compare_init(TIM5, &oc_config_struct, TIM_CHANNEL_1);
    
    /* 设置TIM5为主TIM, 触发从TIM */
    std_tim_trigout_mode_config(TIM5, TIM_TRIG_OUT_OC1REF);
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


/**
* @brief  GPIO初始化
* @retval 无
*/
void gpio_init(void)
{
    std_gpio_init_t tim5_gpio_init = {0};
    
    /* GPIOA 时钟使能 */
    std_rcc_gpio_clk_enable(RCC_PERIPH_CLK_GPIOA);
    
    /* TIM5 GPIO 配置
    PA10     ------> TIM5_CH1 */
    tim5_gpio_init.pin = GPIO_PIN_10;
    tim5_gpio_init.mode = GPIO_MODE_ALTERNATE;
    tim5_gpio_init.pull = GPIO_NOPULL;
    tim5_gpio_init.alternate = GPIO_AF5_TIM5;
    std_gpio_init(GPIOA, &tim5_gpio_init); 
}
