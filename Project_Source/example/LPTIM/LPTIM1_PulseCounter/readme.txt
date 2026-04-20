本示例使用到的资源：
LPTIM1_IN1输入   ----- PB5
LED1             ----- PB14


本示例的功能流程：
1、上电配置系统时钟（RCH的2分频作为时钟源）8M，Systick时钟初始化；
2、LED1的GPIO初始化，配置LED1初始状态熄灭，初始化LPTIM1_IN引脚；
3、初始化LPTIM1：LPTIM1工作时钟设置为RCL，计数时钟为外部时钟，软件触发计数；
4、NVIC初始化：使能LPTIM1的NVIC中断并设置中断优先级；
5、点亮LED1；
6、使能LPTIM1的自动重载匹配中断，启动计数；
7、设置MCU进入Stop模式；
8、当定时器对LPTIM1_IN1的脉冲上升沿计数到1000，则产生自动重载匹配中断，唤醒MCU；
9、MCU唤醒后LED1以200ms的周期闪烁。

注意：
1、MCU唤醒后系统时钟自动切换为RCHSYS，RCH的分频由RCHDIV决定，本示例为2分频。
