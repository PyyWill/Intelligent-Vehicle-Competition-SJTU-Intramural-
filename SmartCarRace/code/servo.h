#ifndef __SERVO_H
#define __SERVO_H

// ����ź��߽ӿ�
#define  SERVO_SIG_PIN ATOM0_CH1_P33_9
// ����ź�Ƶ��,�̼��Ƽ�333Hz
#define SERVO_FRE 333
// С��������ʻ��Ӧ����Ƕ�
#define SERVO_MID 90
// ������ת���Ƕȷ�Χ
#define SERVO_ANGLE_MAX 15
#define SERVO_ANGLE_MIN -15

//====================================================�����������====================================================
void servo_init();// �����ʼ��
void servo_setangle(float angle);// ����Ƕ�����
//====================================================�����������====================================================
#endif
