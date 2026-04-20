#ifndef __TIMER8_H
#define __TIMER8_H

#include "main.h"


/* timer8模块相关宏替换 */
#define TIM8_ARR_VALUE              (100 - 1) 
extern unsigned char tim8_500ms_flag; // 定义一个可供外部调用的标志变量

void TIM8_init(void);
void TIM8_IRQHandler(void);
void bsp_tim8_start(void);

#endif /* __TIMER8_H */