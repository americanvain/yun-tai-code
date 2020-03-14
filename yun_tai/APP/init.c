#include "stm32f4xx.h"
#include "init.h" 
#include "main.h"

//四个24v 输出 依次开启 间隔 709us
#define POWER_CTRL_ONE_BY_ONE_TIME 709

void bsp_init(void)
{
    uint8_t i=0;
    //初始化延时函数
    delay_init(168);  
    //遥控器初始化   
    remote_control_init();
    //流水灯，红绿灯初始化
    led_configuration();
    //蜂鸣器初始化
    buzzer_init(30000, 90);    
    //24输出控制口 初始化
    power_ctrl_configuration();
    //定时器6 初始化
    TIM6_Init();    
    CAN_Device_Init();
    
    for (i = POWER1_CTRL_SWITCH; i < POWER4_CTRL_SWITCH + 1; i++)
    {
        power_ctrl_on(i);
        delay_us(POWER_CTRL_ONE_BY_ONE_TIME);
    }
    //自检循环  
    while(chassisSelfCheck()){
        buzzer_on(31, 20000);
    }
    
    delay_ms(50);
    
    buzzer_off();   
    
    TIM6_Start();
    
    
}

