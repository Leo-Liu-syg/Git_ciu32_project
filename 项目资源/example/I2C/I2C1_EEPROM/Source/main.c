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
    
    /* Systick初始化 */
    std_delay_init();
    
    /* GPIO初始化 */
    gpio_init();
    
    /* I2C1初始化为主机模式 */
    i2c1_master_init();
    
    /* I2C1 EEPROM读写 */
    bsp_i2c1_eeprom_write();
    std_delayms(300);
    bsp_i2c1_eeprom_read();
    
    while(1)
    {
    }
}




