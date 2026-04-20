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
* @brief  GPIO 初始化
* @retval 无
*/
void gpio_init(void)
{
    std_gpio_init_t gpio_config = {0};
    
    /* GPIO外设时钟使能 */  
    std_rcc_gpio_clk_enable(RCC_PERIPH_CLK_GPIOA|RCC_PERIPH_CLK_GPIOB|RCC_PERIPH_CLK_GPIOD);

    /* SCIO初始化 */  
    gpio_config.pin = SC_USART_SCIO_PIN;
    gpio_config.mode = GPIO_MODE_OUTPUT;
    gpio_config.output_type = GPIO_OUTPUT_PUSHPULL;
    gpio_config.pull = GPIO_NOPULL;
    std_gpio_init(SC_USART_PORT, &gpio_config);
    
    /* CLK初始化 */ 
    gpio_config.pin = SC_USART_CLK_PIN;
    gpio_config.mode = GPIO_MODE_ALTERNATE;
    gpio_config.output_type = GPIO_OUTPUT_PUSHPULL;
    gpio_config.pull = GPIO_NOPULL;
    gpio_config.alternate = GPIO_AF1_USART1;
    std_gpio_init(SC_USART_PORT, &gpio_config);
    
    /* RST初始化 */ 
    gpio_config.pin = SC_GPIO_RST_PIN;
    gpio_config.mode = GPIO_MODE_OUTPUT;
    gpio_config.output_type = GPIO_OUTPUT_PUSHPULL;
    gpio_config.pull = GPIO_NOPULL;
    std_gpio_init(SC_GPIO_PORT, &gpio_config);
    
    /* 电源初始化 */ 
    gpio_config.pin = SC_GPIO_POWER_CTRL_PIN;
    gpio_config.mode = GPIO_MODE_OUTPUT;
    gpio_config.output_type = GPIO_OUTPUT_PUSHPULL;
    gpio_config.pull = GPIO_PULLUP;
    std_gpio_init(SC_GPIO_POWER_CTRL_PORT, &gpio_config);   
    /* 使能电源的高驱模式*/
    std_gpio_set_highdrive_pin(SC_GPIO_POWER_CTRL_PORT,SC_GPIO_POWER_CTRL_PIN);

    /* 插卡检测IO初始化 */ 
    gpio_config.pin = SC_GPIO_DETECT_PIN;
    gpio_config.mode = GPIO_MODE_INPUT;
    gpio_config.pull = GPIO_NOPULL;
    std_gpio_init(SC_GPIO_PORT, &gpio_config);
    
}

/**
* @brief  智能卡IO状态初始化
* @retval 无
*/
void smartcard_io_init(void)
{
    /* RST引脚输出低电平 */
    std_gpio_reset_pin(SC_GPIO_PORT,SC_GPIO_RST_PIN);
    
    /* 时钟不输出时CLK引脚默认低电平 */
    
    /* SCIO输出低电平 */
    std_gpio_reset_pin(SC_USART_PORT,SC_USART_SCIO_PIN);
    
    /* 下电状态 */
    std_gpio_reset_pin(SC_GPIO_POWER_CTRL_PORT,SC_GPIO_POWER_CTRL_PIN);
}

/**
* @brief  EXTI初始化
* @retval 无
*/
void exti_init(void)
{
    std_exti_init_t exti_config = {0};

    /* 配置插卡检测IO的EXTI */
    exti_config.line_id = SC_DETECT_EXTI_PIN;
    exti_config.mode = EXTI_MODE_INTERRUPT;
    exti_config.trigger = EXTI_TRIGGER_FALLING;
    exti_config.gpio_id = SC_DETECT_EXTI_PORT;

    std_exti_init(&exti_config);

    /* 配置中断优先级 */
    NVIC_SetPriority(EXTI4_15_IRQn, NVIC_PRIO_0); 
    /* 使能中断 */
    NVIC_EnableIRQ(EXTI4_15_IRQn);

}

/**
* @brief  智能卡模式初始化
* @retval 无
*/
void usart_smartcard_init(void)
{
    std_usart_init_t usart_config = {0};

    /* USART时钟使能 */
    std_rcc_apb2_clk_enable(RCC_PERIPH_CLK_USART1);

    /* 配置输出到智能卡的时钟，输出4MHz（系统时钟48MHz的12分频）*/ 
    /* 配置完成后暂不使能时钟输出，时钟在Cold Reset流程中使能 */
    std_usart_set_prescaler(USART1, SC_CLK_PRESCALER_DIV_12);
    
    /* 配置额外保护时间 默认2etu */ 
    std_usart_smartcard_set_guard_time(USART1,SC_GUARDTIME_DEFAULT);
    
    /* 使能NACK */ 
    std_usart_smartcard_nack_enable(USART1);
    
    /* 配置重试次数4次 */
    std_usart_smartcard_set_retry(USART1, SC_RETRY_COUNT); 

    /* 智能卡模式使能，使能前LINEN、HDEN、IREN位需清零 */ 
    std_usart_smartcard_enable(USART1);
    
    usart_config.baudrate = SC_BAUDRATE;  /* 智能卡时钟4MHz的372分频 */   
    usart_config.wordlength = USART_WORDLENGTH_9BITS;
    usart_config.stopbits = USART_STOPBITS_1_5;
    usart_config.parity = USART_PARITY_EVEN;
    usart_config.hardware_flow = USART_FLOWCONTROL_NONE;
    usart_config.direction = USART_DIRECTION_SEND_RECEIVE;
    
    /* USART初始化 */   
    if(STD_OK != std_usart_init(USART1,&usart_config))
    {
        /* 波特率配置不正确处理代码 */
        while(1);
    }
}

