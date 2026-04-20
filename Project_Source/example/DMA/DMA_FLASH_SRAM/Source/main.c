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
/* DMA 触发后将g_source的数据搬运至g_destination */
const uint32_t g_source[NUMBER_TO_TRANSFER] = {0x01010101, 0x02020202, 0x03030303, 0x04040404, \
                                               0x05050505, 0x06060606, 0x07070707, 0x08080808};
uint32_t g_destination[NUMBER_TO_TRANSFER] = {0};

/* DMA搬运结束标志，搬运结束中断中将其置位 */
__IO uint32_t g_complete = 0;

/*-------------------------------------------functions------------------------------------------*/
int main(void)
{
    /* 配置系统时钟 */
    system_clock_config();
    
    /* DMA通道0初始化，中断初始化 */
    dma_init();
    
    bsp_dma_config((uint32_t *)g_source, (uint32_t *)g_destination, NUMBER_TO_TRANSFER);
    
    /* 等待DMA中断完成 */
    while(g_complete == 0)
    {
        
    }
    
    while(1)
    {

    }
}












