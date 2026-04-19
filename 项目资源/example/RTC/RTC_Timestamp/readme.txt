本示例使用到的资源：
TAMP_IN       --   PC13
USART TX      --   PA2
USART RX      --   PA1

本示例的功能流程：
1、上电配置时钟（PLL 48MHz）；
2、初始化RTC配置：
   使能并开启LXTAL，设置RTC时钟为LXTAL；
3、配置TAMP_IN外部入侵：
   RTC_OUT关闭输出，设为TAMP_IN；
   下降沿触发，使能引脚上拉电阻；
   使能TAMP，使能TAMP中断；
4、按下TAMP_IN按键，产生外部入侵信号，触发中断；
5、时间戳标志置位后，获取时间戳记录值，并通过USART1打印输出。
