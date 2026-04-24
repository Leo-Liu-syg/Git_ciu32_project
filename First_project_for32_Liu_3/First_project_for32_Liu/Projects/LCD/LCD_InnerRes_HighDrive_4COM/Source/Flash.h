
#ifndef __FLASH_H__
#define __FLASH_H__

/*------------------------------------------includes--------------------------------------------*/

#include "main.h"

/*------------------------------------define----------------------------------------------------*/
#define PAGE_WORD_COUNT (0x01)                                               /* Flash编程字的个数 */
#define FLASH_PAGE_NUM (0x30)                                                /* Flash擦写起始页编号：第48页 */
#define FLASH_ERASE_PROGRAM_ADDR (FLASH_MEM_BASE + (FLASH_PAGE_NUM * 0x200)) /* Flash擦写起始地址：0x08006000 */
#define DEFAULT_VOLTAGE_VAL 0xffffffff
void Flash_Write_CO_Voltage(void);
void Flash_Read_Voltage_Check(unsigned int *p_co_volt);
unsigned int Flash_get_Value(unsigned int *p_co_volt);


#endif /* MAIN_H */
