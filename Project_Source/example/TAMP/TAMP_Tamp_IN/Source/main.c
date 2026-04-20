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
__IO uint8_t g_tampin_flag = 0;

/*------------------------------------------functions-------------------------------------------*/
int main(void)
{
    /* 配置系统时钟 */
    system_clock_config(); 

    /* TAMP初始化，RTC/TAMP时钟源为RCL */
    tamp_init();

    /* TAMP IN入侵检测配置 */
    bsp_tamp_tampin_config();
    
    /* 等待TAMP IN外部入侵事件中断产生 */
    while (g_tampin_flag == 0)
    {
    }

    /* 检查备份寄存器是否被清除 */
    if((TAMP_BACKUP_DR0 != 0x00) || (TAMP_BACKUP_DR1 != 0x00) || (TAMP_BACKUP_DR2 != 0x00) \
    || (TAMP_BACKUP_DR3 != 0x00) || (TAMP_BACKUP_DR4 != 0x00))
    {
        /* 备份寄存器清除不成功错误处理 */
        while(1)
        {
        }
    }
   
    while(1)
    {
    }
}
