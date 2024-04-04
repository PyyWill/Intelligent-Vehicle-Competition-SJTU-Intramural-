#include "headfile.h"
#include "PID.h"
#include  <math.h>

#define LEFT_MAX     12//+
#define RIGHT_MAX    -12//-

extern int Search_Stop_Line;
extern int Right_Line[MT9V03X_CSI_H];
extern int Left_Line[MT9V03X_CSI_H];

//��Ȩ����
const uint8 Weight[MT9V03X_CSI_H]=
{
 
         1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
         1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
        1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 
        1, 1, 1, 1, 1, 1, 1, 1, 1, 1,              
        1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
        1, 3, 4, 5, 6, 7, 9,11,13,15,
        17,19,20,20,19,17,15,13,11, 9

};

float Err_Sum(void)
{
    int i;
    float err=0;
    float weight_count=0;
    //�������
    for(i=MT9V03X_CSI_H-1;i>=MT9V03X_CSI_H-Search_Stop_Line-1;i--)//����������
    {
        err+=(MT9V03X_CSI_W/2-((Left_Line[i]+Right_Line[i])>>1))*Weight[i];//����1λ����Ч��2
        weight_count+=Weight[i];
    }
    err=err/weight_count;
    return err;
}

//��������
int PD_Camera(float expect_val,float err)//���PD����
{
   float  u;
   float  P=0.25; //�����������������������Ϊ�ο�
   float  D=0.15;
   volatile static float error_current,error_last;
   float ek,ek1;
   error_current=err-expect_val;
   ek=error_current;
   ek1=error_current-error_last;
   u=P*ek+D*ek1;
   error_last=error_current;
 
   if(u>=LEFT_MAX)//�޷�����
       u=LEFT_MAX;
   else if(u<=RIGHT_MAX)//�޷�����
       u=RIGHT_MAX;
   return (int)u;
}
