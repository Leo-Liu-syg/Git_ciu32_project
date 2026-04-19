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

/*------------------------------------------functions-------------------------------------------*/

int main(void)
{
    __IO uint32_t delay;

    /* 配置系统时钟 */
    system_clock_config(); 

    /* 初始化LED1 */
    led_init();

    /* 初始化Systick */
    std_delay_init();

    /* 初始化配置的参数 */
    wwdg_init();

    /* 计算进入WWDG窗口之前的延时时间(ms)，为了能安全进入WWDG窗口，延时时间增加5ms */
    delay = bsp_timeout_calculation((g_wwdg_window - g_wwdg_counter) + 1) + 5;

    while(1)
    {
        /* 翻转LED1状态 */
        LED_RED_FLIP();

        std_delayms(delay);

        /* WWDG喂狗 */
        std_wwdg_refresh_counter(g_wwdg_counter);
    }
}
