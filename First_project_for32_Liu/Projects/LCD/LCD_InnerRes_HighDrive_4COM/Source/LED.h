#ifndef __LED_H__
#define __LED_H__

#include "main.h"

/* LEDÄ£¿éÏà¹ØºêÌæ»» */
#define LED_RED_PIN GPIO_PIN_15
#define LED_RED_GPIO_PORT GPIOC
#define LED_RED_FLIP() std_gpio_toggle_pin(LED_RED_GPIO_PORT, LED_RED_PIN)

void led_init(void);

#endif /* __LED_H__ */