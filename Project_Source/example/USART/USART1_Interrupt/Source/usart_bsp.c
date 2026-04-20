/************************************************************************************************/
/**
* @file               usart_bsp.c
* @author             MCU Ecosystem Development Team
* @brief              USART BSP函数，实现USART配置等功能。
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

/*--------------------------------------------variables-----------------------------------------*/
extern __IO uint8_t g_flag_tx_complete;     /* 发送完成全局标志 */

__IO uint8_t *g_tx_buffer = 0;            /* 发送缓冲区指针   */
__IO uint8_t g_tx_size = 0;               /* 发送数据总数     */
__IO uint8_t g_tx_count = 0;              /* 已发送数据个数   */
__IO uint8_t *g_rx_buffer = 0;            /* 接收缓冲区指针   */
__IO uint8_t g_rx_size = 0;               /* 接收数据总数     */
__IO uint8_t g_rx_count = 0;              /* 已接收数据个数   */

__IO uint8_t g_temp_rx = 0;               /* 接收数据缓冲区   */
uint8_t g_send_buffer[8] = {0x11, 0x22, 0x33, 0x44, 0x55, 0x66, 0x77, 0x88};
/*-------------------------------------------functions------------------------------------------*/


/**
* @brief  USART1 中断服务函数
* @retval 无
*/
void USART1_IRQHandler(void)
{
    /* 检查上溢错误，RXNE或ERR中断使能均可产生ORE中断*/
    if((std_usart_get_cr1_interrupt_enable(USART1,USART_CR1_INTERRUPT_RXNE)    
       || std_usart_get_cr3_interrupt_enable(USART1,USART_CR3_INTERRUPT_ERR))  
       && std_usart_get_flag(USART1,USART_FLAG_ORE))
    {
        /* 清除上溢错误标志 */
        std_usart_clear_flag(USART1,USART_CLEAR_ORE);
        /* 用户可根据实际使用场景，自定义错误处理流程 */        
    } 
    /* 检查到接收寄存器非空标志后， 读取接收数据 */    
    if(std_usart_get_cr1_interrupt_enable(USART1,USART_CR1_INTERRUPT_RXNE)    
       && std_usart_get_flag(USART1,USART_FLAG_RXNE))
    {
        g_temp_rx = (uint8_t)std_usart_rx_read_data(USART1);
    }
    /* 检查发送寄存器为空标志置1时，写入发送数据 */    
    if(std_usart_get_cr1_interrupt_enable(USART1,USART_CR1_INTERRUPT_TXE)     
       && std_usart_get_flag(USART1,USART_FLAG_TXE))
    {
        std_usart_tx_write_data(USART1, (uint32_t)g_tx_buffer[g_tx_count]);
        g_tx_count++;
        if (8 == g_tx_count)
        {
            /* 完成发送后，关闭TXE中断，开启发送完成中断 */    
            std_usart_cr1_interrupt_disable(USART1,USART_CR1_INTERRUPT_TXE);
            std_usart_cr1_interrupt_enable(USART1,USART_CR1_INTERRUPT_TC);
        }
    }
    /* 检测到发送完成标志置1，将全局标志置1 */    
    if(std_usart_get_cr1_interrupt_enable(USART1,USART_CR1_INTERRUPT_TC)     
       && std_usart_get_flag(USART1,USART_FLAG_TC))
    {
        std_usart_clear_flag(USART1,USART_FLAG_TC);
        std_usart_cr1_interrupt_disable(USART1,USART_CR1_INTERRUPT_TC);
        /* 将发送完成全局标志置1 */    
        g_flag_tx_complete = 1;
    }
}


/**
* @brief  USART1 配置并使能中断发送
* @retval 无
*/
void bsp_usart_tx_interrupt(void)
{
    g_tx_buffer = g_send_buffer;
    /* 预计发送8个数据 */
    g_tx_size = 8; 
    /* 初始化已发送数据个数 */    
    g_tx_count = 0;  
    /* 初始化已发送数据个数 */    
    g_flag_tx_complete = 0; 

    /* 开启发送中断 */
    std_usart_cr1_interrupt_enable(USART1, USART_CR1_INTERRUPT_TXE);
}


