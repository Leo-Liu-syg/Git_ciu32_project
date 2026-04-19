#include "timer8.h"

/*timer8配置*/
void TIM8_init(void)
{
    std_tim_basic_init_t basic_init_struct = {0};
    uint32_t tmp_psc_value;

    /* TIM8时钟使能 */
    std_rcc_apb1_clk_enable(RCC_PERIPH_CLK_TIM8);

    tmp_psc_value = (uint32_t)(((SystemCoreClock) / 10000) - 1);

    /* 配置TIM8计数器参数 */
    basic_init_struct.prescaler = tmp_psc_value;
    basic_init_struct.counter_mode = TIM_COUNTER_MODE_UP;
    basic_init_struct.period = TIM_ARR_VALUE;
    basic_init_struct.clock_div = TIM_CLOCK_DTS_DIV1;
    basic_init_struct.auto_reload_preload = TIM_AUTORELOAD_PRE_DISABLE;
    std_tim_init(TIM8, &basic_init_struct);
}


/**
 * @brief  TIM8中断服务程序
 * @retval 无
 */

unsigned char tim8_500ms_count = 0; // 定义一个计数器变量，用于计数TIM8更新事件的发生次数
unsigned char tim8_500ms_flag = 0; // 定义一个标志变量，用于指示TIM8更新事件发生
void TIM8_IRQHandler(void)
{
    /* TIM8更新中断处理流程 */
    if ((std_tim_get_interrupt_enable(TIM8, TIM_INTERRUPT_UPDATE)) && (std_tim_get_flag(TIM8, TIM_FLAG_UPDATE)))
    {
        tim8_500ms_count++; // 每次TIM8更新事件发生时，计数器加1
        if(tim8_500ms_count >= 50) // 当计数器达到50时，表示已经发生了500ms的时间（假设TIM8的更新事件频率为100Hz）
        {
            tim8_500ms_count = 0; // 重置计数器
            tim8_500ms_flag = 1; // 设置标志变量，表示TIM8更新事件发生
        }

        
        std_tim_clear_flag(TIM8, TIM_FLAG_UPDATE);
        /* TIM8处理定时中断 */
    }
}

/**
 * @brief  TIM8启动计数，并使能更新事件中断
 * @retval 无
 */
void bsp_tim8_start(void)
{
    /* 使能更新中断 */
    std_tim_interrupt_enable(TIM8, TIM_INTERRUPT_UPDATE);

    /* 开启定时器计数 */
    std_tim_enable(TIM8);
}
