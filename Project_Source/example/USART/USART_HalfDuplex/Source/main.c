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

/*-------------------------------------------functions------------------------------------------*/
int main(void)
{    
    /* 配置系统时钟 */
    system_clock_config();
    
    /* USART 单线半双模式 I/O口初始化 */
    gpio_init();

    /* UASRT1 UART2 通用配置初始化 */    
    usart_init();  
    
    /* UASRT1 UART2 使能单线半双工 */    
    bsp_usart_set_half_duplex(USART1); 
    bsp_usart_set_half_duplex(UART2); 

    /* 执行USART1 发送8个数，UART2 接收8个数 */
    bsp_usart1_tx_uart2_rx();
    /* 执行UART2 发送8个数，USART1 接收8个数 */
    bsp_uart2_tx_usart1_rx();
    
    while(1)
    {
    }
}
