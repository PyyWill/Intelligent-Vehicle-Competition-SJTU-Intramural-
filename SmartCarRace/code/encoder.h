#ifndef __ENCODER_H
#define __ENCODER_H

// 定义正交编码器对应端口
#define ENCODER1_TIM TIM4_ENCODER
#define ENCODER2_TIM TIM2_ENCODER
// 定义正交编码器计数引脚
#define ENCODER1_PIN1 TIM4_ENCODER_CH2_P00_9 // TIM4还可选TIM4_ENCODER_CH2_P33_5
#define ENCODER1_PIN2 TIM4_ENCODER_CH1_P02_8
#define ENCODER2_PIN1 TIM2_ENCODER_CH2_P33_6 // TIM2还可选TIM2_ENCODER_CH2_P00_8
#define ENCODER2_PIN2 TIM2_ENCODER_CH1_P33_7 // TIM2还可选TIM2_ENCODER_CH1_P00_7
// 编码器线数
#define PULSE_PRE_CYCLE 1024

//====================================================编码器基础函数====================================================
void encoder_init();// 编码器初始化
void encoder_refresh();// 编码器计数刷新
int16_t encoder1_get();// 读取encoder1的值
int16_t encoder2_get();// 读取encoder2的值
//====================================================编码器基础函数====================================================

#endif
