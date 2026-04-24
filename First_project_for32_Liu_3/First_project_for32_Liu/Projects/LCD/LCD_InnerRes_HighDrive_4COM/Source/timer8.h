#ifndef __TIMER8_H
#define __TIMER8_H

#include "main.h"

/* TIM8 定时周期配置
 * 定时中断周期：10kHz 计数 100 次 = 10ms 中断一次
 */
#define TIM8_ARR_VALUE              (100 - 1) 

/* 全局中断标志位（供外部文件调用） */
extern unsigned char tim8_250ms_flag;  // 250ms 定时标志
extern unsigned char tim8_500ms_flag;  // 500ms 定时标志
extern unsigned char tim8_1s_flag;    // 1s 定时标志
extern unsigned int  tim8_wait;       // 软件延时计数器

/* 函数声明 */
void TIM8_init(void);                  // TIM8 初始化
void TIM8_IRQHandler(void);            // TIM8 中断服务函数
void bsp_tim8_start(void);             // 启动 TIM8 及其中断

#endif /* __TIMER8_H */