本示例使用到的资源：
LED1引脚      --   PB14
USART TX      --   PA2
USART RX      --   PA1

本示例的功能流程：
1、上电配置时钟（PLL 48MHz）；
2、配置点亮LED1；
3、初始化RTC配置：
   使能并开启LXTAL，设置RTC时钟为LXTAL；
4、使能2Hz周期中断与秒周期中断；
5、关闭LED1；
6、进入Stop模式；
7、RTC中断服务程序中清除2Hz周期中断标志与秒周期中断标志；
8、2Hz周期中断后闪烁LED1，秒周期中断后获得当前时间，由USART1打印输出。
