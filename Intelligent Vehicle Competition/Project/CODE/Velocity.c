#include "headfile.h"
#include "Velocity.h"
#include  <math.h>

int PID_Velocity(int set_speed ,int speed)//pid���Ƶ��ת��
{
    volatile static int out;
    volatile static int out_increment;
    volatile static int ek,ek1;
    float kp=1.46,ki=2.3;
  
    //float P_L=30;
    //float I_L=1.6;
    kp = 30;//һ��pi���ӣ��ٶȲ�������̫��
    ki = 1.6;

    ek1 = ek;
    ek = set_speed - speed;
    out_increment= (int)(kp*(ek-ek1) + ki*ek);
    out+= out_increment;

    return (int) out;
}

void Velocity_Control(int speed_left_real,int speed_right_real)//���������б���ѡ���ٶ�
{
    int pwm_R=0,pwm_L=0;
 
      //�ٶȾ���
    pwm_L= PID_Velocity(0 ,speed_left_real );//pid���Ƶ��ת��
    pwm_R= PID_Velocity(0,speed_right_real);//pid���Ƶ��ת��

    // Motor_Left (pwm_L);
    // Motor_Right(pwm_R);
}

