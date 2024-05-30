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
* �ļ�����          cpu0_main
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
#pragma section all "cpu0_dsram"
// ���������#pragma section all restore���֮���ȫ�ֱ���������CPU0��RAM��




// *************************** ����Ӳ������˵�� ***************************
// ���İ��������缴�� �����������


// *************************** ���̲���˵�� ***************************
// 1.���İ���¼��ɱ����̣�����ϵ�
// 2.���Կ������İ��� P20.9 �� P21.5 LED ����
// 3.ʹ�öŰ���һ�˽�GND������һ�˽Ӵ� P15.4 ���ţ��ᷢ�� P20.9 LED�� �л�״̬
// 4.ʹ�öŰ���һ�˽�GND������һ�˽Ӵ� P14.3 ���ţ��ᷢ�� P20.8 LED�� �л�״̬
// 5.ʹ�öŰ���һ�˽�GND������һ�˽Ӵ� P15.5 ���ţ��ᷢ�� P21.5 LED�� �л�״̬
// 6.ʹ�öŰ���һ�˽�GND������һ�˽Ӵ� P15.8 ���ţ��ᷢ�� P21.4 LED�� �л�״̬
// �������������˵�����ز��� ����ձ��ļ����·� ���̳�������˵�� �����Ų�

// **************************** �������� ****************************

#define EXTI_PORT1              (ERU_CH0_REQ0_P15_4 )
#define EXTI_PORT2              (ERU_CH1_REQ10_P14_3)
#define EXTI_PORT3              (ERU_CH4_REQ13_P15_5)
#define EXTI_PORT4              (ERU_CH5_REQ1_P15_8 )

#define LED1                    (P20_9)
#define LED2                    (P20_8)
#define LED3                    (P21_5)
#define LED4                    (P21_4)

int core0_main(void)
{
    clock_init();                   // ��ȡʱ��Ƶ��<��ر���>
    debug_init();                   // ��ʼ��Ĭ�ϵ��Դ���
    // �˴���д�û����� ���������ʼ�������

    gpio_init(LED1, GPO, GPIO_LOW, GPO_PUSH_PULL);          // ��ʼ�� LED1 ��� Ĭ�ϸߵ�ƽ �������ģʽ
    gpio_init(LED2, GPO, GPIO_HIGH, GPO_PUSH_PULL);         // ��ʼ�� LED2 ��� Ĭ�ϸߵ�ƽ �������ģʽ
    gpio_init(LED3, GPO, GPIO_LOW, GPO_PUSH_PULL);          // ��ʼ�� LED3 ��� Ĭ�ϸߵ�ƽ �������ģʽ
    gpio_init(LED4, GPO, GPIO_HIGH, GPO_PUSH_PULL);         // ��ʼ�� LED4 ��� Ĭ�ϸߵ�ƽ �������ģʽ

	exti_init(EXTI_PORT1, EXTI_TRIGGER_RISING);             // ʹ�õ��ⲿ�ж���������
	exti_init(EXTI_PORT2, EXTI_TRIGGER_RISING);             // ʹ�õ��ⲿ�ж���������
	exti_init(EXTI_PORT3, EXTI_TRIGGER_RISING);             // ʹ�õ��ⲿ�ж���������
	exti_init(EXTI_PORT4, EXTI_TRIGGER_RISING);             // ʹ�õ��ⲿ�ж���������

    // �˴���д�û����� ���������ʼ�������
    cpu_wait_event_ready();         // �ȴ����к��ĳ�ʼ�����
	while (TRUE)
	{
        // �˴���д��Ҫѭ��ִ�еĴ���




        // �˴���д��Ҫѭ��ִ�еĴ���
	}
}

IFX_INTERRUPT(exti_ch0_ch4_isr, EXTI_CH0_CH4_INT_VECTAB_NUM, EXTI_CH0_CH4_INT_PRIO)
{
    interrupt_global_enable(0);                     // �����ж�Ƕ��
    if(exti_flag_get(ERU_CH0_REQ0_P15_4))           // ͨ��0�ж�
    {
        exti_flag_clear(ERU_CH0_REQ0_P15_4);

        gpio_toggle_level(LED1);                                        // ��ת LED ���������ƽ ���� LED ����


    }

    if(exti_flag_get(ERU_CH4_REQ13_P15_5))          // ͨ��4�ж�
    {
        exti_flag_clear(ERU_CH4_REQ13_P15_5);

        gpio_toggle_level(LED3);                                        // ��ת LED ���������ƽ ���� LED ����


    }
}

IFX_INTERRUPT(exti_ch1_ch5_isr, EXTI_CH1_CH5_INT_VECTAB_NUM, EXTI_CH1_CH5_INT_PRIO)
{
    interrupt_global_enable(0);                     // �����ж�Ƕ��
    if(exti_flag_get(ERU_CH1_REQ10_P14_3))          // ͨ��1�ж�
    {
        exti_flag_clear(ERU_CH1_REQ10_P14_3);

        gpio_toggle_level(LED2);                                        // ��ת LED ���������ƽ ���� LED ����

    }

    if(exti_flag_get(ERU_CH5_REQ1_P15_8))           // ͨ��5�ж�
    {
        exti_flag_clear(ERU_CH5_REQ1_P15_8);

        gpio_toggle_level(LED4);                                        // ��ת LED ���������ƽ ���� LED ����

    }
}

#pragma section all restore
// **************************** �������� ****************************

// *************************** ���̳�������˵�� ***************************
// ��������ʱ�밴�������������б���
// ����1��LED ����
//      �鿴�����Ƿ�������¼���Ƿ����ر���ȷ���������¸�λ����
//      ���ñ������Ӧ LED ����ѹ���Ƿ�ԼΪ0V�����ԼΪ0V֤������δ����.

