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
extern __IO uint8_t g_temp_rx;
/*-------------------------------------------functions------------------------------------------*/

int main(void)
{
    /* 设置RCHSYS作为系统时钟源: RCH的2分频，8MHz */
    system_clock_config();

    /* GPIO初始化 */
    gpio_init();
    
    /* LED1初始化 */
    led_init();
    
    /* LPUART 外设初始化 */
    lpuart_init();

    /* LPUART 低功耗唤醒配置,使能接收匹配字符唤醒 */
    bsp_lpuart_wakeup_config();
    
    /* 使能超低功耗配置 */
    bsp_pmu_lowpower_config(); 
           
    /* Systick初始化 */
    std_delay_init();
    /* 等待3s */
    std_delayms(3000); 
    /* LED点亮，指示进入Stop模式*/
    LED1_ON();
    
    /* 使用字符匹配唤醒时在进入Stop前需清空接收数据，且上溢标志需保持为0 */    
    bsp_lpuart_prepare_to_stop();    
    /* 进入stop模式 */  
    std_pmu_enter_stop(PMU_ENTRY_LOWPOWER_MODE_WFI);
    
    /* 接收到匹配字符后唤醒，唤醒后系统时钟为RCHSYS,本例中为8MHz，即RCH的2分频 */
    
    /* 唤醒后关闭LPUART唤醒功能 */
    std_lpuart_wakeup_disable(LPUART1);    
    LED1_OFF();
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

