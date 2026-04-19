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

/*------------------------------------------define----------------------------------------------*/
#define SLAVE_ADDRESS          (0x5C)
#define DATA_LENGTH            (260U)

/*------------------------------------------variables-------------------------------------------*/
uint8_t g_tx_buffer[DATA_LENGTH];
uint8_t g_rx_buffer[DATA_LENGTH];

/*-------------------------------------------functions------------------------------------------*/
int main(void)
{    
    uint32_t i;
    
    /* 配置系统时钟 */
    system_clock_config();
    
    /* I2C1和LED GPIO初始化 */
    gpio_init();
    
    /* I2C1初始化为主模式 */
    i2c1_master_init();
    
    /* 初始化发送数据缓冲区 */
    for(i=0; i<DATA_LENGTH; i++)
    {
        g_tx_buffer[i] = i+1;
    }
    
    /* I2C1主机发送 */
    bsp_i2c1_master_send(SLAVE_ADDRESS, g_tx_buffer, DATA_LENGTH);
    
    /* I2C1主机接收 */
    bsp_i2c1_master_receive(SLAVE_ADDRESS, g_rx_buffer, DATA_LENGTH);
    
    /* 等待主机通信结束 */
    while(1)
    {
       
    }
}




