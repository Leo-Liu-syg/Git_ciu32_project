/************************************************************************************************/
/**
* @file               main.c
* @author             MCU Ecosystem Development Team
* @brief              应用程序主流程。
*                     
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
/*------------------------------------------variables-------------------------------------------*/
__IO uint8_t g_temp_rx = 0;
/*-------------------------------------------functions------------------------------------------*/

int main(void)
{
    /* 设置RCH的2分频作为系统时钟源: 8MHz */
    system_clock_config();

    /* GPIO初始化 */
    gpio_init();
    
    /* LPUART 外设初始化 */
    lpuart_init();

    /* LPUART 低功耗唤醒配置，使用LPWK标志在接收一个数据时唤醒 */
    bsp_lpuart_wakeup_config();
           
    /* Systick初始化 */
    std_delay_init();
    /* 等待3s */
    std_delayms(3000); 
   
    /* 使能超低功耗配置 */
    bsp_pmu_lowpower_config();
   
    /* 进入stop模式 */    
    std_pmu_enter_stop(PMU_ENTRY_LOWPOWER_MODE_WFI);

    /* 接收到一帧数据后唤醒，唤醒后系统时钟为RCHSYS，本例中为8MHz，即RCH的2分频 */
   
    /* 唤醒后关闭LPUART唤醒功能 */
    std_lpuart_wakeup_disable(LPUART1);
    
    /* 检查接收标志置1后，读取接收数据*/
    while(!std_lpuart_get_flag(LPUART1,LPUART_FLAG_RXNE))
    {
    }
    g_temp_rx = (std_lpuart_rx_data_read(LPUART1) & 0xFF); 
    
    /* 检查到TXE后写入发送数据 */
    while(!(std_lpuart_get_flag(LPUART1,LPUART_FLAG_TXE)))
    {
    }        
    /* 发送唤醒时接收到的数据 */
    std_lpuart_tx_data_write(LPUART1,g_temp_rx);
    /* 等待发送完成 */
    while(!(std_lpuart_get_flag(LPUART1,LPUART_FLAG_TC)))
    {
    }
    
    while(1)
    { 
    }
}



