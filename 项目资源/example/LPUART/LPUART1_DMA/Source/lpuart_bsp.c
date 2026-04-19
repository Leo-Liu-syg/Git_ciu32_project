/************************************************************************************************/
/**
* @file               lpuart_bsp.c
* @author             MCU Ecosystem Development Team
* @brief              LPUART BSP驱动函数，实现LPUART功能配置
*
*
**************************************************************************************************
* @attention
* Copyright (c) CEC Huada Electronic Design Co.,Ltd. All rights reserved.
*
**************************************************************************************************
*/

/*------------------------------------------includes--------------------------------------------*/
#include "lpuart_bsp.h"

/*-------------------------------------------functions------------------------------------------*/
uint8_t tx_data[8] = {0x11, 0x22, 0x33, 0x44, 0x55, 0x66, 0x77, 0x88};
uint8_t rx_data[8] = {0};

/**
* @brief  LPUART DMA发送配置及发送完成检查
* @retval 无
*/
void  bsp_lpuart_dma_tx(void)
{
    std_dma_config_t dma_tx_config = {0};
    
    /* LPUART DMA 发送配置 */
    dma_tx_config.src_addr = (uint32_t)tx_data;
    dma_tx_config.dst_addr = (uint32_t)&LPUART1->TDR;
    dma_tx_config.data_number = 8;
    dma_tx_config.dma_channel = DMA_CHANNEL_1;
    
    /* DMA 发送配置 */
    std_dma_start_transmit(&dma_tx_config);
    
    /* LPUART DMA请求使能 */
    std_lpuart_dma_tx_enable(LPUART1);
    
    /* 等待DMA传输完成 */
    while (!std_dma_get_flag(DMA_FLAG_TF1))
    {
    }
    
    /* 等待LPUART 发送完成 */
    while (!std_lpuart_get_flag(LPUART1,LPUART_FLAG_TC))
    {
    }
}

/**
* @brief  LPUART DMA接收配置及接收完成检查
* @retval 无
*/
void  bsp_lpuart_dma_rx(void)
{
    std_dma_config_t dma_rx_config = {0};
    
    /* LPUART DMA 接收配置 */
    dma_rx_config.src_addr = (uint32_t)&LPUART1->RDR;
    dma_rx_config.dst_addr = (uint32_t)rx_data;
    dma_rx_config.data_number = 8;
    dma_rx_config.dma_channel = DMA_CHANNEL_0;
    
    /* DMA 接收配置 */
    std_dma_start_transmit(&dma_rx_config);
    
    /* LPUART DMA请求使能 */
    std_lpuart_dma_rx_enable(LPUART1);
    
    /* 等待DMA传输完成 */
    while (!std_dma_get_flag(DMA_FLAG_TF0))
    {
    }
}

