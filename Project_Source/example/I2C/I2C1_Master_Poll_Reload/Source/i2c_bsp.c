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


/*------------------------------------------functions-------------------------------------------*/
/**
* @brief  I2C1查询方式发送数据流程
* @param  slave_address I2C从机地址
* @param  send_data 发送数据缓冲区
* @param  send_size 发送数据长度
* @retval 无
*/
void bsp_i2c1_master_send(uint16_t slave_address, uint8_t *send_data, uint32_t send_size)
{
    uint32_t i;
    uint32_t reload_count;
    uint32_t autoend_count;
    
    /* 等待BUSY位清零 */
    while(std_i2c_get_flag(I2C1, I2C_FLAG_BUSY));
    
    /* 发送数据长度处理 */
    if(send_size > I2C_MAX_NBYTES_SIZE)
    {
        reload_count = send_size / I2C_MAX_NBYTES_SIZE;
        autoend_count = send_size % I2C_MAX_NBYTES_SIZE;
    }
    else
    {
        reload_count = 0;
        autoend_count = send_size;
    }
    
    /* 自动结束模式传输 */
    if(reload_count == 0)
    {
        /* 设置主机传输参数及读写请求 */
        std_i2c_master_transfer_config(I2C1, slave_address, autoend_count, I2C_CR2_AUTOEND);
        std_i2c_set_transfer_direction(I2C1, I2C_REQUEST_WRITE);
        
        /* 发送起始位 */
        std_i2c_start_condition_generate(I2C1);
        
        /* 发送流程 */
        for(i=0; i<autoend_count; i++)
        {
            while(!std_i2c_get_flag(I2C1, I2C_FLAG_TXIS));
            std_i2c_transmit_byte(I2C1, *send_data);
            send_data++;
        }
        
        /* 等待STOP位置起后清除该标志位 */
        while(!std_i2c_get_flag(I2C1, I2C_FLAG_STOP));
        std_i2c_clear_flag(I2C1, I2C_FLAG_STOP);
        
        return;
    }
    
    /* 重载模式传输 */
    while(reload_count > 0U)
    {
        /* 设置主机传输参数及读写请求 */
        std_i2c_master_transfer_config(I2C1, slave_address, I2C_MAX_NBYTES_SIZE, I2C_CR2_RELOAD);
        std_i2c_set_transfer_direction(I2C1, I2C_REQUEST_WRITE);
        
        /* 发送起始位 */
        std_i2c_start_condition_generate(I2C1);
        
        /* 发送流程 */
        for(i=0; i<I2C_MAX_NBYTES_SIZE; i++)
        {
            while(!std_i2c_get_flag(I2C1, I2C_FLAG_TXIS));
            std_i2c_transmit_byte(I2C1, *send_data);
            send_data++;
        }
        
        /* 等待TCR标志置起 */
        while(!std_i2c_get_flag(I2C1, I2C_FLAG_TCR));
        
        reload_count--;
    }
    /* 重载模式切换为自动结束模式 */
    std_i2c_master_transfer_config(I2C1, slave_address, autoend_count, I2C_CR2_AUTOEND);
    
    /* 发送流程 */
    for(i=0; i<autoend_count; i++)
    {
        while(!std_i2c_get_flag(I2C1, I2C_FLAG_TXIS));
        std_i2c_transmit_byte(I2C1, *send_data);
        send_data++;
    }
    
    /* 等待STOP位置起后清除该标志位 */
    while(!std_i2c_get_flag(I2C1, I2C_FLAG_STOP));
    std_i2c_clear_flag(I2C1, I2C_FLAG_STOP);
}

/**
* @brief  I2C1查询方式接收数据流程
* @param  slave_address I2C从机地址
* @param  receive_data 接收地址缓冲区
* @param  receive_size 接收数据长度
* @retval 无
*/
void bsp_i2c1_master_receive(uint16_t slave_address, uint8_t *receive_data, uint32_t receive_size)
{
    uint32_t i;
    uint32_t reload_count;
    uint32_t autoend_count;
    
    /* 等待BUSY位清零 */
    while(std_i2c_get_flag(I2C1, I2C_FLAG_BUSY));
    
    /* 接收数据长度处理 */
    if(receive_size > I2C_MAX_NBYTES_SIZE)
    {
        reload_count = receive_size / I2C_MAX_NBYTES_SIZE;
        autoend_count = receive_size % I2C_MAX_NBYTES_SIZE;
    }
    else
    {
        reload_count = 0;
        autoend_count = receive_size;
    }
    
    /* 自动结束模式传输 */
    if(reload_count == 0)
    {
        /* 设置主机传输参数及读写请求 */
        std_i2c_master_transfer_config(I2C1, slave_address, autoend_count, I2C_CR2_AUTOEND);
        std_i2c_set_transfer_direction(I2C1, I2C_REQUEST_READ);
        
        /* 发送起始位 */
        std_i2c_start_condition_generate(I2C1);
        
        /* 接收流程 */
        for(i=0; i<autoend_count; i++)
        {
            while(!std_i2c_get_flag(I2C1, I2C_FLAG_RXNE));
            *receive_data = std_i2c_receive_byte(I2C1);
            receive_data++;
        }
        
        /* 等待STOP位置起后清除该标志位 */
        while(!std_i2c_get_flag(I2C1, I2C_FLAG_STOP));
        std_i2c_clear_flag(I2C1, I2C_FLAG_STOP);
        
        return;
    }
    
    /* 重载模式传输 */
    while(reload_count > 0U)
    {
        /* 设置主机传输参数及读写请求 */
        std_i2c_master_transfer_config(I2C1, slave_address, I2C_MAX_NBYTES_SIZE, I2C_CR2_RELOAD);
        std_i2c_set_transfer_direction(I2C1, I2C_REQUEST_READ);
        
        /* 发送起始位 */
        std_i2c_start_condition_generate(I2C1);
        
        /* 接收流程 */
        for(i=0; i<I2C_MAX_NBYTES_SIZE; i++)
        {
            while(!std_i2c_get_flag(I2C1, I2C_FLAG_RXNE));
            *receive_data = std_i2c_receive_byte(I2C1);
            receive_data++;
        }
        
        /* 等待TCR标志置起 */
        while(!std_i2c_get_flag(I2C1, I2C_FLAG_TCR));
        
        reload_count--;
    }
    /* 重载模式切换为自动结束模式 */
    std_i2c_master_transfer_config(I2C1, slave_address, autoend_count, I2C_CR2_AUTOEND);
    
    /* 接收流程 */
    for(i=0; i<autoend_count; i++)
    {
        while(!std_i2c_get_flag(I2C1, I2C_FLAG_RXNE));
        *receive_data = std_i2c_receive_byte(I2C1);
        receive_data++;
    }
    
    /* 等待STOP位置起后清除该标志位 */
    while(!std_i2c_get_flag(I2C1, I2C_FLAG_STOP));
    std_i2c_clear_flag(I2C1, I2C_FLAG_STOP);
}
