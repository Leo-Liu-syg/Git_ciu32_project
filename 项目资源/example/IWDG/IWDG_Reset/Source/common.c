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

/*------------------------------------------functions-------------------------------------------*/

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
* @brief  IWDG初始化
* @retval 无
*/
void iwdg_init(void)
{
    /* 使能RCL时钟 */
    std_rcc_rcl_enable();
    while(std_rcc_get_rcl_ready() != 1)
    {
    };

    /* 使能IWDG的写权限 */
    std_iwdg_write_access_enable();

    /* 配置IWDG参数 */
    std_iwdg_set_overflow_period(IWDG_OVERFLOW_PERIOD_2048);

    /* IWDG使能 */
    std_iwdg_start();
}

/**
* @brief  LED1初始化
* @retval 无
*/
void led_init(void)
{
    std_gpio_init_t led_io = {0};

    /* 使能LED1对应的GPIO时钟 */
    std_rcc_gpio_clk_enable(RCC_PERIPH_CLK_GPIOB);

    /* 配置LED1的IO */
    led_io.pin   = LED_RED_PIN;
    led_io.mode  = GPIO_MODE_OUTPUT;
    led_io.pull  = GPIO_PULLUP;
    led_io.output_type = GPIO_OUTPUT_PUSHPULL;

    /* 初始化GPIO */
    std_gpio_init(LED_RED_GPIO_PORT, &led_io);
}
