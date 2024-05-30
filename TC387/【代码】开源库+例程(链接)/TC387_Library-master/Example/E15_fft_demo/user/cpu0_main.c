/*********************************************************************************************************************
* TC387 Opensourec Library ����TC387 ��Դ�⣩��һ�����ڹٷ� SDK �ӿڵĵ�������Դ��
* Copyright (c) 2022 SEEKFREE ��ɿƼ�
*
* ���ļ��� TC387 ��Դ���һ����
*
* TC387 ��Դ�� ��������
* �����Ը��������������ᷢ���� GPL��GNU General Public License���� GNUͨ�ù������֤��������
* �� GPL �ĵ�3�棨�� GPL3.0������ѡ��ģ��κκ����İ汾�����·�����/���޸���
*
* ����Դ��ķ�����ϣ�����ܷ������ã�����δ�������κεı�֤
* ����û�������������Ի��ʺ��ض���;�ı�֤
* ����ϸ����μ� GPL
*
* ��Ӧ�����յ�����Դ���ͬʱ�յ�һ�� GPL �ĸ���
* ���û�У������<https://www.gnu.org/licenses/>
*
* ����ע����
* ����Դ��ʹ�� GPL3.0 ��Դ���֤Э�� �����������Ϊ���İ汾
* �������Ӣ�İ��� libraries/doc �ļ����µ� GPL3_permission_statement.txt �ļ���
* ���֤������ libraries �ļ����� �����ļ����µ� LICENSE �ļ�
* ��ӭ��λʹ�ò����������� ���޸�����ʱ���뱣����ɿƼ��İ�Ȩ����������������
*
* �ļ�����          cpu1_main
* ��˾����          �ɶ���ɿƼ����޹�˾
* �汾��Ϣ          �鿴 libraries/doc �ļ����� version �ļ� �汾˵��
* ��������          ADS v1.9.20
* ����ƽ̨          TC387QP
* ��������          https://seekfree.taobao.com/
*
* �޸ļ�¼
* ����              ����                ��ע
* 2022-11-04       pudding            first version
********************************************************************************************************************/
#include "zf_common_headfile.h"
#include "SysSe/Math/Ifx_FftF32.h"
#pragma section all "cpu0_dsram"
// ���������#pragma section all restore���֮���ȫ�ֱ���������CPU0��RAM��


#define SAMPLE_FREQUENCY    (0.01)          // �������Ƶ��  ��λ�� ��
#define SIZE_N              (2000)          // ����ɼ�����


cfloat32 fft_in [SIZE_N];
cfloat32 fft_out[SIZE_N];
uint32   use_time;



float x1, x2;

// **************************** �������� ****************************
int core0_main(void)
{
    clock_init();                   // ��ȡʱ��Ƶ��<��ر���>
    debug_init();                   // ��ʼ��Ĭ�ϵ��Դ���
    // �˴���д�û����� ���������ʼ�������

    for (int i = 0; i < SIZE_N; i ++)  // ���������ź�
    {
        // ���� x
        x1  =  3 * IFX_PI * (float)i * SAMPLE_FREQUENCY;
        x2  = (7 * IFX_PI * (float)i * SAMPLE_FREQUENCY) + (IFX_PI / 2);

        // ����x����y
        // ����Ϊ y = 1024*(cos(3 * pi * x)) + 512 * (cos(7 * pi * x) + pi / 2) + 2047
        fft_in[i].real = (float32)(1024 * cosf(x1) + 512 * cosf(x2) + 2047);
        fft_in[i].imag = 0.0;
    }

    // �˴���д�û����� ���������ʼ�������
    cpu_wait_event_ready();         // �ȴ����к��ĳ�ʼ�����
    while (TRUE)
    {
        // �˴���д��Ҫѭ��ִ�еĴ���

        system_start();                             // ��ʼ��ʱ
        Ifx_FftF32_radix2(fft_out, fft_in, SIZE_N); // ����FFT���� Ifx_FftF32_radix2I ΪIFT��FFT��任������
        use_time = system_getval_us();              // ��ȡ��ʱʱ��
        printf("use_time: %ldus\n", use_time);      // �����ͨ�����ڴ�ӡ, ��ӡfftһ�κ�ʱ���

        // �˴���д��Ҫѭ��ִ�еĴ���
    }
}

#pragma section all restore
// **************************** �������� ****************************

