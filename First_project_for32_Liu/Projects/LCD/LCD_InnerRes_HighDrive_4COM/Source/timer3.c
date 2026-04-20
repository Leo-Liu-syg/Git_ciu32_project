#include "timer3.h"
/**
* @brief  TIMER初始化
* @retval 无
*/
void TIM3_init(void)
{
    std_tim_basic_init_t basic_init_struct = {0};
    uint32_t tmp_psc_value;
    
    /* TIM3时钟使能 */
    std_rcc_apb1_clk_enable(RCC_PERIPH_CLK_TIM3);
    
    tmp_psc_value = (uint32_t)(((SystemCoreClock) / 1000000) - 1);
        
    /* 配置TIM3计数器参数 */
    basic_init_struct.prescaler = tmp_psc_value;
    basic_init_struct.counter_mode = TIM_COUNTER_MODE_UP;
    basic_init_struct.period = TIM8_ARR_VALUE;
    basic_init_struct.clock_div = TIM_CLOCK_DTS_DIV1;
    basic_init_struct.auto_reload_preload = TIM_AUTORELOAD_PRE_DISABLE;
    std_tim_init(TIM3, &basic_init_struct);

    std_tim_trigout_mode_config(TIM3, TIM_TRIG_OUT_UPDATE);
}

void TIM3_adc_Enable(void)
{
    /* 开启定时器计数 */
    std_tim_enable(TIM3);
    std_adc_start_conversion();
}