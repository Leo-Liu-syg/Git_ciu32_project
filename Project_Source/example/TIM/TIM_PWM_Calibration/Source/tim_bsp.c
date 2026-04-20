/************************************************************************************************/
/**
* @file               tim_bsp.c
* @author             MCU Ecosystem Development Team
* @brief              TIM BSP驱动函数，实现TIM异步触发同步输出功能配置。
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
#define CCX_ARRAY_SIZE           (0x100)


/*--------------------------------------------variables-----------------------------------------*/
uint32_t g_tim3_ch1_ccx_value[CCX_ARRAY_SIZE] = {0};
uint32_t g_tim3_ch1_ccx_sub_value[CCX_ARRAY_SIZE] = {0};

uint16_t g_ccx_sample_size = 100;

__IO uint32_t g_tim3_ch1_counter = 0;
__IO bsp_tim3_capture_info_t  g_tim3_capture_info = {0};

/*-------------------------------------------functions------------------------------------------*/
/**
* @brief  TIM3中断处理程序
* @retval 无
*/
void TIM3_IRQHandler(void)
{
    uint32_t ccx_sub_value_temp;
    uint32_t i;
    
    /* CC1中断处理流程 */
    if ((std_tim_get_flag(TIM3, TIM_FLAG_CC1)) && (std_tim_get_interrupt_enable(TIM3, TIM_INTERRUPT_CC1)))
    {
        g_tim3_ch1_ccx_value[g_tim3_ch1_counter++] = std_tim_get_ccx_value(TIM3, TIM_CHANNEL_1);

        if(g_tim3_ch1_counter == g_ccx_sample_size)
        {            
            /* 关闭CC1中断 */
            std_tim_interrupt_disable(TIM3, TIM_INTERRUPT_CC1);
            
            ccx_sub_value_temp = 0;                
            g_tim3_ch1_counter = 0;
            
            /* 计算相邻捕获的间隔计数值 */
            for(i = 1 ;i < g_ccx_sample_size; i++)
            {
                g_tim3_ch1_ccx_sub_value[i-1] = g_tim3_ch1_ccx_value[i] - g_tim3_ch1_ccx_value[i-1]; 
            }
            
            /* 获取g_tim3_ch1_ccx_sub_value 数组中最大和最小值 */
            g_tim3_capture_info.ch1_cc_sub_value_min =  g_tim3_ch1_ccx_sub_value[0];
            g_tim3_capture_info.ch1_cc_sub_value_max =  g_tim3_ch1_ccx_sub_value[0];
            
            for(i = 0; i < (g_ccx_sample_size - 1); i++)
            {
                ccx_sub_value_temp += g_tim3_ch1_ccx_sub_value[i];
                
                if(g_tim3_capture_info.ch1_cc_sub_value_min > g_tim3_ch1_ccx_sub_value[i])
                {
                    g_tim3_capture_info.ch1_cc_sub_value_min = g_tim3_ch1_ccx_sub_value[i];
                }
                
                if(g_tim3_capture_info.ch1_cc_sub_value_max < g_tim3_ch1_ccx_sub_value[i])
                {
                    g_tim3_capture_info.ch1_cc_sub_value_max = g_tim3_ch1_ccx_sub_value[i];
                }
            }
            
            /* 计算g_tim3_ch1_ccx_sub_value 数组中的平均值 */
            g_tim3_capture_info.ch1_cc_sub_value_avg = ccx_sub_value_temp/(g_ccx_sample_size - 1);
            
            /* 标记捕获完成 */
            g_tim3_capture_info.ch1_cc_end = 1;
        }
    }    
}



/**
* @brief  TIM3输入捕获TIM5输出的PWM，并对PWM校准
* @param  freq PWM输出的目标频率
* @note   PWM输出的频率为精准频率，即能被LXTAL_VALUE(32768Hz)整除，否则校准后的误差较大 
* @retval 无
*/
void bsp_pwm_calibration(uint32_t freq)
{    
    int8_t adjust_direction;
    uint32_t adjust_value;
    uint32_t trimming_arr_value;    
    uint32_t tim5_psc_value;
    uint32_t tim3_pre_value;
    uint32_t tim3_obj_sub_value;
    
    int8_t direction_tmp = 0;

    uint32_t freq_tmp = (LXTAL_VALUE/freq);

    /* 初始化校准参数、调整方向 */
    adjust_value = 100U;
    adjust_direction = 1;
    
    /* 计算TIM3的目标CCx值 */
    tim3_pre_value = (TIM3->SMC & TIM_SMC_ET_PRE) >> TIM_SMC_ET_PRE_POS;
    tim3_obj_sub_value = freq_tmp/(1 << tim3_pre_value); 
    
    tim5_psc_value = std_tim_get_psc(TIM5);
    trimming_arr_value = (SystemCoreClock / ((tim5_psc_value + 1) * freq)) - 1;
    
    while(adjust_value != 0U)
    {
        direction_tmp = 0;
        
        /* 设置TIM ARR值 */
        std_tim_set_autoreload(TIM5, trimming_arr_value);
        
        /* 配置比较匹配值 */
        std_tim_set_ccx_value(TIM5, TIM_CHANNEL_1, (trimming_arr_value >> 1));
        
        /* 启动TIM3输入捕获，并使能TIM5 PWM输出 */
        bsp_tim3_capture_start();
        bsp_tim5_output_start();
        
        /* 等待输入捕获完成 */
        while(!g_tim3_capture_info.ch1_cc_end);
            
        /* TIM3停止计数，并禁止捕获事件中断 */
        bsp_tim3_capture_stop();
            
        /* TIM5停止输出PWM */
        bsp_tim5_output_stop();
            
        /* 通过LXTAL输入捕获的CCx值，校准RCH时钟 */
        if(g_tim3_capture_info.ch1_cc_sub_value_avg > tim3_obj_sub_value)
        {
            /* 本次校准方向，负方向 */
            direction_tmp = -1;
        }
        else if(g_tim3_capture_info.ch1_cc_sub_value_avg < tim3_obj_sub_value)
        {
            /* 本次校准方向，正方向 */
            direction_tmp = 1;
        }
        else
        {
            uint32_t min_tmp = (tim3_obj_sub_value - g_tim3_capture_info.ch1_cc_sub_value_min);
            uint32_t max_tmp = (g_tim3_capture_info.ch1_cc_sub_value_max - tim3_obj_sub_value);
            
            /* 最小值偏离校准目标值远，调整最小值向目标频率靠近 */
            if(min_tmp > max_tmp)
            {
                /* 本次校准方向，正方向 */
                direction_tmp = 1;
            }
            /* 最大值偏离校准目标值远，调整最大值向目标频率靠近 */
            else if(min_tmp < max_tmp)
            {
                /* 本次校准方向，负方向 */
                direction_tmp = -1;
            }
            /* 最大值和最小值都相等，校验成功*/
            else
            {
                 break;
            }
        }
        
        /* 本次调整校准参数 */
        direction_tmp > 0 ? (trimming_arr_value += adjust_value) : (trimming_arr_value -= adjust_value);
        
        /* 检查调整方向 */
        if(adjust_direction != direction_tmp)
        {
            /* 调整方向改变，调整参数右移 */
            adjust_direction = direction_tmp;
            adjust_value >>= 1U;
        }
    }
}


/**
* @brief  计算拟合点的频率
* @param  obj_freq PWM输出的目标频率
* @param  match_freq 拟合点频率指针
* @retval 无
*/
void bsp_cal_match_freq(uint32_t obj_freq, uint32_t *match_freq)
{
    /* 设置精准频率数组，即1，2，4，8...512，1024，2048，4096 */
    const uint16_t arr_freq[13] = {1,2,4,8,16,32,64,128,256,512,1024,2048,4096};
       
    if(obj_freq >= 4096)
    {
        match_freq[0] = 2048;
        match_freq[1] = 4096;
    }
    else
    {
        for(uint8_t i = 0; i < 12; i++)
        {          
            if((obj_freq >= arr_freq[i]) && (obj_freq <= arr_freq[i+1]))
            {
                match_freq[0] = arr_freq[i];
                match_freq[1] = arr_freq[i+1];
                break;
            }
        }
    }
}


/**
* @brief  TIM5输出的PWM频率拟合，拟合公式y = kx + b，其中x是1/freq，y是TIM ARR值;
* @param  freq PWM输出的目标频率
* @note   拟合过程比较耗时
* @retval 无
*/
void bsp_pwm_matching(uint32_t freq)
{
    /* 拟合的两个点 */
    uint32_t match_freq[2];
    uint16_t arr1, arr2;
    
    /* 拟合后的目标 ARR值 */
    uint16_t arr_value;
    float k, b;
    
    /* 设置拟合的两个点的频率(Hz) */
    bsp_cal_match_freq(freq, match_freq);
    
    bsp_pwm_calibration(match_freq[0]);
    arr1 = std_tim_get_autoreload(TIM5);

    bsp_pwm_calibration(match_freq[1]);
    arr2 = std_tim_get_autoreload(TIM5);

    k = (float)(arr2 - arr1)/(1.0/match_freq[1] - 1.0/match_freq[0]);
    b = arr1- k/match_freq[0];
    
    arr_value = (uint16_t)((k / freq + b) + 0.5);
    
    /* 设置TIM ARR值 */
    std_tim_set_autoreload(TIM5, arr_value);
    
    /* 配置比较匹配值 */
    std_tim_set_ccx_value(TIM5, TIM_CHANNEL_1, (arr_value >> 1));
}


/**
* @brief  根据PWM输出的目标频率，调整TIM3、TIM5的预分频值
* @param  freq PWM输出的目标频率
* @retval 无
*/
void bsp_tim_psc_config(uint16_t freq)
{
    uint32_t arr_value = 0xFFFF;
    uint16_t psc_value = 0;
    uint16_t count = 0;
    
    /* 根据目标频率范围，设置TIM5预分频 */
    do
    {
        std_tim_set_psc(TIM5, psc_value);
        arr_value = ((SystemCoreClock / ((psc_value + 1) * freq)) - 1);
        psc_value = ((count++) << 1) + 1;        
    }while(arr_value > (0x10000>>1));
    
    /* 根据目标频率范围，设置TIM3预分频 */
    /* PWM输出频率范围 [1, 32] */
    if((freq >= 1)  && (freq < 32))
    {
        std_tim_etr_config(TIM3, TIM_ETR_POL_HIGH, TIM_ETR_PSC_DIV8, 0x00);
        g_ccx_sample_size = 4;
    }
    /* PWM输出频率范围 [32, 128] */
    else if(freq >= 32  && freq < 128)
    {
        std_tim_etr_config(TIM3, TIM_ETR_POL_HIGH, TIM_ETR_PSC_DIV4, 0x00);
        g_ccx_sample_size = 10;
    }
    /* PWM输出频率范围 [128, 512] */
    else if(freq >= 128  && freq < 512)
    {
        std_tim_etr_config(TIM3, TIM_ETR_POL_HIGH, TIM_ETR_PSC_DIV2, 0x00);
        g_ccx_sample_size = 50;
    }
    /* PWM输出频率范围 (512, 5000] */
    else
    {
        std_tim_etr_config(TIM3, TIM_ETR_POL_HIGH, TIM_ETR_PSC_DIV1, 0x00);
        g_ccx_sample_size = 100;
    }    
}


/**
* @brief  检查PWM目标频率是否是精准频率，即1，2，4，8...512，1024，2048，4096
* @param  freq PWM输出的目标频率
* @retval bool 返回逻辑表达式的判断结果
*             @arg true： 表示PWM输出频率为精准频率
*             @arg false：表示PWM输出频率非精准频率
*/
bool bsp_check_pwm_freq(uint16_t freq)
{
    if((LXTAL_VALUE % freq))
    {
        return false;
    }
    else
    {
        return true;
    }
}


/**
* @brief  TIM3启动计数，并使能捕获事件中断
* @retval 无
*/
void bsp_tim3_capture_start(void)
{
    g_tim3_capture_info.ch1_cc_end = 0;
    
    /* 清除通道1输入捕获标志 */
    std_tim_clear_flag(TIM3, TIM_FLAG_CC1);
    
    /* 使能TIM3输入捕获通道1中断 */
    std_tim_interrupt_enable(TIM3, TIM_INTERRUPT_CC1);
    
    /* 使能输入捕获 */
    std_tim_ccx_channel_enable(TIM3, TIM_CHANNEL_1);
    
    /* 启动TIM3计数 */
    std_tim_enable(TIM3);
}


/**
* @brief  TIM3停止计数，并禁止捕获事件中断
* @retval 无
*/
void bsp_tim3_capture_stop(void)
{
    /* 禁止输入捕获 */
    std_tim_ccx_channel_disable(TIM3, TIM_CHANNEL_1);
    
    /* 停止TIM3计数 */
    std_tim_disable(TIM3);
    
    /* TIM计数值清0 */
    std_tim_set_counter(TIM3, 0x00);
}


/**
* @brief  TIM5通道1启动PWM输出
* @retval 无
*/
void bsp_tim5_output_start(void)
{
    /* 使能PWM输出通道 */
    std_tim_ccx_channel_enable(TIM5, TIM_CHANNEL_1);

    /* 使能计数 */
    std_tim_enable(TIM5);    
}


/**
* @brief  TIM5通道1停止PWM输出 
* @retval 无
*/
void bsp_tim5_output_stop(void)
{
    /* 停止PWM输出通道 */
    std_tim_ccx_channel_disable(TIM5, TIM_CHANNEL_1);

    /* 禁止计数 */
    std_tim_disable(TIM5);
}


