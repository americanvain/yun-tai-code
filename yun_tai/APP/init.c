#include "stm32f4xx.h"
#include "init.h" 
#include "main.h"

//�ĸ�24v ��� ���ο��� ��� 709us
#define POWER_CTRL_ONE_BY_ONE_TIME 709

void bsp_init(void)
{
    uint8_t i=0;
    //��ʼ����ʱ����
    delay_init(168);  
    //ң������ʼ��   
    remote_control_init();
    //��ˮ�ƣ����̵Ƴ�ʼ��
    led_configuration();
    //��������ʼ��
    buzzer_init(30000, 90);    
    //24������ƿ� ��ʼ��
    power_ctrl_configuration();
    //��ʱ��6 ��ʼ��
    TIM6_Init();    
    CAN_Device_Init();
    
    for (i = POWER1_CTRL_SWITCH; i < POWER4_CTRL_SWITCH + 1; i++)
    {
        power_ctrl_on(i);
        delay_us(POWER_CTRL_ONE_BY_ONE_TIME);
    }
    //�Լ�ѭ��  
    while(chassisSelfCheck()){
        buzzer_on(31, 20000);
    }
    
    delay_ms(50);
    
    buzzer_off();   
    
    TIM6_Start();
    
    
}

