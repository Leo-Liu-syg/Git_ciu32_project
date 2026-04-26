#include "lcd.h"

// 中间显示数值
uint32_t mid_num;

// 全零标志
uint8_t f_all_zero = 0;

// LCD 显示寄存器
uint32_t com1 = 0XFFFF;
uint32_t com2 = 0XFFFF;
uint32_t com3 = 0XFFFF;
uint32_t com4 = 0XFFFF;
uint32_t com5 = 0XFFFF;

/**
 * @brief  LCD时钟配置
 * @retval 无
 */
void lcd_clock_config()
{
    std_rcc_apb1_clk_enable(RCC_PERIPH_CLK_PMU);
    std_rcc_rcl_enable();
    std_delayus(100);

    /* 开启VAON寄存器写使能 */
    std_pmu_vaon_write_enable();

    /* 设置LCD时钟源为内部RCL时钟 */
    std_rcc_set_rtcclk_source(RCC_RTC_ASYNC_CLK_SRC_RCL);

    /* 关闭VAON寄存器写使能 */
    std_pmu_vaon_write_disable();
}

/**
 * @brief  LCD初始化配置
 * @retval 无
 */
void lcd_config_init(void)
{
    std_lcd_init_t lcd_config = {0};
    lcd_clock_config();

    std_rcc_apb1_clk_enable(RCC_PERIPH_CLK_LCD);

    /* 设置LCD对比度极性 */
    std_lcd_set_anti_polar(LCD_ANTIPOLAR_GND);

    std_lcd_v1_cap_enable();
    std_lcd_v2_cap_enable();
    std_lcd_v3_cap_enable();
    std_lcd_vlcd_cap_enable();

    /* LCD工作模式配置 */
    lcd_config.bias = LCD_BIAS_1_3;
    lcd_config.duty = LCD_DUTY_1_6;
    lcd_config.prescaler = LCD_PRESCALER_2;
    lcd_config.divider = LCD_DIVIDER_31;
    lcd_config.driver_mode = LCD_DRIVER_MODE_OUTER_CAPACITY;
    lcd_config.vlcd_voltage = LCD_INNER_RESISTER_1_000_VDD;
    lcd_config.high_drive = LCD_HIGH_DRIVE;
    lcd_config.mux_segment = LCD_MUX_SEGMENT_ENABLE;

    /* 初始化LCD */
    std_lcd_init(&lcd_config);

    /* 配置LCD COM和SEG引脚 */
    std_lcd_set_scoc(LCD_SCOC_OUT);
}

/**
 * @brief  将com寄存器值写入LCD RAM
 * @retval 无
 */
void lcd_write_to_RAM(void)
{
    std_lcd_write(LCD_RAM_REGISTER1, com1, com1);
    std_lcd_write(LCD_RAM_REGISTER2, com2, com2);
    std_lcd_write(LCD_RAM_REGISTER3, com3, com3);
    std_lcd_write(LCD_RAM_REGISTER4, com4, com4);
    std_lcd_write(LCD_RAM_REGISTER5, com5, com5);
}

/**
 * @brief  清空所有LCD显示
 * @retval 无
 */
void lcd_clean_all(void)
{
    com1 = 0X0000;
    com2 = 0X0000;
    com3 = 0X0000;
    com4 = 0X0000;
    com5 = 0X0000;
}

/**
 * @brief  全显LCD所有段码
 * @retval 无
 */
void lcd_show_all()
{
    com1 = 0XFFFF;
    com2 = 0XFFFF;
    com3 = 0XFFFF;
    com4 = 0XFFFF;
    com5 = 0XFFFF;
    lcd_write_to_RAM();
}

/**
 * @brief  显示温度值
 * @param  Temp_value: 温度值
 * @retval 无
 */
void lcd_show_temperature(uint16_t Temp_value)
{
    uint8_t i, j, k;

    // 清除温度显示区域
    com1 &= ~((1 << 11) | (1 << 12) | (1 << 13) | (1 << 14));
    com2 &= ~((1 << 11) | (1 << 12) | (1 << 13) | (1 << 14));
    com3 &= ~((1 << 11) | (1 << 12) | (1 << 13) | (1 << 14));
    com4 &= ~((1 << 11) | (1 << 13) | (1 << 14));
    com5 &= ~((1 << 14));

    com4 |= (1 << 15); // 温度图标
    com5 |= (1 << 11); // 温度标志
    com4 |= (1 << 12);
    com5 &= ~(1 << 12);

    // 温度值限制
    if (Temp_value > 188)
        Temp_value = 188;

    // 百位
    i = Temp_value / 100;
    if (i)
        com4 |= (1 << 14);

    // 十位
    Temp_value %= 100;
    j = Temp_value / 10;
    switch (j)
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

    // 个位
    k = Temp_value % 10;
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

/**
 * @brief  显示湿度值
 * @param  Hum_vlaue: 湿度值
 * @retval 无
 */
void lcd_show_humidity(uint16_t Hum_vlaue)
{
    uint8_t j, k;

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
    com5 &= ~((1 << 1) | (1 << 2) | (1 << 5) | (1 << 6) | (1 << 7) | (1 << 8) | (1 << 9));

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

/**
 * @brief  中间区域显示横杠
 * @retval 无
 */
void lcd_show_dash(void)
{
    com1 &= ~((1 << 3) | (1 << 4) | (1 << 5) | (1 << 6) | (1 << 7) | (1 << 8) | (1 << 9) | (1 << 10));
    com2 &= ~((1 << 3) | (1 << 4) | (1 << 5) | (1 << 6) | (1 << 7) | (1 << 8) | (1 << 9) | (1 << 10));
    com3 &= ~((1 << 3) | (1 << 4) | (1 << 5) | (1 << 6) | (1 << 7) | (1 << 8) | (1 << 9) | (1 << 10));
    com4 &= ~((1 << 3) | (1 << 4) | (1 << 5) | (1 << 7) | (1 << 9));

    com2 |= 1 << 10;
    com2 |= 1 << 8;
    com2 |= 1 << 6;
    com2 |= 1 << 4;
    lcd_write_to_RAM();
}

/**
 * @brief  清空中间显示区域
 * @retval 无
 */
void lcd_mid_clear(void)
{
    com1 &= ~((1 << 3) | (1 << 4) | (1 << 5) | (1 << 6) | (1 << 7) | (1 << 8) | (1 << 9) | (1 << 10));
    com2 &= ~((1 << 3) | (1 << 4) | (1 << 5) | (1 << 6) | (1 << 7) | (1 << 8) | (1 << 9) | (1 << 10));
    com3 &= ~((1 << 3) | (1 << 4) | (1 << 5) | (1 << 6) | (1 << 7) | (1 << 8) | (1 << 9) | (1 << 10));
    com4 &= ~((1 << 3) | (1 << 4) | (1 << 5) | (1 << 7) | (1 << 9));
    lcd_write_to_RAM();
}

/**
 * @brief  显示中间4位数值
 * @param  Mid_vlaue: 待显示数值
 * @retval 无
 */
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

    // 千位
    i = Mid_vlaue / 1000;
    switch (i)
    {
    case 0:
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

    // 百位
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

    // 十位
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

    // 个位
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

/**
 * @brief  综合显示：中间值+温度+湿度
 * @param  Mid_vlaue: 中间数值
 * @param  Temp_value: 温度
 * @param  Hum_vlaue: 湿度
 * @retval 无
 */
void lcd_show_data(uint32_t Mid_vlaue, uint16_t Temp_value, uint16_t Hum_vlaue)
{
    lcd_show_mid_num(Mid_vlaue);
    lcd_show_temperature(Temp_value);
    lcd_show_humidity(Hum_vlaue);
}