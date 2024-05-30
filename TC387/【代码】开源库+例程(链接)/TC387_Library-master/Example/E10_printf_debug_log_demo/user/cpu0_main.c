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


// *************************** ���̲���˵�� ***************************
// 1.���İ���¼��ɱ����̣�����ʹ�ú��İ���������������� USB-TTL ģ�飬�ڶϵ�������������
// 2.���������������� USB-TTL ģ�����ӵ��ԣ�����ϵ�
// 3.������ʹ�ô������ִ򿪶�Ӧ�Ĵ��ڣ����ڲ�����Ϊ zf_common_debug.h �ļ��� DEBUG_UART_BAUDRATE �궨�� Ĭ�� 115200�����İ尴�¸�λ����
// 4.�����ڴ��������Ͽ������´�����Ϣ��
//      Log message from ../USER/src/main.c line 64 :"01_printf_debug_log_demo zf_log with false.".
//      Time: xxx s.
//      ...
//      Assert error in ../USER/src/main.c line 73.
// 5.�����Ҫʹ�� debug uart ���յ��Է��͵����� ��Ҫ�� zf_common_debug.h �п��� DEBUG_UART_USE_INTERRUPT �궨��
// 6.���� DEBUG_UART_USE_INTERRUPT �궨��������¼����
// 7.��λ��Ƭ�����ڴ������ַ������� ���ɿ������´�����Ϣ��
//      Log message from ../USER/src/main.c line 82 :"...".
// �������������˵�����ز��� ����ձ��ļ����·� ���̳�������˵�� �����Ų�

// **************************** �������� ****************************

#if DEBUG_UART_USE_INTERRUPT
uint8   debug_uart_data_buffer[64];
uint8   debug_uart_data_len;
#endif
uint16  assert_count = 0;

int core0_main(void)
{
    clock_init();                   // ��ȡʱ��Ƶ��<��ر���>
    debug_init();                   // ��ʼ��Ĭ�ϵ��Դ���
    // �˴���д�û����� ���������ʼ�������

	zf_log(1, "01_printf_debug_log_demo zf_log with true.");                    // zf_log �궨�庯��ʹ�÷��� ��һ������Ϊ���������
    zf_log(0, "01_printf_debug_log_demo zf_log with false.");                   // zf_log �궨�庯��ʹ�÷��� ��һ������Ϊ������� log

    debug_assert_disable();                                                     // ��ֹ���Թ���
    zf_assert(0);                                                               // ���� �����ڶ��Թ����ѽ�ֹ ���Բ�����Ч
    debug_assert_enable();                                                      // ʹ�ܶ��Թ���


    // �˴���д�û����� ���������ʼ�������
    cpu_wait_event_ready();         // �ȴ����к��ĳ�ʼ�����
	while (TRUE)
	{
        // �˴���д��Ҫѭ��ִ�еĴ���

	    zf_assert(20000 > assert_count ++);                                     // �ȴ� 20 ���Ӻ�������
	            system_delay_ms(1);                                                     // ������ʱ

	            if(assert_count % 1000 == 0)
	            {
	                printf("Time: %d s.\r\n", assert_count / 1000);                     // printf ʹ�÷�����������
	            }
	    #if DEBUG_UART_USE_INTERRUPT                                                    // ��������� debug uart �����ж�
	            debug_uart_data_len = (uint8)debug_read_ring_buffer(debug_uart_data_buffer, sizeof(debug_uart_data_buffer));   // ��ȡ����
	            if(debug_uart_data_len != 0)                                            // �ж��Ƿ��յ� debug uart ����
	            {
	                printf("\r\n");                                                     // �������
	                zf_log(0, (char *)debug_uart_data_buffer);                          // ͨ�� zf_log ����Ϣ�ط�
	                memset(debug_uart_data_buffer, 0, debug_uart_data_len);             // �����Ϣ��
	            }
	    #endif


        // �˴���д��Ҫѭ��ִ�еĴ���
	}
}

#pragma section all restore
// **************************** �������� ****************************

// ��������ʱ�밴�������������б���
// ����1������û������
//      �鿴�������ִ򿪵��Ƿ�����ȷ�Ĵ��ڣ����򿪵� COM ���Ƿ��Ӧ���ǵ������������� USB-TTL ģ��� COM ��
//      �����ʹ����ɿƼ� Ӣ����TriCore �������������ӣ���ô������������Ƿ��ɶ��������İ崮�������Ƿ��Ѿ����ӣ��������߲鿴���İ�ԭ��ͼ�����ҵ�
//      �����ʹ�� USB-TTL ģ�����ӣ���ô��������Ƿ������Ƿ��ɶ���ģ�� TX �Ƿ����ӵĺ��İ�� RX��ģ�� RX �Ƿ����ӵĺ��İ�� TX
// ����2��������������
//      �鿴�����������õĲ������Ƿ����������һ�£������� zf_common_debug.h �ļ��� DEBUG_UART_BAUDRATE �궨��Ϊ debug uart ʹ�õĴ��ڲ�����

