#include "zf_common_headfile.h"
#include "imagprocess.h"

uint8 binary_imag[MT9V03X_H][MT9V03X_W]; // 定义二值化后的图像

//-------------------------------------------------------------------------------------------------------------------
// 函数简介     自适应二值化
// 参数说明     img_data 原图像
// 参数说明     output_data 二值化后图像
// 参数说明     width 图像宽度
// 参数说明     height 图像高度
// 参数说明     block 图像大小（正方形边长）
// 参数说明     clip_value 避免过分割阈值
// 返回参数     void
// 使用示例     imag_adaptiveThreshold((const uint8 *)mt9v03x_image, binary_imag, MT9V03X_W, MT9V03X_H, 7, 4);
// 备注信息
//-------------------------------------------------------------------------------------------------------------------
void imag_adaptiveThreshold(uint8_t* img_data, uint8_t* output_data, int width, int height, int block, uint8_t clip_value)
{
  if(block % 2 == 0) return; // block必须为奇数
  int half_block = block / 2;
  uint8_t* img_data_copy
  int grey_acc[MT9V03X_H][MT9V03X_W]; // 左上方灰度累计值
  for(int y = 0; y < height; y++)
  {
    for(int x = 0; x < width; x++)
    {
        int up_acc, left_acc, leftup_acc; // 上方、左侧点、左上方点的灰度累计值
        if(y - 1 < 0) up_acc = 0;
        else up_acc = grey_acc[y - 1][x];
        if(x - 1 < 0) left_acc = 0;
        else left_acc = grey_acc[y][x - 1];
        if(x - 1 < 0 || y - 1 < 0)leftup_acc = 0;
        else leftup_acc = grey_acc[y - 1][x - 1];
        grey_acc[y][x] = img_data[x+y*width] + up_acc + left_acc - leftup_acc;
    }
  }
  for(int y = half_block + 1; y <= height-1-half_block; y++)
  {
    for(int x = half_block + 1; x <= width-1-half_block; x++)
    {
      // 计算局部阈值
      int thres = 0;
      thres = grey_acc[y + half_block][x + half_block] - grey_acc[y - half_block - 1][x + half_block] - grey_acc[y + half_block][x - half_block - 1] + grey_acc[y - half_block - 1][x - half_block - 1];
      thres = thres / (block * block) - clip_value;
      // 进行二值化
      output_data[x+y*width] = img_data[x+y*width]>thres ? 255 : 0;
    }
  }
}

//-------------------------------------------------------------------------------------------------------------------
// 函数简介     图像处理（当前进度：全图二值化）
// 参数说明     void
// 返回参数     void
// 使用示例     imag_proc();
// 备注信息
//-------------------------------------------------------------------------------------------------------------------
void imag_proc()
{
    // 自适应二值化，每个区块的大小为7 * 7，避免强行分割的阈值为4
    imag_adaptiveThreshold((const uint8 *)mt9v03x_image, binary_imag, MT9V03X_W, MT9V03X_H, 7, 4);
}
