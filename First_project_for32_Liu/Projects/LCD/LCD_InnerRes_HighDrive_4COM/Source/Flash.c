/*------------------------------------------includes--------------------------------------------*/
#include "flash.h"
volatile uint32_t buffer[PAGE_WORD_COUNT]; // 缓冲区：2个32位元素
volatile uint32_t word_count;
volatile uint32_t verify_data;

/* Flash写入函数（删除memset初始化） */
void Flash_Write_CO_Voltage(void)
{

    std_status_t status;
    uint32_t verify_data;
    // system_clock_config();
    // memset(buffer, 0xBBBBBBBB, sizeof(uint32_t)*PAGE_WORD_COUNT);
    /* 核心：直接给buffer赋值真实电压值（无任何初始化步骤） */
    buffer[0] = adc_co_calibration_value; // CO电压
    // buffer[1] = g_smoke_voltage;// 烟雾电压

    /* Flash操作逻辑（保持不变） */
    std_flash_clear_flag(FLASH_FLAG_ALL_ERR);
    std_flash_unlock();

    if (std_flash_get_lock_status() == false)
    {
        // 擦除Flash页
        status = std_flash_page_erase(FLASH_PAGE_NUM);
        std_flash_word_program(FLASH_ERASE_PROGRAM_ADDR, buffer[0]);
        while (status != STD_OK)
        {
            std_flash_lock();
            return;
        }

        status = std_flash_word_program(FLASH_ERASE_PROGRAM_ADDR, buffer[0]);
        while (status != STD_OK)
        {
            std_flash_lock();
            return;
        }
        // 写入校验
        verify_data = *(uint32_t *)FLASH_ERASE_PROGRAM_ADDR;
        while (verify_data != buffer[0])
        {
        } // 校验失败卡死
    }
    //

    std_flash_lock();
}

/* Flash读取函数（保持不变） */
void Flash_Read_Voltage_Check(unsigned int *p_co_volt)
{
    if (p_co_volt == NULL)
    {
        return;
    }

    // 读取Flash中的数据
    *p_co_volt = *(uint32_t *)(FLASH_ERASE_PROGRAM_ADDR + 0);

    // 读取校验（判断是否为默认值）
    if (*p_co_volt == DEFAULT_VOLTAGE_VAL)
    {
        Status = Status_IDLE;
    }
    else
    {
        Status = Status_RUNNING;
    }
}

unsigned int Flash_get_Value(unsigned int *p_co_volt)
{
    unsigned int value;
    if (p_co_volt == NULL)
    {
        return 0xFFFFFFFF;
    }

    // 读取Flash中的数据
    *p_co_volt = *(uint32_t *)(FLASH_ERASE_PROGRAM_ADDR + 0);
    value = *p_co_volt;
    return value;
}