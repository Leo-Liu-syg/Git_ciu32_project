本示例使用到的资源：
RTC_OUT      --   PC13


本示例的功能流程：
1、上电配置系统时钟为48MHz（PLL）；
2、初始化RTC，且RTC时钟源为LXTAL（晶体模式），并输出RTC_OUT信号（PC13）；
3、手动进入VBAT模式，此时RTC_OUT1正常输出（256Hz）。