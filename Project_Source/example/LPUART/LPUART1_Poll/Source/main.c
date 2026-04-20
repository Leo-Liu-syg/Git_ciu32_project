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
__IO  uint8_t  g_tx_data[8] = {0x11, 0x22, 0x33, 0x44, 0x55, 0x66, 0x77, 0x88};
__IO  uint32_t g_rx_data = 0;
/*-------------------------------------------functions------------------------------------------*/

int main(void)
{   
    uint16_t i = 0;
    
    /* 配置系统时钟 */
    system_clock_config();
    
    /* GPIO初始化 */
    gpio_init();
    
    /* LPUART 外设初始化 */
    lpuart_init();

    /* LPUART 查询方式发送8个数据 */
    for (i = 0; i < 8; i++)
    {
        while(!(std_lpuart_get_flag(LPUART1,LPUART_FLAG_TXE)))
        {
        }
        std_lpuart_tx_data_write(LPUART1,(uint32_t)g_tx_data[i]);
    }
    /* 等待发送完成 */
    while(!(std_lpuart_get_flag(LPUART1,LPUART_FLAG_TC)))
    {
    }
    
    while(1)
    {
        /* LPUART 查询方式接收，*/
        while(!(std_lpuart_get_flag(LPUART1,LPUART_FLAG_RXNE)))
        {
        }
        g_rx_data = (uint8_t)std_lpuart_rx_data_read(LPUART1);
    }
}

