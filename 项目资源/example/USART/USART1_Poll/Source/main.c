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
__IO uint32_t g_rx_data = 0;

/*-------------------------------------------functions------------------------------------------*/
int main(void)
{
    uint16_t i = 0;
    
    /* 配置系统时钟 */
    system_clock_config();
    
    /* USART GPIO配置 */
    gpio_init();

    /* UASRT初始化 */    
    usart_init();  

    /* USART 查询发送8个数据 */
    for(i = 0; i < 8; i++)
    {
        while(!(std_usart_get_flag(USART1,USART_FLAG_TXE)))
        {
        }
        std_usart_tx_write_data(USART1,(uint32_t)g_send_buffer[i]);
    }    
    
    /* 等待发送完成 */
    while(!(std_usart_get_flag(USART1,USART_FLAG_TC)))
    {
    }
    
    while(1)
    {
        /* USART 查询接收 */
        if(std_usart_get_flag(USART1,USART_FLAG_RXNE))
        {
            g_rx_data = (uint8_t)std_usart_rx_read_data(USART1);
        }
    }
}



