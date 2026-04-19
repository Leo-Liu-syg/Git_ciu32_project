/************************************************************************************************/
/**
* @file               spi_bsp.c
* @author             MCU Ecosystem Development Team
* @brief              SPI BSP驱动函数，实现SPI DMA配置及中断服务等功能。
*
*
**************************************************************************************************
* @attention
* Copyright (c) CEC Huada Electronic Design Co.,Ltd. All rights reserved.
*
**************************************************************************************************
*/

/*------------------------------------------includes--------------------------------------------*/
#include "spi_bsp.h"

/*--------------------------------------------variables-----------------------------------------*/
uint8_t g_send_buffer[8] = {0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07, 0x08};
uint8_t g_recv_buffer[8] = {0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00};
__IO uint32_t g_send_complete = 0;
__IO uint32_t g_recv_complete = 0;
__IO uint32_t g_exti_gpio_flag = 0;

/*-------------------------------------------functions------------------------------------------*/
/**
* @brief  DMA通道0中断服务函数
* @retval 无
*/
void DMA_Channel0_IRQHandler(void)
{
    if(std_dma_get_flag(DMA_FLAG_TF0))
    {
        std_dma_interrupt_disable(DMA_CHANNEL_0, DMA_INTERRUPT_TF);
        std_dma_clear_flag(DMA_FLAG_TF0);
        
        /* 根据需求添加逻辑代码 */
        g_recv_complete = 1;  
    }
}


/**
* @brief  DMA 通道1中断服务函数
* @retval 无
*/
void DMA_Channel1_IRQHandler(void)
{
    if(std_dma_get_flag(DMA_FLAG_TF1))
    {
        std_dma_interrupt_disable(DMA_CHANNEL_1, DMA_INTERRUPT_TF);
        std_dma_clear_flag(DMA_FLAG_TF1);
        
        /* 根据需求添加逻辑代码 */
        g_send_complete = 1;        
    }
}


/**
* @brief  SPI 主机DMA模式收发参数配置
* @retval 无
*/
void bsp_spi_dma_config(void)
{
    std_dma_config_t dma0_config = {0};
    std_dma_config_t dma1_config = {0};  
    
    /* 配置SPI发送, DMA传输通道 */    
    dma1_config.dma_channel = DMA_CHANNEL_1;
    dma1_config.data_number = 8;
    dma1_config.src_addr = (uint32_t)g_send_buffer;
    dma1_config.dst_addr = (uint32_t)&SPI1->DR;  
    std_dma_start_transmit(&dma1_config);
    
    /* 配置SPI接收 DMA传输通道 */
    dma0_config.dma_channel = DMA_CHANNEL_0;
    dma0_config.data_number = 8;
    dma0_config.src_addr = (uint32_t)&SPI1->DR;
    dma0_config.dst_addr = (uint32_t)g_recv_buffer;
    std_dma_start_transmit(&dma0_config);
    
    /* 使能传输完成中断 */
    std_dma_interrupt_enable(dma0_config.dma_channel, DMA_INTERRUPT_TF);
    std_dma_interrupt_enable(dma1_config.dma_channel, DMA_INTERRUPT_TF);    
    
}
