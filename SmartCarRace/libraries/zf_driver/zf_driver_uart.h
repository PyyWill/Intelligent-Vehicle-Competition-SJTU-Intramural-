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
* �ļ�����          zf_driver_uart
* ��˾����          �ɶ���ɿƼ����޹�˾
* �汾��Ϣ          �鿴 libraries/doc �ļ����� version �ļ� �汾˵��
* ��������          ADS v1.9.20
* ����ƽ̨          TC387QP
* ��������          https://seekfree.taobao.com/
*
* �޸ļ�¼
* ����              ����                ��ע
* 2022-11-04       pudding            first version
* 2023-10-10       pudding            ��������4-����11
********************************************************************************************************************/

#ifndef _zf_driver_uart_h_
#define _zf_driver_uart_h_

#include "ifxAsclin_Asc.h"
#include "zf_common_typedef.h"


typedef enum            // ö�ٴ������� ��ö�ٶ��岻�����û��޸�
{
    UART0_TX_P14_0,     // ����0 �������ſ�ѡ��Χ
    UART0_TX_P14_1,
    UART0_TX_P15_2,
    UART0_TX_P15_3,

    UART1_TX_P02_2,     // ����1 �������ſ�ѡ��Χ
    UART1_TX_P11_12,
    UART1_TX_P15_0,
    UART1_TX_P15_1,
    UART1_TX_P15_4,
    UART1_TX_P15_5,
    UART1_TX_P20_10,
    UART1_TX_P33_12,
    UART1_TX_P33_13,

    UART2_TX_P02_0,     // ����2 �������ſ�ѡ��Χ
    UART2_TX_P10_5,
    UART2_TX_P14_2,
    UART2_TX_P14_3,
    UART2_TX_P33_8,
    UART2_TX_P33_9,

    UART3_TX_P00_0,     // ����3 �������ſ�ѡ��Χ
    UART3_TX_P00_1,
    UART3_TX_P15_6,
    UART3_TX_P15_7,
    UART3_TX_P20_0,
    UART3_TX_P20_3,
    UART3_TX_P21_7,
    UART4_TX_P00_9,     // ����4 �������ſ�ѡ��Χ

    UART5_TX_P00_7,     // ����5 �������ſ�ѡ��Χ
    UART5_TX_P22_2,

    UART6_TX_P22_0,     // ����6 �������ſ�ѡ��Χ

    UART8_TX_P33_7,     // ����8 �������ſ�ѡ��Χ

    UART9_TX_P20_7,     // ����9 �������ſ�ѡ��Χ

    UART10_TX_P00_8,    // ����10 �������ſ�ѡ��Χ
    UART10_TX_P13_0,

    UART11_TX_P21_5,    // ����11 �������ſ�ѡ��Χ
}uart_tx_pin_enum;


typedef enum            // ö�ٴ������� ��ö�ٶ��岻�����û��޸�
{

    UART0_RX_P14_1,     // ����0 �������ſ�ѡ��Χ
    UART0_RX_P15_3,

    UART1_RX_P02_3,     // ����1 �������ſ�ѡ��Χ
    UART1_RX_P11_10,
    UART1_RX_P15_1,
    UART1_RX_P15_5,
    UART1_RX_P20_9,
    UART1_RX_P33_13,

    UART2_RX_P02_0,     // ����2 �������ſ�ѡ��Χ
    UART2_RX_P02_1,
    UART2_RX_P10_6,
    UART2_RX_P14_3,
    UART2_RX_P33_8,

    UART3_RX_P00_1,     // ����3 �������ſ�ѡ��Χ
    UART3_RX_P15_7,
    UART3_RX_P20_3,
    UART3_RX_P21_6,
    UART4_RX_P00_12,    // ����4 �������ſ�ѡ��Χ

    UART5_RX_P00_6,     // ����5 �������ſ�ѡ��Χ
    UART5_RX_P22_3,
    UART5_RX_P33_4,

    UART6_RX_P22_0,     // ����6 �������ſ�ѡ��Χ
    UART6_RX_P23_1,

    UART8_RX_P33_6,     // ����8 �������ſ�ѡ��Χ

    UART9_RX_P20_6,     // ����9 �������ſ�ѡ��Χ

    UART10_RX_P00_4,    // ����10 �������ſ�ѡ��Χ
    UART10_RX_P00_8,
    UART10_RX_P13_0,
    UART10_RX_P13_1,

    UART11_RX_P21_2,    // ����11 �������ſ�ѡ��Χ
}uart_rx_pin_enum;


typedef enum            // ö�ٴ��ں� ��ö�ٶ��岻�����û��޸�
{
    UART_0,
    UART_1,
    UART_2,
    UART_3,
    UART_4,
    UART_5,
    UART_6,
    UART_7,             // ���İ�û����������7������ �˴�������ռλ
    UART_8,
    UART_9,
    UART_10,
    UART_11,
}uart_index_enum;

//��������handle����
extern IfxAsclin_Asc uart0_handle;
extern IfxAsclin_Asc uart1_handle;
extern IfxAsclin_Asc uart2_handle;
extern IfxAsclin_Asc uart3_handle;
extern IfxAsclin_Asc uart4_handle;
extern IfxAsclin_Asc uart5_handle;
extern IfxAsclin_Asc uart6_handle;
extern IfxAsclin_Asc uart8_handle;
extern IfxAsclin_Asc uart9_handle;
extern IfxAsclin_Asc uart10_handle;
extern IfxAsclin_Asc uart11_handle;

//====================================================���� ��������====================================================
void    uart_write_byte                     (uart_index_enum uartn, const uint8 dat);
void    uart_write_buffer                   (uart_index_enum uartn, const uint8 *buff, uint32 len);
void    uart_write_string                   (uart_index_enum uartn, const char *str);

uint8   uart_read_byte                      (uart_index_enum uartn);
uint8   uart_query_byte                     (uart_index_enum uartn, uint8 *dat);

void    uart_tx_interrupt                   (uart_index_enum uartn, uint32 status);
void    uart_rx_interrupt                   (uart_index_enum uartn, uint32 status);

void    uart_sbus_init                      (uart_index_enum uartn, uint32 baud, uart_tx_pin_enum tx_pin, uart_rx_pin_enum rx_pin);
void    uart_init                           (uart_index_enum uartn, uint32 baud, uart_tx_pin_enum tx_pin, uart_rx_pin_enum rx_pin);
//====================================================���� ��������====================================================

//=================================================���ݾɰ汾��Դ��ӿ�����=================================================
#ifdef  COMPATIBLE_WITH_OLDER_VERSIONS
#define uart_putchar(uart_n, dat)           (uart_write_byte((uart_n), (dat)))
#define uart_putbuff(uart_n, buff, len)     (uart_write_buffer((uart_n), (buff), (len)))
#define uart_putstr(uart_n, str)            (uart_write_string((uart_n), (str)))

#define uart_getchar(uart_n, dat)           (*(dat) = uart_read_byte((uart_n)))
#define uart_query(uart_n, dat)             (uart_query_byte((uart_n), (dat)))

#define uart_tx_irq(uart_n, status)         (uart_tx_interrupt((uart_n), (status)))
#define uart_rx_irq(uart_n, status)         (uart_rx_interrupt((uart_n), (status)))
#endif
//=================================================���ݾɰ汾��Դ��ӿ�����=================================================

#endif
