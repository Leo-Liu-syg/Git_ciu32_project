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
* @brief  EXTI初始化
* @retval 无
*/
void exti_init(void)
{
    std_exti_init_t button_exti = {0};
    std_gpio_init_t button_gpio = {0};

    /* GPIOC时钟使能 */
    std_rcc_gpio_clk_enable(RCC_PERIPH_CLK_GPIOC);
    
    /* BUTTON_USER初始化 */
    button_gpio.pin = BUTTON_USER_PIN;
    button_gpio.mode = GPIO_MODE_INPUT;
    button_gpio.pull = GPIO_NOPULL;
    std_gpio_init(BUTTON_USER_GPIO, &button_gpio);    
            
    button_exti.gpio_id = BUTTON_USER_EXTI_PORT;
    button_exti.line_id = BUTTON_USER_EXTI_LINE;
    button_exti.trigger = EXTI_TRIGGER_FALLING;
    button_exti.mode = EXTI_MODE_INTERRUPT;
    std_exti_init(&button_exti);
    
    /* 使能EXTI中断 */
    NVIC_SetPriority(EXTI4_15_IRQn, NVIC_PRIO_0);
    NVIC_EnableIRQ(EXTI4_15_IRQn);    
}

