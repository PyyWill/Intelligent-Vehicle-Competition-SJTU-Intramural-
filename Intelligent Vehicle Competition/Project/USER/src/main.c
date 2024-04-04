/*********************************************************************************************************************
 * COPYRIGHT NOTICE
 * Copyright (c) 2019,逐飞科技
 * All rights reserved.
 * 技术讨论QQ群：一群：179029047(已满)  二群：244861897
 *
 * 以下所有内容版权均属逐飞科技所有，未经允许不得用于商业用途，
 * 欢迎各位使用并传播本程序，修改内容时必须保留逐飞科技的版权声明。
 *
 * @file       		main
 * @company	   		成都逐飞科技有限公司
 * @author     		逐飞科技(QQ3184284598)
 * @version    		查看doc内version文件 版本说明
 * @Software 		IAR 8.3 or MDK 5.28
 * @Target core		NXP RT1064DVL6A
 * @Taobao   		https://seekfree.taobao.com/
 * @date       		2019-04-30
 ********************************************************************************************************************/


//整套推荐IO查看Projecct文件夹下的TXT文本


//打开新的工程或者工程移动了位置务必执行以下操作
//第一步 关闭上面所有打开的文件
//第二步 project  clean  等待下方进度条走完

//下载代码前请根据自己使用的下载器在工程里设置下载器为自己所使用的

#include "headfile.h"
#include "IMAGEPROCESS.h"
#include "PID.h"


// 限幅函数
#define clip(x, xmin, xmax) (((x)<(xmin))?(xmin):(((x)>(xmax))?(xmax):(x)))


// 驱动电机
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
    s1 = clip(s1, -50000, 50000);       // s1与s2范围：-50000 到 50000   不可更改
    s2 = clip(s2, -50000, 50000);       // s1与s2范围：-50000 到 50000   不可更改
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


// 转向舵机
#define SERVO_PWM_PIN   PWM4_MODULE2_CHA_C30        // 定义舵机引脚
#define SERVO_FREQ      50                          // 舵机频率
#define SERVO_MID       92                          // 舵机中位角度(+左 -右)
int16 servo_duty(float angle){          
    angle = clip(angle, -12, 12);       // 舵角范围：-12 到 12       可适当更改
    angle += SERVO_MID;
    return (angle * 2 / 180 + 0.5) * PWM_DUTY_MAX * SERVO_FREQ / 1000;
}
void servo_init() {
    pwm_init(SERVO_PWM_PIN, SERVO_FREQ, servo_duty(0));
}

void servo_angle(float angle) {
    pwm_duty(SERVO_PWM_PIN, servo_duty(angle));
}


// 编码器
#define ENCODER1_TIM      QTIMER_1
#define ENCODER1_A        QTIMER1_TIMER0_C0
#define ENCODER1_B        QTIMER1_TIMER1_C1
#define ENCODER2_TIM      QTIMER_1
#define ENCODER2_A        QTIMER1_TIMER2_C2
#define ENCODER2_B        QTIMER1_TIMER3_C24
int encoder1 = 0, encoder2 = 0;                 // 编码值
void encoder_init() {
    qtimer_quad_init(ENCODER1_TIM, ENCODER1_A, ENCODER1_B);
    qtimer_quad_init(ENCODER2_TIM, ENCODER2_A, ENCODER2_B);
}
void encoder_refresh() {                        // 更新编码值，需要定时调用，建议调用周期1ms-5ms
    encoder1 = qtimer_quad_get(ENCODER1_TIM, ENCODER1_A);
    encoder2 = qtimer_quad_get(ENCODER2_TIM, ENCODER2_A);
    qtimer_quad_clear(ENCODER1_TIM, ENCODER1_A);
    qtimer_quad_clear(ENCODER2_TIM, ENCODER2_A);
}

int Threshold;//大津法阈值
extern uint8 image_two_value[MT9V03X_CSI_H][MT9V03X_CSI_W];//二值化后的原数组
float Err;//中线加权误差
int Steer_Angle;//PD计算角度值

extern volatile int Island_State;
extern volatile int Left_Line[MT9V03X_CSI_H]; //?????????
extern volatile int Right_Line[MT9V03X_CSI_H];//?????????
extern volatile int Mid_Line[MT9V03X_CSI_H];  //????????

extern volatile int Junction_Flag;

// 主函数：初始化
// 主循环：图像处理和方向控制
int main(void)
{
    DisableGlobalIRQ();
    board_init();//务必保留，本函数用于初始化MPU 时钟 调试串口
	
    systick_delay_ms(200);
    
	//此处编写用户代码(例如：外设初始化代码等)
    lcd_init();
    lcd_showstr(0, 0, "Initializing...");
    mt9v03x_csi_init();
    servo_init();
    motor_init();
    encoder_init();
    pit_init();
    pit_interrupt_ms(PIT_CH0, 2);  // 2ms定时器中断，可适当调节
	
	//总中断最后开启
    EnableGlobalIRQ(0);

    while (1)
    {
        // 图像基本处理（二值化+找边线）
        if(mt9v03x_csi_finish_flag)//先清除志位，再处理图像
        {
        mt9v03x_csi_finish_flag = 0;//标志位清除
        Threshold=my_adapt_threshold(mt9v03x_csi_image[0],MT9V03X_CSI_W,MT9V03X_CSI_H);//大津计算阈值
        Image_Binarization(Threshold);//图像二值化
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
        
        // PD循线
        Err=Err_Sum();      //误差计算
        Steer_Angle=PD_Camera(0,Err);//摄像头舵机PD调
        servo_angle(Steer_Angle);          //实际占空比控制
        }
        
        
        
        
        // motor_set(10000, 10000);
    
    
        //if(mt9v03x_csi_finish_flag) {
        //    mt9v03x_csi_finish_flag = 0;
        //    // do something with mt9v03x_csi_image
        //    lcd_displayimage032(mt9v03x_csi_image[0], MT9V03X_CSI_W, MT9V03X_CSI_H);
        //}
            
        
        
        // TODO：根据图像控制舵机
        
          
        
        
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

// PIT0定时器回调，用于刷新编码器和控制电机
void timer0_callback() {
    // 更新编码器
    encoder_refresh();
    motor_set(10000, 10000);
    // TODO：速度闭环
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


