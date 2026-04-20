/************************************************************************************************/
/**
* @file               i2c_bsp.c
* @author             MCU Ecosystem Development Team
* @brief              I2C BSP驱动函数，实现I2C主模式通信。
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
#define SLAVE_ADDRESS          (0x5C)
#define DATA_LENGTH            (8U)

/*------------------------------------------variables-------------------------------------------*/
uint8_t g_tx_buffer[DATA_LENGTH] = {0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07, 0x08};
uint8_t g_rx_buffer[DATA_LENGTH];


/*------------------------------------------functions-------------------------------------------*/
/**
* @brief  I2C1查询方式发送数据流程
* @retval 无
*/
void bsp_i2c1_master_send(void)
{
    uint32_t i;
    
    /* 等待BUSY位清零 */
    while(std_i2c_get_flag(I2C1, I2C_FLAG_BUSY));
    
    /* 设置主机传输参数及读写请求 */
    std_i2c_master_transfer_config(I2C1, SLAVE_ADDRESS, DATA_LENGTH, I2C_CR2_AUTOEND);
    std_i2c_set_transfer_direction(I2C1, I2C_REQUEST_WRITE);
    
    /* 发送起始位 */
    std_i2c_start_condition_generate(I2C1);
    
    /* 发送流程 */
    for(i=0; i<DATA_LENGTH; i++)
    {
        while(!std_i2c_get_flag(I2C1, I2C_FLAG_TXIS));
        std_i2c_transmit_byte(I2C1, g_tx_buffer[i]);
    }
    
    /* 等待STOP位置起后清除该标志位 */
    while(!std_i2c_get_flag(I2C1, I2C_FLAG_STOP));
    std_i2c_clear_flag(I2C1, I2C_FLAG_STOP);
}

/**
* @brief  I2C1查询方式接收数据流程
* @retval 无
*/
void bsp_i2c1_master_receive(void)
{
    uint32_t i;
    
    /* 等待BUSY位清零 */
    while(std_i2c_get_flag(I2C1, I2C_FLAG_BUSY));    
    
    /* 设置主机传输参数及读写请求 */
    std_i2c_master_transfer_config(I2C1, SLAVE_ADDRESS, DATA_LENGTH, I2C_CR2_AUTOEND);
    std_i2c_set_transfer_direction(I2C1, I2C_REQUEST_READ);
    
    /* 发送起始位 */
    std_i2c_start_condition_generate(I2C1);
    
    /* 接收流程 */
    for(i=0; i<DATA_LENGTH; i++)
    {
        while(!std_i2c_get_flag(I2C1, I2C_FLAG_RXNE));
        g_rx_buffer[i] = std_i2c_receive_byte(I2C1);
    }
    
    /* 等待STOP位置起后清除该标志位 */
    while(!std_i2c_get_flag(I2C1, I2C_FLAG_STOP));
    std_i2c_clear_flag(I2C1, I2C_FLAG_STOP);
}
