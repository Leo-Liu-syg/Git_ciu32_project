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
/*------------------------------------------variables-------------------------------------------*/
extern __IO uint8_t g_temp_rx;  /* 全局接收缓冲区 */
/*-------------------------------------------functions------------------------------------------*/

int main(void)
{
    /* 配置系统时钟为RCH分频2分频（8MHz）*/
    system_clock_config();   

    /* Systick初始化 */
    std_delay_init();
    
    /* GPIO初始化 */
    gpio_init();
    
    /* LPUART 外设初始化 */
    lpuart_init();
    
    /* LPUART 低功耗唤醒配置,使能接收到一帧数据唤醒 */
    bsp_lpuart_wakeup_config();
    
    /* Systick初始化 */
    std_delay_init();
    /* 进入Stop前等待3s */
    std_delayms(3000); 

    /* 使能超低功耗配置 */
    bsp_pmu_lowpower_config();
    
    /* 进入stop模式 */  
    std_pmu_enter_stop(PMU_ENTRY_LOWPOWER_MODE_WFI);
    
    /* 接收到一帧数据后唤醒，唤醒后系统时钟为RCHSYS: 本例中为 RCH的2分频 8MHz*/
    
    /* 唤醒后关闭LPUART唤醒功能 */
    std_lpuart_wakeup_disable(LPUART1);    
    
    /* 等待发送空标志置1 */
    while(!std_lpuart_get_flag(LPUART1,LPUART_FLAG_TXE))
    {            
    }        
    std_lpuart_tx_data_write(LPUART1,g_temp_rx);
    
    /* 等待发送完成标志置1 */
    while(!std_lpuart_get_flag(LPUART1,LPUART_FLAG_TC))
    {            
    }
    
    while(1)
    {
    }
}



