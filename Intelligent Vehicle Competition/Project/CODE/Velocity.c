#include "headfile.h"
#include "Velocity.h"
#include  <math.h>

int PID_Velocity(int set_speed ,int speed)//pid控制电机转速
{
    volatile static int out;
    volatile static int out_increment;
    volatile static int ek,ek1;
    float kp=1.46,ki=2.3;
  
    //float P_L=30;
    //float I_L=1.6;
    kp = 30;//一套pi足矣，速度拨动不会太大
    ki = 1.6;

    ek1 = ek;
    ek = set_speed - speed;
    out_increment= (int)(kp*(ek-ek1) + ki*ek);
    out+= out_increment;

    return (int) out;
}

void Velocity_Control(int speed_left_real,int speed_right_real)//赛道类型判别，来选定速度
{
    int pwm_R=0,pwm_L=0;
 
      //速度决策
    pwm_L= PID_Velocity(0 ,speed_left_real );//pid控制电机转速
    pwm_R= PID_Velocity(0,speed_right_real);//pid控制电机转速

    // Motor_Left (pwm_L);
    // Motor_Right(pwm_R);
}

