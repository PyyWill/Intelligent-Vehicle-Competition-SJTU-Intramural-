#ifndef __MOTOR_H
#define __MOTOR_H

// ����������
#define MOTOR1_DIR_PIN P21_5
#define MOTOR1_PWM_PIN ATOM0_CH0_P21_2
#define MOTOR2_DIR_PIN P21_3
#define MOTOR2_PWM_PIN ATOM0_CH2_P21_4

// ������pwm�������Сռ�ձ�
#define MOTOR_DUTY_MAX 100
#define MOTOR_DUTY_MIN 0

// ������ת��ö��
typedef enum{Forward = 1, Backward = 0} motor_dir;

//====================================================�����������====================================================
void motor_init();// �����ʼ��
void motor1_setduty(float motor_duty);// ���õ��1ռ�ձ�
void motor2_setduty(float motor_duty);// ���õ��2ռ�ձ�
void motor1_setdir(motor_dir dir);// ���õ��1ת��
void motor2_setdir(motor_dir dir);// ���õ��2ת��
//====================================================�����������====================================================
#endif
