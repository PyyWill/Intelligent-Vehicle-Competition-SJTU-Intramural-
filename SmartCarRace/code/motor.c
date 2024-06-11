#include "zf_common_headfile.h"
#include "motor.h"

// 限幅函数，仅用于当前文件
float constrain_motor_duty(float amt)//限幅
{
  return ((amt)<(MOTOR_DUTY_MIN)?(MOTOR_DUTY_MIN):((amt) >(MOTOR_DUTY_MAX)?(MOTOR_DUTY_MAX):(amt)));
}

//-------------------------------------------------------------------------------------------------------------------
// 函数简介     初始化电机
// 参数说明     void
// 返回参数     void
// 使用示例     motor_init();
// 备注信息
//-------------------------------------------------------------------------------------------------------------------
void motor_init()
{
    // 初始化电机方向输出为正向(0:正向；1：反向)
    gpio_init(MOTOR1_DIR_PIN, GPO, 0, GPO_PUSH_PULL);
    gpio_init(MOTOR2_DIR_PIN, GPO, 0, GPO_PUSH_PULL);
    // 初始化PWM输出，占空比百分之5000/PWM_DUTY_MAX*100
    // PWM_DUTY_MAX宏定义在zf_driver_pwm.h默认为10000
    pwm_init(MOTOR1_PWM_PIN, 200, 0);
    pwm_init(MOTOR2_PWM_PIN, 200, 0);
}

//-------------------------------------------------------------------------------------------------------------------
// 函数简介     通过占空比设定电机1转速
// 参数说明     motor_duty 占空比,浮点数
// 返回参数     void
// 使用示例     motor1_setduty(50); // 设置控制电机1的PWM波占空比为50%
// 备注信息
//-------------------------------------------------------------------------------------------------------------------
void motor1_setduty(float motor_duty)
{
    pwm_set_duty(MOTOR1_PWM_PIN, (uint32_t)(constrain_motor_duty(motor_duty) / 100.0 * PWM_DUTY_MAX));
}

//-------------------------------------------------------------------------------------------------------------------
// 函数简介     通过占空比设定电机2转速
// 参数说明     motor_duty 占空比,浮点数
// 返回参数     void
// 使用示例     motor2_setduty(50); // 设置控制电机2的PWM波占空比为50%
// 备注信息
//-------------------------------------------------------------------------------------------------------------------
void motor2_setduty(float motor_duty)
{
    pwm_set_duty(MOTOR2_PWM_PIN, (uint32_t)(constrain_motor_duty(motor_duty) / 100.0 * PWM_DUTY_MAX));
}

//-------------------------------------------------------------------------------------------------------------------
// 函数简介     设置电机1转向
// 参数说明     dir 电机转向, 由motor.h中motor_dir枚举类型决定
// 返回参数     void
// 使用示例     motor1_setdir(Forward); // 设置控制电机1向正转(小车向前行驶)
// 备注信息
//-------------------------------------------------------------------------------------------------------------------
void motor1_setdir(motor_dir dir)
{
    if (dir == Forward) gpio_set_level(MOTOR1_DIR_PIN, 0);
    else if (dir == Backward) gpio_set_level(MOTOR1_DIR_PIN, 1);
}

//-------------------------------------------------------------------------------------------------------------------
// 函数简介     设置电机2转向
// 参数说明     dir 电机转向, 由motor.h中motor_dir枚举类型决定
// 返回参数     void
// 使用示例     motor2_setdir(Forward); // 设置控制电机2向正转(小车向前行驶)
// 备注信息
//-------------------------------------------------------------------------------------------------------------------
void motor2_setdir(motor_dir dir)
{
    if (dir == Forward) gpio_set_level(MOTOR2_DIR_PIN, 0);
    else if (dir == Backward) gpio_set_level(MOTOR2_DIR_PIN, 1);
}
