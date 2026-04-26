
#ifndef __FLASH_H__
#define __FLASH_H__

/*------------------------------------------ 头文件包含 --------------------------------------------*/
#include "main.h"

/*------------------------------------ 宏定义 ----------------------------------------------------*/
// 单次写入Flash的字数量
#define PAGE_WORD_COUNT (0x02)

// Flash操作起始页号（第48页）
#define FLASH_PAGE_NUM (0x30)

// Flash擦写起始地址
#define FLASH_ERASE_PROGRAM_ADDR (FLASH_MEM_BASE + (FLASH_PAGE_NUM * 0x200)) /*0x08006000 */

// 默认电压值（未写入时的初始值）
#define DEFAULT_VOLTAGE_VAL 0xffffffff



/*------------------------------------ 函数声明 ----------------------------------------------------*/
// 向Flash写入CO校准电压
void Flash_Write_CO_Voltage(void);

// 从Flash读取电压并校验
void Flash_Read_Voltage_Check(unsigned int *p_co_volt);

// 从Flash获取校准值
unsigned int Flash_Get_Value(unsigned int address);

#endif /* __FLASH_H__ */
