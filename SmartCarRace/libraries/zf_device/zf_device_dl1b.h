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
* �ļ�����          zf_device_dl1b
* ��˾����          �ɶ���ɿƼ����޹�˾
* �汾��Ϣ          �鿴 libraries/doc �ļ����� version �ļ� �汾˵��
* ��������          ADS v1.9.20
* ����ƽ̨          TC387QP
* ��������          https://seekfree.taobao.com/
*
* �޸ļ�¼
* ����              ����                ��ע
* 2022-11-04       pudding           first version
********************************************************************************************************************/
/*********************************************************************************************************************
* ���߶��壺
*                   ------------------------------------
*                   ģ��ܽ�            ��Ƭ���ܽ�
*                   SCL                 �鿴 zf_device_dl1b.h �� DL1B_SCL_PIN  �궨��
*                   SDA                 �鿴 zf_device_dl1b.h �� DL1B_SDA_PIN  �궨��
*                   XS                  �鿴 zf_device_dl1b.h �� DL1B_XS_PIN  �궨��
*                   VCC                 5V ��Դ
*                   GND                 ��Դ��
*                   ------------------------------------
*                   ------------------------------------
********************************************************************************************************************/

#ifndef _ZF_DEVICE_DL1B_H_
#define _ZF_DEVICE_DL1B_H_

#include "zf_common_typedef.h"

// ��Ҫע����� DL1B ���֧�� 400KHz �� IIC ͨ������
// ��Ҫע����� DL1B ���֧�� 400KHz �� IIC ͨ������
// ��Ҫע����� DL1B ���֧�� 400KHz �� IIC ͨ������

#define DL1B_USE_SOFT_IIC           ( 1 )                                       // Ĭ��ʹ������ IIC ��ʽ���� ����ʹ������ IIC ��ʽ
#if DL1B_USE_SOFT_IIC                                                           // ������ ��ɫ�����Ĳ�����ȷ�� ��ɫ�ҵľ���û���õ�
//====================================================���� IIC ����====================================================
#define DL1B_SOFT_IIC_DELAY         ( 10 )                                      // ���� IIC ��ʱ����ʱ���� ��ֵԽС IIC ͨ������Խ��
#define DL1B_SCL_PIN                ( P33_4 )                                      // ���� IIC SCL ���� ���� DL1B �� SCL ����
#define DL1B_SDA_PIN                ( P33_5 )                                      // ���� IIC SDA ���� ���� DL1B �� SDA ����
//====================================================���� IIC ����====================================================
#else
#error "�ݲ�֧��Ӳ��IICͨѶ"
#endif

#define DL1B_XS_PIN                 ( P20_10 )

#define DL1B_INT_ENABLE             ( 0 )                                       // �Ƿ����� INT ���� ��������Զ���������
#if DL1B_INT_ENABLE
#define DL1B_INT_PIN                ( ERU_CH1_REQ10_P14_3 )
#endif

#define DL1B_TIMEOUT_COUNT          ( 1000 )                                    // DL1B ��ʱ����

//================================================���� DL1B �ڲ���ַ================================================

#define DL1B_DEV_ADDR                                           ( 0x52 >> 1 )   // 0b0101001

#define DL1B_I2C_SLAVE__DEVICE_ADDRESS                          ( 0x0001 )
#define DL1B_GPIO__TIO_HV_STATUS                                ( 0x0031 )
#define DL1B_SYSTEM__INTERRUPT_CLEAR                            ( 0x0086 )
#define DL1B_RESULT__RANGE_STATUS                               ( 0x0089 )
#define DL1B_RESULT__FINAL_CROSSTALK_CORRECTED_RANGE_MM_SD0     ( 0x0096 )
#define DL1B_FIRMWARE__SYSTEM_STATUS                            ( 0x00E5 )

//================================================���� DL1B �ڲ���ַ================================================

extern uint8 dl1b_finsh_flag;
extern uint16 dl1b_distance_mm;

void   dl1b_get_distance (void);

void   dl1b_int_handler  (void);
uint8  dl1b_init         (void);

#endif
