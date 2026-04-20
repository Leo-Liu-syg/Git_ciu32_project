/************************************************************************************************/
/**
* @file               i2c_bsp.c
* @author             MCU Ecosystem Development Team
* @brief              I2C BSP驱动函数，实现I2C EEPROM读写操作功能。
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

/*------------------------------------------variables-------------------------------------------*/
uint8_t g_write_buffer[DATA_LENGTH] = {0x11, 0x22, 0x33, 0x44, 0x55, 0x66, 0x77, 0x88};
uint8_t g_read_buffer[DATA_LENGTH];

/*------------------------------------------functions-------------------------------------------*/
/**
* @brief  I2C1查询方式写EEPROM
* @retval 无
*/
void bsp_i2c1_eeprom_write(void)
{
    uint32_t i;
    
    /* 等待BUSY标志位清零 */
    while(std_i2c_get_flag(I2C1, I2C_FLAG_BUSY));
    
    /* 设置从设备地址及读写方向等参数 */
    std_i2c_master_transfer_config(I2C1, EEPROM_ADDRESS, MEM_ADDRESS_SIZE, I2C_MODE_RELOAD);
    std_i2c_set_transfer_direction(I2C1, I2C_REQUEST_WRITE);
    
    /* 发送访问memory地址 */
    std_i2c_start_condition_generate(I2C1);
    while(!std_i2c_get_flag(I2C1, I2C_FLAG_TXIS));
    std_i2c_transmit_byte(I2C1, (uint8_t)((MEM_ADDRESS & 0xFF00) >> 8));
    while(!std_i2c_get_flag(I2C1, I2C_FLAG_TXIS));
    std_i2c_transmit_byte(I2C1, (uint8_t)(MEM_ADDRESS & 0x00FF));
    
    /* 等待TCR置起 */
    while(!std_i2c_get_flag(I2C1, I2C_FLAG_TCR));
    
    /* 设置写入EEPROM的数据长度 */
    std_i2c_master_transfer_config(I2C1, EEPROM_ADDRESS, DATA_LENGTH, I2C_MODE_AUTOEND);
    
    /* 写EEPROM流程 */
    for (i=0; i<DATA_LENGTH; i++)
    {
        /* 查询TXIS标志 */
        while(!std_i2c_get_flag(I2C1, I2C_FLAG_TXIS));
        std_i2c_transmit_byte(I2C1, g_write_buffer[i]);
    }
    
    /* 自动结束模式下自动发送停止位，需查询STOPF标志置起后清除该位 */
    while(!std_i2c_get_flag(I2C1, I2C_FLAG_STOP));
    std_i2c_clear_flag(I2C1, I2C_FLAG_STOP);

}


/**
* @brief  I2C1查询方式读EEPROM
* @retval 无
*/
void bsp_i2c1_eeprom_read(void)
{
    uint32_t i;
    
    /* 等待BUSY标志位清零 */
    while(std_i2c_get_flag(I2C1, I2C_FLAG_BUSY));   
    
    /* 设置从设备地址及读写方向等参数 */
    std_i2c_master_transfer_config(I2C1, EEPROM_ADDRESS, MEM_ADDRESS_SIZE, I2C_MODE_SOFTEND);
    std_i2c_set_transfer_direction(I2C1, I2C_REQUEST_WRITE);
    
    /* 发送访问memory地址 */
    std_i2c_start_condition_generate(I2C1);
    while(!std_i2c_get_flag(I2C1, I2C_FLAG_TXIS));
    std_i2c_transmit_byte(I2C1, (uint8_t)((MEM_ADDRESS & 0xFF00) >> 8));
    while(!std_i2c_get_flag(I2C1, I2C_FLAG_TXIS));
    std_i2c_transmit_byte(I2C1, (uint8_t)(MEM_ADDRESS & 0x00FF));
    
    /* 等待TC置起 */
    while(!std_i2c_get_flag(I2C1, I2C_FLAG_TC));
    
    /* 设置读EEPROM的数据长度及发送起始位 */
    std_i2c_master_transfer_config(I2C1, EEPROM_ADDRESS, DATA_LENGTH, I2C_MODE_AUTOEND);
    std_i2c_set_transfer_direction(I2C1, I2C_REQUEST_READ);
    std_i2c_start_condition_generate(I2C1);
    
    /* 读EEPROM流程 */
    for (i=0; i<DATA_LENGTH; i++)
    {
        /* 查询RXNE状态 */
        while(!std_i2c_get_flag(I2C1, I2C_FLAG_RXNE));
        g_read_buffer[i] = std_i2c_receive_byte(I2C1);
    }
    
    /* 自动结束模式下自动发送停止位，需查询STOPF标志置起后清除该位 */
    while(!std_i2c_get_flag(I2C1, I2C_FLAG_STOP));
    std_i2c_clear_flag(I2C1, I2C_FLAG_STOP);
}
