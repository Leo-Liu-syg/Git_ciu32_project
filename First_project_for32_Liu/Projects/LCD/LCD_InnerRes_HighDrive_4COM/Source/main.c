#include "main.h"

int main(void)
{
    /*????*/
    system_clock_config();
    std_delay_init();

    /*LED*/
    led_init();

    TIM8_init();      // ?????
    bsp_tim8_start(); // ?????
    nvic_init();         // NVIC??
    while (1)
    {
        if (tim8_500ms_flag) // ??TIM8??????
        {
            tim8_500ms_flag = 0; // ????
            /* ?????TIM8?????????LED */
            LED_RED_FLIP();
        }
    }
}