/*********************************************************************************************************************
* TC387 Opensourec Library ����TC387 ��Դ�⣩��һ�����ڹٷ� SDK �ӿڵĵ�������Դ��
* Copyright (c) 2022 SEEKFREE ��ɿƼ�
*
* ���ļ��� TC387 ��Դ���һ����
*
* TC387 ��Դ�� ���������
* �����Ը���������������ᷢ���� GPL��GNU General Public License���� GNUͨ�ù�������֤��������
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
* ����Դ��ʹ�� GPL3.0 ��Դ����֤Э�� ������������Ϊ���İ汾
* ��������Ӣ�İ��� libraries/doc �ļ����µ� GPL3_permission_statement.txt �ļ���
* ����֤������ libraries �ļ����� �����ļ����µ� LICENSE �ļ�
* ��ӭ��λʹ�ò����������� ���޸�����ʱ���뱣����ɿƼ��İ�Ȩ����������������
*
* �ļ�����          zf_common_clock
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

#ifndef _zf_common_clock_h_
#define _zf_common_clock_h_

#include "zf_common_typedef.h"

#define TC387QP                     (0)
#define TC387TP                     (1)

#define AURIX_MCU_NAME              (TC387QP)       // ����387�������� TC387TP ����ζ��ֻ����������   �밴��ʵ��оƬ˿ӡ�޸Ĵ˴�(����TC387TPҲ��ǿ�п������ĸ�����)

#define AURIX_MCU_FREQUENCY         (300*1000*1000) // ���õ�Ƭ��Ƶ��Ϊ300M ��ѡ���� 300M(Ĭ��Ƶ��)  240M(��Ƶ)  200M(��Ƶ)

void set_clock              (void);                 // ����ϵͳƵ��
void get_clock              (void);                 // ��ȡϵͳƵ��
void disable_Watchdog       (void);                 // �رտ��Ź�
void clock_init             (void);                 // ʱ�ӳ�ʼ��
void cpu_wait_event_ready   (void);                 // �ȴ����к��ĳ�ʼ�����


#endif