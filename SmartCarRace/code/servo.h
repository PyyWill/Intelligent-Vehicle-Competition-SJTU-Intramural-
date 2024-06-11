#ifndef __SERVO_H
#define __SERVO_H

// 舵机信号线接口
#define  SERVO_SIG_PIN ATOM0_CH1_P33_9
// 舵机信号频率,商家推荐333Hz
#define SERVO_FRE 333
// 小车正向行驶对应舵机角度
#define SERVO_MID 90
// 定义舵机转动角度范围
#define SERVO_ANGLE_MAX 15
#define SERVO_ANGLE_MIN -15

//====================================================舵机基础函数====================================================
void servo_init();// 舵机初始化
void servo_setangle(float angle);// 舵机角度设置
//====================================================舵机基础函数====================================================
#endif
