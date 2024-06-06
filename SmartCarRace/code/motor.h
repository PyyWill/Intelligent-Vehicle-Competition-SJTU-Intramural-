#ifndef __MOTOR_H
#define __MOTOR_H

// 定义电机引脚
#define MOTOR1_DIR_PIN P21_5
#define MOTOR1_PWM_PIN ATOM0_CH0_P21_2
#define MOTOR2_DIR_PIN P21_3
#define MOTOR2_PWM_PIN ATOM0_CH2_P21_4

// 定义电机pwm波最大最小占空比
#define MOTOR_DUTY_MAX 100
#define MOTOR_DUTY_MIN 0

// 定义电机转向枚举
typedef enum{Forward = 1, Backward = 0} motor_dir;

//====================================================电机基础函数====================================================
void motor_init();// 电机初始化
void motor1_setduty(float motor_duty);// 设置电机1占空比
void motor2_setduty(float motor_duty);// 设置电机2占空比
void motor1_setdir(motor_dir dir);// 设置电机1转向
void motor2_setdir(motor_dir dir);// 设置电机2转向
//====================================================电机基础函数====================================================
#endif
