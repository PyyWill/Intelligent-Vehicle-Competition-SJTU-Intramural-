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
* �ļ�����          zf_device_camera
* ��˾����          �ɶ���ɿƼ����޹�˾
* �汾��Ϣ          �鿴 libraries/doc �ļ����� version �ļ� �汾˵��
* ��������          ADS v1.9.20
* ����ƽ̨          TC387QP
* ��������          https://seekfree.taobao.com/
*
* �޸ļ�¼
* ����              ����                ��ע
* 2022-11-04       pudding           first version
* 2023-04-25       pudding           ��������ע��˵��
********************************************************************************************************************/

#include "zf_common_debug.h"
#include "zf_common_interrupt.h"
#include "zf_driver_gpio.h"
#include "zf_driver_dma.h"
#include "zf_driver_exti.h"
#include "zf_device_mt9v03x.h"
#include "zf_device_ov7725.h"
#include "zf_device_scc8660.h"
#include "zf_device_camera.h"


fifo_struct display_receiver_fifo;                                           // ��������ͷ��������fifo�ṹ��
uint8 display_receiver_buffer[CAMERA_RECEIVER_BUFFER_SIZE];                  // ��������ͷ�������ݻ�����
uint8 display_send_image_frame_header[4] = {0x00, 0xFF, 0x01, 0x01};         // ��������ͷ���ݷ��͵���λ����֡ͷ

//-------------------------------------------------------------------------------------------------------------------
// @brief       ����ͷͼ��������λ���鿴ͼ��
// @param       uartn           ʹ�õĴ��ں�
// @param       *image_addr     ��Ҫ���͵�ͼ���ַ
// @param       image_size      ͼ��Ĵ�С
// @return      void
// Sample usage:                camera_send_image(DEBUG_UART_INDEX, &mt9v03x_image[0][0], MT9V03X_IMAGE_SIZE);
//-------------------------------------------------------------------------------------------------------------------
void display_send_image (uart_index_enum uartn, const uint8 *image_addr, uint32 image_size)
{
    zf_assert(NULL != image_addr);
    // ��������
    uart_write_buffer(uartn, display_send_image_frame_header, 4);

    // ����ͼ��
    uart_write_buffer(uartn, (uint8 *)image_addr, image_size);
}

//-------------------------------------------------------------------------------------------------------------------
// �������     ����ͷ���� FIFO ��ʼ��
// ����˵��     void
// ���ز���     void
// ʹ��ʾ��     camera_fifo_init();
// ��ע��Ϣ
//-------------------------------------------------------------------------------------------------------------------
void display_fifo_init (void)
{
    fifo_init(&display_receiver_fifo, FIFO_DATA_8BIT, display_receiver_buffer, CAMERA_RECEIVER_BUFFER_SIZE);
}


//-------------------------------------------------------------------------------------------------------------------
// @brief       ����ͷ�ɼ���ʼ��
// @param       image_size      ͼ��Ĵ�С
// @param       data_addr       ������Դ�����ַ
// @param       buffer_addr     ͼ�񻺳�����ַ
// @return      void
// Sample usage:                camera_init();
//-------------------------------------------------------------------------------------------------------------------
uint8 gray_scale_init (void)
{
    uint8 num;
    uint8 link_list_num;
    uint8 *source_addr = &display_receiver_buffer[0];
    uint8 *destination_addr = &mt9v03x_image[0][0];
    uint32 image_size = MT9V03X_W*MT9V03X_H;
    switch(camera_type)
    {
        case CAMERA_GRAYSCALE:                                                  // �����
            for(num = 0; num < 8; num ++)
            {
                gpio_init((gpio_pin_enum)(MT9V03X_DATA_PIN + num), GPI, GPIO_LOW, GPI_FLOATING_IN);
            }
            link_list_num = dma_init(MT9V03X_DMA_CH,
                                     source_addr,
                                     destination_addr,
                                     MT9V03X_PCLK_PIN,
                                     EXTI_TRIGGER_RISING,
                                     image_size);                               // �����Ƶ��300M �����ڶ�������������ΪFALLING

            exti_init(MT9V03X_VSYNC_PIN, EXTI_TRIGGER_FALLING);                 // ��ʼ�����жϣ�������Ϊ�½��ش����ж�
            break;
        default:
            break;
    }
    return link_list_num;
}
