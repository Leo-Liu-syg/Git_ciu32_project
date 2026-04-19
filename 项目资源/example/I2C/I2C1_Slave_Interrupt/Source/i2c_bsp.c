/************************************************************************************************/
/**
* @file               i2c_bsp.c
* @author             MCU Ecosystem Development Team
* @brief              I2C BSP驱动函数，实现I2C从模式中断方式通信。
*
*
**************************************************************************************************
* @attention
* Copyright (c) CEC Huada Electronic Design Co.,Ltd. All rights reserved.
*
**************************************************************************************************
*/

/*------------------------------------------includes--------------------------------------------*/
#include "i2c_bsp.h"

/*------------------------------------------define----------------------------------------------*/
#define BUFF_SIZE     (8U)

/*------------------------------------------variables-------------------------------------------*/
uint8_t g_tx_buffer[BUFF_SIZE] = {0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07, 0x08};
uint8_t g_rx_buffer[BUFF_SIZE] = {0};
uint32_t g_tx_count;
uint32_t g_rx_count;

/*------------------------------------------functions-------------------------------------------*/
/**
* @brief  I2C中断服务程序
* @retval 无
*/
void I2C1_IRQHandler(void)
{
    uint32_t current_status;
    
    /* 获取当前I2C1的状态 */
    current_status = I2C1->ISR;
    
    /* ADDRIE中断处理流程 */
    if (std_i2c_get_interrupt_enable(I2C1, I2C_INTERRUPT_ADDR) && ((current_status & I2C_FLAG_ADDR) == I2C_FLAG_ADDR))
    {
        /* 清除地址匹配中断标志位 */
        std_i2c_clear_flag(I2C1, I2C_FLAG_ADDR);
        
        /* 主机为读请求 */
        if(std_i2c_get_flag(I2C1, I2C_FLAG_DIR))
        {
            /* 使能发送中断 */
            std_i2c_interrupt_enable(I2C1, I2C_INTERRUPT_TX);
        }
        
        /* 主机为写请求 */
        else
        {
            /* 使能接收中断 */
            std_i2c_interrupt_enable(I2C1, I2C_INTERRUPT_RX);
        }
    }
    
    /* TXIE中断处理流程 */
    if (std_i2c_get_interrupt_enable(I2C1, I2C_INTERRUPT_TX) && ((current_status & I2C_FLAG_TXIS) == I2C_FLAG_TXIS))
    {
        if (g_tx_count < BUFF_SIZE)
        {
            std_i2c_transmit_byte(I2C1, g_tx_buffer[g_tx_count]);
            g_tx_count++;
        }
    }
    
    /* RXIE中断处理流程 */
    if (std_i2c_get_interrupt_enable(I2C1, I2C_INTERRUPT_RX) && ((current_status & I2C_FLAG_RXNE) == I2C_FLAG_RXNE))
    {
        if (g_rx_count < BUFF_SIZE)
        {
            g_rx_buffer[g_rx_count] = std_i2c_receive_byte(I2C1);
            g_rx_count++;
        }
    }
    
    /* NACKIE中断处理流程 */
    if (std_i2c_get_interrupt_enable(I2C1, I2C_INTERRUPT_NACK) && ((current_status & I2C_FLAG_NACK) == I2C_FLAG_NACK))
    {
        /* 清除NACKF */
        std_i2c_clear_flag(I2C1, I2C_FLAG_NACK);
    }
    
    /* STOPIE中断处理流程 */
    if (std_i2c_get_interrupt_enable(I2C1, I2C_INTERRUPT_STOP) && ((current_status & I2C_FLAG_STOP) == I2C_FLAG_STOP))
    {
        /* 清除STOPF */
        std_i2c_clear_flag(I2C1, I2C_FLAG_STOP);
    }
}

/**
* @brief  I2C中断配置
* @retval 无
*/
void bsp_i2c1_interrupt_config(void)
{
    /* 等待BUSY标志位清零 */
    while(std_i2c_get_flag(I2C1, I2C_FLAG_BUSY));
    
    /* 清除地址匹配标志、NACK标志和停止位标志 */
    std_i2c_clear_flag(I2C1, I2C_FLAG_ADDR | I2C_FLAG_NACK | I2C_FLAG_STOP);
    
    /* 使能地址匹配中断、接收NACK中断和停止位检测中断  */
    std_i2c_interrupt_enable(I2C1, I2C_INTERRUPT_ADDR | I2C_INTERRUPT_STOP | I2C_INTERRUPT_NACK);
}

