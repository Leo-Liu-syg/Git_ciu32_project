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

/*------------------------------------------define----------------------------------------------*/    
/* BGR使能后，等待其稳定时间，单位:us */
#define BGR_RDY_DELAY            (30UL)

/* VREFBUF使能后，等待其稳定时间，用户可根据对VREFBUF精度要求，调整该时间参数，单位:us */
#define VREFBUF_RDY_DELAY        (1000UL)

/*------------------------------------------functions-------------------------------------------*/
/**
* @brief  系统时钟配置
* @retval 无
*/
void system_clock_config(void)
{
    /* 设置Flash读访问等待时间 */
    std_flash_set_latency(FLASH_LATENCY_2CLK);

    /* 使能RCH */
    std_rcc_rch_enable();
    while(!std_rcc_get_rch_ready());
    
    /* 配置PLL相关参数，并使能 */
    std_rcc_pll_config(RCC_PLLSRC_RCH, RCC_PLLM_MUL12, RCC_PLLN_DIV2, RCC_PLL_DIV2);
    std_rcc_pll_enable();
    while(!std_rcc_get_pll_ready());
    
    std_rcc_pll_output_enable();    

    /* 设置系统时钟源为PLL */
    std_rcc_set_sysclk_source(RCC_SYSCLK_SRC_PLLCLK);
    while(std_rcc_get_sysclk_source() != RCC_SYSCLK_SRC_STATUS_PLLCLK);

    /* 设置AHB分频因子 */
    std_rcc_set_ahbdiv(RCC_HCLK_DIV1);
    
    /* 设置APB1、APB2分频因子 */
    std_rcc_set_apb1div(RCC_PCLK1_DIV1);
    std_rcc_set_apb2div(RCC_PCLK2_DIV1);
    
    SystemCoreClockUpdate();
}


/**
* @brief  COMP初始化
* @note   初始化COMP1和COMP2作为窗口比较器
* @retval 无
*/
void comp_init(void)
{
    std_comp_init_t comp_config = {0};

    /* 使能COMP时钟 */
    std_rcc_apb2_clk_enable(RCC_PERIPH_CLK_SYSCFG);

    /* COMP1初始化 */
    comp_config.inpsel = COMP_INPSEL_IO3;          /* COMP1正相输入为PB2引脚 */
    comp_config.inmsel = COMP_INMSEL_VBGR;         /* 反相输入信号VBGR */
    comp_config.speed = COMP_LOWSPEED;             /* 低响应速度 */
    comp_config.hyst = COMP_HYST_LOW;              /* 低迟滞 */
    comp_config.outpol = COMP_OUTPOL_NON_INVERTED; /* 输出不反相 */
    std_comp_init(COMP1, &comp_config);

    std_comp_set_input_plus_mode(COMP1, COMP_INPMODE_EACH_INPUT); /* COMP1输入独立，非窗口比较器模式 */
    std_comp_set_output_mode(COMP1, COMP_OUTMODE_COMMON_XOR_OUT); /* 比较器输出为比较器1、2电压比较结果异或 */

    /* COMP2初始化 */
    comp_config.inmsel = COMP_INMSEL_6BIT_DAC;     /* 反相输入信号6BIT DAC输出1分压，2.048V */
    comp_config.speed = COMP_LOWSPEED;             /* 低响应速度 */
    comp_config.hyst = COMP_HYST_LOW;              /* 低迟滞 */
    comp_config.outpol = COMP_OUTPOL_NON_INVERTED; /* 输出不反相 */
    std_comp_init(COMP2, &comp_config);

    std_comp_set_input_plus_mode(COMP2, COMP_INPMODE_COMMON_INPUT); /* COMP2窗口比较器模式：COMP2的正相输入来自COMP1的正相输入 */
    std_comp_set_output_mode(COMP2, COMP_OUTMODE_EACH_OUT);         /* COMP2输出独立，为其原始信号输出 */

    /* 使能COMP1 */
    std_comp_enable(COMP1);

    /* 使能COMP2 */
    std_comp_enable(COMP2);

    /* 等待COMP1使能启动时间 
       响应速度模式为高  速  使能启动时间23US
       响应速度模式为中  速  使能启动时间24US
       响应速度模式为低  速  使能启动时间30US
       响应速度模式为超低速  使能启动时间39US
    */
    std_delayus(COMP_EN_DELAY);
}


/**
* @brief  VBGR初始化
* @retval 无
*/
void vbgr_init(void)
{
    /* 使能PMU时钟 */
    std_rcc_apb1_clk_enable(RCC_PERIPH_CLK_PMU);
   
    /* 使能VBGR */    
    std_pmu_bgr_enable();
    
    /* 等待BGR稳定 */
    std_delayus(BGR_RDY_DELAY);
}


/**
* @brief  6BIT DAC初始化
* @retval 无
*/
void syscfg_6bit_dac_init(void)
{
    /* VREFBUF初始化
       校准VREFBUF的2.048V电压档位
       配置VREFBUF的2.048V电压档位输出
    */
    std_rcc_apb2_clk_enable(RCC_PERIPH_CLK_SYSCFG);           
    std_vrefbuf_disable();                                    
    std_rcc_write_vrefbuf_calibration(VREFBUF_CAL_VALUE_2048);
    std_vrefbuf_output_config(VREFBUF_VOLTAGE_2048);          
    std_vrefbuf_enable();                                     

    /* 等待VREFBUF稳定，用户可根据对VREFBUF精度要求，调整该时间参数，单位:us */
    std_delayus(VREFBUF_RDY_DELAY);   

    /* 6BIT DAC初始化
       配置6BIT DAC输入电压源为VREFBUF的2.048V输出
       配置6BIT DAC直接输出2.048V不分压
    */
    std_syscfg_6bit_dac_disable();
    std_syscfg_set_6bit_dac_reference_voltage(SYSCFG_6BIT_DAC_VREFBUF);
    std_syscfg_set_6bit_dac_divide_voltage(SYSCFG_6BIT_DAC_VOLTAGE_DIV_NONE);
    std_syscfg_6bit_dac_enable();

    /* 等待6BIT DAC输出稳定 */
    std_delayus(SYSCFG_6BIT_DAC_EN_DELAY);
}


/**
* @brief  GPIO初始化
* @retval 无
*/
void gpio_init(void)
{
    std_gpio_init_t comp_gpio_config = {0};
    std_gpio_init_t led_gpio_config = {0};
    std_gpio_init_t vrefp_gpio_config = {0}; 
 
    std_rcc_gpio_clk_enable(RCC_PERIPH_CLK_GPIOA | RCC_PERIPH_CLK_GPIOB);
    
    /* 配置COMP1正相输入信号GPIO模式 */
    comp_gpio_config.pin = GPIO_PIN_2;
    comp_gpio_config.mode = GPIO_MODE_ANALOG;
    std_gpio_init(GPIOB, &comp_gpio_config);
    
    /* 配置COMP输出IO PA5 */
    comp_gpio_config.pin = GPIO_PIN_5;
    comp_gpio_config.mode = GPIO_MODE_ALTERNATE;
    comp_gpio_config.alternate = GPIO_AF7_COMP1;
    std_gpio_init(GPIOA, &comp_gpio_config);
   
    /* 配置VREF+引脚为模拟模式 */
    vrefp_gpio_config.pin = GPIO_PIN_0;
    vrefp_gpio_config.mode = GPIO_MODE_ANALOG;
    std_gpio_init(GPIOA, &vrefp_gpio_config);

    /* LED1初始化 */
    led_gpio_config.pin = LED_RED_PIN;
    led_gpio_config.mode = GPIO_MODE_OUTPUT;
    led_gpio_config.output_type = GPIO_OUTPUT_PUSHPULL;
    led_gpio_config.pull = GPIO_PULLUP;
    std_gpio_init(LED_RED_GPIO_PORT, &led_gpio_config);
    
    /* 熄灭LED1 */
    LED1_OFF();
}
