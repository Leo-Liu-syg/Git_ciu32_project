#ifndef __I2C_H__
#define __I2C_H__

#include "main.h"

extern uint16_t temperature_F;// »ªÊÏ¶È
extern uint32_t temperature_Raw;
extern uint16_t temperature_low;
extern uint32_t humidity;
extern uint16_t temperature_S; // ÉãÊÏ¶È

void GXHTC3_sleep(void);
void GXHTC3_read(void);

#endif 

