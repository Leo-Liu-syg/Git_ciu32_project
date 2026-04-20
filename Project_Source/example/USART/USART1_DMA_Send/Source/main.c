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
uint8_t g_send_buffer[8]={0x01,0x02,0x03,0x04,0x05,0x06,0x07,0x08};

/*-------------------------------------------functions------------------------------------------*/
int main(void)
{
    /* 配置系统时钟 */
    system_clock_config();
    
    /* DMA初始化 */
    dma_init();
    
    /* GPIO初始化 */
    gpio_init();

    /* UASRT初始化 */
    usart1_init();

    /* 配置USART DMA发送相关信息 */
    bsp_usart_dma_config(g_send_buffer,8);
    
    /* 使能USART DMA发送 */
    std_usart_dma_tx_enable(USART1);
    
    /* 等待DMA传输完成 */
    while(g_complete == 0);
    
    /* DMA通道传输完成不代表全部数据发送完成，
       需等待TC标志置1，指示全部数据已发送完成 */
    while(!std_usart_get_flag(USART1,USART_FLAG_TC));
    
    while(1)
    {

    }
}


