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
* @brief  GPIO初始化
* @retval 无
*/
void gpio_init(void)
{
    std_gpio_init_t tmp_gpio_cfg = {0};
    
    /* LPUART1 GPIO引脚配置    
       PB11    ------> LPUART1发送引脚
       PB10    ------> LPUART1接收引脚 
    */    
    std_rcc_gpio_clk_enable(RCC_PERIPH_CLK_GPIOB);
    tmp_gpio_cfg.pin = GPIO_PIN_10 | GPIO_PIN_11;
    tmp_gpio_cfg.mode = GPIO_MODE_ALTERNATE;
    tmp_gpio_cfg.output_type = GPIO_OUTPUT_PUSHPULL;
    tmp_gpio_cfg.pull = GPIO_PULLUP;
    tmp_gpio_cfg.alternate = GPIO_AF6_LPUART1;
    std_gpio_init(GPIOB, &tmp_gpio_cfg);
    
}

/**
* @brief  LPUART初始化配置，时钟源为RCH
* @retval 无
*/
void lpuart_init(void)
{
    std_lpuart_init_t lpuart_config = {0};
    
    /* 配置LPUART 时钟源为RCH */
    std_rcc_set_lpuart1clk_source(RCC_LPUART1_ASYNC_CLK_SRC_RCH);
    
    /* 配置LPUART 外设时钟 */
    std_rcc_apb1_clk_enable(RCC_PERIPH_CLK_LPUART1);
    
    /* LPUART 模块初始化 */
    lpuart_config.prescaler = LPUART_PRESCALER_DIV1;
    lpuart_config.baudrate = 9600;
    lpuart_config.parity = LPUART_PARITY_NONE;
    lpuart_config.stopbits = LPUART_STOPBITS_1;
    lpuart_config.wordlength = LPUART_WORDLENGTH_8BITS;
    lpuart_config.direction = LPUART_DIRECTION_SEND_RECEIVE;
    /* LPUART初始化 */
    if(STD_OK != std_lpuart_init(LPUART1,&lpuart_config))
    {
        /* 波特率配置不正确处理代码 */
        while(1);
    } 

    /* 配置LPUART中断优先级以及使能LPUART的NVIC中断 */   
    NVIC_SetPriority(LPUART1_IRQn,NVIC_PRIO_1);    
    NVIC_EnableIRQ(LPUART1_IRQn);
}

