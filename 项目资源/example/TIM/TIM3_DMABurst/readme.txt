本示例使用到的资源：
TIM3 CH1     --   PB4
TIM3 CH2     --   PB5
TIM3 CH3     --   PC7
TIM3 CH4     --   PC8

本示例的功能流程：
1、上电配置时钟（PLL 48MHz）；
2、TIM3的通道1~4分别配置为PWM输出；
3、配置TIM3的更新事件触发DMA传输；
5、循环执行如下操作，以实现改变占空比功能：
   a、当计数发生上溢，触发更新事件时，启动DMA Burst传输；
   b、DMA传输时将g_src_buffer中的数据分别传输到CC1~CC4寄存器中。