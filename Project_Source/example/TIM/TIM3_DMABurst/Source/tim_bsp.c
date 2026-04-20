/************************************************************************************************/
/**
* @file               tim_bsp.c
* @author             MCU Ecosystem Development Team
* @brief              TIM BSP驱动函数，实现TIM DMA Burst功能配置。
*                           
*
**************************************************************************************************
* @attention
* Copyright (c) CEC Huada Electronic Design Co.,Ltd. All rights reserved.
*
**************************************************************************************************
*/


/*------------------------------------------includes--------------------------------------------*/
#include "tim_bsp.h"


/*--------------------------------------------define--------------------------------------------*/
#define TIM_DMAREQ_BURST_LEN4    0x04U

/*--------------------------------------------variables-----------------------------------------*/
uint32_t g_src_buffer[4] = {200, 400, 600, 800};

/*-------------------------------------------functions------------------------------------------*/
/**
* @brief  TIM3使能输出，并启动计数
* @retval 无
*/
void bsp_tim3_output_start(void)
{
    /* 使能PWM输出通道 */
    std_tim_ccx_channel_enable(TIM3, TIM_CHANNEL_1);
    std_tim_ccx_channel_enable(TIM3, TIM_CHANNEL_2);
    std_tim_ccx_channel_enable(TIM3, TIM_CHANNEL_3);
    std_tim_ccx_channel_enable(TIM3, TIM_CHANNEL_4);

    /* 使能计数 */
    std_tim_enable(TIM3);
}


/**
* @brief  配置TIM3的DMA Burst功能
* @retval 无
*/
void bsp_tim_dma_config(void)
{
    std_dma_config_t tim3_dma_config = {0};
    
    /* TIM3 DMA 发送配置 */
    tim3_dma_config.dma_channel =DMA_CHANNEL_1;    
    tim3_dma_config.src_addr = (uint32_t)g_src_buffer;
    tim3_dma_config.dst_addr = (uint32_t)&(TIM3->CC1);
    tim3_dma_config.data_number = TIM_DMAREQ_BURST_LEN4;
    
    /* DMA 接收配置 */
    std_dma_start_transmit(&tim3_dma_config);
}


/**
* @brief  配置TIM3的DMA Burst功能，4路通道循环输出不同占空比的PWM波形
* @note   如果外接LED灯，可观测呼吸流水灯现象
* @retval 无
*/
void bsp_tim_dma_burst_loop(void)
{
    static uint32_t dir[4] = {1, 1, 1, 1};
    uint32_t i;
    
    /* 修改PWM输出占空比信息 */
    for(i = 0; i < 4; i++)
    {
        if(dir[i])
        { 
            g_src_buffer[i] += 50; 
        }
        else
        {
            g_src_buffer[i] -= 50; 
        }

        if(g_src_buffer[i] >= 1000) 
        { 
            dir[i] = 0; 
        }
        else if(g_src_buffer[i] <= 50)
        {
            dir[i] = 1;         
        }
    }    
    
    /* 使能TIM3 DMA请求 */
    std_tim_dma_req_enable(TIM3, TIM_DMA_REQ_UPDATE);
    
    /* 配置DMA传输参数 */
    bsp_tim_dma_config();
    
    while(!std_dma_get_flag(DMA_FLAG_TF1));
    
    /* 清除DMA传输完成标志 */
    std_dma_clear_flag(DMA_FLAG_TF1);
    
    /* 禁止DMA传输 */
    std_tim_dma_req_disable(TIM3, TIM_DMA_REQ_UPDATE);
    std_dma_disable(DMA_CHANNEL_1);
    
    /* 延时用于控制呼吸流水灯变化速度 */
    std_delayms(25);
}
