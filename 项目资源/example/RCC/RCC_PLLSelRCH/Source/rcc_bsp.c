/************************************************************************************************/
/**
* @file               rcc_bsp.c
* @author             MCU Ecosystem Development Team
* @brief              RCC BSP驱动函数，实现RCC时钟配置、切换等功能。
*
*
**************************************************************************************************
* @attention
* Copyright (c) CEC Huada Electronic Design Co.,Ltd. All rights reserved.
*
**************************************************************************************************
*/

/*------------------------------------------includes--------------------------------------------*/
#include "rcc_bsp.h"

/*-------------------------------------------functions------------------------------------------*/
/**
* @brief  配置系统时钟为PLL(48MHz)，且PLL时钟源为RCH
* @retval 无
*/
void bsp_rcc_pll_selrch(void)
{
    /* 设置Flash读访问等待时间 */
    std_flash_set_latency(FLASH_LATENCY_2CLK);

    /* 配置RCH，并使能 */
    std_rcc_rch_enable();
    while(!std_rcc_get_rch_ready());
        
    /* 设置PLL相关参数，并使能 */
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
* @brief  配置MCO输出使能
* @param  mco_source MCO输出时钟源
*             @arg  RCC_MCO_SRC_DISABLE：禁止MCO
*             @arg  RCC_MCO_SRC_SYSCLK： MCO的时钟源为系统时钟
*             @arg  RCC_MCO_SRC_RCH：    MCO的时钟源为RCH
*             @arg  RCC_MCO_SRC_HXTAL：  MCO的时钟源为HXTAL
*             @arg  RCC_MCO_SRC_PLLCLK： MCO的时钟源为PLL
*             @arg  RCC_MCO_SRC_RCL：    MCO的时钟源为RCL
*             @arg  RCC_MCO_SRC_LXTAL：  MCO的时钟源为LXTAL
* @param  mco_psc MCO输出时钟分频值
*             @arg  RCC_MCO_DIV1：  1分频
*             @arg  RCC_MCO_DIV2：  2分频
*             @arg  RCC_MCO_DIV4：  4分频
*             @arg  RCC_MCO_DIV8：  8分频
*             @arg  RCC_MCO_DIV16： 16分频
*             @arg  RCC_MCO_DIV32： 32分频
*             @arg  RCC_MCO_DIV64： 64分频
*             @arg  RCC_MCO_DIV128：128分频
* @retval 无
*/
void bsp_rcc_mco_config(uint32_t mco_source, uint32_t mco_psc)
{
    std_gpio_init_t mco_gpio_cfg = {0};
    
    /* 配置PA8作为MCO输出引脚 */    
    std_rcc_gpio_clk_enable(RCC_PERIPH_CLK_GPIOA);
    
    mco_gpio_cfg.pin = GPIO_PIN_8;
    mco_gpio_cfg.mode = GPIO_MODE_ALTERNATE;
    mco_gpio_cfg.alternate = GPIO_AF0_MCO;
    mco_gpio_cfg.pull = GPIO_NOPULL;
    
    std_gpio_init(GPIOA, &mco_gpio_cfg);
    
    std_rcc_mco_config(mco_source, mco_psc);
}

