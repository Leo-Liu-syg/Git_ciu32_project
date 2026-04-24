#include "timer8.h"

/*
 * 函数名：TIM8_init
 * 功能  ：定时器8初始化
 * 配置  ：定时中断频率 10000Hz = 10kHz，中断周期 10ms
 */
void TIM8_init(void)
{
    std_tim_basic_init_t basic_init_struct = {0};
    uint32_t tmp_psc_value;

    /* 开启TIM8时钟 */
    std_rcc_apb1_clk_enable(RCC_PERIPH_CLK_TIM8);

    // 预分频配置：SystemCoreClock 分频到 10000Hz
    tmp_psc_value = (uint32_t)(((SystemCoreClock) / 10000) - 1);

    /* TIM8基本参数配置 */
    basic_init_struct.prescaler         = tmp_psc_value;          // 预分频值
    basic_init_struct.counter_mode     = TIM_COUNTER_MODE_UP;    // 向上计数模式
    basic_init_struct.period           = TIM8_ARR_VALUE;         // 自动重装载值
    basic_init_struct.clock_div        = TIM_CLOCK_DTS_DIV1;     // 时钟分频
    basic_init_struct.auto_reload_preload = TIM_AUTORELOAD_PRE_DISABLE; // 重装载预加载使能
    std_tim_init(TIM8, &basic_init_struct);
}

/**
 * @brief  TIM8中断服务函数
 * @note   每10ms进入一次中断，用于计时250ms/500ms/1s标志
 * @retval 无
 */
unsigned char tim8_250ms_count = 0;   // 250ms 计时计数器
unsigned char tim8_250ms_flag  = 0;   // 250ms 时间到标志
unsigned char tim8_500ms_count = 0;   // 500ms 计时计数器
unsigned char tim8_500ms_flag  = 0;   // 500ms 时间到标志
unsigned char tim8_1s_count   = 0;    // 1s 计时计数器
unsigned char tim8_1s_flag    = 0;    // 1s 时间到标志
unsigned int  tim8_wait       = 0;    // 软件延时计数器

void TIM8_IRQHandler(void)
{
    /* 判断是否为TIM8更新中断 */
    if ((std_tim_get_interrupt_enable(TIM8, TIM_INTERRUPT_UPDATE)) && (std_tim_get_flag(TIM8, TIM_FLAG_UPDATE)))
    {
        // 1秒计时
        tim8_1s_count++;
        if (tim8_1s_count >= 100)  // 0.1ms * 100 = 10ms → 你的工程里是1s
        {
            tim8_1s_count = 0;
            tim8_1s_flag = ~tim8_1s_flag;  // 翻转标志
        }

        // 500ms计时
        tim8_500ms_count++;
        if (tim8_500ms_count >= 50)  // 0.1ms * 50 = 5ms → 你的工程里是500ms
        {
            tim8_500ms_count = 0;
            tim8_500ms_flag = ~tim8_500ms_flag;
            Running_LCD_Flag = 1;
        }

        // 250ms计时
        tim8_250ms_count++;
        if (tim8_250ms_count >= 25)
        {
            tim8_250ms_count = 0;
            tim8_250ms_flag = ~tim8_250ms_flag;
        }

        // 软件延时递减
        if (tim8_wait)
        {
            tim8_wait--;
        }

        // 清除TIM8更新中断标志位
        std_tim_clear_flag(TIM8, TIM_FLAG_UPDATE);
    }
}

/**
 * @brief  启动TIM8，并开启更新中断
 * @retval 无
 */
void bsp_tim8_start(void)
{
    /* 使能TIM8更新中断 */
    std_tim_interrupt_enable(TIM8, TIM_INTERRUPT_UPDATE);

    /* 启动TIM8计数器 */
    std_tim_enable(TIM8);
}