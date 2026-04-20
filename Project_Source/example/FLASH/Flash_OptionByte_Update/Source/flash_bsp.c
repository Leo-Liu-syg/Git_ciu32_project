/************************************************************************************************/
/**
* @file               flash_bsp.c
* @author             MCU Ecosystem Development Team
* @brief              FLASH BSP驱动函数，实现FLASH功能配置。
*
*
**************************************************************************************************
* @attention
* Copyright (c) CEC Huada Electronic Design Co.,Ltd. All rights reserved.
*
**************************************************************************************************
*/

/*------------------------------------------includes--------------------------------------------*/
#include "flash_bsp.h"

/*------------------------------------------functions-------------------------------------------*/
/**
* @brief  Flash Option Bytes编程
* @retval 无
*/
void bsp_flash_option_byte_config(void)
{
    uint32_t opt_value[OB_BYTES_NUM], index;
    uint32_t *opt_pointer = (uint32_t *)FLASH_OB_OPTR1_ADDR;
    std_status_t status;
    
    /* 备份Option Bytes区数据 */
    for(index = 0; index < OB_BYTES_NUM; index++)
    {
        opt_value[index] = ((uint32_t *)FLASH_OB_OPTR1_ADDR)[index];
    }

    /* Flash选项字节解锁 */
    std_flash_opt_unlock();
    
    if (std_flash_get_opt_lock_status() == false)
    {
        /* 设置选项字节 */
        if ((opt_value[1] & FLASH_STOP_RESET_DISABLE) == FLASH_STOP_RESET_DISABLE)
        {
            /* 设置RST_STOP = 0 */
            opt_value[1] &= (~FLASH_STOP_RESET_DISABLE);
            /* 设置IWDG_STOP = 0 */
            opt_value[1] &= (~FLASH_IWDG_STOP_MODE_NORMAL);
        }
        else
        {
            /* 设置RST_STOP = 1 */
            opt_value[1] |= FLASH_STOP_RESET_DISABLE;
            /* 设置IWDG_STOP = 1 */
            opt_value[1] |= FLASH_IWDG_STOP_MODE_NORMAL;
        }
        
        /* 选项字节擦除 */
        status = std_flash_opt_erase();
        while(status != STD_OK)
        {
            /* Option Bytes区擦除错误处理代码 */
        }

        /* 编程Option Bytes区 */
        for(index = 0; index < OB_BYTES_NUM; index++)
        {
            status = std_flash_word_program((FLASH_OB_OPTR1_ADDR + (index << 2)), opt_value[index]);
            while(status != STD_OK)
            {
                /* Option Bytes区编程错误处理代码 */
            }
        }
        /* 编程Option Bytes芯片会在高16bit自动填充低16bit的反码，因此需要在编程结束后校验 */
        for(index = 0; index < OB_BYTES_NUM; index++)
        {
            /* 分别确认低16bit Option Bytes和高16bit的反码的正确性 */
            if (((uint16_t)opt_pointer[index] != (uint16_t)opt_value[index])
             || (((~opt_pointer[index]) >> 16) != (uint16_t)opt_value[index]))
            {
                while(1)
                {
                    /* 选项字节校验错误处理代码 */
                }
            }
        }
        
        
    }
    else
    {
        while(1)
        {
            /* 选项字节解锁错误处理代码 */
        }
    }
}

