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

/*------------------------------------------functions-------------------------------------------*/

/**
* @brief  系统时钟配置
* @note   RCH时钟2分频
* @retval 无
*/
void system_clock_config(void)
{
    /* 设置RCH时钟 2分频 */
    std_rcc_set_rchdiv(RCC_RCH_DIV2);
  
    /* 更新系统时钟全局变量 */
    SystemCoreClockUpdate();
}

/**
* @brief  LCD的GPIO配置
* @retval 无
*/
void gpio_init(void)
{
    std_gpio_init_t  lcd_gpio_config = {0};

    /* GPIO时钟使能 */
    std_rcc_gpio_clk_enable(RCC_PERIPH_CLK_GPIOA | RCC_PERIPH_CLK_GPIOB | RCC_PERIPH_CLK_GPIOC);
    /* LCD端口配置：
       PA10 COM0
       PA11 COM1
       PA12 COM2
       PC9  COM3
       PA15 SEG0
       PC10 SEG1
       PC11 SEG2
       PB3  SEG3
       PB4  SEG4
       PB5  SEG5
       PB6  SEG6
       PB7  SEG7
    */

    /* 端口 A */    
    lcd_gpio_config.pin = GPIO_PIN_10 | GPIO_PIN_11 | GPIO_PIN_12 | GPIO_PIN_15;
    lcd_gpio_config.mode = GPIO_MODE_ALTERNATE;
    lcd_gpio_config.alternate = GPIO_AF3_LCD;
    std_gpio_init(GPIOA, &lcd_gpio_config);
    
    /* 端口 B */
    lcd_gpio_config.pin = GPIO_PIN_3 | GPIO_PIN_4 | GPIO_PIN_5 | GPIO_PIN_6 | GPIO_PIN_7; 
    std_gpio_init(GPIOB, &lcd_gpio_config);
    
    /* 端口 C */      
    lcd_gpio_config.pin = GPIO_PIN_9 | GPIO_PIN_10 | GPIO_PIN_11;
    std_gpio_init(GPIOC, &lcd_gpio_config);
}

/**
* @brief  LCD时钟源配置
* @retval 无
*/
void lcd_clock_config(void)
{    
    std_rcc_apb1_clk_enable(RCC_PERIPH_CLK_PMU);
    
    /* 要更新LXTAL配置，必须解除备份域的写保护 */
    std_pmu_vaon_write_enable();
         
    /* 配置LXTAL驱动能力 */
    std_rcc_lxtal_drive_mode_config(RCC_LXTAL_DRIVE_MODE_ENHANCE);
    std_rcc_lxtal_drive_config(RCC_LXTAL_DRIVE_LEVEL2);

    /* 使能LXTAL */
    std_rcc_lxtal_enable(RCC_LXTAL_ON);
    
    /* 等待LXTAL时钟稳定 */
    while(!std_rcc_get_lxtal_ready());

    /* 配置LCD 时钟源为LXTAL */
    std_rcc_set_rtcclk_source(RCC_RTC_ASYNC_CLK_SRC_LXTAL); 
    /* 恢复备份域的写保护 */
    std_pmu_vaon_write_disable();                        
}

/**
* @brief  LCD初始化配置，时钟源为RCL
* @retval 无
*/
void lcd_init(void)
{ 
    std_lcd_init_t lcd_config = {0};
 
    std_rcc_apb1_clk_enable(RCC_PERIPH_CLK_LCD); 
    /* LCD防极化 */
    std_lcd_set_anti_polar(LCD_ANTIPOLAR_GND);
    /* LCD输出禁止，COM和SEG端子在LCD使能后连接到GND */
    std_lcd_set_scoc(LCD_SCOC_GND);

    /* VLCD、LCD_V1、LCD_V2引脚外接电容配置 */
    std_lcd_v1_cap_enable();
    std_lcd_v2_cap_enable();

    /* 片外电容分压时钟分频，可根据应用需求调整分频值 */
    std_lcd_set_chargepump_clock(LCD_CPCLK_DIV16);

    /* LCD参数配置 */
    lcd_config.mux_segment = LCD_MUX_SEGMENT_ENABLE;        /* 1/6 和 1/4duty需使能区段多路复用，1/8duty为无关项 */
    lcd_config.bias = LCD_BIAS_1_3;
    lcd_config.duty = LCD_DUTY_1_4;
    lcd_config.driver_mode = LCD_DRIVER_MODE_OUTER_CAPACITY;
    lcd_config.prescaler = LCD_PRESCALER_8;                 /* 帧速率配置为32Hz */
    lcd_config.divider = LCD_DIVIDER_31;
 
    /* LCD 初始化 */
    std_lcd_init(&lcd_config);

    /* LCD COM和SEG波形输出 */
    std_lcd_set_scoc(LCD_SCOC_OUT);
}

/**
* @brief  EXTI初始化
* @retval 无
*/
void exti_init(void)
{
    std_exti_init_t exti_init_config = {0};
    std_gpio_init_t button_init_config = {0};    

    /* 使能BUTTON_USER对应的GPIO时钟 */
    std_rcc_gpio_clk_enable(RCC_PERIPH_CLK_GPIOC);

    /* 配置BUTTON_USER的GPIO */
    button_init_config.pin = BUTTON_USER_PIN;
    button_init_config.mode = GPIO_MODE_INPUT;
    button_init_config.pull = GPIO_NOPULL;
    std_gpio_init(BUTTON_USER_PORT, &button_init_config);
    
    /* 配置BUTTON_USER的EXTI */
    exti_init_config.line_id = BUTTON_USER_EXTI_LINE;
    exti_init_config.mode = EXTI_MODE_INTERRUPT;
    exti_init_config.trigger = EXTI_TRIGGER_FALLING;
    exti_init_config.gpio_id = BUTTON_USER_EXTI_PORT;
    std_exti_init(&exti_init_config);

    /* 配置中断优先级 */
    NVIC_SetPriority(EXTI4_15_IRQn, NVIC_PRIO_3); 
    /* 使能中断 */
    NVIC_EnableIRQ(EXTI4_15_IRQn);
}

/**
* @brief  LED1初始化
* @retval 无
*/
void led_init(void)
{
    std_gpio_init_t led_gpio_config = {0};

    /* 使能LED1对应的GPIO时钟 */
    std_rcc_gpio_clk_enable(RCC_PERIPH_CLK_GPIOB);

    /* 配置LED1的IO */
    led_gpio_config.pin = LED_RED_PIN;
    led_gpio_config.mode = GPIO_MODE_OUTPUT;
    led_gpio_config.output_type = GPIO_OUTPUT_PUSHPULL;
    led_gpio_config.pull = GPIO_PULLUP;

    /* 初始化GPIO */
    std_gpio_init(LED_RED_GPIO_PORT, &led_gpio_config);

    /* 熄灭LED1 */
    LED1_OFF();
}

