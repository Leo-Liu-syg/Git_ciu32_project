/*------------------------------------------includes--------------------------------------------*/
#include "flash.h"
volatile uint32_t buffer[PAGE_WORD_COUNT]; // 缓冲区：2个32位元素
volatile uint32_t word_count;
volatile uint32_t verify_data;
// /*-------------------------------------------functions------------------------------------------*/
// void Flash_init(void)
// {
//     uint32_t buffer[PAGE_WORD_COUNT];
//     uint32_t word_count;
//     std_status_t status;

//     /* 初始化编程缓冲区 */
//     memset(buffer, 0xBBBBBBBB, sizeof(uint32_t)*PAGE_WORD_COUNT);

//     /* 清除错误标志 */
//     std_flash_clear_flag(FLASH_FLAG_ALL_ERR);

//     /* Flash控制寄存器解锁 */
//     std_flash_unlock();

//     if (std_flash_get_lock_status() == false)
//     {
//         status = std_flash_page_erase(FLASH_PAGE_NUM);
//         /* 擦除异常，加入自定义处理代码 */
//         while(status != STD_OK)
//         {
//         }

//         /* Flash编程 */
//         for (word_count = 0; word_count < PAGE_WORD_COUNT; word_count++)
//         {
//             status = std_flash_word_program(FLASH_ERASE_PROGRAM_ADDR + (word_count << 2), buffer[word_count]);
//             /* 编程异常，加入自定义处理代码 */
//             while(status != STD_OK)
//             {
//             }

//             /* 校验编程数据 */
//             if (*(uint32_t *)(FLASH_ERASE_PROGRAM_ADDR + (word_count << 2)) != buffer[word_count])
//             {
//                 /* 校验异常，加入自定义处理代码 */
//                 while(1)
//                 {
//                 }
//             }
//         }

//         /* Flash控制寄存器加锁 */
//         std_flash_lock();
//     }

// }

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
void Flash_Read_Voltage(unsigned int *p_co_volt)
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