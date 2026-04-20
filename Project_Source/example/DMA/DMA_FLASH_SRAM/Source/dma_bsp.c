/************************************************************************************************/
/**
* @file               dma_bsp.c
* @author             MCU Ecosystem Development Team
* @brief              DMA BSP函数，实现DMA配置、DMA中断服务等功能。
*
*
**************************************************************************************************
* @attention
* Copyright (c) CEC Huada Electronic Design Co.,Ltd. All rights reserved.
*
**************************************************************************************************
*/

/*------------------------------------------includes--------------------------------------------*/
#include "dma_bsp.h"

/*-------------------------------------------functions------------------------------------------*/
/**
* @brief  DMA通道0中断服务函数
* @retval 无
*/
void DMA_Channel0_IRQHandler(void)
{
    /* DMA传输完成中断服务 */
    if((std_dma_get_interrupt_enable(DMA_CHANNEL_0, DMA_INTERRUPT_TF)) && (std_dma_get_flag(DMA_FLAG_TF0)))
    {
        std_dma_interrupt_disable(DMA_CHANNEL_0, DMA_INTERRUPT_TF);
        std_dma_clear_flag(DMA_CLEAR_TF0);
        
        /* 根据应用需求，添加不同应用程序代码 */
        g_complete = 1;
    }
}


/**
* @brief  DMA 配置函数,配置DMA 源地址、目的地址和传输数据大小，并使能DMA
* @retval 无
*/
void bsp_dma_config(uint32_t *source, uint32_t *destination, uint32_t number)
{
    std_dma_config_t dma_config = {0};

    dma_config.src_addr    = (uint32_t)source;
    dma_config.dst_addr    = (uint32_t)destination;
    dma_config.data_number = number;
    dma_config.dma_channel = DMA_CHANNEL_0;
    std_dma_start_transmit(&dma_config);
}

