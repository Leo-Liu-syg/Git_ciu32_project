本示例使用到的资源：
LED1         --   PB14
COMP1
COMP2
COMP1 input  --   PB2
COMP1 output --   PA5
VREF+引脚    --   PA0（配置模拟模式，外接0.1uF+1uF电容）

本示例的功能流程：
1、上电配置时钟（PLL 48MHz）；
2、初始化LED1灯熄灭；
3、初始化COMP1，正向输入是IO PB2，反向输入VBGR，非窗口比较器模式，输出结果为COMP1\2输出异或;
4、初始化COMP2，正向输入COMP1的正向信号，反向输入6BIT DAC输出2.048V,窗口比较模式，独立输出；
5、检查COMP1输出，手动切换PB2引脚输入电压，如果电压大于6BIT DAC输出或小于VBGR，LED1点亮，反之，保持熄灭。

注意：
1)COMP比较器输入端PB2悬空时,输入是个不定态，推荐输入端不要悬空。