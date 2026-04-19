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
* @note   系统时钟配置为RCH 2分频
* @retval 无
*/
void system_clock_config(void)
{
    /* 设置RCH时钟2分频 */
    std_rcc_set_rchdiv(RCC_RCH_DIV2);
 
    /* 更新系统时钟全局变量 */
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
    std_gpio_init_t led_gpio_congig = {0};
 
    /* 使能GPIO时钟 */
    std_rcc_gpio_clk_enable(RCC_PERIPH_CLK_GPIOA | RCC_PERIPH_CLK_GPIOB);
    
    /* 配置COMP1正相输入GPIO模拟模式 */
    comp_gpio_config.pin = GPIO_PIN_5;
    comp_gpio_config.mode = GPIO_MODE_ANALOG;
    std_gpio_init(GPIOA, &comp_gpio_config);

    /* LED1初始化 */
    led_gpio_congig.pin = LED1_PIN;
    led_gpio_congig.mode = GPIO_MODE_OUTPUT;
    led_gpio_congig.output_type = GPIO_OUTPUT_PUSHPULL;
    led_gpio_congig.pull = GPIO_PULLUP;
    std_gpio_init(LED1_GPIO_PORT, &led_gpio_congig);
}


/**
* @brief  EXTI初始化配置
* @retval 无
*/
void exti_init(void)
{   
    std_exti_init_t exti_config = {0};

    /* 配置EXTI参数 */
    exti_config.line_id = EXTI_LINE_COMP1;         /* EXTI 通道  */
    exti_config.mode = EXTI_MODE_INTERRUPT;        /* 中断唤醒   */
    exti_config.trigger = EXTI_TRIGGER_RISING;     /* 上升沿触发 */
    std_exti_init(&exti_config);
}


/**
* @brief  NVIC初始化配置
* @retval 无
*/
void nvic_init(void)
{
    NVIC_SetPriority(ADC_COMP_IRQn, NVIC_PRIO_0);
    NVIC_EnableIRQ(ADC_COMP_IRQn);
}
