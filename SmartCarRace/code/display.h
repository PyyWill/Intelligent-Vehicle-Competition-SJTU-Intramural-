#ifndef __DISPLAY_H
#define __DISPLAY_H

#include "zf_common_headfile.h"

//=================================================摄像头公共库 基本配置================================================
#define CAMERA_RECEIVER_BUFFER_SIZE     (8)         // 定义摄像头接收数据缓冲区大小
#define MT9V03X_H                       (240)
#define MT9V03X_W                       (376)
extern uint8 mt9v03x_image[MT9V03X_H][MT9V03X_W];   // 声明摄像头数据存储矩阵
extern fifo_struct display_receiver_fifo;            // 声明摄像头接收数据fifo结构体
extern uint8 display_send_image_frame_header[4];     // 声明摄像头数据发送到上位机的帧头
//=================================================摄像头公共库 基本配置================================================

//=================================================摄像头公共库 基础函数================================================
void  diplay_send_image                 (uart_index_enum uartn, const uint8 *image_addr, uint32 image_size);    // 摄像头图像发送至上位机查看图像
void  display_fifo_init                  (void);                                                                 // 摄像头串口 FIFO 初始化
uint8 gray_scale_init                   (void);       // 摄像头采集初始化
//=================================================摄像头公共库 基础函数================================================

#endif
