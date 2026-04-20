/************************************************************************************************/
/**
* @file               adc_bsp.c
* @author             MCU Ecosystem Development Team
* @brief              ADC BSP驱动函数。
*
*
**************************************************************************************************
* @attention
* Copyright (c) CEC Huada Electronic Design Co.,Ltd. All rights reserved.
*
**************************************************************************************************
*/

/*-------------------------------------------includes-------------------------------------------*/
#include "adc_bsp.h"
/*-------------------------------------------variables------------------------------------------*/
__IO uint8_t g_adc_complete = 0;
__IO uint16_t g_dma_result;

/*-------------------------------------------functions------------------------------------------*/
/**
* @brief  DMA通道0中断服务函数
* @retval 无
*/
void DMA_Channel0_IRQHandler(void)
{ 
    if((std_dma_get_interrupt_enable(DMA_CHANNEL_0, DMA_INTERRUPT_TF)) && (std_dma_get_flag(DMA_FLAG_TF0)))
    {
        /* 清除DMA通道转换完成标志 */
        std_dma_clear_flag(DMA_FLAG_TF0);
        g_adc_complete = 1;
    }
}

/**
* @brief  配置DMA源地址、目的地址和传输数据大小，并使能DMA
* @retval 无
*/
void bsp_adc_dma_config(void)
{
    std_dma_config_t dma_config = {0};
    
    dma_config.dma_channel = DMA_CHANNEL_0;
    dma_config.src_addr = (uint32_t)&ADC->DR;
    dma_config.dst_addr = (uint32_t)&g_dma_result;
    dma_config.data_number = 1;
    std_dma_start_transmit(&dma_config);
}
