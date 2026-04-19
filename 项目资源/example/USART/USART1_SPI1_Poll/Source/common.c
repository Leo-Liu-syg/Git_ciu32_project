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
* @brief  GPIO初始化
* @retval 无
*/
void gpio_init(void)
{
    /* GPIO外设时钟使能 */  
    std_rcc_gpio_clk_enable(RCC_PERIPH_CLK_GPIOA);
    std_rcc_gpio_clk_enable(RCC_PERIPH_CLK_GPIOD);
    
    std_gpio_init_t usart_gpio_init = {0};

    /* USART1 GPIO引脚配置   
    USART1_TX   --   PA11 
    USART1_RX   --   PA12
    USART1_CK   --   PA10 
    USART1_NSS  --   PD3
    */
    
    usart_gpio_init.pin = GPIO_PIN_11 | GPIO_PIN_12 | GPIO_PIN_10;
    usart_gpio_init.mode = GPIO_MODE_ALTERNATE;
    usart_gpio_init.pull = GPIO_PULLUP;
    usart_gpio_init.alternate = GPIO_AF1_USART1;
    std_gpio_init(GPIOA, &usart_gpio_init);
    
    /* 作为SPI主设备时，设置片选 */
    usart_gpio_init.pin = NSS_PIN;
    usart_gpio_init.mode = GPIO_MODE_OUTPUT;
    usart_gpio_init.output_type = GPIO_OUTPUT_PUSHPULL;
    std_gpio_init(NSS_PORT, &usart_gpio_init);
    
    /* 初始化主设备NSS输出引脚 */
    std_gpio_set_pin(NSS_PORT,NSS_PIN);
}


/**
* @brief  USART1初始化
* @retval 无
*/
void usart1_init(void)
{
    /* USART1时钟使能 */
    std_rcc_apb2_clk_enable(RCC_PERIPH_CLK_USART1);
    
    std_usart_init_t usart_config = {0};
    std_usart_sync_clk_init_t usart_sync_clock_config = {0};

    /* 禁止外设 */
    std_usart_disable(USART1);    
    /* 设置为大端 */
    std_usart_set_data_order(USART1,USART_DATA_ORDER_MSBFIRST);   
    
    /* 注意：同步模式下奇偶校验配置为无校验 */
    usart_config.direction = USART_DIRECTION_SEND_RECEIVE;
    usart_config.baudrate = 115200;
    usart_config.wordlength = USART_WORDLENGTH_8BITS;
    usart_config.stopbits = USART_STOPBITS_1;
    usart_config.parity = USART_PARITY_NONE;
    usart_config.hardware_flow = USART_FLOWCONTROL_NONE;
    
    /* USART初始化 */   
    if(STD_OK != std_usart_init(USART1,&usart_config))
    {
        /* 波特率配置不正确处理代码 */
        while(1);
    }   
    
    /* USART配置为同步主模式 */
    usart_sync_clock_config.sync_clk = USART_CLOCK_ENABLE;
    usart_sync_clock_config.sync_clock_polarity = USART_SYNC_POLARITY_LOW;
    usart_sync_clock_config.sync_clock_phase = USART_SYNC_PHASE_1EDGE;
    
    /* USART同步时钟初始化 */
    std_usart_sync_clk_init(USART1,&usart_sync_clock_config);
    std_usart_enable(USART1);    
}



