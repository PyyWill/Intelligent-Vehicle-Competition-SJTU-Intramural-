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
* �ļ�����          zf_driver_adc
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

#include "Evadc/Adc/IfxEvadc_Adc.h"
#include "zf_common_debug.h"
#include "zf_driver_adc.h"

#define ADC_SAMPLE_FREQUENCY	10000000 // ���10Mhz

static uint8 adc_resolution[150];

//-------------------------------------------------------------------------------------------------------------------
// �������     ADC ת��һ��
// ����˵��     ch              ѡ�� ADC ͨ�� (��� zf_driver_adc.h ��ö�� adc_channel_enum ����)
// ���ز���     uint16          ת���� ADC ֵ
// ʹ��ʾ��     adc_convert(ADC1_CH0_A0);
// ��ע��Ϣ
//-------------------------------------------------------------------------------------------------------------------
uint16 adc_convert (adc_channel_enum vadc_chn)
{
    Ifx_EVADC_G_RES result;
    uint8 temp;
    do
    {
        result = IfxEvadc_getResult(&MODULE_EVADC.G[(vadc_chn / 16)], vadc_chn%16);
    } while (!result.B.VF);

    temp = 4 - (adc_resolution[vadc_chn] * 2);
    return((result.U&0x0fff)>>temp);
}

//-------------------------------------------------------------------------------------------------------------------
// �������     ADC ��ֵ�˲�ת��
// ����˵��     ch              ѡ�� ADC ͨ�� (��� zf_driver_adc.h ��ö�� adc_channel_enum ����)
// ����˵��     count           ��ֵ�˲�����
// ���ز���     uint16          ת���� ADC ֵ
// ʹ��ʾ��     adc_mean_filter_convert(ADC1_CH0_A0, 5);                        // �ɼ�5�� Ȼ�󷵻�ƽ��ֵ
// ��ע��Ϣ
//-------------------------------------------------------------------------------------------------------------------
uint16 adc_mean_filter_convert (adc_channel_enum vadc_chn, uint8 count)
{
    uint8 i;
    uint32 sum;

    zf_assert(count);// ���Դ�������Ϊ0

    sum = 0;
    for(i=0; i<count; i++)
    {
        sum += adc_convert(vadc_chn);
    }

    sum = sum/count;

    return (uint16)sum;
}

//-------------------------------------------------------------------------------------------------------------------
//-------------------------------------------------------------------------------------------------------------------
// �������     ADC ��ʼ��
// ����˵��     ch              ѡ�� ADC ͨ�� (��� zf_driver_adc.h ��ö�� adc_channel_enum ����)
// ����˵��     resolution      ѡ��ѡ��ͨ���ֱ���(���ͬһ�� ADC ģ���ʼ��ʱ�����˲�ͬ�ķֱ��� �����һ����ʼ���ķֱ�����Ч)
// ���ز���     void
// ʹ��ʾ��     adc_init(ADC1_CH0_A0, ADC_8BIT);                                // ��ʼ�� A0 Ϊ ADC1 �� channel0 ���빦�� �ֱ���Ϊ8λ
// ��ע��Ϣ
//-------------------------------------------------------------------------------------------------------------------
void adc_init (adc_channel_enum vadc_chn, adc_resolution_enum resolution)
{
    static uint8 mudule_init_flag = 0;
    static uint8 group_init_flag[9];
    IfxEvadc_Adc evadc;
    IfxEvadc_Adc_Group adcGroup;
    IfxEvadc_Adc_Config adcConfig;
    IfxEvadc_Adc_GroupConfig adcGroupConfig;

    IfxEvadc_Adc_initModuleConfig(&adcConfig, &MODULE_EVADC);
    if(!mudule_init_flag)
    {
        mudule_init_flag = 1;
        IfxEvadc_Adc_initModule(&evadc, &adcConfig);
    }
    else
    {
        evadc.evadc = adcConfig.evadc;
    }

    IfxEvadc_Adc_initGroupConfig(&adcGroupConfig, &evadc);

    adcGroupConfig.groupId = (IfxEvadc_GroupId)(vadc_chn / 16);
    adcGroupConfig.master  = adcGroupConfig.groupId;
    adcGroupConfig.arbiter.requestSlotQueue0Enabled  = TRUE;
    adcGroupConfig.queueRequest[0].triggerConfig.gatingMode = IfxEvadc_GatingMode_always;
    adcGroupConfig.inputClass[0].sampleTime = 1.0f/ADC_SAMPLE_FREQUENCY;
    adcGroupConfig.inputClass[1].sampleTime = 1.0f/ADC_SAMPLE_FREQUENCY;

    if(!group_init_flag[(vadc_chn / 16)])
    {
        group_init_flag[(vadc_chn / 16)] = 1;
        IfxEvadc_Adc_initGroup(&adcGroup, &adcGroupConfig);
    }
    else
    {
        adcGroup.groupId = adcGroupConfig.groupId;
        adcGroup.module = *adcGroupConfig.module;
        adcGroup.group = &adcGroupConfig.module->evadc->G[adcGroup.groupId];
    }

    IfxEvadc_Adc_Channel       adcChannel;
    IfxEvadc_Adc_ChannelConfig adcChannelConfig;

    IfxEvadc_Adc_initChannelConfig(&adcChannelConfig, &adcGroup);
    adcChannelConfig.channelId         = (IfxEvadc_ChannelId)(vadc_chn%16);
    adcChannelConfig.resultRegister    = (IfxEvadc_ChannelResult)(vadc_chn%16);
    IfxEvadc_Adc_initChannel(&adcChannel, &adcChannelConfig);
    IfxEvadc_Adc_addToQueue(&adcChannel, IfxEvadc_RequestSource_queue0, IFXEVADC_QUEUE_REFILL);
    IfxEvadc_Adc_startQueue(&adcGroup, IfxEvadc_RequestSource_queue0);

    adc_resolution[vadc_chn] = resolution;
}

