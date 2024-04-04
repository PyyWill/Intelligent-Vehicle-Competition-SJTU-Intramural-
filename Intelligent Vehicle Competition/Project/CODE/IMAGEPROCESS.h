#ifndef __IMAGEPROCESS_H
#define __IMAGEPROCESS_H

#include "headfile.h"

#define IMG_BLACK     0X00      //0x00ÊÇºÚ
#define IMG_WHITE     0Xff      //0xffÎª°×

void Image_Binarization(int threshold);
int my_adapt_threshold(uint8 *image, uint16 col, uint16 row);
void Longest_White_Column(void);
void Show_Boundry(void);

void Left_Add_Line(int x1,int y1,int x2,int y2);
void Right_Add_Line(int x1,int y1,int x2,int y2);
void Lengthen_Left_Boundry(int start,int end);
void Lengthen_Right_Boundry(int start,int end);

void Straight_Detect(void);
void Find_Down_Point(int start,int end);
void Find_Up_Point(int start,int end);
void Cross_Detect(void);


void Junction_Detect(void);
void Find_Ding_Point(void);
void Find_Down_Point_Junction(int start,int end);

int Continuity_Change_Left(int start,int end);
int Continuity_Change_Right(int start,int end);
int Monotonicity_Change_Left(int start,int end);
int Monotonicity_Change_Right(int start,int end);
int Find_Left_Down_Point(int start,int end);
int Find_Right_Down_Point(int start,int end);
void K_Add_Boundry_Right(float k,int startX,int startY,int endY);
void Island_Detect(void);
#endif

