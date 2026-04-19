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

/*-------------------------------------------functions------------------------------------------*/
uint8_t g_tx_data[8] = {0x11, 0x22, 0x33, 0x44, 0x55, 0x66, 0x77, 0x88};
uint8_t g_uart2_rx_data[8] = {0};
uint8_t g_usart1_rx_data[8] = {0};

/**
* @brief  配置USART为单线半双工模式
* @retval 无
*/
void bsp_usart_set_half_duplex(USART_t *usartx)
{
    std_usart_disable(usartx);
    std_usart_half_duplex_enable(usartx);
    std_usart_enable(usartx);
}    

/**
* @brief  配置USART1 发送 -> UART2 接收流程
* @retval 无
*/
void bsp_usart1_tx_uart2_rx(void)
{    
    for(int i = 0; i < 8; i++)
    {
        /* USART1 查询发送 */
        while(!(std_usart_get_flag(USART1,USART_FLAG_TXE)))
        {
        }
        std_usart_tx_write_data(USART1, (uint32_t)g_tx_data[i]);
        
        /* UART2 查询接收 */
        while(!(std_usart_get_flag(UART2,USART_FLAG_RXNE)))
        {
        }        
        g_uart2_rx_data[i]= (uint8_t)(std_usart_rx_read_data(UART2) & 0xFF);
    }
}

/**
* @brief  配置UART2 发送 -> USART1 接收流程
* @retval 无
*/
void bsp_uart2_tx_usart1_rx(void)
{    
    for(int i = 0; i < 8; i++)
    {
        /* UART2查询发送 */
        while(!(std_usart_get_flag(UART2,USART_FLAG_TXE)))
        {
        }
        std_usart_tx_write_data(UART2, (uint32_t)g_tx_data[i]);
        
        /* USART1 查询接收 */
        while(!(std_usart_get_flag(USART1,USART_FLAG_RXNE)))
        {
        }
        g_usart1_rx_data[i]=  (uint8_t)(std_usart_rx_read_data(USART1) & 0xFF);
    }
}



