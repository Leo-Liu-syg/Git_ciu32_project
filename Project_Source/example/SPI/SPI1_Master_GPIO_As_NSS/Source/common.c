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
    std_gpio_init_t spi_gpio_init = {0};
    std_gpio_init_t nss_gpio_init = {0};
    
    /* GPIO外设时钟使能 */  
    std_rcc_gpio_clk_enable(RCC_PERIPH_CLK_GPIOB);
    std_rcc_gpio_clk_enable(RCC_PERIPH_CLK_GPIOD);
    /* SPI1 GPIO 配置    
        PB3     ------> SPI1_SCK
        PB4     ------> SPI1_MISO
        PB5     ------> SPI1_MOSI
    */
    spi_gpio_init.pin = GPIO_PIN_4 | GPIO_PIN_5;
    spi_gpio_init.mode = GPIO_MODE_ALTERNATE;
    spi_gpio_init.pull = GPIO_NOPULL;
    spi_gpio_init.output_type = GPIO_OUTPUT_PUSHPULL;
    spi_gpio_init.alternate = GPIO_AF0_SPI1;
    std_gpio_init(GPIOB, &spi_gpio_init);
    
    spi_gpio_init.pin = GPIO_PIN_3;
    spi_gpio_init.mode = GPIO_MODE_ALTERNATE;
    spi_gpio_init.pull = GPIO_PULLUP;
    spi_gpio_init.output_type = GPIO_OUTPUT_PUSHPULL;
    spi_gpio_init.alternate = GPIO_AF0_SPI1;
    std_gpio_init(GPIOB, &spi_gpio_init);


    /* NSS GPIO 配置    
        PD2     ------> SPI1_NSS
    */
    nss_gpio_init.pin = GPIO_PIN_2;
    nss_gpio_init.mode = GPIO_MODE_OUTPUT;
    nss_gpio_init.pull = GPIO_PULLUP;
    nss_gpio_init.output_type = GPIO_OUTPUT_PUSHPULL;
    std_gpio_init(GPIOD, &nss_gpio_init);  
    
    /* NSS默认高电平 */
    std_gpio_set_pin(GPIOD, GPIO_PIN_2);
}


/**
* @brief  SPI1初始化
* @retval 无
*/
void spi1_init(void)
{
    std_spi_init_t spi_init = {0};
    
    /* SPI1时钟使能 */
    std_rcc_apb2_clk_enable(RCC_PERIPH_CLK_SPI1);
    
    /* 禁止SPI */
    std_spi_disable(SPI1);
    
    /* SPI结构体初始化 */
    spi_init.mode = SPI_MODE_MASTER;
    spi_init.baud_rate_prescaler = SPI_BAUDRATEPRESCALER_32;
    spi_init.clk_polarity = SPI_POLARITY_HIGH;
    spi_init.clk_phase = SPI_PHASE_1EDGE;
    std_spi_init(SPI1, &spi_init); 
    /* 数据位传输顺序设置为高位优先 */
    std_spi_set_first_bit(SPI1, SPI_FIRSTBIT_MSB);
    /* SPI主机模式，配置NSS输出使能 */   
    std_spi_nss_output_enable(SPI1);
    
    /* SPI使能 */
    std_spi_enable(SPI1);
}



