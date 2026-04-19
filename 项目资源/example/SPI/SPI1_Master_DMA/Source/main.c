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

/*-------------------------------------------functions------------------------------------------*/
int main(void)
{
    /* 配置系统时钟 */
    system_clock_config();
    
    /* GPIO初始化 */
    gpio_init();

    /* SPI初始化 */
    spi1_init();
    
    /* DMA初始化，通道0接收，通道1发送 */
    dma_init();

    /* NVIC初始化 */
    nvic_init();
     
    /* 配置DMA传输地址，传输数据大小 */
    bsp_spi_dma_config();
    
    /* SPI主机模式,拉低片选信号 */
    std_spi_set_nss_output(SPI1, SPI_NSS_OUTPUT_LOW);
    
    /* 使能SPI DMA接收请求 */
    std_spi_dma_rx_enable(SPI1);
    
    /* 使能SPI DMA发送请求 */
    std_spi_dma_tx_enable(SPI1);    
    
    /* 等待发送完成 */
    while(g_send_complete != 1);
    
    /* 等待接收完成 */
    while(g_recv_complete != 1);
    
    /* SPI主机拉高片选信号，结束通信 */
    while(!std_spi_get_flag(SPI1, SPI_FLAG_TXFE));
    while(std_spi_get_flag(SPI1, SPI_FLAG_BUSY));
    std_spi_set_nss_output(SPI1, SPI_NSS_OUTPUT_HIGH);
    
    while(1)
    {
    }
}












