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
__IO uint8_t g_flag_tx_complete = 0;     /* 发送完成标志 */
/*-------------------------------------------functions------------------------------------------*/
int main(void)
{    
    /* 配置系统时钟 */
    system_clock_config();
    
    /* USART 通信IO口初始化 */
    gpio_init();

    /* UASRT初始化 */    
    usart_init();  

    /* 配置并使能中断发送 */
    bsp_usart_tx_interrupt();    
    
    /* 开启接收中断 */
    std_usart_cr1_interrupt_enable(USART1,USART_CR1_INTERRUPT_RXNE);
    
    /* 等待发送完成 */
    while (0 == g_flag_tx_complete)
    {
    }
    
    while(1)
    {
    }
}



