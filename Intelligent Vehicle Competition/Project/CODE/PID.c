#include "headfile.h"
#include "PID.h"
#include  <math.h>

#define LEFT_MAX     12//+
#define RIGHT_MAX    -12//-

extern int Search_Stop_Line;
extern int Right_Line[MT9V03X_CSI_H];
extern int Left_Line[MT9V03X_CSI_H];

//加权控制
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
    //常规误差
    for(i=MT9V03X_CSI_H-1;i>=MT9V03X_CSI_H-Search_Stop_Line-1;i--)//常规误差计算
    {
        err+=(MT9V03X_CSI_W/2-((Left_Line[i]+Right_Line[i])>>1))*Weight[i];//右移1位，等效除2
        weight_count+=Weight[i];
    }
    err=err/weight_count;
    return err;
}

//函数本体
int PD_Camera(float expect_val,float err)//舵机PD调节
{
   float  u;
   float  P=0.25; //参数需自行整定，这里仅作为参考
   float  D=0.15;
   volatile static float error_current,error_last;
   float ek,ek1;
   error_current=err-expect_val;
   ek=error_current;
   ek1=error_current-error_last;
   u=P*ek+D*ek1;
   error_last=error_current;
 
   if(u>=LEFT_MAX)//限幅处理
       u=LEFT_MAX;
   else if(u<=RIGHT_MAX)//限幅处理
       u=RIGHT_MAX;
   return (int)u;
}
