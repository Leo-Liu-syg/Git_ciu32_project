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
#define TIM_ARR_VALUE           (0x03FFU)
#define TIM_PULSE_VALUE         (0x00U)

#define TIM_PRESCALER_VALUE     (0x07U)

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
    basic_init.prescaler = TIM_PRESCALER_VALUE;
    basic_init.counter_mode = TIM_COUNTER_MODE_UP;
    basic_init.period = TIM_ARR_VALUE;
    basic_init.clock_div = TIM_CLOCK_DTS_DIV1;
    basic_init.auto_reload_preload = TIM_AUTORELOAD_PRE_DISABLE;
    std_tim_init(TIM3, &basic_init);
    
    /* 配置通道1输出模式为PWM2模式 */
    oc_config.output_compare_mode = TIM_OUTPUT_MODE_PWM2;
    oc_config.output_preloadccx = TIM_OUTPUT_PRECC_ENABLE;
    oc_config.output_pol = TIM_OUTPUT_POL_HIGH;
    oc_config.pulse = TIM_PULSE_VALUE;
    std_tim_output_compare_init(TIM3, &oc_config, TIM_CHANNEL_1);
    
    /* 配置通道2输出模式为PWM2模式 */
    std_tim_output_compare_init(TIM3, &oc_config, TIM_CHANNEL_2);
    
    /* 配置通道3输出模式为PWM2模式 */
    std_tim_output_compare_init(TIM3, &oc_config, TIM_CHANNEL_3);
    
    /* 配置通道4输出模式为PWM2模式 */
    std_tim_output_compare_init(TIM3, &oc_config, TIM_CHANNEL_4);
}


/**
* @brief  DMA初始化配置
* @retval 无
*/
void dma_init(void)
{
    std_dma_init_t dma_tim3_init = {0};
    
    /* 使能DMA时钟*/
    std_rcc_ahb_clk_enable(RCC_PERIPH_CLK_DMA);
    
    /* TIM DMA 初始化 */
    dma_tim3_init.dma_channel = DMA_CHANNEL_1;
    dma_tim3_init.transfer_type = DMA_BURST_TRANSFER;
    dma_tim3_init.dma_req_id = DMA_REQUEST_TIM3_UP;
    dma_tim3_init.src_addr_inc = DMA_SRC_INC_ENABLE;
    dma_tim3_init.dst_addr_inc = DMA_DST_INC_ENABLE;
    dma_tim3_init.data_size = DMA_DATA_SIZE_WORD;
    dma_tim3_init.mode = DMA_MODE_NORMAL;
    std_dma_init(&dma_tim3_init);
}


/**
* @brief  GPIO初始化
* @retval 无
*/
void gpio_init(void)
{
    std_gpio_init_t tim3_gpio_init = {0};
    
    /* GPIOB、GPIOC 时钟使能 */
    std_rcc_gpio_clk_enable(RCC_PERIPH_CLK_GPIOB);
    std_rcc_gpio_clk_enable(RCC_PERIPH_CLK_GPIOC);
    
    /* TIM3 GPIO 配置
    PB4     ------> TIM3_CH1
    PB5     ------> TIM3_CH2 
    PC7     ------> TIM3_CH3 
    PC8     ------> TIM3_CH4 */
    tim3_gpio_init.pin = GPIO_PIN_4 | GPIO_PIN_5;
    tim3_gpio_init.mode = GPIO_MODE_ALTERNATE;
    tim3_gpio_init.pull = GPIO_NOPULL;
    tim3_gpio_init.alternate = GPIO_AF2_TIM3;    
    std_gpio_init(GPIOB, &tim3_gpio_init);
        
    tim3_gpio_init.pin = GPIO_PIN_7 | GPIO_PIN_8;
    tim3_gpio_init.mode = GPIO_MODE_ALTERNATE;
    tim3_gpio_init.pull = GPIO_NOPULL;
    tim3_gpio_init.alternate = GPIO_AF1_TIM3;
    std_gpio_init(GPIOC, &tim3_gpio_init);
}

