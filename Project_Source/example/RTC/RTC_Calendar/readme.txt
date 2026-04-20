本示例使用到的资源：
RTC_OUT引脚   --   PC13
USART TX      --   PA2
USART RX      --   PA1

本示例的功能流程：
1、上电配置时钟（PLL 48MHz）；
2、初始化RTC配置：
   使能并开启LXTAL，设置RTC时钟为LXTAL；
3、配置RTC日历时间；
4、配置RTC_OUT输出SPRE 1Hz；
5、每隔1s获取日历时间，格式化后由USART1打印输出。

