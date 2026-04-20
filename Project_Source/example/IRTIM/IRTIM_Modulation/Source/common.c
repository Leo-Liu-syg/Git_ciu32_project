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
#define TIM_PERIOD_VALUE        (0x04EEU)
#define TIM_PRESCALER_VALUE     (0x0U)
#define TIM_PULSE1_VALUE        (TIM_PERIOD_VALUE >> 1)


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
    std_gpio_init_t irtim_gpio_config = {0};
    std_gpio_init_t uart_gpio_config = {0};
    
    /* GPIO时钟使能 */
    std_rcc_gpio_clk_enable(RCC_PERIPH_CLK_GPIOB | RCC_PERIPH_CLK_GPIOD);
    
    /* IRTIM_OUT GPIO PB9 配置 */
    irtim_gpio_config.pin = GPIO_PIN_9;
    irtim_gpio_config.mode = GPIO_MODE_ALTERNATE;
    irtim_gpio_config.pull = GPIO_PULLUP;
    irtim_gpio_config.output_type = GPIO_OUTPUT_PUSHPULL;
    irtim_gpio_config.alternate = GPIO_AF1_IROUT;
    std_gpio_init(GPIOB, &irtim_gpio_config); 
    
    /* UART4 GPIO配置
       PD5    UART4_RX  */
    uart_gpio_config.pin = GPIO_PIN_5;
    uart_gpio_config.mode = GPIO_MODE_ALTERNATE;
    uart_gpio_config.pull = GPIO_NOPULL;
    uart_gpio_config.output_type = GPIO_OUTPUT_PUSHPULL;
    uart_gpio_config.alternate = GPIO_AF1_UART4;
    std_gpio_init(GPIOD, &uart_gpio_config); 
}


/**
* @brief  TIM4初始化
* @retval 无
*/
void tim4_init(void)
{
    std_tim_basic_init_t basic_init_config = {0};
    std_tim_output_compare_init_t oc_config = {0};
    
    /* TIM4时钟使能 */
    std_rcc_apb1_clk_enable(RCC_PERIPH_CLK_TIM4);
    
    /* 配置TIM4计数器参数 */
    basic_init_config.prescaler = TIM_PRESCALER_VALUE;
    basic_init_config.counter_mode = TIM_COUNTER_MODE_UP;
    basic_init_config.period = TIM_PERIOD_VALUE;
    basic_init_config.clock_div = TIM_CLOCK_DTS_DIV1;
    basic_init_config.auto_reload_preload = TIM_AUTORELOAD_PRE_DISABLE;
    std_tim_init(TIM4, &basic_init_config);
    
    /* 配置通道1输出模式为PWM1模式 */
    oc_config.output_compare_mode = TIM_OUTPUT_MODE_PWM1;
    oc_config.output_pol = TIM_OUTPUT_POL_HIGH;
    oc_config.pulse = TIM_PULSE1_VALUE;
    std_tim_output_compare_init(TIM4, &oc_config, TIM_CHANNEL_1);
}

/**
* @brief  UART4初始化
* @retval 无
*/
void uart4_init(void)
{
    std_usart_init_t uart_config = {0};    
    
    /* UART4时钟使能 */
    std_rcc_apb1_clk_enable(RCC_PERIPH_CLK_UART4);
    
    uart_config.direction = USART_DIRECTION_SEND_RECEIVE;
    uart_config.baudrate = 2400;
    uart_config.wordlength = USART_CR1_WL8BITS;
    uart_config.stopbits = USART_STOPBITS_1;
    uart_config.parity = USART_PARITY_NONE;
    uart_config.hardware_flow = USART_FLOWCONTROL_NONE;
    (void)std_usart_init(UART4, &uart_config);
    
    /* 禁止单线半双工 */
    std_usart_half_duplex_disable(UART4);
    
    /* UART使能 */
    std_usart_enable(UART4);
}

/**
* @brief  IRTIM初始化
* @retval 无
*/
void irtim_init(void)
{
    /* SYSCFG时钟使能 */
    std_rcc_apb2_clk_enable(RCC_PERIPH_CLK_SYSCFG);
    
    /* 设置载波信号在调制信号高电平输出，低电平不输出 */
    std_syscfg_set_ir_polarity(SYSCFG_IRPOLARITY_INVERSE);

    /* 选择载波信号输入源为UART4 */
    std_syscfg_set_ir_signal_source(SYSCFG_IRMODE_UART4_TX);
}

