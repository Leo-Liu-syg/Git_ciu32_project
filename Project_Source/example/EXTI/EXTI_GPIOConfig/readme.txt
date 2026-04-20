本示例使用到的资源：
LED1         --   PB14
BUTTON_USER  --   PC13

本示例的功能流程：
1、上电配置时钟；
   PLL 48MHz，PLL时钟源RCH。
2、初始化LED1和USER按键；
   当USER按键按下时，生成EXTI中断，在EXTI中断函数中读取/清除中断状态并置全局标志。
3、每按一次按键，LED1切换一次亮灭；