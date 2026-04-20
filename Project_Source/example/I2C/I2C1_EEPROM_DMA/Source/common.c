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

/*-------------------------------------------define---------------------------------------------*/
#define SPEED_100K_TIMING       (0x10305397)
#define MASTER_ADDRESS          (0x4C)

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
* @brief  GPIO初始化配置
* @retval 无
*/
void gpio_init(void)
{
    std_gpio_init_t i2c_gpio_config = {0};
    
    /* 使能I2C GPIO时钟 */
    std_rcc_gpio_clk_enable(RCC_PERIPH_CLK_GPIOB);

    /* 
    I2C GPIO初始化
    I2C_SCL: PB8
    I2C_SDA: PB9
    */
    i2c_gpio_config.pin = GPIO_PIN_8 | GPIO_PIN_9;
    i2c_gpio_config.mode = GPIO_MODE_ALTERNATE;
    i2c_gpio_config.pull = GPIO_PULLUP;
    i2c_gpio_config.output_type = GPIO_OUTPUT_OPENDRAIN;
    i2c_gpio_config.alternate = GPIO_AF6_I2C1;
    std_gpio_init(GPIOB, &i2c_gpio_config);
}

/**
* @brief  DMA初始化
* @retval 无
*/
void dma_init(void)
{
    std_dma_init_t dma_i2c_tx_config = {0};
    std_dma_init_t dma_i2c_rx_config = {0};
    
    /* 使能DMA时钟 */
    std_rcc_ahb_clk_enable(RCC_PERIPH_CLK_DMA);

    /* I2C发送DMA通道初始化 */
    dma_i2c_tx_config.dma_channel = DMA_CHANNEL_1;
    dma_i2c_tx_config.dma_req_id = DMA_REQUEST_I2C1_TX;
    dma_i2c_tx_config.transfer_type = DMA_BLOCK_TRANSFER;
    dma_i2c_tx_config.mode = DMA_MODE_NORMAL;
    dma_i2c_tx_config.data_size = DMA_DATA_SIZE_BYTE;
    dma_i2c_tx_config.src_addr_inc = DMA_SRC_INC_ENABLE;
    dma_i2c_tx_config.dst_addr_inc = DMA_DST_INC_DISABLE;
    std_dma_init(&dma_i2c_tx_config);
    
    /* I2C接收DMA通道初始化 */
    dma_i2c_rx_config.dma_channel = DMA_CHANNEL_0;
    dma_i2c_rx_config.dma_req_id = DMA_REQUEST_I2C1_RX;
    dma_i2c_rx_config.transfer_type = DMA_BLOCK_TRANSFER;
    dma_i2c_rx_config.mode = DMA_MODE_NORMAL;
    dma_i2c_rx_config.data_size = DMA_DATA_SIZE_BYTE;
    dma_i2c_rx_config.src_addr_inc = DMA_SRC_INC_DISABLE;
    dma_i2c_rx_config.dst_addr_inc = DMA_DST_INC_ENABLE;
    std_dma_init(&dma_i2c_rx_config);
}

/**
* @brief  I2C1 主模式初始化
* @retval 无
*/
void i2c1_master_init(void)
{
    std_i2c_init_t i2c1_init_config = {0};
    
    /* 使能I2C1外设时钟 */
    std_rcc_apb1_clk_enable(RCC_PERIPH_CLK_I2C1);

    /* 禁止I2C1 */
    std_i2c_disable(I2C1);
    
    /* 初始化I2C */
    i2c1_init_config.timing = SPEED_100K_TIMING;
    i2c1_init_config.digital_filter = I2C_DIGITALFILTER_1CLK;
    i2c1_init_config.address1_length = I2C_ADDRESS1MODE_7BIT;
    i2c1_init_config.address1 = MASTER_ADDRESS;
    i2c1_init_config.acknowledge_type = I2C_DATA_ACK;
    std_i2c_init(I2C1, &i2c1_init_config);
}

