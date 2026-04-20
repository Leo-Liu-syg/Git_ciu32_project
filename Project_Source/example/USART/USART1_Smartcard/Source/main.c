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
__IO uint32_t g_card_inserted = 0;   /* 插卡检测全局标志 */

/*-------------------------------------------functions------------------------------------------*/

int main(void)
{   
    /* 配置系统时钟 */
    system_clock_config();
    
    /* Systick初始化 */
    std_delay_init();
    
    /* GPIO配置 */
    gpio_init();

    /* 接口初始化，本例中T=0 */    
    usart_smartcard_init(); 
    
    /* 智能卡IO口初始化 */ 
    smartcard_io_init();
    
    /* EXTI初始化 */
    exti_init();

    /* 等待插卡 */ 
    while(g_card_inserted == 0);
    /* 插卡标志清零 */ 
    g_card_inserted = 0;
    
    /* 智能卡交互流程 */  
    if (STD_OK != bsp_smartcard_transmission())
    {
        /* 错误处理：智能卡触点释放 */ 
        bsp_smartcard_deactivation();
    }        

    while(1)
    {
    }
}
