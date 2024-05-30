/*********************************************************************************************************************
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
* 文件名称          cpu1_main
* 公司名称          成都逐飞科技有限公司
* 版本信息          查看 libraries/doc 文件夹内 version 文件 版本说明
* 开发环境          ADS v1.9.20
* 适用平台          TC387QP
* 店铺链接          https://seekfree.taobao.com/
*
* 修改记录
* 日期              作者                备注
* 2022-11-04       pudding            first version
********************************************************************************************************************/
#include "zf_common_headfile.h"
#include "SysSe/Math/Ifx_FftF32.h"
#pragma section all "cpu0_dsram"
// 将本语句与#pragma section all restore语句之间的全局变量都放在CPU0的RAM中


#define SAMPLE_FREQUENCY    (0.01)          // 定义采样频率  单位： 秒
#define SIZE_N              (2000)          // 定义采集数量


cfloat32 fft_in [SIZE_N];
cfloat32 fft_out[SIZE_N];
uint32   use_time;



float x1, x2;

// **************************** 代码区域 ****************************
int core0_main(void)
{
    clock_init();                   // 获取时钟频率<务必保留>
    debug_init();                   // 初始化默认调试串口
    // 此处编写用户代码 例如外设初始化代码等

    for (int i = 0; i < SIZE_N; i ++)  // 生成输入信号
    {
        // 计算 x
        x1  =  3 * IFX_PI * (float)i * SAMPLE_FREQUENCY;
        x2  = (7 * IFX_PI * (float)i * SAMPLE_FREQUENCY) + (IFX_PI / 2);

        // 代入x计算y
        // 函数为 y = 1024*(cos(3 * pi * x)) + 512 * (cos(7 * pi * x) + pi / 2) + 2047
        fft_in[i].real = (float32)(1024 * cosf(x1) + 512 * cosf(x2) + 2047);
        fft_in[i].imag = 0.0;
    }

    // 此处编写用户代码 例如外设初始化代码等
    cpu_wait_event_ready();         // 等待所有核心初始化完毕
    while (TRUE)
    {
        // 此处编写需要循环执行的代码

        system_start();                             // 开始计时
        Ifx_FftF32_radix2(fft_out, fft_in, SIZE_N); // 进行FFT运算 Ifx_FftF32_radix2I 为IFT（FFT逆变换）运算
        use_time = system_getval_us();              // 获取计时时间
        printf("use_time: %ldus\n", use_time);      // 将结果通过串口打印, 打印fft一次耗时多久

        // 此处编写需要循环执行的代码
    }
}

#pragma section all restore
// **************************** 代码区域 ****************************

