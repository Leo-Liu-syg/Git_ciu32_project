/************************************************************************************************/
/**
* @file               common.c
* @author             MCU Ecosystem Development Team
* @brief              通用函数或本外设相关的配置实现函数。
*
*
**************************************************************************************************
* @attention
* Copyright (c) CEC Huada Electronic Design Co.,Ltd. All rights reserved.
*
**************************************************************************************************
*/

/*------------------------------------------includes--------------------------------------------*/
#include "ssc.h"

/**
 * @brief 系统时钟配置函数（核心！给单片机设置工作频率）
 * @note  最终目标：把内部时钟(RCH)通过PLL倍频，作为系统主时钟
 */
void system_clock_config(void)
{
    /*==================== 1. Flash等待周期配置 ====================*/
    // Flash(程序存储器)的读取速度比CPU慢，CPU高速运行时，必须给Flash加"等待时间"
    // FLASH_LATENCY_2CLK：Flash读数据等待 2个时钟周期（匹配高频运行，保证稳定）
    std_flash_set_latency(FLASH_LATENCY_2CLK);

    /*==================== 2. 使能内部高速时钟(RCH) ====================*/
    // RCH = 内部高速RC振荡器（单片机自带的时钟源，不用外部晶振）
    std_rcc_rch_enable();
    
    // 等待RCH时钟稳定就绪（必须等！时钟不稳定会导致系统崩溃）
    while(!std_rcc_get_rch_ready());

    /*==================== 3. PLL锁相环配置（核心：倍频提升时钟速度） ====================*/
    // PLL = 锁相环，作用：把低速时钟 倍频→ 变成高速时钟
    // 4个参数详解：
    // ① RCC_PLLSRC_RCH   ：PLL的时钟源选择 → 用内部高速时钟RCH
    // ② RCC_PLLM_MUL12   ：PLL倍频系数M → ×12倍频
    // ③ RCC_PLLN_DIV2    ：PLL分频系数N → ÷2分频
    // ④ RCC_PLL_DIV2     ：PLL最终输出分频 → ÷2分频
    std_rcc_pll_config(RCC_PLLSRC_RCH, RCC_PLLM_MUL12, RCC_PLLN_DIV2, RCC_PLL_DIV2);
    
    // 使能PLL（打开锁相环）
    std_rcc_pll_enable();
    // 等待PLL时钟稳定就绪
    while(!std_rcc_get_pll_ready());
    
    // 使能PLL输出（让PLL产生的时钟可以被系统使用）
    std_rcc_pll_output_enable();    

    /*==================== 4. 选择系统主时钟源 ====================*/
    // 设置系统时钟(SYSCLK)来源 → 选择PLL输出的时钟作为主时钟
    std_rcc_set_sysclk_source(RCC_SYSCLK_SRC_PLLCLK);
    
    // 等待时钟切换完成（确认系统时钟已经成功切换为PLL时钟）
    while(std_rcc_get_sysclk_source() != RCC_SYSCLK_SRC_STATUS_PLLCLK);

    /*==================== 5. 总线时钟分频配置 ====================*/
    // AHB/APB1/APB2 是单片机内部的总线，给外设传输数据、指令、时钟信号

    // DIV1 = 1分频 = 不分频 → 总线频率 = 系统时钟频率
    
    // AHB总线分频（核心总线，CPU、内存都挂在这）
    std_rcc_set_ahbdiv(RCC_HCLK_DIV1);
    // APB1总线分频（低速外设挂在这）
    std_rcc_set_apb1div(RCC_PCLK1_DIV1);
    // APB2总线分频（高速外设挂在这）
    std_rcc_set_apb2div(RCC_PCLK2_DIV1);

    /*==================== 6. 更新系统时钟变量 ====================*/
    // 更新全局变量 SystemCoreClock 的值
    // 让程序知道当前系统时钟频率是多少（所有外设初始化都依赖这个值）
    SystemCoreClockUpdate();
}

