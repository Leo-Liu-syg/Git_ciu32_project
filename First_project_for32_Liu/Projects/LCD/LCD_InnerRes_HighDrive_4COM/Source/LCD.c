#include "lcd.h"

uint32_t mid_num;

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

void lcd_write_to_RAM(void)
{
    std_lcd_write(LCD_RAM_REGISTER1, com1, com1);
    std_lcd_write(LCD_RAM_REGISTER2, com2, com2);
    std_lcd_write(LCD_RAM_REGISTER3, com3, com3);
    std_lcd_write(LCD_RAM_REGISTER4, com4, com4);
    std_lcd_write(LCD_RAM_REGISTER5, com5, com5);
}

void lcd_clean_all(void)
{
    com1 = 0X0000;
    com2 = 0X0000;
    com3 = 0X0000;
    com4 = 0X0000;
    com5 = 0X0000;
}

void lcd_show_all()
{
    com1 = 0XFFFF;
    com2 = 0XFFFF;
    com3 = 0XFFFF;
    com4 = 0XFFFF;
    com5 = 0XFFFF;
    lcd_write_to_RAM();
}

void lcd_show_temperature(uint16_t Temp_value)
{
    uint8_t i, j, k;
    // 清空温度显示区域原有段码（防止残留显示）
    com1 &= ~((1 << 11) | (1 << 12) | (1 << 13) | (1 << 14)); // 清空COM1的S11~S14
    com2 &= ~((1 << 11) | (1 << 12) | (1 << 13) | (1 << 14)); // 清空COM2的S11~S14
    com3 &= ~((1 << 11) | (1 << 12) | (1 << 13) | (1 << 14)); // 清空COM3的S11~S14
    com4 &= ~((1 << 11) | (1 << 13) | (1 << 14));             // 清空COM4的S11、S13、S14
    com5 &= ~((1 << 14));                                     // 清空COM5的S14（负数图标位）

    com4 |= (1 << 15); // 温度图标
    // 温度标志
    com5 |= (1 << 11);

    // if (temperature_F)//华氏度显示
    // {
    // com4 &= ~(1 << 12);
    // com5 |= (1 << 12);
    // }
    // else//摄氏度显示
    // {
    com4 |= (1 << 12);
    com5 &= ~(1 << 12);
    // }

    // if (temperature_low)
    // {
    //     com5 |= (1 << 14); // 负数图标
    //     if (Temp_value > 99)
    //         Temp_value = 99;
    // }
    // else
    // {
    if (Temp_value > 188)
        Temp_value = 188;
    // }

    // 处理百位（仅常规模式可能有值，低温模式最大99无百位）
    i = Temp_value / 100;
    if (i)
        com4 |= (1 << 14); // 有百位时点亮COM4+S14 → 温度百位显示位
                           // 处理十位和个位
    Temp_value %= 100;
    j = Temp_value / 10;
    switch (j) // 十位转换器
    {
    case 0:
        if (i != 0)
        {
            com1 |= ((1 << 13) | (1 << 14));
            com2 |= ((1 << 13));
            com3 |= ((1 << 13) | (1 << 14));
            com4 |= ((1 << 13));
        }
        break;
    case 1:
        com2 |= ((1 << 13));
        com3 |= ((1 << 13));
        break;
    case 2:
        com1 |= ((1 << 13));
        com2 |= ((1 << 13) | (1 << 14));
        com3 |= ((1 << 14));
        com4 |= ((1 << 13));
        break;
    case 3:
        com1 |= ((1 << 13));
        com2 |= ((1 << 13) | (1 << 14));
        com3 |= ((1 << 13));
        com4 |= ((1 << 13));
        break;
    case 4:
        com1 |= ((1 << 14));
        com2 |= ((1 << 13) | (1 << 14));
        com3 |= ((1 << 13));
        break;
    case 5:
        com1 |= ((1 << 13) | (1 << 14));
        com2 |= ((1 << 14));
        com3 |= ((1 << 13));
        com4 |= ((1 << 13));
        break;
    case 6:
        com1 |= ((1 << 13) | (1 << 14));
        com2 |= ((1 << 14));
        com3 |= ((1 << 13) | (1 << 14));
        com4 |= ((1 << 13));
        break;
    case 7:
        com1 |= ((1 << 13));
        com2 |= ((1 << 13));
        com3 |= ((1 << 13));
        break;
    case 8:
        com1 |= ((1 << 13) | (1 << 14));
        com2 |= ((1 << 13) | (1 << 14));
        com3 |= ((1 << 13) | (1 << 14));
        com4 |= ((1 << 13));
        break;
    case 9:
        com1 |= ((1 << 13) | (1 << 14));
        com2 |= ((1 << 13) | (1 << 14));
        com3 |= ((1 << 13));
        com4 |= ((1 << 13));
        break;
    }

    k = Temp_value % 10; // 个位转换器
    switch (k)
    {
    case 0:
        com1 |= ((1 << 11) | (1 << 12));
        com2 |= ((1 << 11) | (1 << 12));
        com3 |= ((1 << 11) | (1 << 12));
        com4 |= ((1 << 11));
        break;
    case 1:
        com2 |= ((1 << 11));
        com3 |= ((1 << 11));
        break;
    case 2:
        com1 |= ((1 << 11));
        com2 |= ((1 << 11) | (1 << 12));
        com3 |= ((1 << 12));
        com4 |= ((1 << 11));
        break;
    case 3:
        com1 |= ((1 << 11));
        com2 |= ((1 << 11) | (1 << 12));
        com3 |= ((1 << 11));
        com4 |= ((1 << 11));
        break;
    case 4:
        com1 |= ((1 << 12));
        com2 |= ((1 << 11) | (1 << 12));
        com3 |= ((1 << 11));
        break;
    case 5:
        com1 |= ((1 << 11) | (1 << 12));
        com2 |= ((1 << 12));
        com3 |= ((1 << 11));
        com4 |= ((1 << 11));
        break;
    case 6:
        com1 |= ((1 << 11) | (1 << 12));
        com2 |= ((1 << 12));
        com3 |= ((1 << 11) | (1 << 12));
        com4 |= ((1 << 11));
        break;
    case 7:
        com1 |= ((1 << 11));
        com2 |= ((1 << 11));
        com3 |= ((1 << 11));
        break;
    case 8:
        com1 |= ((1 << 11) | (1 << 12));
        com2 |= ((1 << 11) | (1 << 12));
        com3 |= ((1 << 11) | (1 << 12));
        com4 |= ((1 << 11));
        break;
    case 9:
        com1 |= ((1 << 11) | (1 << 12));
        com2 |= ((1 << 11) | (1 << 12));
        com3 |= ((1 << 11));
        com4 |= ((1 << 11));
        break;
    }
    lcd_write_to_RAM();
}

void lcd_show_humidity(uint16_t Hum_vlaue)
{
    uint8_t j, k;
    // 水滴&RH
    com4 |= (1 << 1);
    com5 |= (1 << 10);

    if (Hum_vlaue > 99)
    {
        Hum_vlaue = 99;
    }

    com1 &= ~((1 << 1) | (1 << 2));
    com2 &= ~((1 << 1) | (1 << 2));
    com3 &= ~((1 << 1) | (1 << 2));
    com4 &= ~((1 << 2) | (1 << 6));
    com5 &= ~((1 << 1) | (1 << 5) | (1 << 6) | (1 << 7) | (1 << 8) | (1 << 9));

    j = Hum_vlaue / 10;
    switch (j)
    {
    case 0:
        break;
    case 1:
        com4 |= ((1 << 6));
        com5 |= ((1 << 5));
        break;
    case 2:
        com5 |= ((1 << 7) | (1 << 6) | (1 << 9) | (1 << 1));
        com4 |= ((1 << 6));
        break;
    case 3:
        com5 |= ((1 << 7) | (1 << 6) | (1 << 5) | (1 << 1));
        com4 |= ((1 << 6));
        break;
    case 4:
        com5 |= ((1 << 8) | (1 << 6) | (1 << 5));
        com4 |= ((1 << 6));
        break;
    case 5:
        com5 |= ((1 << 7) | (1 << 8) | (1 << 6) | (1 << 5) | (1 << 1));
        break;
    case 6:
        com5 |= ((1 << 7) | (1 << 8) | (1 << 6) | (1 << 9) | (1 << 5) | (1 << 1));
        break;
    case 7:
        com5 |= ((1 << 7) | (1 << 5));
        com4 |= ((1 << 6));
        break;
    case 8:
        com5 |= ((1 << 7) | (1 << 8) | (1 << 9) | (1 << 5) | (1 << 1) | (1 << 6));
        com4 |= ((1 << 6));
        break;
    case 9:
        com5 |= ((1 << 7) | (1 << 8) | (1 << 6) | (1 << 5) | (1 << 1));
        com4 |= ((1 << 6));
        break;
    }

    k = Hum_vlaue % 10;
    switch (k)
    {
    case 0:
        com1 |= ((1 << 1) | (1 << 2));
        com2 |= ((1 << 2));
        com3 |= ((1 << 1) | (1 << 2));
        com4 |= ((1 << 2));
        break;
    case 1:
        com2 |= ((1 << 2));
        com3 |= ((1 << 2));
        break;
    case 2:
        com1 |= ((1 << 1) | (1 << 2));
        com2 |= ((1 << 1));
        com3 |= ((1 << 2));
        com4 |= ((1 << 2));
        break;
    case 3:
        com1 |= ((1 << 2));
        com2 |= ((1 << 1) | (1 << 2));
        com3 |= ((1 << 2));
        com4 |= ((1 << 2));
        break;
    case 4:
        com2 |= ((1 << 1) | (1 << 2));
        com3 |= ((1 << 1) | (1 << 2));
        break;
    case 5:
        com1 |= ((1 << 2));
        com2 |= ((1 << 1) | (1 << 2));
        com3 |= ((1 << 1));
        com4 |= ((1 << 2));
        break;
    case 6:
        com1 |= ((1 << 1) | (1 << 2));
        com2 |= ((1 << 1) | (1 << 2));
        com3 |= ((1 << 1));
        com4 |= ((1 << 2));
        break;
    case 7:
        com2 |= ((1 << 2));
        com3 |= ((1 << 2));
        com4 |= ((1 << 2));
        break;
    case 8:
        com1 |= ((1 << 1) | (1 << 2));
        com2 |= ((1 << 1) | (1 << 2));
        com3 |= ((1 << 1) | (1 << 2));
        com4 |= ((1 << 2));
        break;
    case 9:
        com1 |= ((1 << 2));
        com2 |= ((1 << 1) | (1 << 2));
        com3 |= ((1 << 1) | (1 << 2));
        com4 |= ((1 << 2));
        break;
    }

    lcd_write_to_RAM();
}

void lcd_show_dash(void)
{
    com1 &= ~((1 << 3) | (1 << 4) | (1 << 5) | (1 << 6) | (1 << 7) | (1 << 8) | (1 << 9) | (1 << 10));
    com2 &= ~((1 << 3) | (1 << 4) | (1 << 5) | (1 << 6) | (1 << 7) | (1 << 8) | (1 << 9) | (1 << 10));
    com3 &= ~((1 << 3) | (1 << 4) | (1 << 5) | (1 << 6) | (1 << 7) | (1 << 8) | (1 << 9) | (1 << 10));
    com4 &= ~((1 << 3) | (1 << 4) | (1 << 5)| (1 << 7) | (1 << 9));//| (1 << 4) 小数点也要关闭
    com2 |= 1 << 10;
    com2 |= 1 << 8;
    com2 |= 1 << 6;
    com2 |= 1 << 4;
    lcd_write_to_RAM();
}

void lcd_mid_clear(void)
{
    com1 &= ~((1 << 3) | (1 << 4) | (1 << 5) | (1 << 6) | (1 << 7) | (1 << 8) | (1 << 9) | (1 << 10));
    com2 &= ~((1 << 3) | (1 << 4) | (1 << 5) | (1 << 6) | (1 << 7) | (1 << 8) | (1 << 9) | (1 << 10));
    com3 &= ~((1 << 3) | (1 << 4) | (1 << 5) | (1 << 6) | (1 << 7) | (1 << 8) | (1 << 9) | (1 << 10));
    com4 &= ~((1 << 3) | (1 << 4) | (1 << 5)| (1 << 7) | (1 << 9));//| (1 << 4) 小数点也要关闭
    lcd_write_to_RAM();
}

void lcd_show_mid_num(uint32_t Mid_vlaue)
{
    uint8_t i, j, k;
    if (Mid_vlaue > 9999)
    {
        Mid_vlaue = 9999;
    }

    com1 &= ~((1 << 3) | (1 << 4) | (1 << 5) | (1 << 6) | (1 << 7) | (1 << 8) | (1 << 9) | (1 << 10));
    com2 &= ~((1 << 3) | (1 << 4) | (1 << 5) | (1 << 6) | (1 << 7) | (1 << 8) | (1 << 9) | (1 << 10));
    com3 &= ~((1 << 3) | (1 << 4) | (1 << 5) | (1 << 6) | (1 << 7) | (1 << 8) | (1 << 9) | (1 << 10));
    com4 &= ~((1 << 3) | (1 << 5) | (1 << 7) | (1 << 9));

    // if ((app_status == DEV_SELF_TEST) || (app_status == DEV_SELF_TEST_WAIT) || (app_status == DEV_SELF_TEST_SETZERO) || (app_status == DEV_SELF_TEST_SETV5))
    // {
    //     com2 |= (1 << 4) | (1 << 6) | (1 << 8) | (1 << 10);
    //     return;
    // }//显示横线

    i = Mid_vlaue / 1000;

    switch (i)
    {
    case 0:
        // if (f_all_zero)
        // {
        //     com1 |= ((1 << 9) | (1 << 10));
        //     com2 |= ((1 << 9));
        //     com3 |= ((1 << 9) | (1 << 10));
        //     com4 |= ((1 << 9));
        // }
        break;
    case 1:
        com2 |= ((1 << 9));
        com3 |= ((1 << 9));
        break;
    case 2:
        com1 |= ((1 << 9));
        com2 |= ((1 << 9) | (1 << 10));
        com3 |= ((1 << 10));
        com4 |= ((1 << 9));
        break;
    case 3:
        com1 |= ((1 << 9));
        com2 |= ((1 << 9) | (1 << 10));
        com3 |= ((1 << 9));
        com4 |= ((1 << 9));
        break;
    case 4:
        com1 |= ((1 << 10));
        com2 |= ((1 << 9) | (1 << 10));
        com3 |= ((1 << 9));
        break;
    case 5:
        com1 |= ((1 << 9) | (1 << 10));
        com2 |= ((1 << 10));
        com3 |= ((1 << 9));
        com4 |= ((1 << 9));
        break;
    case 6:
        com1 |= ((1 << 9) | (1 << 10));
        com2 |= ((1 << 10));
        com3 |= ((1 << 9) | (1 << 10));
        com4 |= ((1 << 9));
        break;
    case 7:
        com1 |= ((1 << 9));
        com2 |= ((1 << 9));
        com3 |= ((1 << 9));
        break;
    case 8:
        com1 |= ((1 << 9) | (1 << 10));
        com2 |= ((1 << 9) | (1 << 10));
        com3 |= ((1 << 9) | (1 << 10));
        com4 |= ((1 << 9));
        break;
    case 9:
        com1 |= ((1 << 9) | (1 << 10));
        com2 |= ((1 << 9) | (1 << 10));
        com3 |= ((1 << 9));
        com4 |= ((1 << 9));
        break;
    }
    Mid_vlaue %= 1000;

    j = Mid_vlaue / 100;
    switch (j)
    {
    case 0:
        if ((i != 0))
        {
            com1 |= ((1 << 7) | (1 << 8));
            com2 |= ((1 << 7));
            com3 |= ((1 << 7) | (1 << 8));
            com4 |= ((1 << 7));
        }
        break;
    case 1:
        com2 |= ((1 << 7));
        com3 |= ((1 << 7));
        break;
    case 2:
        com1 |= ((1 << 7));
        com2 |= ((1 << 7) | (1 << 8));
        com3 |= ((1 << 8));
        com4 |= ((1 << 7));
        break;
    case 3:
        com1 |= ((1 << 7));
        com2 |= ((1 << 7) | (1 << 8));
        com3 |= ((1 << 7));
        com4 |= ((1 << 7));
        break;
    case 4:
        com1 |= ((1 << 8));
        com2 |= ((1 << 7) | (1 << 8));
        com3 |= ((1 << 7));
        break;
    case 5:
        com1 |= ((1 << 7) | (1 << 8));
        com2 |= ((1 << 8));
        com3 |= ((1 << 7));
        com4 |= ((1 << 7));
        break;
    case 6:
        com1 |= ((1 << 7) | (1 << 8));
        com2 |= ((1 << 8));
        com3 |= ((1 << 7) | (1 << 8));
        com4 |= ((1 << 7));
        break;
    case 7:
        com1 |= ((1 << 7));
        com2 |= ((1 << 7));
        com3 |= ((1 << 7));
        break;
    case 8:
        com1 |= ((1 << 7) | (1 << 8));
        com2 |= ((1 << 7) | (1 << 8));
        com3 |= ((1 << 7) | (1 << 8));
        com4 |= ((1 << 7));
        break;
    case 9:
        com1 |= ((1 << 7) | (1 << 8));
        com2 |= ((1 << 7) | (1 << 8));
        com3 |= ((1 << 7));
        com4 |= ((1 << 7));
        break;
    }

    Mid_vlaue %= 100;
    // 增加烟感显示0.
    // if (com1 & (1 << 9))
    // {
    //     com2 |= (1 << 10);
    //     com3 |= (1 << 10) | (1 << 11);
    //     com4 |= (1 << 10);
    //     com5 |= (1 << 10) | (1 << 11);
    // }
    // else

    k = Mid_vlaue / 10;
    switch (k)
    {
    case 0:
        if ((i != 0 || j != 0) || (com4 & (1 << 4)))
        {
            com1 |= ((1 << 5) | (1 << 6));
            com2 |= ((1 << 5));
            com3 |= ((1 << 5) | (1 << 6));
            com4 |= ((1 << 5));
        }
        break;
    case 1:
        com2 |= ((1 << 5));
        com3 |= ((1 << 5));
        break;
    case 2:
        com1 |= ((1 << 5));
        com2 |= ((1 << 5) | (1 << 6));
        com3 |= ((1 << 6));
        com4 |= ((1 << 5));
        break;
    case 3:
        com1 |= ((1 << 5));
        com2 |= ((1 << 5) | (1 << 6));
        com3 |= ((1 << 5));
        com4 |= ((1 << 5));
        break;
    case 4:
        com1 |= ((1 << 6));
        com2 |= ((1 << 5) | (1 << 6));
        com3 |= ((1 << 5));
        break;
    case 5:
        com1 |= ((1 << 5) | (1 << 6));
        com2 |= ((1 << 6));
        com3 |= ((1 << 5));
        com4 |= ((1 << 5));
        break;
    case 6:
        com1 |= ((1 << 5) | (1 << 6));
        com2 |= ((1 << 6));
        com3 |= ((1 << 5) | (1 << 6));
        com4 |= ((1 << 5));
        break;
    case 7:
        com1 |= ((1 << 5));
        com2 |= ((1 << 5));
        com3 |= ((1 << 5));
        break;
    case 8:
        com1 |= ((1 << 5) | (1 << 6));
        com2 |= ((1 << 5) | (1 << 6));
        com3 |= ((1 << 5) | (1 << 6));
        com4 |= ((1 << 5));
        break;
    case 9:
        com1 |= ((1 << 5) | (1 << 6));
        com2 |= ((1 << 5) | (1 << 6));
        com3 |= ((1 << 5));
        com4 |= ((1 << 5));
        break;
    }

    Mid_vlaue %= 10;
    switch (Mid_vlaue)
    {
    case 0:
        com1 |= (1 << 3) | (1 << 4);
        com2 |= (1 << 3);
        com3 |= (1 << 3) | (1 << 4);
        com4 |= (1 << 3);
        break;
    case 1:
        com2 |= (1 << 3);
        com3 |= (1 << 3);
        break;
    case 2:
        com1 |= ((1 << 3));
        com2 |= ((1 << 3) | (1 << 4));
        com3 |= ((1 << 4));
        com4 |= ((1 << 3));
        break;
    case 3:
        com1 |= (1 << 3);
        com2 |= (1 << 3) | (1 << 4);
        com3 |= (1 << 3);
        com4 |= (1 << 3);
        break;
    case 4:
        com1 |= (1 << 4);
        com2 |= (1 << 3) | (1 << 4);
        com3 |= (1 << 3);
        break;
    case 5:
        com1 |= (1 << 3) | (1 << 4);
        com2 |= (1 << 4);
        com3 |= (1 << 3);
        com4 |= (1 << 3);
        break;
    case 6:
        com1 |= (1 << 3) | (1 << 4);
        com2 |= (1 << 4);
        com3 |= (1 << 3) | (1 << 4);
        com4 |= (1 << 3);
        break;
    case 7:
        com1 |= (1 << 3);
        com2 |= (1 << 3);
        com3 |= (1 << 3);
        break;
    case 8:
        com1 |= (1 << 3) | (1 << 4);
        com2 |= (1 << 3) | (1 << 4);
        com3 |= (1 << 3) | (1 << 4);
        com4 |= (1 << 3);
        break;
    case 9:
        com1 |= (1 << 3) | (1 << 4);
        com2 |= (1 << 3) | (1 << 4);
        com3 |= (1 << 3);
        com4 |= (1 << 3);
        break;
    }
    f_all_zero = 0;
    lcd_write_to_RAM();
}

void lcd_show_data(uint32_t Mid_vlaue, uint16_t Temp_value, uint16_t Hum_vlaue)
{
    lcd_show_mid_num(Mid_vlaue);
    lcd_show_temperature(Temp_value);
    lcd_show_humidity(Hum_vlaue);
}