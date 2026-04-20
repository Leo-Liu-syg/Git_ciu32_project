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
    /* 设置RCHSYS作为系统时钟源: RCH的2分频，8MHz */
    system_clock_config();
    
    /* Systick初始化 */
    std_delay_init();

    /* 配置按键，作为stop唤醒源 */    
    exti_init();
    
    /* 等待3S，便于用户调试 */
    std_delayms(3000); 

    /* 使能超低功耗配置 */
    bsp_pmu_lowpower_config();
    
    /* 进入stop模式 */  
    std_pmu_enter_stop(PMU_ENTRY_LOWPOWER_MODE_WFI);
    
    /* 唤醒后,系统时钟仍为8MHz，即RCH的2分频 */
    
    while(1)
    {
       
    }
}




