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
/*------------------------------------------variables-------------------------------------------*/
__IO uint8_t g_flag_tx_complete = 0;     /* 发送完成标志 */
/*-------------------------------------------functions------------------------------------------*/
int main(void)
{
    /* 配置系统时钟 */
    system_clock_config();

    /* GPIO 初始化 */
    gpio_init();
    
    /* LPUART 外设初始化 */
    lpuart_init();
    
    /* NVIC配置 */
    nvic_init();
    
    /* 配置并使能中断发送 */
    bsp_lpuart_tx_interrupt();
                  
    /* 开启接收中断 */
    std_lpuart_cr1_interrupt_enable(LPUART1,LPUART_CR1_INTERRUPT_RXNE);
    
    /* 等待发送完成 */
    while (0 == g_flag_tx_complete)
    {
    }
    while(1)
    {
    }
}


