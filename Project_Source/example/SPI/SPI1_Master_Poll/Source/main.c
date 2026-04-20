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

/*--------------------------------------------variables-----------------------------------------*/
uint8_t g_send_buffer[8] = {0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07, 0x08};
uint8_t g_recv_buffer[8] = {0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00};

/*-------------------------------------------functions------------------------------------------*/
int main(void)
{
    uint32_t index = 0;
    
    /* 配置系统时钟 */
    system_clock_config();
    
    /* GPIO初始化 */
    gpio_init();

    /* SPI初始化 */
    spi1_init();
    
    /* SPI主机模式,拉低片选信号 */
    std_spi_set_nss_output(SPI1, SPI_NSS_OUTPUT_LOW);

    /* SPI数据传输 */
    for(index=0; index<8; index++)
    {
        /* 等待发送数据寄存器为空 */
        while(!std_spi_get_flag(SPI1, SPI_FLAG_TXFE))
        {
        }
        
        /* 发送数据 */
        std_spi_write_data(SPI1, g_send_buffer[index]);
        
        /* 等待接收数据寄存器非空 */
        while(!std_spi_get_flag(SPI1, SPI_FLAG_RXFNE))
        {
        }
        
        /* 接收数据 */ 
        g_recv_buffer[index] = std_spi_read_data(SPI1);
    }

    /* SPI主机拉高片选信号，结束通信 */
    while(!std_spi_get_flag(SPI1, SPI_FLAG_TXFE));
    while(std_spi_get_flag(SPI1, SPI_FLAG_BUSY));
    std_spi_set_nss_output(SPI1, SPI_NSS_OUTPUT_HIGH);
    
    while(1)
    {
    }
}












