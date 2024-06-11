#ifndef __DISPLAY_H
#define __DISPLAY_H

#include "zf_common_headfile.h"

//=================================================����ͷ������ ��������================================================
#define CAMERA_RECEIVER_BUFFER_SIZE     (8)         // ��������ͷ�������ݻ�������С
#define MT9V03X_H                       (240)
#define MT9V03X_W                       (376)
extern uint8 mt9v03x_image[MT9V03X_H][MT9V03X_W];   // ��������ͷ���ݴ洢����
extern fifo_struct display_receiver_fifo;            // ��������ͷ��������fifo�ṹ��
extern uint8 display_send_image_frame_header[4];     // ��������ͷ���ݷ��͵���λ����֡ͷ
//=================================================����ͷ������ ��������================================================

//=================================================����ͷ������ ��������================================================
void  diplay_send_image                 (uart_index_enum uartn, const uint8 *image_addr, uint32 image_size);    // ����ͷͼ��������λ���鿴ͼ��
void  display_fifo_init                  (void);                                                                 // ����ͷ���� FIFO ��ʼ��
uint8 gray_scale_init                   (void);       // ����ͷ�ɼ���ʼ��
//=================================================����ͷ������ ��������================================================

#endif
