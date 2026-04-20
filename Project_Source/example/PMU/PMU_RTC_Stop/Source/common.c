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
* @brief  RTC时钟配置
* @note   配置LXTAL时，需要解除备份域的写保护，故调用该函数后，会使能PMU时钟，同时使能备份域的写保护。
* @retval 无
*/
void rtc_clock_config(void)
{
    /* RTC APB时钟使能 */
    std_rcc_apb1_clk_enable(RCC_PERIPH_CLK_RTC);
    
    /* 使能PMU时钟，开启VCORE_AON写使能 */
    std_rcc_apb1_clk_enable(RCC_PERIPH_CLK_PMU);
    std_pmu_vaon_write_enable();
    
    /* 配置LXTAL驱动能力 */
    std_rcc_lxtal_drive_mode_config(RCC_LXTAL_DRIVE_MODE_ENHANCE);
    std_rcc_lxtal_drive_config(RCC_LXTAL_DRIVE_LEVEL2);    
    
    /* 使能LXTAL时钟 */
    std_rcc_lxtal_enable(RCC_LXTAL_ON);
    while(!std_rcc_get_lxtal_ready());    
                    
    /* 选择LXTAL作为RTC时钟源 */
    std_rcc_set_rtcclk_source(RCC_RTC_ASYNC_CLK_SRC_LXTAL);
    
    /* RTC外设时钟使能 */
    std_rcc_rtc_enable();
}


/**
* @brief  EXTI初始化
* @retval 无
*/
void exti_init(void)
{
    std_exti_init_t button_exti = {0};
    std_gpio_init_t  button_gpio = {0};

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


/**
* @brief  LED1 GPIO初始化配置
* @retval 无
*/
void led_init(void)
{
    std_gpio_init_t led_gpio_init = {0};
    
    /* 使能LED时钟 */
    std_rcc_gpio_clk_enable(RCC_PERIPH_CLK_GPIOB);
    
    /* 初始化LED GPIO */
    led_gpio_init.pin = LED_RED_PIN;
    led_gpio_init.mode = GPIO_MODE_OUTPUT;
    led_gpio_init.pull = GPIO_PULLUP;
    std_gpio_init(LED_GPIO_PORT, &led_gpio_init);    

    LED1_ON(); 
}


/**
* @brief  gpio初始化
* @retval 无
*/
void gpio_init(void)
{
    std_gpio_init_t usart_gpio_init = {0};  
    
    /* 使能GPIOA时钟 */
    std_rcc_gpio_clk_enable(RCC_PERIPH_CLK_GPIOA);
       
    /* USART1 GPIO配置
       TX --------- PA1
       RX --------- PA2 */
    usart_gpio_init.pin = GPIO_PIN_1 | GPIO_PIN_2;
    usart_gpio_init.mode = GPIO_MODE_ALTERNATE;
    usart_gpio_init.pull = GPIO_PULLUP;
    usart_gpio_init.alternate = GPIO_AF1_USART1;
    std_gpio_init(GPIOA, &usart_gpio_init);
}


/**
* @brief  uart初始化
* @retval 无
*/
void uart_init(void)
{
    std_usart_init_t usart_init = {0};
     /* 使能USART1时钟 */
    std_rcc_apb2_clk_enable(RCC_PERIPH_CLK_USART1);

    usart_init.baudrate = 115200;
    usart_init.direction = USART_DIRECTION_SEND_RECEIVE;
    usart_init.hardware_flow = USART_FLOWCONTROL_NONE;
    usart_init.parity = USART_PARITY_NONE;
    usart_init.stopbits = USART_STOPBITS_1;
    usart_init.wordlength = USART_WORDLENGTH_8BITS;
    (void)std_usart_init(USART1, &usart_init);
}


#if defined ( __GNUC__ )

/**
* @brief  重定向c库函数printf到串口，重定向后可使用printf函数
* @param  fd 文件描述符
* @param  ptr 待发送字符串
* @param  len 待发送字符串长度
* @retval len 待发送字符串长度
*/
int _write(int fd, char* ptr, int len)
{
    uint32_t i = 0;

    /* 发送一个字节数据到串口 */
    for (; i<len; i++) 
    {
        std_usart_tx_write_data(USART1, (uint32_t)ptr[i]);

        /* 等待发送完成 */
        while(!std_usart_get_flag(USART1, USART_FLAG_TC));
    }

    return len;
}

#else

/**
* @brief  重定向c库函数printf到串口，重定向后可使用printf函数
* @param  ch 待发送字符
* @retval ch 发送的字符
*/
int fputc(int ch, FILE *f)
{
    /* 发送一个字节数据到串口 */
    std_usart_tx_write_data(USART1, (uint32_t)ch);
    
    /* 等待发送完毕 */
    while(!std_usart_get_flag(USART1, USART_FLAG_TC));
    return ch;
}

#endif

