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
* @brief  DMA初始化
* @retval 无
*/
void dma_init(void)
{
    std_dma_init_t dma_init_param = {0};
    
    /* DMA外设时钟使能 */
    std_rcc_ahb_clk_enable(RCC_PERIPH_CLK_DMA);
    
    /* DMA发送通道初始化 */
    dma_init_param.dma_channel = DMA_CHANNEL_1;
    dma_init_param.transfer_type = DMA_BLOCK_TRANSFER;
    dma_init_param.dma_req_id = DMA_REQUEST_SPI1_TX;
    dma_init_param.src_addr_inc = DMA_SRC_INC_ENABLE;
    dma_init_param.dst_addr_inc = DMA_DST_INC_DISABLE;
    dma_init_param.data_size = DMA_DATA_SIZE_BYTE;
    dma_init_param.mode = DMA_MODE_NORMAL;
    std_dma_init(&dma_init_param);
    
    /* DMA接收通道0 */
    dma_init_param.dma_channel = DMA_CHANNEL_0;
    dma_init_param.transfer_type = DMA_BLOCK_TRANSFER;
    dma_init_param.dma_req_id = DMA_REQUEST_SPI1_RX;  
    dma_init_param.src_addr_inc = DMA_SRC_INC_DISABLE;
    dma_init_param.dst_addr_inc = DMA_DST_INC_ENABLE;
    dma_init_param.data_size = DMA_DATA_SIZE_BYTE;
    dma_init_param.mode = DMA_MODE_NORMAL;
    std_dma_init(&dma_init_param);
    
}

/**
* @brief  GPIO初始化
* @retval 无
*/
void gpio_init(void)
{
    std_gpio_init_t spi_gpio_init = {0};
    
    /* GPIO外设时钟使能 */  
    std_rcc_gpio_clk_enable(RCC_PERIPH_CLK_GPIOB);
    std_rcc_gpio_clk_enable(RCC_PERIPH_CLK_GPIOD);
    /* SPI1 GPIO 配置    
        PD2     ------> SPI1_NSS
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
    
    spi_gpio_init.pin = GPIO_PIN_2;
    spi_gpio_init.mode = GPIO_MODE_ALTERNATE;
    spi_gpio_init.pull = GPIO_PULLUP;
    spi_gpio_init.output_type = GPIO_OUTPUT_PUSHPULL;
    spi_gpio_init.alternate = GPIO_AF2_SPI1;
    std_gpio_init(GPIOD, &spi_gpio_init);  
}


/**
* @brief  SPI1初始化
* @retval 无
*/
void spi1_init(void)
{
    std_spi_init_t spi_init = {0};
    
    /* SPI时钟使能 */
    std_rcc_apb2_clk_enable(RCC_PERIPH_CLK_SPI1);
    
    /* 禁止SPI */
    std_spi_disable(SPI1);
    
    /* SPI结构体初始化 */
    spi_init.mode = SPI_MODE_MASTER;
    spi_init.baud_rate_prescaler = SPI_BAUDRATEPRESCALER_32;
    spi_init.clk_polarity = SPI_POLARITY_HIGH;
    spi_init.clk_phase = SPI_PHASE_1EDGE;
    std_spi_init(SPI1,&spi_init); 
    
    /* 数据位传输顺序设置为高位优先 */
    std_spi_set_first_bit(SPI1, SPI_FIRSTBIT_MSB);
    
    /* SPI主机模式，配置NSS输出使能 */   
    std_spi_nss_output_enable(SPI1);
    
    /* SPI使能 */
    std_spi_enable(SPI1);
}

/**
* @brief  NVIC初始化
* @retval 无
*/
void nvic_init(void)
{
    /* SPI发送通道NVIC初始化 */
    NVIC_SetPriority(DMA_Channel1_IRQn, NVIC_PRIO_0);
    NVIC_EnableIRQ(DMA_Channel1_IRQn);    
    
    /* SPI接收通道NVIC初始化 */
    NVIC_SetPriority(DMA_Channel0_IRQn, NVIC_PRIO_1);
    NVIC_EnableIRQ(DMA_Channel0_IRQn);     
}

