本示例使用到的资源：
LED1         --   PB14
COMP input   --   PA5
EXTI

本示例的功能流程：
1、上电配置时钟（RCH 8MHz）；
2、初始化LED1灯常亮；
3、初始化COMP1，正向输入是IO PA5，反向输入VBGR；
4、初始化EXTI，配置EXTI通道是COMP1，中断唤醒，上升沿触发;
5、延时3s后，进入STOP模式；
6、手动将PA5引脚电压由GND切换至大于VBGR，产生上升沿唤醒Stop；
7、唤醒后，LED1闪烁

注意:
COMP比较器输入端PA5悬空时，输入是个不定态，推荐输入端不要悬空。