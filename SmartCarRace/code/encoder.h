#ifndef __ENCODER_H
#define __ENCODER_H

// ����������������Ӧ�˿�
#define ENCODER1_TIM TIM4_ENCODER
#define ENCODER2_TIM TIM2_ENCODER
// ����������������������
#define ENCODER1_PIN1 TIM4_ENCODER_CH2_P00_9 // TIM4����ѡTIM4_ENCODER_CH2_P33_5
#define ENCODER1_PIN2 TIM4_ENCODER_CH1_P02_8
#define ENCODER2_PIN1 TIM2_ENCODER_CH2_P33_6 // TIM2����ѡTIM2_ENCODER_CH2_P00_8
#define ENCODER2_PIN2 TIM2_ENCODER_CH1_P33_7 // TIM2����ѡTIM2_ENCODER_CH1_P00_7
// ����������
#define PULSE_PRE_CYCLE 1024

//====================================================��������������====================================================
void encoder_init();// ��������ʼ��
void encoder_refresh();// ����������ˢ��
int16_t encoder1_get();// ��ȡencoder1��ֵ
int16_t encoder2_get();// ��ȡencoder2��ֵ
//====================================================��������������====================================================

#endif
