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

/*------------------------------------------define----------------------------------------------*/    
/* BGR使能后，等待其稳定时间，单位:us */
#define BGR_RDY_DELAY            (30UL)

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
* @brief  VBGR初始化
* @retval 无
*/
void vbgr_init(void)
{
    /* 使能PMU时钟 */
    std_rcc_apb1_clk_enable(RCC_PERIPH_CLK_PMU);

    /* 使能VBGR */
    std_pmu_bgr_enable();

    /* 等待BGR稳定 */
    std_delayus(BGR_RDY_DELAY);
}


/**
* @brief  COMP1初始化配置函数
* @retval 无
*/
void comp1_init(void)
{
    std_comp_init_t comp_config = {0};

    /* 使能COMP时钟 */
    std_rcc_apb2_clk_enable(RCC_PERIPH_CLK_SYSCFG);

    /* 配置COMP1参数 */
    comp_config.inpsel = COMP_INPSEL_IO2;          /* COMP1正相输入PA5引脚 */
    comp_config.inmsel = COMP_INMSEL_VBGR;         /* 反相输入信号VBGR */
    comp_config.speed = COMP_LOWSPEED;             /* 低响应速度 */
    comp_config.hyst = COMP_HYST_LOW;              /* 低迟滞 */
    comp_config.outpol = COMP_OUTPOL_NON_INVERTED; /* 输出不反相 */
    std_comp_init(COMP1, &comp_config);

    /* 使能COMP1 */
    std_comp_enable(COMP1);

    /* 等待COMP1使能启动时间 
       响应速度模式为高  速  使能启动时间23US
       响应速度模式为中  速  使能启动时间24US
       响应速度模式为低  速  使能启动时间30US
       响应速度模式为超低速  使能启动时间39US
    */
    std_delayus(COMP_EN_DELAY);
}


/**
* @brief  GPIO初始化
* @retval 无
*/
void gpio_init(void)
{
    std_gpio_init_t comp_gpio_config = {0};
    std_gpio_init_t led_gpio_config = {0};
    
    std_rcc_gpio_clk_enable(RCC_PERIPH_CLK_GPIOA | RCC_PERIPH_CLK_GPIOB);
    
    /* 配置COMP1正相输入GPIO模拟模式 */
    comp_gpio_config.pin = GPIO_PIN_5;
    comp_gpio_config.mode = GPIO_MODE_ANALOG;
    std_gpio_init(GPIOA, &comp_gpio_config);

    /* LED1初始化 */
    led_gpio_config.pin = LED_RED_PIN;
    led_gpio_config.mode = GPIO_MODE_OUTPUT;
    led_gpio_config.output_type = GPIO_OUTPUT_PUSHPULL;
    led_gpio_config.pull = GPIO_PULLUP;
    std_gpio_init(LED_RED_GPIO_PORT, &led_gpio_config);

    /* 初始化设置LED1为熄灭状态 */
    LED1_OFF();
}


