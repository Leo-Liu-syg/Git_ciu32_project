/*------------------------------------------ 头文件包含 --------------------------------------------*/
#include "ssc.h"

/**
 * @brief 系统时钟配置
 * @note  时钟来源：内部高速时钟 RCH，通过 PLL 倍频后作为系统主时钟
 * @note  作用：为 CPU、外设提供稳定工作频率
 */
void system_clock_config(void)
{
    // 设置Flash等待周期，保证高频下稳定运行
    std_flash_set_latency(FLASH_LATENCY_2CLK);

    // 使能内部高速时钟 RCH
    std_rcc_rch_enable();
    // 等待 RCH 时钟稳定
    while(!std_rcc_get_rch_ready());

    // PLL配置：时钟源=RCH，倍频系数=12，分频系数=2
    std_rcc_pll_config(RCC_PLLSRC_RCH, RCC_PLLM_MUL12, RCC_PLLN_DIV2, RCC_PLL_DIV2);
    // 使能 PLL
    std_rcc_pll_enable();
    // 等待 PLL 锁定稳定
    while(!std_rcc_get_pll_ready());

    // 使能 PLL 输出
    std_rcc_pll_output_enable();    

    // 选择 PLL 作为系统主时钟
    std_rcc_set_sysclk_source(RCC_SYSCLK_SRC_PLLCLK);
    // 等待时钟切换完成
    while(std_rcc_get_sysclk_source() != RCC_SYSCLK_SRC_STATUS_PLLCLK);

    // AHB 不分频
    std_rcc_set_ahbdiv(RCC_HCLK_DIV1);
    // APB1 不分频
    std_rcc_set_apb1div(RCC_PCLK1_DIV1);
    // APB2 不分频
    std_rcc_set_apb2div(RCC_PCLK2_DIV1);

    // 更新系统核心时钟频率变量
    SystemCoreClockUpdate();
}