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
#define PAGE_WORD_COUNT           (0x20)               /* Flash编程字的个数 */
#define FLASH_PAGE_NUM            (0x30)               /* Flash擦写起始页编号：第48页 */
#define FLASH_ERASE_PROGRAM_ADDR  (FLASH_MEM_BASE + (FLASH_PAGE_NUM * 0x200))  /* Flash擦写起始地址：0x08006000 */

/*-------------------------------------------functions------------------------------------------*/
int main(void)
{
    uint32_t buffer[PAGE_WORD_COUNT];
    uint32_t word_count;
    std_status_t status;
    
    /* 配置系统时钟 */
    system_clock_config(); 

    /* 初始化编程缓冲区 */
    memset(buffer, 0xBBBBBBBB, sizeof(uint32_t)*PAGE_WORD_COUNT);

    /* 清除错误标志 */
    std_flash_clear_flag(FLASH_FLAG_ALL_ERR);
        
    /* Flash控制寄存器解锁 */
    std_flash_unlock();
    
    if (std_flash_get_lock_status() == false)
    {
        status = std_flash_page_erase(FLASH_PAGE_NUM);
        /* 擦除异常，加入自定义处理代码 */
        while(status != STD_OK)
        {
        }

        /* Flash编程 */
        for (word_count = 0; word_count < PAGE_WORD_COUNT; word_count++) 
        {
            status = std_flash_word_program(FLASH_ERASE_PROGRAM_ADDR + (word_count << 2), buffer[word_count]);
            /* 编程异常，加入自定义处理代码 */
            while(status != STD_OK)
            {
            }
            
            /* 校验编程数据 */
            if (*(uint32_t *)(FLASH_ERASE_PROGRAM_ADDR + (word_count << 2)) != buffer[word_count])
            {
                /* 校验异常，加入自定义处理代码 */
                while(1)
                {
                }
            }
        }
                                        
        /* Flash控制寄存器加锁 */
        std_flash_lock();
    }
    
    while(1)
    {
    }
}
