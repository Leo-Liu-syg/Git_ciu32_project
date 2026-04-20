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
#include "common.h"


/*-------------------------------------------functions------------------------------------------*/
/**
* @brief  系统时钟配置
* @retval 无
*/
void system_clock_config(void)
{
    /* 设置Flash读访问等待时间 */
    std_flash_set_latency(FLASH_LATENCY_2CLK);

    /* 使能RCH */
    std_rcc_rch_enable();
    while(!std_rcc_get_rch_ready());

    /* 配置PLL相关参数，并使能 */
    std_rcc_pll_config(RCC_PLLSRC_RCH, RCC_PLLM_MUL12, RCC_PLLN_DIV2, RCC_PLL_DIV2);
    std_rcc_pll_enable();
    while(!std_rcc_get_pll_ready());
    
    std_rcc_pll_output_enable();
    
    /* 设置系统时钟源为PLL */
    std_rcc_set_sysclk_source(RCC_SYSCLK_SRC_PLLCLK);
    while(std_rcc_get_sysclk_source() != RCC_SYSCLK_SRC_STATUS_PLLCLK);

    /* 设置AHB分频因子 */
    std_rcc_set_ahbdiv(RCC_HCLK_DIV1);
    /* 设置APB1、APB2分频因子 */
    std_rcc_set_apb1div(RCC_PCLK1_DIV1);
    std_rcc_set_apb2div(RCC_PCLK2_DIV1);
    
    SystemCoreClockUpdate();
}

/**
* @brief  AES加密初始化
* @retval 无
*/
void aes_encrypt_init(void)
{
    std_aes_init_t aes_init_param;
    
    /* 使能时钟 */
    std_rcc_ahb_clk_enable(RCC_PERIPH_CLK_AES);

    /* 初始化加密模式、按字节交换数据，该数据交换支持的输入输出数据为大端模式 */
    aes_init_param.mode = AES_MODE_ENC;
    aes_init_param.swapped_data_type = AES_SWAPPED_DATA_8BIT;
    std_aes_init(&aes_init_param);
}

/**
* @brief  AES解密初始化
* @retval 无
*/
void aes_decrypt_init(void)
{
    std_aes_init_t aes_init_param;
    
    /* 使能时钟 */
    std_rcc_ahb_clk_enable(RCC_PERIPH_CLK_AES);

    /* 初始化解密模式、按字节交换数据，该数据交换支持的输入输出数据为大端模式 */
    aes_init_param.mode = AES_MODE_DEC;
    aes_init_param.swapped_data_type = AES_SWAPPED_DATA_8BIT;
    std_aes_init(&aes_init_param);
}

