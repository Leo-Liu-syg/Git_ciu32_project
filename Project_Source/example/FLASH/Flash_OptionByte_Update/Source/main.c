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
    /* 配置系统时钟 */
    system_clock_config(); 
    
    /* 用户按键初始化 */
    button_init();

    /* 等待用户按下USER按键以启动选项字节更新 */
    while (std_gpio_get_input_pin(BUTTON_USER_PORT, BUTTON_USER_PIN) == true)
    {
    }
        
    /* 清除错误标志 */
    std_flash_clear_flag(FLASH_FLAG_ALL_ERR);

    /* Flash控制寄存器解锁 */
    std_flash_unlock();

    if (std_flash_get_lock_status() == false)
    {
        /* 选项字节擦除与编程 */
        bsp_flash_option_byte_config();
        
        /* Flash选项字节加载，产生复位，之后语句无法得到执行 */
        std_flash_opt_launch();
    }
    
    while(1)
    {

    }
}
