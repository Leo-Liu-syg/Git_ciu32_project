本示例使用到的资源：
LPTIM的Trigger外部触发  --   PB6
LED                     --   PB14
LXTAL_IN                --   PC14
LXTAL_OUT               --   PC15

本示例的功能流程：
1、上电配置系统时钟PLL、初始化LED1 GPIO后关闭LED；
2、配置LPTIM1：初始化LPTIM1（工作时钟为内部（LXTAL），计数时钟为内部（LXTAL），外部GPIO触发），配置EXTI开启LPTIM1事件唤醒MCU，
   相位设置为32767，周期设置为65535，启动超时模式；
3、点亮LED1，进入stop模式；
4、外部信号上升沿触发LPTIM1启动计数。当LPTIM1计数到相位值（32767）时都无有效的触发信号则产生超时事件，唤醒MCU。唤醒后LPTIM1计数停止；
5、唤醒后LED1熄灭。

注意：
1、进入stop模式系统时钟PLL会关闭，唤醒后系统时钟为RCH的2分频。

