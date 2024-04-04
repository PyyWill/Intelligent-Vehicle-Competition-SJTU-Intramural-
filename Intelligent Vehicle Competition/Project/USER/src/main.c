/*********************************************************************************************************************
 * COPYRIGHT NOTICE
 * Copyright (c) 2019,��ɿƼ�
 * All rights reserved.
 * ��������QQȺ��һȺ��179029047(����)  ��Ⱥ��244861897
 *
 * �����������ݰ�Ȩ������ɿƼ����У�δ��������������ҵ��;��
 * ��ӭ��λʹ�ò������������޸�����ʱ���뱣����ɿƼ��İ�Ȩ������
 *
 * @file       		main
 * @company	   		�ɶ���ɿƼ����޹�˾
 * @author     		��ɿƼ�(QQ3184284598)
 * @version    		�鿴doc��version�ļ� �汾˵��
 * @Software 		IAR 8.3 or MDK 5.28
 * @Target core		NXP RT1064DVL6A
 * @Taobao   		https://seekfree.taobao.com/
 * @date       		2019-04-30
 ********************************************************************************************************************/


//�����Ƽ�IO�鿴Projecct�ļ����µ�TXT�ı�


//���µĹ��̻��߹����ƶ���λ�����ִ�����²���
//��һ�� �ر��������д򿪵��ļ�
//�ڶ��� project  clean  �ȴ��·�����������

//���ش���ǰ������Լ�ʹ�õ��������ڹ���������������Ϊ�Լ���ʹ�õ�

#include "headfile.h"
#include "IMAGEPROCESS.h"
#include "PID.h"


// �޷�����
#define clip(x, xmin, xmax) (((x)<(xmin))?(xmin):(((x)>(xmax))?(xmax):(x)))


// �������
#define MOTOR1_DIR_PIN    D0
#define MOTOR1_PWM_PIN    PWM2_MODULE3_CHA_D2
#define MOTOR2_DIR_PIN    D1
#define MOTOR2_PWM_PIN    PWM2_MODULE3_CHB_D3
void motor_init() {
    gpio_init(MOTOR1_DIR_PIN, GPO, 0, GPIO_PIN_CONFIG);
    gpio_init(MOTOR2_DIR_PIN, GPO, 0, GPIO_PIN_CONFIG);
    pwm_init(MOTOR1_PWM_PIN, 17000, 0);
    pwm_init(MOTOR2_PWM_PIN, 17000, 0);
}
void motor_set(int s1, int s2) {        
    s1 = clip(s1, -50000, 50000);       // s1��s2��Χ��-50000 �� 50000   ���ɸ���
    s2 = clip(s2, -50000, 50000);       // s1��s2��Χ��-50000 �� 50000   ���ɸ���
    if(s1 > 0) {
        gpio_set(MOTOR1_DIR_PIN, 0);
        pwm_duty(MOTOR1_PWM_PIN, s1);
    }else {
        gpio_set(MOTOR1_DIR_PIN, 1);
        pwm_duty(MOTOR1_PWM_PIN, -s1);        
    }
    if(s2 > 0) {
        gpio_set(MOTOR2_DIR_PIN, 0);
        pwm_duty(MOTOR2_PWM_PIN, s2);
    }else {
        gpio_set(MOTOR2_DIR_PIN, 1);
        pwm_duty(MOTOR2_PWM_PIN, -s2);        
    }
}


// ת����
#define SERVO_PWM_PIN   PWM4_MODULE2_CHA_C30        // ����������
#define SERVO_FREQ      50                          // ���Ƶ��
#define SERVO_MID       92                          // �����λ�Ƕ�(+�� -��)
int16 servo_duty(float angle){          
    angle = clip(angle, -12, 12);       // ��Ƿ�Χ��-12 �� 12       ���ʵ�����
    angle += SERVO_MID;
    return (angle * 2 / 180 + 0.5) * PWM_DUTY_MAX * SERVO_FREQ / 1000;
}
void servo_init() {
    pwm_init(SERVO_PWM_PIN, SERVO_FREQ, servo_duty(0));
}

void servo_angle(float angle) {
    pwm_duty(SERVO_PWM_PIN, servo_duty(angle));
}


// ������
#define ENCODER1_TIM      QTIMER_1
#define ENCODER1_A        QTIMER1_TIMER0_C0
#define ENCODER1_B        QTIMER1_TIMER1_C1
#define ENCODER2_TIM      QTIMER_1
#define ENCODER2_A        QTIMER1_TIMER2_C2
#define ENCODER2_B        QTIMER1_TIMER3_C24
int encoder1 = 0, encoder2 = 0;                 // ����ֵ
void encoder_init() {
    qtimer_quad_init(ENCODER1_TIM, ENCODER1_A, ENCODER1_B);
    qtimer_quad_init(ENCODER2_TIM, ENCODER2_A, ENCODER2_B);
}
void encoder_refresh() {                        // ���±���ֵ����Ҫ��ʱ���ã������������1ms-5ms
    encoder1 = qtimer_quad_get(ENCODER1_TIM, ENCODER1_A);
    encoder2 = qtimer_quad_get(ENCODER2_TIM, ENCODER2_A);
    qtimer_quad_clear(ENCODER1_TIM, ENCODER1_A);
    qtimer_quad_clear(ENCODER2_TIM, ENCODER2_A);
}

int Threshold;//�����ֵ
extern uint8 image_two_value[MT9V03X_CSI_H][MT9V03X_CSI_W];//��ֵ�����ԭ����
float Err;//���߼�Ȩ���
int Steer_Angle;//PD����Ƕ�ֵ

extern volatile int Island_State;
extern volatile int Left_Line[MT9V03X_CSI_H]; //?????????
extern volatile int Right_Line[MT9V03X_CSI_H];//?????????
extern volatile int Mid_Line[MT9V03X_CSI_H];  //????????

extern volatile int Junction_Flag;

// ����������ʼ��
// ��ѭ����ͼ����ͷ������
int main(void)
{
    DisableGlobalIRQ();
    board_init();//��ر��������������ڳ�ʼ��MPU ʱ�� ���Դ���
	
    systick_delay_ms(200);
    
	//�˴���д�û�����(���磺�����ʼ�������)
    lcd_init();
    lcd_showstr(0, 0, "Initializing...");
    mt9v03x_csi_init();
    servo_init();
    motor_init();
    encoder_init();
    pit_init();
    pit_interrupt_ms(PIT_CH0, 2);  // 2ms��ʱ���жϣ����ʵ�����
	
	//���ж������
    EnableGlobalIRQ(0);

    while (1)
    {
        // ͼ�����������ֵ��+�ұ��ߣ�
        if(mt9v03x_csi_finish_flag)//�����־λ���ٴ���ͼ��
        {
        mt9v03x_csi_finish_flag = 0;//��־λ���
        Threshold=my_adapt_threshold(mt9v03x_csi_image[0],MT9V03X_CSI_W,MT9V03X_CSI_H);//��������ֵ
        Image_Binarization(Threshold);//ͼ���ֵ��
        Longest_White_Column();
        
        
        Cross_Detect();
        Junction_Detect();
        Island_Detect();
        lcd_showint8(6,6,Island_State);
        //lcd_showint8(5, 5, Island_State);
        lcd_showint8(1,1,Right_Line[110]);
        lcd_showint8(2,2,Left_Line[110]);
          
        Show_Boundry();
        lcd_displayimage032(image_two_value[0], MT9V03X_CSI_W, MT9V03X_CSI_H);
        
        // PDѭ��
        Err=Err_Sum();      //������
        Steer_Angle=PD_Camera(0,Err);//����ͷ���PD��
        servo_angle(Steer_Angle);          //ʵ��ռ�ձȿ���
        }
        
        
        
        
        // motor_set(10000, 10000);
    
    
        //if(mt9v03x_csi_finish_flag) {
        //    mt9v03x_csi_finish_flag = 0;
        //    // do something with mt9v03x_csi_image
        //    lcd_displayimage032(mt9v03x_csi_image[0], MT9V03X_CSI_W, MT9V03X_CSI_H);
        //}
            
        
        
        // TODO������ͼ����ƶ��
        
          
        
        
        /*
            int x1, x2;
            for(x1=MT9V03X_CSI_W/2; x1>=0; x1--) {
                if(mt9v03x_csi_image[MT9V03X_CSI_H*2/3][x1] < 200) break;
            }
            for(x2=MT9V03X_CSI_W/2; x2<MT9V03X_CSI_W; x2++) {
                if(mt9v03x_csi_image[MT9V03X_CSI_H*2/3][x2] < 200) break;
            }
            float angle = -(float)((x1+x2)/2-MT9V03X_CSI_W/2)/(MT9V03X_CSI_W/2)*60;
            servo_angle(angle);
        */
    }
}

// PIT0��ʱ���ص�������ˢ�±������Ϳ��Ƶ��
void timer0_callback() {
    // ���±�����
    encoder_refresh();
    motor_set(10000, 10000);
    // TODO���ٶȱջ�
    /*if(Junction_Flag != 0)
    {
      motor_set(8000, 8000);
    }
    else if (Island_State >= 4)
    {
      motor_set(9000, 9000);    
    }
    else
    {
      motor_set(10000, 10000);
    }*/
}


