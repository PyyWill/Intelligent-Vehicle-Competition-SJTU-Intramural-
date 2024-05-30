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
// ʹ����ɿƼ� Ӣ����TriCore ��������������
//      ֱ�ӽ���������ȷ�����ں��İ�ĵ������ؽӿڼ���
// ʹ�� USB-TTL ģ������
//      ģ��ܽ�            ��Ƭ���ܽ�
//      USB-TTL-RX          �鿴 zf_common_debug.h �ļ��� DEBUG_UART_TX_PIN �궨������� Ĭ�� P14_0
//      USB-TTL-TX          �鿴 zf_common_debug.h �ļ��� DEBUG_UART_RX_PIN �궨������� Ĭ�� P14_1
//      USB-TTL-GND         ���İ��Դ�� GND
//      USB-TTL-3V3         ���İ� 3V3 ��Դ
// ������������������
//      ģ��ܽ�            ��Ƭ���ܽ�
//      A                   ENCODER_QUADDEC_A �궨������� Ĭ�� B4
//      B                   ENCODER_QUADDEC_B �궨������� Ĭ�� B5
//      GND                 ���İ��Դ�� GND
//      3V3                 ���İ� 3V3 ��Դ
// ���뷽�����������
//      ģ��ܽ�            ��Ƭ���ܽ�
//      LSB                 ENCODER_DIR_PULSE �궨������� Ĭ�� B6
//      DIR                 ENCODER_DIR_DIR �궨������� Ĭ�� B7
//      GND                 ���İ��Դ�� GND
//      3V3                 ���İ� 3V3 ��Դ



// *************************** ���̲���˵�� ***************************
// 1.���İ���¼��ɱ����̣�����ʹ�ú��İ���������������� USB-TTL ģ�飬�����Ӻñ��������ڶϵ�������������
// 2.���������������� USB-TTL ģ�����ӵ��ԣ�����ϵ�
// 3.������ʹ�ô������ִ򿪶�Ӧ�Ĵ��ڣ����ڲ�����Ϊ zf_common_debug.h �ļ��� DEBUG_UART_BAUDRATE �궨�� Ĭ�� 115200�����İ尴�¸�λ����
// 4.�����ڴ��������Ͽ������´�����Ϣ��
//      ENCODER_QUADDEC counter     x .
//      ENCODER_DIR counter         x .
// 5.ת���������ͻῴ����ֵ�仯
// �������������˵�����ز��� ����ձ��ļ����·� ���̳�������˵�� �����Ų�


// **************************** �������� ****************************

#define PIT0                            (CCU60_CH0 )                            // ʹ�õ������жϱ��

#define ENCODER_QUADDEC                 (TIM5_ENCODER)                         // ������������Ӧʹ�õı������ӿ�
#define ENCODER_QUADDEC_A               (TIM5_ENCODER_CH1_P10_3)               // A ���Ӧ������
#define ENCODER_QUADDEC_B               (TIM5_ENCODER_CH2_P10_1)               // B ���Ӧ������

#define ENCODER_DIR                     (TIM6_ENCODER)                         // �������������Ӧʹ�õı������ӿ�
#define ENCODER_DIR_PULSE               (TIM6_ENCODER_CH1_P20_3)               // PULSE ��Ӧ������
#define ENCODER_DIR_DIR                 (TIM6_ENCODER_CH2_P20_0)               // DIR ��Ӧ������

int16 encoder_data_quaddec = 0;
int16 encoder_data_dir = 0;

int core0_main(void)
{
    clock_init();                   // ��ȡʱ��Ƶ��<��ر���>
    debug_init();                   // ��ʼ��Ĭ�ϵ��Դ���
    // �˴���д�û����� ���������ʼ�������

    encoder_quad_init(ENCODER_QUADDEC, ENCODER_QUADDEC_A, ENCODER_QUADDEC_B);   // ��ʼ��������ģ�������� �������������ģʽ
    encoder_dir_init(ENCODER_DIR, ENCODER_DIR_PULSE, ENCODER_DIR_DIR);          // ��ʼ��������ģ�������� ����������������ģʽ

    pit_ms_init(PIT0, 100);                                                     // ��ʼ�� PIT0 Ϊ�����ж� 100ms ��


    // �˴���д�û����� ���������ʼ�������
    cpu_wait_event_ready();         // �ȴ����к��ĳ�ʼ�����
	while (TRUE)
	{
        // �˴���д��Ҫѭ��ִ�еĴ���

        printf("ENCODER_QUADDEC counter \t%d .\r\n", encoder_data_quaddec);     // ���������������Ϣ
        printf("ENCODER_DIR counter \t\t%d .\r\n", encoder_data_dir);           // ���������������Ϣ
        system_delay_ms(100);

        // �˴���д��Ҫѭ��ִ�еĴ���
	}
}

IFX_INTERRUPT(cc60_pit_ch0_isr, CCU6_0_CH0_INT_VECTAB_NUM, CCU6_0_CH0_ISR_PRIORITY)
{
    interrupt_global_enable(0);                     // �����ж�Ƕ��
    pit_clear_flag(CCU60_CH0);

    encoder_data_quaddec = encoder_get_count(ENCODER_QUADDEC);                  // ��ȡ����������
    encoder_data_dir = encoder_get_count(ENCODER_DIR);                          // ��ȡ����������

    encoder_clear_count(ENCODER_QUADDEC);                                       // ��ձ���������
    encoder_clear_count(ENCODER_DIR);                                           // ��ձ���������
}

#pragma section all restore
// **************************** �������� ****************************
// *************************** ���̳�������˵�� ***************************
// ��������ʱ�밴�������������б���
// ����1������û������
//      �鿴�������ִ򿪵��Ƿ�����ȷ�Ĵ��ڣ����򿪵� COM ���Ƿ��Ӧ���ǵ������������� USB-TTL ģ��� COM ��
//      �����ʹ����ɿƼ� Ӣ����TriCore �������������ӣ���ô������������Ƿ��ɶ��������İ崮�������Ƿ��Ѿ����ӣ��������߲鿴���İ�ԭ��ͼ�����ҵ�
//      �����ʹ�� USB-TTL ģ�����ӣ���ô��������Ƿ������Ƿ��ɶ���ģ�� TX �Ƿ����ӵĺ��İ�� RX��ģ�� RX �Ƿ����ӵĺ��İ�� TX
// ����2��������������
//      �鿴�����������õĲ������Ƿ����������һ�£������� zf_common_debug.h �ļ��� DEBUG_UART_BAUDRATE �궨��Ϊ debug uart ʹ�õĴ��ڲ�����
// ����3����ֵһֱ������һ��ת
//      ��Ҫ�ѷ����������������������ģʽ�Ľӿ���
// ����4����ֵ�������ת���仯
//      ���ʹ��������ԣ��������Ҫ�õ�ع���
//      ���������Ƿ��������ģ����Ƿ��ɶ����������Ƿ������ˣ��Ƿ�Ӵ���
