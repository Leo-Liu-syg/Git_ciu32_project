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
uint8_t g_send_buffer[8] = {0x01,0x02,0x03,0x04,0x05,0x06,0x07,0x08};
__IO uint32_t g_complete = 0;

/*-------------------------------------------functions------------------------------------------*/
int main(void)
{
    /* 配置系统时钟 */
    system_clock_config();
  
    /* 初始化DMA */
    dma_init();
    
    /* GPIO初始化 */
    gpio_init();

    /* UASRT1初始化 */
    usart1_init();
    
    /* 配置USART DMA发送8个数 */
    bsp_usart_dma_config(g_send_buffer,8);
    
    /* 使能USART DMA发送 */
    std_usart_dma_tx_enable(USART1);
    
    /* 使能完成中断 */
    std_usart_cr1_interrupt_enable(USART1,USART_CR1_INTERRUPT_TC);
    
    /* 等待中断发送完成 */
    while(g_complete == 0)
    {
        
    }
    
    while(1)
    {

    }
}


