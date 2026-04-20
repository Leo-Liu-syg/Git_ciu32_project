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

/*------------------------------------------variables-------------------------------------------*/
uint8_t g_write_buffer[DATA_LENGTH] = {0x11, 0x22, 0x33, 0x44, 0x55, 0x66, 0x77, 0x88};
uint8_t g_read_buffer[DATA_LENGTH];

/*------------------------------------------functions-------------------------------------------*/
/**
* @brief  I2C1 DMA方式写EEPROM
* @retval 无
*/
void bsp_i2c1_dma_eeprom_write(void)
{
    /* 等待BUSY标志位清零 */
    while(std_i2c_get_flag(I2C1, I2C_FLAG_BUSY));
    
    /* 设置主机传输参数 */
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
    
    /* 使能I2C DMA发送请求 */
    std_i2c_dma_tx_req_enable(I2C1);
    
    /* 设置写入EEPROM的数据长度 */
    std_i2c_master_transfer_config(I2C1, EEPROM_ADDRESS, DATA_LENGTH, I2C_MODE_AUTOEND);
    
    /* 等待DMA传输完成 */
    while(!std_dma_get_flag(DMA_FLAG_TF1));
     
    /* 自动结束模式下自动发送停止位，需查询STOPF标志置起后清除该位 */
    while(!std_i2c_get_flag(I2C1, I2C_FLAG_STOP));
    std_i2c_clear_flag(I2C1, I2C_FLAG_STOP);
}


/**
* @brief  I2C1 DMA方式读EEPROM
* @retval 无
*/
void bsp_i2c1_dma_eeprom_read(void)
{
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
    
    /* 使能I2C DMA接收请求 */
    std_i2c_dma_rx_req_enable(I2C1);
    
    /* 设置读EEPROM的数据长度及发送起始位 */
    std_i2c_master_transfer_config(I2C1, EEPROM_ADDRESS, DATA_LENGTH, I2C_MODE_AUTOEND);
    std_i2c_set_transfer_direction(I2C1, I2C_REQUEST_READ);
    std_i2c_start_condition_generate(I2C1);

    /* 等待DMA传输完成 */
    while(!std_dma_get_flag(DMA_FLAG_TF0));

    /* 自动结束模式下自动发送停止位，需查询STOPF标志置起后清除该位 */
    while(!std_i2c_get_flag(I2C1, I2C_FLAG_STOP));
    std_i2c_clear_flag(I2C1, I2C_FLAG_STOP);
}

/**
* @brief  I2C DMA Tx通道配置
* @retval 无
*/
void bsp_i2c1_dma_tx_config(void)
{
    std_dma_config_t i2c_dma_config = {0};
    
    i2c_dma_config.dma_channel = DMA_CHANNEL_1;
    i2c_dma_config.src_addr = (uint32_t)g_write_buffer;
    i2c_dma_config.dst_addr = (uint32_t)&(I2C1->TDR);
    i2c_dma_config.data_number = DATA_LENGTH;
    std_dma_start_transmit(&i2c_dma_config);
}

/**
* @brief  I2C DMA Rx通道配置
* @retval 无
*/
void bsp_i2c1_dma_rx_config(void)
{
    std_dma_config_t i2c_dma_config = {0};
    
    i2c_dma_config.dma_channel = DMA_CHANNEL_0;
    i2c_dma_config.src_addr = (uint32_t)&(I2C1->RDR);
    i2c_dma_config.dst_addr = (uint32_t)g_read_buffer;
    i2c_dma_config.data_number = DATA_LENGTH;
    std_dma_start_transmit(&i2c_dma_config);
}



