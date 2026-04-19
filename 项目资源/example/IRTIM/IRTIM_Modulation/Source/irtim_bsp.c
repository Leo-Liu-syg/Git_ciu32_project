/************************************************************************************************/
/**
* @file               irtim_bsp.c
* @author             MCU Ecosystem Development Team
* @brief              IRTIM BSP驱动函数,实现IRTIM红外信号调制输出
*
*
**************************************************************************************************
* @attention
* Copyright (c) CEC Huada Electronic Design Co.,Ltd. All rights reserved.
*
**************************************************************************************************
*/

/*------------------------------------------includes--------------------------------------------*/
#include "irtim_bsp.h"

/*------------------------------------------variables-------------------------------------------*/
uint8_t tx_data[8] = {0x11, 0x22, 0x33, 0x44, 0x55, 0x66, 0x77, 0x88};
uint8_t rx_data[8] = {0};

/*------------------------------------------functions-------------------------------------------*/
/**
* @brief  使用IRTIM红外调制波形后发送接收数据
* @retval 无
*/
void bsp_irtim_modulation(void)
{
    /* 使能TIM4输出IRTIM载波信号 */
    std_tim_ccx_channel_enable(TIM4, TIM_CHANNEL_1);
    std_tim_enable(TIM4);
    
    /* UART4发送IRTIM调制信号，接收IRTIM解调信号 */
    for(uint8_t i=0; i<8; i++)
    {
        while(!std_usart_get_flag(UART4, USART_FLAG_TXE));
        std_usart_tx_write_data(UART4, tx_data[i]);
        
        while(!std_usart_get_flag(UART4, USART_FLAG_RXNE));
        rx_data[i] = std_usart_rx_read_data(UART4);       
    }
    
    while(!std_usart_get_flag(UART4, USART_FLAG_TC));
}
