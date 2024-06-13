
/********************************************************************************************************************
* TC387 Opensourec Library 即（TC387 开源库）是一个基于官方 SDK 接口的第三方开源库
* Copyright (c) 2022 SEEKFREE 逐飞科技
*
* 本文件是 TC387 开源库的一部分
*
* TC387 开源库 是免费软件
* 您可以根据自由软件基金会发布的 GPL（GNU General Public License，即 GNU通用公共许可证）的条款
* 即 GPL 的第3版（即 GPL3.0）或（您选择的）任何后来的版本，重新发布和/或修改它
*
* 本开源库的发布是希望它能发挥作用，但并未对其作任何的保证
* 甚至没有隐含的适销性或适合特定用途的保证
* 更多细节请参见 GPL
*
* 您应该在收到本开源库的同时收到一份 GPL 的副本
* 如果没有，请参阅<https://www.gnu.org/licenses/>
*
* 额外注明：
* 本开源库使用 GPL3.0 开源许可证协议 以上许可申明为译文版本
* 许可申明英文版在 libraries/doc 文件夹下的 GPL3_permission_statement.txt 文件中
* 许可证副本在 libraries 文件夹下 即该文件夹下的 LICENSE 文件
* 欢迎各位使用并传播本程序 但修改内容时必须保留逐飞科技的版权声明（即本声明）
*
* 文件名称          cpu0_main
* 公司名称          成都逐飞科技有限公司
* 版本信息          查看 libraries/doc 文件夹内 version 文件 版本说明
* 开发环境          ADS v1.9.20
* 适用平台          TC387QP
* 店铺链接          https://seekfree.taobao.com/
*
* 修改记录
* 日期              作者                备注
* 2022-11-04       pudding            first version
*******************************************************************************************************************/
#include "zf_common_headfile.h"
#pragma section all "cpu0_dsram"
// 将本语句与#pragma section all restore语句之间的全局变量都放在CPU0的RAM中

// 本例程是开源库空工程 可用作移植或者测试各类内外设
// 本例程是开源库空工程 可用作移植或者测试各类内外设
// 本例程是开源库空工程 可用作移植或者测试各类内外设




#define LED1                    (P20_9)
uint8_t i = 0;
uint32_t speed = 10086; // 定义并赋值 speed 变量
// **************************** 代码区域 ****************************
int core0_main(void)
{
    clock_init();                   // 获取时钟频率<务必保留>
    debug_init();                   // 初始化默认调试串口


       //gpio_init(LED1, GPO, GPIO_HIGH, GPO_PUSH_PULL);
    seekfree_assistant_interface_init(SEEKFREE_ASSISTANT_WIRELESS_UART);

       cpu_wait_event_ready();
      if(wireless_uart_init()) // 判断无线串口是否通过初始化
       {
           while(1)
           { // 初始化失败就在这进入死循环
               gpio_toggle_level(LED1); // 翻转 LED 引脚输出电平 控制 LED 亮灭
               system_delay_ms(100); // 短延时快速闪灯表示异常
           }
       }
       wireless_uart_send_byte('\r');
       wireless_uart_send_byte('\n');
       wireless_uart_send_string("SEEKFREE wireless uart demo.\r\n"); // 初始化正常 输出测试信息
       //seekfree_assistant_parameter[1]=10;
       // 此处编写用户代码 例如外设初始化代码等
       while(true) {

           seekfree_assistant_data_analysis();
           for(uint8_t i = 0; i < SEEKFREE_ASSISTANT_SET_PARAMETR_COUNT; i++)
                  {
                      // 更新标志位
                      if(seekfree_assistant_parameter_update_flag[i])
                      {
                          seekfree_assistant_parameter_update_flag[i] = 0;

                          // 通过DEBBUG串口发送信息
                          printf("receive data channel : %d ", i);
                          printf("data : %f ", seekfree_assistant_parameter[i]);
                          printf("\r\n");
                          wireless_send_number(i);
                         wireless_send_number(seekfree_assistant_parameter[i]);

                          //system_delay_ms(50);
                      }
                  }

           // 此处编写需要循环执行的代码
       }


}


#pragma section all restore
// **************************** 代码区域 ****************************

