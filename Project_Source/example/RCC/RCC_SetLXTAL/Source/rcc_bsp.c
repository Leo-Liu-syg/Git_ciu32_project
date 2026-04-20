/************************************************************************************************/
/**
* @file               rcc_bsp.c
* @author             MCU Ecosystem Development Team
* @brief              RCC BSP函数，实现RCC时钟配置、切换等功能。
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
* @brief  配置LXTAL时钟参数，包含模式及驱动能力
* @param  lxtal_drv_mode LXTAL驱动模式选择
*             @arg RCC_LXTAL_DRIVE_MODE_NORMAL： 普通模式
*             @arg RCC_LXTAL_DRIVE_MODE_ENHANCE：增强模式
* @param  lxtal_drv_level LXTAL驱动能力配置
*             @arg RCC_LXTAL_DRIVE_LEVEL0
*             @arg RCC_LXTAL_DRIVE_LEVEL1
*             @arg RCC_LXTAL_DRIVE_LEVEL2
*             @arg RCC_LXTAL_DRIVE_LEVEL3
* @note   当LXTAL已使能，则不能配置LXTAL时钟相关参数，需将该时钟禁止后，再进行配置。
* @note   配置LXTAL时，需要解除备份域的写保护，故调用该函数后，会使能PMU时钟，同时使能备份域的写保护。
* @retval 无
*/
void bsp_rcc_lxtal_config(uint32_t lxtal_drv_mode, uint32_t lxtal_drv_level)
{    
    std_rcc_apb1_clk_enable(RCC_PERIPH_CLK_PMU);
    
    /* 要更新LXTAL配置，必须解除备份域的写保护 */
    std_pmu_vaon_write_enable();
    
    /* 配置LXTAL驱动模式参数 */
    std_rcc_lxtal_drive_mode_config(lxtal_drv_mode);
    
    /* 配置LXTAL驱动能力 */
    std_rcc_lxtal_drive_config(lxtal_drv_level);
    
    /* 使能LXTAL */
    std_rcc_lxtal_enable(RCC_LXTAL_ON);
    
    /* 等待LXTAL时钟稳定 */
    while(!std_rcc_get_lxtal_ready());
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

