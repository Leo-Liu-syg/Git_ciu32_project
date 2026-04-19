#include "lcd.h"

uint32_t mid_num;
uint32_t temperature;
uint8_t temperature_low; // 负温度
uint32_t humidity;
uint8_t f_all_zero = 0;

uint32_t com1 = 0XFFFF;
uint32_t com2 = 0XFFFF;
uint32_t com3 = 0XFFFF;
uint32_t com4 = 0XFFFF;
uint32_t com5 = 0XFFFF;

void lcd_clock_config()
{
    std_rcc_apb1_clk_enable(RCC_PERIPH_CLK_PMU);
    std_rcc_rcl_enable();
    // while (!std_rcc_get_rcl_ready)
    //     ;
    std_delayus(100);
    /* 要更新LXTAL配置，必须解除备份域的写保护 */
    std_pmu_vaon_write_enable();

    /* 配置LCD 时钟源为内部低速时钟 */
    std_rcc_set_rtcclk_source(RCC_RTC_ASYNC_CLK_SRC_RCL);

    /* 恢复备份域的写保护 */
    std_pmu_vaon_write_disable();
}

void lcd_config_init(void)
{
    std_lcd_init_t lcd_config = {0};
    lcd_clock_config();

    std_rcc_apb1_clk_enable(RCC_PERIPH_CLK_LCD);
    /* LCD防极化 */
    std_lcd_set_anti_polar(LCD_ANTIPOLAR_GND);

    std_lcd_v1_cap_enable();
    std_lcd_v2_cap_enable();
    std_lcd_v3_cap_enable();
    std_lcd_vlcd_cap_enable();
    /* LCD参数配置 */
    lcd_config.bias = LCD_BIAS_1_3;
    lcd_config.duty = LCD_DUTY_1_6;
    lcd_config.prescaler = LCD_PRESCALER_2; /* 配置帧速率为32Hz */
    lcd_config.divider = LCD_DIVIDER_31;
    lcd_config.driver_mode = LCD_DRIVER_MODE_OUTER_CAPACITY;
    lcd_config.vlcd_voltage = LCD_INNER_RESISTER_1_000_VDD; /* VLCD = VDD */
    lcd_config.high_drive = LCD_HIGH_DRIVE;
    lcd_config.mux_segment = LCD_MUX_SEGMENT_ENABLE; /* 1/6 和 1/4duty需使能区段多路复用，1/8duty为无关项 */

    /* LCD 初始化 */
    std_lcd_init(&lcd_config);
    /* LCD COM和SEG波形输出 */
    std_lcd_set_scoc(LCD_SCOC_OUT);
}

void lcd_show_all()
{
    com5 |= (1 << 15);
    com5 |= (1 << 3);
    com4 |= (1 << 10);
    com5 |= (1 << 4);
    std_lcd_write(LCD_RAM_REGISTER1, com1, com1);
    std_lcd_write(LCD_RAM_REGISTER2, com2, com2);
    std_lcd_write(LCD_RAM_REGISTER3, com3, com3);
    std_lcd_write(LCD_RAM_REGISTER4, com4, com4);
    std_lcd_write(LCD_RAM_REGISTER5, com5, com5);
}