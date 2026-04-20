/************************************************************************************************/
/**
* @file               main.c
* @author             MCU Ecosystem Development Team
* @brief              应用程序主流程。
*                     
*
**************************************************************************************************
* @attention
* Copyright (c) CEC Huada Electronic Design Co.,Ltd. All rights reserved.
*
**************************************************************************************************
*/

/*------------------------------------------includes--------------------------------------------*/
#include "main.h"

/*--------------------------------------------variables-----------------------------------------*/
uint8_t g_recv_buffer[8]={0x00};

/*-------------------------------------------functions------------------------------------------*/
int main(void)
{
    /* 配置系统时钟 */
    system_clock_config();

    /* DMA初始化 */
    dma_init();
    
    /* GPIO初始化 */
    gpio_init();

    /* UASRT1初始化 */
    usart1_init();

    /* 配置USART DMA接收相关信息 */
    bsp_usart_dma_config(g_recv_buffer,8);
    
    /* 使能USART DMA接收 */
    std_usart_dma_rx_enable(USART1);
    
    /* 等待DMA中断接收完成 */
    while(g_complete == 0)
    {
        
    }
    
    while(1)
    {

    }
}












