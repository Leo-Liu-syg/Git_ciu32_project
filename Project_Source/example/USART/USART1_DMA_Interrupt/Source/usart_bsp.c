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

/*-------------------------------------------functions------------------------------------------*/
/**
* @brief  USART1中断服务函数
* @retval 无
*/
void USART1_IRQHandler(void)
{
     /* 检查发送完成中断 */
    if(std_usart_get_flag(USART1,USART_FLAG_TC) 
        && std_usart_get_cr1_interrupt_enable(USART1,USART_CR1_INTERRUPT_TC))
    {
        std_usart_clear_flag(USART1,USART_CLEAR_TC);
        std_usart_cr1_interrupt_disable(USART1,USART_CR1_INTERRUPT_TC);
        
        /* 自定义逻辑代码 */
        g_complete = 1;
    }
}

/**
* @brief  DMA配置函数
* @param  source DMA源地址
* @param  number DMA传输字符数
* @retval 无
*/
void bsp_usart_dma_config(uint8_t *source,uint32_t number)
{
    std_dma_config_t dma_config = {0};

    /* 配置DMA 源地址、目的地址和传输数据大小，并使能DMA  */
    dma_config.src_addr = (uint32_t)source;
    dma_config.dst_addr = (uint32_t)&USART1->TDR;
    dma_config.data_number = number;
    dma_config.dma_channel = DMA_CHANNEL_1;
    std_dma_start_transmit(&dma_config);
}



