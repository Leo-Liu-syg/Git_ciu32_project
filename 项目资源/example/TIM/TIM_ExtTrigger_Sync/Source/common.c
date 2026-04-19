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
#define TIM_ARR_VALUE                 (0xFFU)
#define PRESCALER_VALUE               (0x05U)

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
    std_tim_output_compare_init_t oc_config = {0};
    std_tim_basic_init_t basic_init = {0}; 
    
    /* TIM3时钟使能 */
    std_rcc_apb1_clk_enable(RCC_PERIPH_CLK_TIM3);
               
    /* 配置TIM3计数器参数 */
    basic_init.prescaler = PRESCALER_VALUE;
    basic_init.counter_mode = TIM_COUNTER_MODE_UP;
    basic_init.period = TIM_ARR_VALUE;
    basic_init.clock_div = TIM_CLOCK_DTS_DIV1;
    basic_init.auto_reload_preload = TIM_AUTORELOAD_PRE_DISABLE;
    std_tim_init(TIM3, &basic_init);    
        
    /* 配置通道2为输入模式 */
    std_tim_set_icmode(TIM3, TIM_CHANNEL_2, TIM_INPUT_CAPTURE_SEL_DIRECTTI);
    
    /* 配置从模式为门控模式，且TI2FP2为触发源，上升沿有效 */
    std_tim_slave_mode_config(TIM3, TIM_SLAVE_MODE_GATED);
    std_tim_trig_source_config(TIM3, TIM_TRIG_SOURCE_TI2FP2);
    std_tim_set_input_pol(TIM3, TIM_CHANNEL_2, TIM_INPUT_POL_RISING);

    /* 配置主模式有效 */
    std_tim_trigout_mode_config(TIM3,TIM_TRIG_OUT_ENABLE);
    std_tim_master_mode_enable(TIM3);
    
    /* 配置通道1为比较输出模式 */
    oc_config.output_compare_mode = TIM_OUTPUT_MODE_TOGGLE;
    oc_config.output_pol = TIM_OUTPUT_POL_HIGH;
    std_tim_output_compare_init(TIM3, &oc_config, TIM_CHANNEL_1);
}

/**
* @brief  TIM5初始化
* @retval 无
*/
void tim5_init(void)
{
    std_tim_output_compare_init_t oc_config = {0};
    std_tim_basic_init_t base_config = {0};
        
    /* TIM5时钟使能 */
    std_rcc_apb1_clk_enable(RCC_PERIPH_CLK_TIM5);

    /* 配置TIM5计数器参数 */
    base_config.prescaler = PRESCALER_VALUE;
    base_config.counter_mode = TIM_COUNTER_MODE_UP;
    base_config.period = TIM_ARR_VALUE;
    base_config.clock_div = TIM_CLOCK_DTS_DIV1;
    base_config.auto_reload_preload = TIM_AUTORELOAD_PRE_DISABLE;
    std_tim_init(TIM5, &base_config);
        
    /* 配置从模式为门控模式，且ITR1为触发源 */
    std_tim_slave_mode_config(TIM5, TIM_SLAVE_MODE_GATED);
    std_tim_trig_source_config(TIM5, TIM_TRIG_SOURCE_ITR1);
    
    /* 配置比较输出模式 */
    oc_config.output_compare_mode = TIM_OUTPUT_MODE_TOGGLE;
    oc_config.output_pol = TIM_OUTPUT_POL_HIGH;
    std_tim_output_compare_init(TIM5, &oc_config, TIM_CHANNEL_1);    
}

/**
* @brief  TIM3、TIM5 GPIO初始化
* @retval 无
*/
void gpio_init(void)
{
    std_gpio_init_t gpio_config = {0};

    /* GPIOB外设时钟使能 */
    std_rcc_gpio_clk_enable(RCC_PERIPH_CLK_GPIOB);   
    
    /* TIM3 GPIO 配置
    PB4     ------> TIM3_CH1
    PB5     ------> TIM3_CH2  */
    gpio_config.pin = GPIO_PIN_4;
    gpio_config.mode = GPIO_MODE_ALTERNATE;
    gpio_config.pull = GPIO_NOPULL;
    gpio_config.alternate = GPIO_AF2_TIM3;
    std_gpio_init(GPIOB, &gpio_config);
 
    gpio_config.pin = GPIO_PIN_5;
    gpio_config.mode = GPIO_MODE_ALTERNATE;
    gpio_config.pull = GPIO_PULLDOWN;
    gpio_config.alternate = GPIO_AF2_TIM3;
    std_gpio_init(GPIOB, &gpio_config);
    
    /* TIM5 GPIO 配置
    PB8   ------> TIM5_CH1 */  
    gpio_config.pin = GPIO_PIN_8;
    gpio_config.mode = GPIO_MODE_ALTERNATE;
    gpio_config.pull = GPIO_NOPULL;
    gpio_config.alternate = GPIO_AF5_TIM5;
    std_gpio_init(GPIOB, &gpio_config);
}


