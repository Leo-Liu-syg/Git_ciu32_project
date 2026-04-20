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
__IO uint8_t g_local_addr = 0x03;
__IO uint8_t g_rx_temp = 0;
/*-------------------------------------------functions------------------------------------------*/
int main(void)
{
    /* 配置系统时钟 */
    system_clock_config();
    
    /* Systick初始化 */
    std_delay_init();
    
    /* LED1初始化 */
    led_init();
    
    /* 用户按键初始化 */  
    exti_init();
    
    /* USART1、UART2 GPIO口初始化 */
    gpio_init();
    
    /* UASRT1、UART2 通用配置初始化 */    
    usart_init();  
    
    /* USART1配置为多机通信，地址匹配模式 */
    bsp_set_usart_multiprocessor_mode(USART1,g_local_addr);
    
    /*发送静默请求USART1进入静默模式 */
    std_usart_request_enter_mutemode(USART1);
    /* 确认进入静默模式，静默标志置1 */
    while(!std_usart_get_flag(USART1,USART_FLAG_RWU))
    {
    }
    
    while(1)
    {
        /* UART2向USART1 发送数据 */
        while (!std_usart_get_flag(UART2, USART_FLAG_TXE));
        std_usart_tx_write_data(UART2, 0x55);
        /* 等待发送完成 */
        while (!std_usart_get_flag(UART2, USART_FLAG_TC));
        
        /* 按键按下后UART2向USART1发送匹配地址，USART1退出静默模式 */ 
        /* 从静默模式退出后，将接收数据存入缓冲区 */
        if(std_usart_get_flag(USART1, USART_FLAG_RXNE))
        {
            g_rx_temp = (uint8_t)(std_usart_rx_read_data(USART1)& 0xFF);
            /* 避免编译报警 */  
            (void)g_rx_temp; 
        }
        if(!std_usart_get_flag(USART1, USART_FLAG_RWU))
        {
            LED_RED_FLIP();
            std_delayms(300);            
        } 
        else
        {
            LED1_OFF(); 
        }            
    }
}



