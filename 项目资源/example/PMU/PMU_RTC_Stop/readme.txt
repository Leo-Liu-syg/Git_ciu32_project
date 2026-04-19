本示例使用到的资源：
LED1          --   PB14
BUTTON_USER   --   PC13


本示例的功能流程：
1、上电配置系统时钟为48MHz（PLL），并初始化LED1，常亮；
2、初始化RTC，且RTC时钟源为LXTAL；
3、初始化USER BUTTON，并作为Stop唤醒源，且下降沿有效；
5、打印输出当前时间，并进入Stop模式，此时LED1灭；
7、等待USER BUTTON按下，唤醒Stop，此时LED1亮；
8、再次读取RTC日历时间，并打印；
9、两次时间不一致，则说明Stop模式下RTC正常工作。