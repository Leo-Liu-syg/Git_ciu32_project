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
#define TIM_ARR_VALUE           (0xFFFFU)
#define TIM_PULSE_VALUE         (0x3FFFU)


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
    uint32_t tmp_psc_value;
    
    std_tim_basic_init_t basic_init = {0};    
    std_tim_output_compare_init_t output_init = {0};
    
    /* TIM3时钟使能 */
    std_rcc_apb1_clk_enable(RCC_PERIPH_CLK_TIM3);
        
    tmp_psc_value = (uint32_t)(((SystemCoreClock) / 1000000) - 1);
        
    /* 配置TIM3计数器参数 */
    basic_init.prescaler = tmp_psc_value;
    basic_init.counter_mode = TIM_COUNTER_MODE_UP;    
    basic_init.period = TIM_ARR_VALUE;
    basic_init.clock_div = TIM_CLOCK_DTS_DIV1;
    std_tim_init(TIM3, &basic_init);
    
    /* 配置单脉冲触发模式为TRIG，且触发源为TI2 */
    std_tim_slave_mode_config(TIM3, TIM_SLAVE_MODE_TRIG);
    std_tim_trig_source_config(TIM3, TIM_TRIG_SOURCE_ETRF);
    
    /* 配置ETR输入极性，上升沿有效 */
    std_tim_etr_config(TIM3, TIM_ETR_POL_HIGH, TIM_ETR_PSC_DIV1, 0x00);
    
    /* 配置通道1输出模式为PWM2模式 */
    output_init.output_compare_mode = TIM_OUTPUT_MODE_PWM2;
    output_init.pulse = TIM_PULSE_VALUE;
    output_init.output_pol = TIM_OUTPUT_POL_HIGH; 
    std_tim_output_compare_init(TIM3, &output_init, TIM_CHANNEL_1);
}

/**
* @brief  GPIO初始化
* @retval 无
*/
void gpio_init(void)
{
    std_gpio_init_t gpio_config = {0};
    
    /* GPIOB/C时钟使能 */
    std_rcc_gpio_clk_enable(RCC_PERIPH_CLK_GPIOB | RCC_PERIPH_CLK_GPIOC);
    
    /* TIM3 GPIO 配置
    PB4     ------> TIM3_CH1
    PC10    ------> TIM3_ETR  */
    gpio_config.pin = GPIO_PIN_4;
    gpio_config.mode = GPIO_MODE_ALTERNATE;
    gpio_config.pull = GPIO_PULLDOWN;
    gpio_config.alternate = GPIO_AF2_TIM3;
    std_gpio_init(GPIOB, &gpio_config);
    
    gpio_config.pin = GPIO_PIN_10;
    gpio_config.mode = GPIO_MODE_ALTERNATE;
    gpio_config.pull = GPIO_PULLDOWN;
    gpio_config.alternate = GPIO_AF4_TIM3;
    std_gpio_init(GPIOC, &gpio_config);
}

