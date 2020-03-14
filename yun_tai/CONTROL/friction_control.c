#include "friction_control.h"
#include "can_task.h"
#include "can_data.h"

#include "pid.h"
#include "ramp.h"
///*ȱ���ٱ仯*/
#define Friction_Max_Duty   1000

RampGen_t FrictionRamp = FRICTION_RAMP_GEN_DAFAULT;

volatile int FRICTION_WHEEL_MAX_DUTY = Friction_Max_Duty;   //�����˵ȼ�����������ٱ��

/////*�ڲ�����*/////
        int16_t limit(int16_t data,int16_t min,int16_t max)
        {
            if(data<min)
                data=min;
            if(data>max)
                data=max;
            return data;
        }

        int16_t Fric_speedUpLoop(void){
            int16_t target_w=(FRICTION_WHEEL_MAX_DUTY)*FrictionRamp.Calc(&FrictionRamp);
            if(FrictionRamp.IsOverflow(&FrictionRamp))
                {
                    target_w=FRICTION_WHEEL_MAX_DUTY;
                    FrictionRamp.ResetCounter(&FrictionRamp);
                }
            target_w=limit(target_w,0,FRICTION_WHEEL_MAX_DUTY);
            return target_w;    
        }

        int16_t Fric_stopLoop(void){
            int16_t target_w=FRICTION_WHEEL_MAX_DUTY-(FRICTION_WHEEL_MAX_DUTY)*FrictionRamp.Calc(&FrictionRamp);
            if(FrictionRamp.IsOverflow(&FrictionRamp))
                {
                    target_w=0;
                    FrictionRamp.ResetCounter(&FrictionRamp);
                }
            target_w=limit(target_w,0,FRICTION_WHEEL_MAX_DUTY);
            return target_w;
        }
////////////////////
        
        
void Fric_controlLoop(char fric_circuit){    //�˺�������������ѭ����controlLoop��
    int16_t targetspeed=0;;

    Motor_Data_t fric_data1=GetMotorData(FRIC1_MOTOR);
    Motor_Data_t fric_data2=GetMotorData(FRIC2_MOTOR);
    if(fric_circuit>1)  //Ħ���ֿ�
        
//        targetspeed=Fric_speedUpLoop();
        targetspeed=8000;
    else                //Ħ���ֹ�
        targetspeed=0;
//        targetspeed=Fric_stopLoop();
    SetMotorCurrent(FRIC1_MOTOR, IncPID(1,fric_data1.speed,targetspeed));
    SetMotorCurrent(FRIC2_MOTOR, IncPID(2,fric_data2.speed,-targetspeed));
    SendMotorCurrent(FRIC1_MOTOR);
    SendMotorCurrent(FRIC2_MOTOR);
}

