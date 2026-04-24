#ifndef __LCD_H_
#define __LCD_H_

#include "main.h"

extern uint32_t com1;
extern uint32_t com2;
extern uint32_t com3;
extern uint32_t com4;
extern uint32_t com5;

extern uint32_t mid_num;


void lcd_clock_config(void);
void lcd_config_init(void);
void lcd_write_to_RAM(void);
void lcd_clean_all(void);
void lcd_show_all(void);
void lcd_show_temperature(uint16_t Temp_value);
void lcd_show_humidity(uint16_t Hum_vlaue);
void lcd_show_mid_num(uint32_t Mid_vlaue);
void lcd_show_data(uint32_t Mid_vlaue, uint16_t Temp_value, uint16_t Hum_vlaue);
void lcd_show_dash(void);
void lcd_mid_clear(void);

#endif