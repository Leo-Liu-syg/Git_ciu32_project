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
    
    /* Systick 初始化 */
    std_delay_init();
    
    /* GPIO初始化 */
    gpio_init();
    
    /* DMA初始化 */
    dma_init();
    
    /* TIM3初始化 */
    tim3_init();
  
    /* 启动通道1 PWM输出 */
    bsp_tim3_output_start();
    
    while(1)
    {
        /* 循环执行，更新事件触发DMA Burst传输，并更新PWM占空比 */
        bsp_tim_dma_burst_loop();
    }  
}





