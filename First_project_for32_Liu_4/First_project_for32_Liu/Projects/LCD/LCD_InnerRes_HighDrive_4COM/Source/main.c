#include "main.h"

int main(void)
{
    /* 系统基础初始化 */
    system_clock_config();      // 系统时钟配置
    std_delay_init();           // 延时函数初始化
    gpio_init();                // GPIO初始化

    /* ADC相关初始化 */
    adc_init();                 // ADC初始化
    vrefbuf_init();             // 内部参考电压初始化
    TIM3_init();                // 定时器3初始化（用于触发ADC）
    dma_init();                 // DMA初始化
    bsp_adc_dma_config();       // ADC DMA传输配置
    TIM3_adc_Enable();          // 使能TIM3和ADC

    /* LED初始化 */
    led_init();

    /* 定时器与中断初始化 */
    TIM8_init();                // 定时器8初始化
    bsp_tim8_start();           // 启动定时器8
    nvic_init();                // 中断优先级配置

    /* LCD初始化 */
    lcd_clock_config();
    lcd_config_init();
    lcd_show_all();

    /* 获取ADC初始数据 */
    app_get_adc_data_process();

    /* Flash读取CO校准电压 */
    Flash_Read_Voltage_Check(&adc_co_calibration_value);

    /* 测试宏开关控制 */
#ifdef Temp_test
    lcd_show_temperature(25);   // 温度测试显示
#endif

#ifdef Hum_test
    lcd_show_humidity(50);      // 湿度测试显示
#endif

#ifdef Mid_test
    lcd_show_mid_num(1234);     // 中间值测试显示
#endif

#ifdef CO_test
    Get_CO_Voltage = 3333;      // CO电压测试值
#endif

#ifdef QUICK_RUN
    Running_LCD_counter = 20;   // 快速运行模式
#else
    Running_LCD_counter = 60;   // 正常运行模式
#endif

    /* 主循环 */
    while (1)
    {
        app_Status_Control();    // 系统状态控制
        app_LED_Control();       // LED状态控制
        app_LCD_Control();       // LCD显示控制

        if (tim8_500ms_flag)     // 500ms定时任务
        {
            app_get_adc_data_process();  // 获取ADC数据
            GXHTC3_read();                // 温湿度传感器读取
            lcd_show_temperature(temperature_S);  // 显示温度
            lcd_show_humidity(humidity);          // 显示湿度
        }
    }
}