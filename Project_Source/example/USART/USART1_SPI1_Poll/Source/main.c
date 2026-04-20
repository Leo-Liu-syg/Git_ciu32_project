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
__IO uint8_t g_recv_buffer[8] = {0x00};

/*-------------------------------------------functions------------------------------------------*/
int main(void)
{
    uint32_t index=0;
    
    /* 配置系统时钟 */
    system_clock_config();
  
    /* GPIO初始化 */
    gpio_init();

    /* UASRT初始化 */
    usart1_init();
    
    /* 拉低片选(NSS) */
    std_gpio_reset_pin(NSS_PORT,NSS_PIN);

    /* 主机数据通信 */
    for(index = 0; index < 8; index++)
    {
        /* 等待发送数据寄存器为空 */      
        while(!std_usart_get_flag(USART1,USART_FLAG_TXE))
        {
        }
        /* 发送数据 */
        std_usart_tx_write_data(USART1,(uint32_t)g_send_buffer[index]);
        
        /* 等待接收数据寄存器非空 */
        while(!std_usart_get_flag(USART1,USART_FLAG_RXNE))
        {         
        }       
        /* 接收数据 */
        g_recv_buffer[index] = (uint8_t)std_usart_rx_read_data(USART1);
    }
    /* 交互完成拉高片选(NSS) */
    std_gpio_set_pin(NSS_PORT,NSS_PIN);
    
    while(1)
    {

    }
}

