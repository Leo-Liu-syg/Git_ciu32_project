#ifndef __GPIO_H__
#define __GPIO_H__

#include "main.h"

/************************** LED 指示灯 GPIO 宏定义 **************************/
#define GPIO_LED_W GPIOC
#define PIN_LED_W  GPIO_PIN_13
#define GPIO_LED_W_HIGH()   std_gpio_set_pin(GPIO_LED_W, PIN_LED_W)
#define GPIO_LED_W_LOW()    std_gpio_reset_pin(GPIO_LED_W, PIN_LED_W)
#define GPIO_LED_W_flip()   std_gpio_toggle_pin(GPIO_LED_W, PIN_LED_W)

#define GPIO_LED_G GPIOC
#define PIN_LED_G  GPIO_PIN_14
#define GPIO_LED_G_HIGH()   std_gpio_set_pin(GPIO_LED_G, PIN_LED_G)
#define GPIO_LED_G_LOW()    std_gpio_reset_pin(GPIO_LED_G, PIN_LED_G)
#define GPIO_LED_G_flip()   std_gpio_toggle_pin(GPIO_LED_G, PIN_LED_G)

#define GPIO_LED_R GPIOC
#define PIN_LED_R  GPIO_PIN_15
#define GPIO_LED_R_HIGH()   std_gpio_set_pin(GPIO_LED_R, PIN_LED_R)
#define GPIO_LED_R_LOW()    std_gpio_reset_pin(GPIO_LED_R, PIN_LED_R)
#define GPIO_LED_R_flip()   std_gpio_toggle_pin(GPIO_LED_R, PIN_LED_R)

// 黄色LED 已屏蔽，在gpio.c中注释
/*
#define GPIO_LED_Y GPIOA
#define PIN_LED_Y GPIO_PIN_13
#define GPIO_LED_Y_HIGH() std_gpio_set_pin(GPIO_LED_Y, PIN_LED_Y)
#define GPIO_LED_Y_LOW() std_gpio_reset_pin(GPIO_LED_Y, PIN_LED_Y)
#define GPIO_LED_Y_flip() std_gpio_toggle_pin(GPIO_LED_Y, PIN_LED_Y)
*/

/************************** V358 / IR 模块 GPIO 宏定义 **************************/
#define GPIO_V358 GPIOA
#define PIN_V358  GPIO_PIN_15
#define GPIO_V358_HIGH()    std_gpio_set_pin(GPIO_V358, PIN_V358)
#define GPIO_V358_LOW()     std_gpio_reset_pin(GPIO_V358, PIN_V358)

#define GPIO_IR GPIOB
#define PIN_IR  GPIO_PIN_2
#define GPIO_IR_HIGH()       std_gpio_set_pin(GPIO_IR, PIN_IR)
#define GPIO_IR_LOW()        std_gpio_reset_pin(GPIO_IR, PIN_IR)

/************************** VOICE 语音模块 GPIO 宏定义 **************************/
#define GPIO_VOICE GPIOA
#define PIN_VOICE  GPIO_PIN_1
#define GPIO_VOICE_HIGH()    std_gpio_reset_pin(GPIO_VOICE, PIN_VOICE)
#define GPIO_VOICE_LOW()     std_gpio_set_pin(GPIO_VOICE, PIN_VOICE)
#define GPIO_VOICE_flip()    std_gpio_toggle_pin(GPIO_VOICE, PIN_VOICE)

/************************** 按键 GPIO 宏定义 **************************/
#define GPIO_KEY_LED GPIOA
#define PIN_KEY_LED  GPIO_PIN_5
#define GPIO_GET_KEY_LED()   std_gpio_get_input_pin(GPIO_KEY_LED, PIN_KEY_LED)

#define GPIO_KEY_TEST GPIOA
#define PIN_KEY_TEST  GPIO_PIN_10
#define GPIO_GET_KEY_TEST()  std_gpio_get_input_pin(GPIO_KEY_TEST, PIN_KEY_TEST)

/************************** IIC 总线 GPIO 宏定义 **************************/
#define GPIO_IIC GPIOF
#define PIN_IIC_SCL  GPIO_PIN_1
#define PIN_IIC_SDA  GPIO_PIN_0
#define GPIO_IIC_SCL_HIGH()  std_gpio_set_pin(GPIO_IIC, PIN_IIC_SCL)
#define GPIO_IIC_SCL_LOW()   std_gpio_reset_pin(GPIO_IIC, PIN_IIC_SCL)
#define GPIO_IIC_SDA_HIGH()  std_gpio_set_pin(GPIO_IIC, PIN_IIC_SDA)
#define GPIO_IIC_SDA_LOW()   std_gpio_reset_pin(GPIO_IIC, PIN_IIC_SDA)
#define GPIO_GET_IIC_SDA()   std_gpio_get_input_pin(GPIO_IIC, PIN_IIC_SDA)

/************************** ADC 采集通道 GPIO 宏定义 **************************/
// 电池电压检测
#define GPIO_VBAT GPIOA
#define PIN_VBAT  GPIO_PIN_3
#define ADC_CHANNEL_VBAT    ADC_CHANNEL_2

// CO传感器电压检测
#define GPIO_VCO GPIOA
#define PIN_VCO  GPIO_PIN_4
#define ADC_CHANNEL_VCO     ADC_CHANNEL_3

// 烟雾传感器电压检测
#define GPIO_VSMOKE GPIOA
#define PIN_VSMOKE  GPIO_PIN_2
#define ADC_CHANNEL_VSMOKE  ADC_CHANNEL_1

/************************** 对外接口函数 **************************/
void gpio_init(void);

#endif