/************************************************************************************************/
/**
* @file               usart_bsp.c
* @author             MCU Ecosystem Development Team
* @brief              USART BSP函数，实现USART配置、DMA中断服务等功能。
*
*
**************************************************************************************************
* @attention
* Copyright (c) CEC Huada Electronic Design Co.,Ltd. All rights reserved.
*
**************************************************************************************************
*/

/*------------------------------------------includes--------------------------------------------*/
#include "usart_bsp.h"

/*--------------------------------------------variables-----------------------------------------*/
__IO uint32_t g_complete = 0;

/*-------------------------------------------functions------------------------------------------*/
/**
* @brief  DMA通道中断服务函数
* @retval 无
*/
void DMA_Channel1_IRQHandler(void)
{
    if(std_dma_get_flag(DMA_FLAG_TF1))
    {
        std_dma_interrupt_disable(DMA_CHANNEL_1,DMA_INTERRUPT_TF); 
        std_dma_clear_flag(DMA_FLAG_TF1);
        
        /* 自定义逻辑 */
        g_complete = 1;
    }
}


/**
* @brief  DMA配置函数
* @param  source DMA 传输源地址
* @param  number DMA 传输字符数
* @retval 无
*/
void bsp_usart_dma_config(uint8_t *source,uint32_t number)
{
    std_dma_config_t dma_config = {0};

    /* 配置DMA 源地址、目的地址和传输数据大小，并使能DMA */
    dma_config.src_addr = (uint32_t)source;
    dma_config.dst_addr = (uint32_t)&USART1->TDR;
    dma_config.data_number = number;
    dma_config.dma_channel = DMA_CHANNEL_1;
    std_dma_start_transmit(&dma_config); 
}



