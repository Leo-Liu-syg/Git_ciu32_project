#ifndef __GPIO_H__
#define __GPIO_H__

#include "main.h"

/********** 分类注释：LCD/指示灯 LED 相关 GPIO 宏定义 **********/
#define GPIO_LED_W GPIOC
#define PIN_LED_W GPIO_PIN_13
#define GPIO_LED_W_HIGH() std_gpio_set_pin(GPIO_LED_W, PIN_LED_W)
#define GPIO_LED_W_LOW() std_gpio_reset_pin(GPIO_LED_W, PIN_LED_W)
#define GPIO_LED_W_flip() std_gpio_toggle_pin(GPIO_LED_W, PIN_LED_W)

#define GPIO_LED_G GPIOC
#define PIN_LED_G GPIO_PIN_14
#define GPIO_LED_G_HIGH() std_gpio_set_pin(GPIO_LED_G, PIN_LED_G)
#define GPIO_LED_G_LOW() std_gpio_reset_pin(GPIO_LED_G, PIN_LED_G)
#define GPIO_LED_G_flip() std_gpio_toggle_pin(GPIO_LED_G, PIN_LED_G)

#define GPIO_LED_R GPIOC
#define PIN_LED_R GPIO_PIN_15
#define GPIO_LED_R_HIGH() std_gpio_set_pin(GPIO_LED_R, PIN_LED_R)
#define GPIO_LED_R_LOW() std_gpio_reset_pin(GPIO_LED_R, PIN_LED_R)
#define GPIO_LED_R_flip() std_gpio_toggle_pin(GPIO_LED_R, PIN_LED_R)

/********** 分类注释：V358、IR 外设控制 GPIO 宏定义 **********/
#define GPIO_V358 GPIOA
#define PIN_V358 GPIO_PIN_15
#define GPIO_V358_HIGH() std_gpio_set_pin(GPIO_V358, PIN_V358)
#define GPIO_V358_LOW() std_gpio_reset_pin(GPIO_V358, PIN_V358)

#define GPIO_IR GPIOB
#define PIN_IR GPIO_PIN_2
#define GPIO_IR_HIGH() std_gpio_set_pin(GPIO_IR, PIN_IR)
#define GPIO_IR_LOW() std_gpio_reset_pin(GPIO_IR, PIN_IR)

/********** 分类注释：VOICE 语音模块控制 GPIO 宏定义 **********/
#define GPIO_VOICE GPIOA
#define PIN_VOICE GPIO_PIN_1
#define GPIO_VOICE_HIGH() std_gpio_reset_pin(GPIO_VOICE, PIN_VOICE)
#define GPIO_VOICE_LOW() std_gpio_set_pin(GPIO_VOICE, PIN_VOICE)
#define GPIO_VOICE_flip() std_gpio_toggle_pin(GPIO_VOICE, PIN_VOICE)

/********** 分类注释：按键输入相关 GPIO 宏定义 **********/
#define GPIO_KEY_LED GPIOA
#define PIN_KEY_LED GPIO_PIN_5
#define GPIO_GET_KEY_LED() std_gpio_get_input_pin(GPIO_KEY_LED, PIN_KEY_LED)

#define GPIO_KEY_TEST GPIOA
#define PIN_KEY_TEST GPIO_PIN_10
#define GPIO_GET_KEY_TEST() std_gpio_get_input_pin(GPIO_KEY_TEST, PIN_KEY_TEST)

/********** 分类注释：IIC 总线控制 GPIO 宏定义 **********/
#define GPIO_IIC GPIOF
#define PIN_IIC_SCL GPIO_PIN_1
#define PIN_IIC_SDA GPIO_PIN_0
#define GPIO_IIC_SCL_HIGH() std_gpio_set_pin(GPIO_IIC, PIN_IIC_SCL)
#define GPIO_IIC_SCL_LOW() std_gpio_reset_pin(GPIO_IIC, PIN_IIC_SCL)
#define GPIO_IIC_SDA_HIGH() std_gpio_set_pin(GPIO_IIC, PIN_IIC_SDA)
#define GPIO_IIC_SDA_LOW() std_gpio_reset_pin(GPIO_IIC, PIN_IIC_SDA)


/********** 函数声明：仅保留目标函数 **********/
void gpio_init(void);

#endif