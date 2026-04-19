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
* @brief  Flash快速编程流程
* @param  address 编程地址
* @param  data_buf 编程数据
* @retval std_status_t 本函数执行结果
*/
std_status_t bsp_flash_fast_program(uint32_t address, uint32_t *data_buf)
{
    std_status_t status = STD_OK;
    uint32_t primask, prog_count;
    
    /* 检查Flash上一次操作忙状态、错误状态 */
    status = std_flash_wait_for_last_operation(FLASH_WAIT_PROGRAM);
    
    if(status == STD_OK)
    {
        /* 进入编程模式 */
        std_flash_program_enable();
        
        /* 短时间屏蔽中断 */
        primask = __get_PRIMASK();
        __disable_irq();
        
        /* 执行快速编程 */
        status = bsp_flash_fast_write(address, data_buf);

        /* 恢复中断 */
        __set_PRIMASK(primask);
        
        /* 退出编程模式 */
        std_flash_program_disable();
        
        /* 快速编程中无法对Flash执行读操作，需在编程结束后统一校验数据 */
        if (status == STD_OK)
        {
            for (prog_count=0; prog_count < FSTPG_WORD_COUNT; prog_count++)
            {
                if (((__IO uint32_t *)address)[prog_count] != data_buf[prog_count])
                {
                    status = STD_ERR;
                    break;
                }
            }
        }
    }
    return (status);
}


#if   defined ( __CC_ARM )
#pragma arm section code = "FAST_PROGRAM"
#elif defined ( __ICCARM__ )
__ramfunc
#elif defined ( __GNUC__ )
__attribute__  ((section (".RamFunc")))
#endif

/**
* @brief  Flash快速编程，往目标地址快速编程半页数据
* @param  address 编程地址
* @param  data_buf 编程数据
* @retval std_status_t 本函数执行结果
*/
std_status_t bsp_flash_fast_write(uint32_t address, uint32_t *data_buf)
{
    std_status_t status = STD_OK;
    uint32_t prog_count = 0;
    
    /* 启动快速编程模式 */
    FLASH->CR |= FLASH_CR_FSTPG_MODE;
    
    /* 向目标地址写入数据 */
    for (prog_count=0; prog_count < FSTPG_WORD_COUNT; prog_count++)
    {
        ((uint32_t *)address)[prog_count] = data_buf[prog_count];
        
        /* 查询等待BSY标志被清除 */
        while ((FLASH->SR & FLASH_FLAG_BSY) == FLASH_FLAG_BSY);
        
        /* 若出现错误，则退出编程循环 */
        if (FLASH->SR & FLASH_FLAG_ALL_ERR)
        {
            status = STD_ERR;
            break;
        }
    }
    
    /* 查询等待FSTPG_MODE状态被自动清零 */
    while ((FLASH->CR & FLASH_CR_FSTPG_MODE) == FLASH_CR_FSTPG_MODE);
    
    /* 清除Flash标志 */
    FLASH->SR = (FLASH_FLAG_ALL_ERR | FLASH_SR_EOP);
    
    return (status);
}

#if   defined ( __CC_ARM )
#pragma arm section
#endif
