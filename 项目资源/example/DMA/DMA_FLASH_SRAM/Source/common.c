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
* @brief  DMA通道0中断初始化  
* @retval 无
*/
void dma_init(void)
{
    std_dma_init_t dma_init_param = {0};
    
    /* DMA外设时钟使能 */
    std_rcc_ahb_clk_enable(RCC_PERIPH_CLK_DMA);
    
    /* dma_init 结构体初始化 */
    dma_init_param.dma_channel      = DMA_CHANNEL_0;
    dma_init_param.transfer_type    = DMA_BURST_TRANSFER;
    dma_init_param.dma_req_id       = DMA_REQUEST_SOFTWARE;
    dma_init_param.src_addr_inc     = DMA_SRC_INC_ENABLE;
    dma_init_param.dst_addr_inc     = DMA_DST_INC_ENABLE;
    dma_init_param.data_size        = DMA_DATA_SIZE_WORD;
    dma_init_param.mode             = DMA_MODE_NORMAL;
    
    /* DMA初始化 */
    std_dma_init(&dma_init_param);
    
    /* 使能传输完成中断 */
    std_dma_interrupt_enable(DMA_CHANNEL_0, DMA_INTERRUPT_TF);
    
    /* NVIC初始化 */
    NVIC_SetPriority(DMA_Channel0_IRQn, NVIC_PRIO_1);    
    NVIC_EnableIRQ(DMA_Channel0_IRQn);    
}

