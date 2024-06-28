#ifndef __IMAGPROCESS_H
#define __IMAGPROCESS_H

#include "zf_common_headfile.h"

// 一些宏定义
#define IMAGE_BLACK 0 // 二值化的黑点
#define IMAGE_WHITE 255 // 二值化的白点，用在二值化图像以及扫线那里
#define GrayScale 256 // 灰度值范围，用于定义数组大小
#define BORDER_BIAS 1 // 扫线误差

//点坐标的结构体
typedef struct
{
    int X;
    int Y;
} Point;

//================================================声明全局变量================================================
extern uint8 binary_imag[MT9V03X_H][MT9V03X_W]; // 二值化后的图像
//================================================声明全局变量================================================

//====================================================图像处理基础函数====================================================
uint8 otsuThreshold(uint8 *image, uint16 width, uint16 height); // 计算二值化阈值: 大津法
void imag_binary(); // 全图二值化
void imag_getline(int *LeftLine, int *CentreLine, int *RightLine ,char path); // 二值化后寻找左右边线
void imag_GetDownInflection(int startline, int endline, Point *InflectionL, Point *InflectionR); // 根据左右边界线来得到下拐点（十字、三岔、环岛的判断会用上）
void image_Filter(); // 图像滤波
void imag_drawline(); // 赛道边线和中线绘制
//====================================================图像处理基础函数====================================================

//====================================================图像处理====================================================
void imag_process(); // 图像基础处理：二值化、找边线、找下面两个拐点并显示在屏幕上
//====================================================图像处理====================================================

#endif
