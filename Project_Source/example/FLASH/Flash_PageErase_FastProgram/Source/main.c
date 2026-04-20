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

/*------------------------------------define----------------------------------------------------*/
#define FLASH_PAGE_NUM            (0x30)               /* Flash擦写起始页编号：第48页 */
#define FLASH_ERASE_PROGRAM_ADDR  (FLASH_MEM_BASE + (FLASH_PAGE_NUM * 0x200))  /* Flash擦写起始地址：0x08006000 */

/*-------------------------------------------functions------------------------------------------*/
int main(void)
{
    uint32_t buffer[FSTPG_WORD_COUNT];
    uint32_t i;
    std_status_t status;
    
    /* 配置系统时钟 */
    system_clock_config(); 

    /* 初始化编程缓冲区 */
    for(i = 0; i < FSTPG_WORD_COUNT; i++)
    {
        buffer[i] = 0x5a5a0000 + i;
    }

    /* 清除错误标志 */
    std_flash_clear_flag(FLASH_FLAG_ALL_ERR);
        
    /* Flash控制寄存器解锁 */
    std_flash_unlock();
    
    if (std_flash_get_lock_status() == false)
    {
        /* 页擦第48页 */
        status = std_flash_page_erase(FLASH_PAGE_NUM);
        /* 擦除异常，加入自定义处理代码 */
        while(status != STD_OK)
        {
        }

        /* 快速编程64个字：第48页前半页 */
        status = bsp_flash_fast_program(FLASH_ERASE_PROGRAM_ADDR, buffer);
        /* 快速编程异常，加入自定义处理代码 */
        while(status != STD_OK)
        {
        }

        /* 快速编程64个字：第48页后半页 */
        status = bsp_flash_fast_program((FLASH_ERASE_PROGRAM_ADDR + (FSTPG_WORD_COUNT << 2)), buffer);
        /* 快速编程异常，加入自定义处理代码 */
        while(status != STD_OK)
        {
        }
        
        /* Flash控制寄存器加锁 */
        std_flash_lock();
    }

    while(1)
    {
    }
}
