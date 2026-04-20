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
* @brief  DMA CH1 配置为LPUART发送，CH0 配置为LPUART接收
* @retval 无
*/
void dma_init(void)
{
    std_dma_init_t dma_config={0};
    
    std_rcc_ahb_clk_enable(RCC_PERIPH_CLK_DMA);
    
    /*  DMA CH1 配置为 LPUART 发送通道 */
    dma_config.dma_channel = DMA_CHANNEL_1;
    dma_config.dma_req_id = DMA_REQUEST_LPUART1_TX;
    dma_config.transfer_type = DMA_BLOCK_TRANSFER;
    dma_config.mode = DMA_MODE_NORMAL;
    dma_config.data_size = DMA_DATA_SIZE_BYTE;
    dma_config.src_addr_inc = DMA_SRC_INC_ENABLE;
    dma_config.dst_addr_inc = DMA_DST_INC_DISABLE;
    
    /* DMA初始化 */
    std_dma_init(&dma_config);
    
    /*  DMA CH0 配置为 LPUART 接收通道 */
    dma_config.dma_channel = DMA_CHANNEL_0;
    dma_config.dma_req_id = DMA_REQUEST_LPUART1_RX;
    dma_config.transfer_type = DMA_BLOCK_TRANSFER;
    dma_config.mode = DMA_MODE_NORMAL;
    dma_config.data_size = DMA_DATA_SIZE_BYTE;
    dma_config.src_addr_inc = DMA_SRC_INC_DISABLE;
    dma_config.dst_addr_inc = DMA_DST_INC_ENABLE;
    
    /* DMA初始化 */
    std_dma_init(&dma_config);
}

/**
* @brief  配置LPUART通信用GPIO
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
* @brief  LPUART初始化配置，时钟源为系统时钟
* @retval 无
*/
void lpuart_init(void)
{
    std_lpuart_init_t lpuart_config = {0};
    
    /* 配置LPUART 时钟源为系统时钟 */
    std_rcc_set_lpuart1clk_source(RCC_LPUART1_ASYNC_CLK_SRC_SYSCLK);
    
    /* 配置LPUART 外设时钟 */
    std_rcc_apb1_clk_enable(RCC_PERIPH_CLK_LPUART1);
    
    /* LPUART 模块初始化 */
    lpuart_config.prescaler = LPUART_PRESCALER_DIV16; /* 配置预分频系数支持较低通信波特率 */
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
}

