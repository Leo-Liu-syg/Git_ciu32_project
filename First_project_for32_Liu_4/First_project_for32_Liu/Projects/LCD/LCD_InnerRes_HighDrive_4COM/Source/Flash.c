/*------------------------------------------ 头文件包含 --------------------------------------------*/
#include "flash.h"

// 定义缓冲区数组（一页Flash大小）
volatile uint32_t buffer[PAGE_WORD_COUNT];
// 数据长度计数
volatile uint32_t word_count;
// 校验数据
volatile uint32_t verify_data;

/*
 * 函数名：Flash_Write_CO_Voltage
 * 功能  ：将CO校准电压值写入Flash指定地址
 * 步骤  ：解锁Flash → 擦除页 → 写入数据 → 校验写入结果 → 锁定Flash
 */
void Flash_Write_CO_Voltage(void)
{
    std_status_t status;
    uint32_t verify_data;

    // 直接给缓冲区赋值，用于存储需要写入的电压值
    buffer[0] = adc_co_calibration_value; // CO校准电压
    buffer[1] = Get_CO_Zero;
    // 清除Flash所有错误标志位
    std_flash_clear_flag(FLASH_FLAG_ALL_ERR);
    // 解锁Flash
    std_flash_unlock();

    // 判断Flash是否解锁成功
    if (std_flash_get_lock_status() == false)
    {
        // 擦除指定Flash页
        status = std_flash_page_erase(FLASH_PAGE_NUM);
        // 向Flash地址写入一个字（32位）
        std_flash_word_program(FLASH_ERASE_PROGRAM_ADDR, buffer[0]);
        std_flash_word_program(FLASH_ERASE_PROGRAM_ADDR + 4, buffer[1]);
        // 擦除失败处理
        while (status != STD_OK)
        {
            std_flash_lock();
            return;
        }
        // 读取Flash数据进行校验
        verify_data = *(uint32_t *)FLASH_ERASE_PROGRAM_ADDR;
        // 校验失败则循环等待
        while (verify_data != buffer[0])
        {
        }
        verify_data = *(uint32_t *)(FLASH_ERASE_PROGRAM_ADDR + 4);
        // 校验失败则循环等待
        while (verify_data != buffer[1])
        {
        }
    }
    // 锁定Flash
    std_flash_lock();
}

/*
 * 函数名：Flash_Read_Voltage_Check
 * 功能  ：从Flash读取CO电压值，并判断是否为默认值
 * 输入  ：p_co_volt → 存储读取到的电压值
 */
void Flash_Read_Voltage_Check(unsigned int *p_co_volt)
{
    if (p_co_volt == NULL)
    {
        return;
    }

    // 从Flash指定地址读取数据
    *p_co_volt = *(uint32_t *)(FLASH_ERASE_PROGRAM_ADDR + 0);

    // 判断读取到的数据是否为默认值
    if (*p_co_volt == DEFAULT_VOLTAGE_VAL)
    {
        Status = Status_IDLE; // 空闲状态（未写入有效电压）
    }
    else
    {
        Status = Status_RUNNING; // 运行状态（已写入有效电压）
    }
}

/*
 * 函数名：Flash_get_Value
 * 功能  ：从Flash读取电压值并返回
 * 输出  ：读取到的Flash数据
 */
unsigned int Flash_Get_Value(unsigned int address)
{
    unsigned int value;

    if ((uint32_t *)(FLASH_ERASE_PROGRAM_ADDR + address) == NULL)
    {
        return 0xFFFFFFFF;
    }

    // 读取Flash指定地址数据
    value = *(uint32_t *)(FLASH_ERASE_PROGRAM_ADDR + address);

    return value;
}