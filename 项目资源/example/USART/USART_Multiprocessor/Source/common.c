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
    std_rcc_gpio_clk_enable(RCC_PERIPH_CLK_GPIOA);
    std_rcc_gpio_clk_enable(RCC_PERIPH_CLK_GPIOD);

     /* GPIO引脚配置 
       PA11    ------> USART1 TX引脚     
       PA12    ------> USART1 RX引脚 

       PD3    ------>  UART2 TX引脚 
       PD4    ------>  UART2 RX引脚 
    */  
    gpio_config.pin = GPIO_PIN_12 | GPIO_PIN_11;
    gpio_config.mode = GPIO_MODE_ALTERNATE;
    gpio_config.output_type = GPIO_OUTPUT_OPENDRAIN;
    gpio_config.pull = GPIO_PULLUP;
    gpio_config.alternate = GPIO_AF1_USART1;
    std_gpio_init(GPIOA, &gpio_config);
    
    gpio_config.pin = GPIO_PIN_3 | GPIO_PIN_4; 
    gpio_config.mode = GPIO_MODE_ALTERNATE;
    gpio_config.output_type = GPIO_OUTPUT_OPENDRAIN;
    gpio_config.pull = GPIO_PULLUP;
    gpio_config.alternate = GPIO_AF4_UART2;
    std_gpio_init(GPIOD, &gpio_config);
}

/**
* @brief  USART 初始化
* @retval 无
*/
void usart_init(void)
{
    std_usart_init_t usart_config ={0};
    
    /* USART1、2时钟使能 */
    std_rcc_apb2_clk_enable(RCC_PERIPH_CLK_USART1);
    std_rcc_apb1_clk_enable(RCC_PERIPH_CLK_UART2);
    
    /* USART1 初始化 */     
    usart_config.baudrate = 115200;
    usart_config.wordlength = USART_WORDLENGTH_8BITS;
    usart_config.stopbits = USART_STOPBITS_1;
    usart_config.parity = USART_PARITY_NONE;
    usart_config.hardware_flow = USART_FLOWCONTROL_NONE;
    usart_config.direction = USART_DIRECTION_SEND_RECEIVE; 
    /* USART初始化 */   
    if(STD_OK != std_usart_init(USART1,&usart_config))
    {
        /* 波特率配置不正确处理代码 */
        while(1);
    }
    std_usart_enable(USART1);

    /* UART2 初始化 */        
    usart_config.baudrate = 115200;
    usart_config.wordlength = USART_WORDLENGTH_8BITS;
    usart_config.stopbits = USART_STOPBITS_1;
    usart_config.hardware_flow = USART_FLOWCONTROL_NONE;
    usart_config.direction = USART_DIRECTION_SEND_RECEIVE;
    /* UART初始化 */   
    if(STD_OK != std_usart_init(UART2,&usart_config))
    {
        /* 波特率配置不正确处理代码 */
        while(1);
    }
    std_usart_enable(UART2);
}

/**
* @brief  LED初始化
* @retval 无
*/
void led_init(void)
{
    std_gpio_init_t led_gpio_init = {0};

    /* 使能LED对应的GPIO时钟 */
    std_rcc_gpio_clk_enable(RCC_PERIPH_CLK_GPIOB);

    /* 配置LED1的IO */
    led_gpio_init.pin = LED_RED_PIN;
    led_gpio_init.mode = GPIO_MODE_OUTPUT;
    led_gpio_init.pull = GPIO_PULLUP;
    led_gpio_init.output_type = GPIO_OUTPUT_PUSHPULL;

    /* 初始化GPIO */
    std_gpio_init(LED_RED_GPIO_PORT, &led_gpio_init);
}


/**
* @brief  EXTI初始化
* @retval 无
*/
void exti_init(void)
{
    std_exti_init_t exti_init_config = {0};
    std_gpio_init_t button_init_config = {0};    

    /* 使能BUTTON_USER对应的GPIO时钟 */
    std_rcc_gpio_clk_enable(RCC_PERIPH_CLK_GPIOC);

    /* 配置BUTTON_USER的GPIO */
    button_init_config.pin = BUTTON_USER_PIN;
    button_init_config.mode = GPIO_MODE_INPUT;
    button_init_config.pull = GPIO_NOPULL;
    std_gpio_init(BUTTON_USER_PORT, &button_init_config);
    
    /* 配置BUTTON_USER的EXTI */
    exti_init_config.line_id = BUTTON_USER_EXTI_LINE;
    exti_init_config.mode = EXTI_MODE_INTERRUPT;
    exti_init_config.trigger = EXTI_TRIGGER_FALLING;
    exti_init_config.gpio_id = BUTTON_USER_EXTI_PORT;
    std_exti_init(&exti_init_config);

    /* 配置中断优先级 */
    NVIC_SetPriority(EXTI4_15_IRQn, NVIC_PRIO_3); 
    /* 使能中断 */
    NVIC_EnableIRQ(EXTI4_15_IRQn);
}
