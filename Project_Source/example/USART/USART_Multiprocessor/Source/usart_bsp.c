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

/*------------------------------------------variables-------------------------------------------*/
extern __IO uint8_t g_local_addr;
/*-------------------------------------------functions------------------------------------------*/
/**
* @brief  EXTI4_15中断服务函数
* @retval 无
*/
void EXTI4_15_IRQHandler(void)
{
    uint8_t match_addr = 0;
    if (std_exti_get_pending_status(EXTI_LINE_GPIO_PIN13))
    {
        std_exti_clear_pending(EXTI_LINE_GPIO_PIN13);
    }
    /* 在按键中断中 UART2向USART1发送匹配地址 */
    match_addr = g_local_addr | 0x80;
    /* 等待上一帧数据发送完成 */
    while(!std_usart_get_flag(UART2,USART_FLAG_TC))
    {
    }
    /* 向USART1发送匹配地址*/
    std_usart_tx_write_data(UART2, match_addr);
    /* 等待数据发送完成 */
    while(!std_usart_get_flag(UART2,USART_FLAG_TC))
    {
    }
}

/**
* @brief  配置USART 为多机通信模式，使用地址匹配退出静默
* @param  usartx USART外设
* @param  addr   USART的本地地址
* @retval 无
*/
void bsp_set_usart_multiprocessor_mode(USART_t *usartx, uint8_t addr)
{
    /* 外设禁用 */    
    std_usart_disable(usartx);
    /* 配置USART 为多机通信模式 */
    std_usart_multi_mode_enable(usartx);
    /* 配置USART 为地址匹配退出静默模式*/
    std_usart_set_mute_exit_mode(usartx, USART_MUTE_EXIT_ADDRESSMARK);
    /* 配置USART地址匹配模式为7bits匹配 */
    std_usart_set_addr_match(usartx,USART_DETECT_7BITS);
    /* 配置本地地址 */
    std_usart_set_local_address(usartx, (uint32_t)addr);
    /* 外设使能 */    
    std_usart_enable(usartx);
}



