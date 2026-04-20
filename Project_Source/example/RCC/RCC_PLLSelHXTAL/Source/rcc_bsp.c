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
* @brief  NMI中断服务函数
* @retval 无
*/
void NMI_Handler(void)
{    
    /* 清除HXTAL CSS标志 */
    std_rcc_clear_flag(RCC_CLEAR_HXTALCSS);
    
    /* 切换系统时钟为RCH 2分频(8MHz) */
    
    /* 用户可根据实际应用，实现HXTAL CSS中断处理函数 */
    
}

/**
* @brief  配置系统时钟为PLL，且PLL时钟源为HXTAL(HXTAL频率默认为8MHz)
* @retval 无
*/
void bsp_rcc_pll_selhxtal(void)
{
    /* 设置Flash读访问等待时间 */
    std_flash_set_latency(FLASH_LATENCY_2CLK);

    /* 配置HXTAL为晶体模式 */
    bsp_rcc_hxtal_config(RCC_HXTAL_ON, RCC_HXTAL_DRIVE_LEVEL1);
    
    /* 设置PLL相关参数，并使能 */
    std_rcc_pll_config(RCC_PLLSRC_HXTAL, RCC_PLLM_MUL12, RCC_PLLN_DIV1, RCC_PLL_DIV2);
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
* @brief  配置HXTAL时钟参数，包含模式及驱动能力
* @param  mode HXTAL模式选择
*             @arg RCC_HXTAL_ON：    使能晶体模式
*             @arg RCC_HXTAL_BYPASS：使能外部时钟模式
* @param  hxtal_drv HXTAL驱动能力配置
*             @arg RCC_HXTAL_DRIVE_LEVEL0
*             @arg RCC_HXTAL_DRIVE_LEVEL1
*             @arg RCC_HXTAL_DRIVE_LEVEL2
*             @arg RCC_HXTAL_DRIVE_LEVEL3
* @note   当HXTAL已使能，则不能配置HXTAL时钟相关参数，需将该时钟禁止后，再进行配置。
* @retval 无
*/
void bsp_rcc_hxtal_config(uint32_t mode, uint32_t hxtal_drv)
{   
    if (mode == RCC_HXTAL_ON)
    {        
        /* 配置HXTAL驱动能力 */
        std_rcc_hxtal_drive_config(hxtal_drv);
    }
    
    /* 使能HXTAL */
    std_rcc_hxtal_enable(mode);
    
    /* 等待HXTAL时钟稳定 */
    while(!std_rcc_get_hxtal_ready());
}

/**
* @brief  配置MCO输出使能
* @param  mco_source MCO输出时钟源
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

