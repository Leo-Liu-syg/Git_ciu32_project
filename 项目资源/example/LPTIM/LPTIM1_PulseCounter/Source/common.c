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
* @brief  配置RCHSYS作为系统时钟，即RCH 2分频
* @retval 无
*/
void system_clock_config(void)
{
    /* 设置RCH 2分频 */
    std_rcc_set_rchdiv(RCC_RCH_DIV2);
    
    /* 更新系统时钟全局变量 */
    SystemCoreClockUpdate();
}

/**
* @brief  LED1和LPTIM1 GPIO初始化配置
* @retval 无
*/
void gpio_init(void)
{
    std_gpio_init_t led_gpio_config = {0};
    std_gpio_init_t lptim_gpio_config = {0};
    
    /* 使能GPIO时钟 */
    std_rcc_gpio_clk_enable(RCC_PERIPH_CLK_GPIOB);
    
    /* 初始化LED1 GPIO */
    led_gpio_config.pin = LED1_PIN;
    led_gpio_config.mode = GPIO_MODE_OUTPUT;
    led_gpio_config.pull = GPIO_PULLUP;
    led_gpio_config.output_type = GPIO_OUTPUT_PUSHPULL;
    std_gpio_init(LED_GPIO_PORT, &led_gpio_config);
    
    /* 关闭LED */
    LED1_OFF();
    
    /* 初始化LPTIM1_IN1 GPIO PB5 */
    lptim_gpio_config.pin = GPIO_PIN_5;
    lptim_gpio_config.mode = GPIO_MODE_ALTERNATE;
    lptim_gpio_config.pull = GPIO_NOPULL;
    lptim_gpio_config.output_type = GPIO_OUTPUT_PUSHPULL;
    lptim_gpio_config.alternate = GPIO_AF5_LPTIM1;
    std_gpio_init(GPIOB, &lptim_gpio_config);
}

/**
* @brief  NVIC初始化
* @retval 无
*/
void nvic_init(void)
{
    NVIC_SetPriority(LPTIM1_IRQn, NVIC_PRIO_0);
    NVIC_EnableIRQ(LPTIM1_IRQn);
}

/**
* @brief  LPTIM1初始化配置
* @note   Vcore_aon域写使能开启后未关闭，用户可根据需求进行修改
* @retval 无
*/
void lptim1_init(void)
{
    std_lptim_init_t lptim_config; 
    
    /* 等待RCL时钟稳定后使能RCL时钟 */
    std_rcc_rcl_enable();
    while(!std_rcc_get_rcl_ready());

    /* 选择RCL时钟作为LPTIM时钟源 */
    std_rcc_set_lptim1clk_source(RCC_LPTIM1_ASYNC_CLK_SRC_RCL);
    
    /* 使能LPTIM外设时钟 */
    std_rcc_apb1_clk_enable(RCC_PERIPH_CLK_LPTIM1);
    
    /* 初始化LPTIM */
    lptim_config.clock_source = LPTIM_CLK_SRC_INT;
    lptim_config.prescaler = LPTIM_PRESCALER_DIV1;
    lptim_config.count_source = LPTIM_COUNTER_CLK_EXT;
    lptim_config.out_polarity = LPTIM_OUTPUT_POLARITY_REGULAR;
    std_lptim_init(LPTIM1, &lptim_config);
    
    /* 配置LPTIM外部时钟 */
    std_lptim_set_external_clk_polarity(LPTIM1, LPTIM_EXTCLK_POL_RISING);
    std_lptim_set_extern_clk_filter(LPTIM1, LPTIM_EXTCLK_SAMPLETIME_NOFLT);
    std_lptim_set_software_trig(LPTIM1);
    
    std_lptim_set_input1_source(LPTIM1, LPTIM_IN1_SRC_GPIO);
}

