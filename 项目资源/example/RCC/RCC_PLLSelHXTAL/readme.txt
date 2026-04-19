本示例使用到的资源：
MCO          --   PA8

本示例的功能流程：
1、上电默认系统时钟RCHSYS 16MHz；
2、使能HXTAL，且HXTAL为晶体模式，频率为8MHz；
3、配置系统时钟配置为PLL，PLL时钟源为HXTAL；
4、打开HXTAL CSS；
5、当发生HXTAL CSS故障时，进入NMI中断处理。

注意：
1、发生HXTAL CSS故障后，系统时钟自动切换为RCHSYS，RCH的分频由RCHDIV决定，本示例为2分频。
