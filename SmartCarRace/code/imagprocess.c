#include "zf_common_headfile.h"
#include "imagprocess.h"

uint8 binary_imag[MT9V03X_H][MT9V03X_W]; // �����ֵ�����ͼ��

//-------------------------------------------------------------------------------------------------------------------
// �������     ����Ӧ��ֵ��
// ����˵��     img_data ԭͼ��
// ����˵��     output_data ��ֵ����ͼ��
// ����˵��     width ͼ����
// ����˵��     height ͼ��߶�
// ����˵��     block ͼ���С�������α߳���
// ����˵��     clip_value ������ָ���ֵ
// ���ز���     void
// ʹ��ʾ��     imag_adaptiveThreshold((const uint8 *)mt9v03x_image, binary_imag, MT9V03X_W, MT9V03X_H, 7, 4);
// ��ע��Ϣ
//-------------------------------------------------------------------------------------------------------------------
void imag_adaptiveThreshold(uint8_t* img_data, uint8_t* output_data, int width, int height, int block, uint8_t clip_value)
{
  if(block % 2 == 0) return; // block����Ϊ����
  int half_block = block / 2;
  uint8_t* img_data_copy
  int grey_acc[MT9V03X_H][MT9V03X_W]; // ���Ϸ��Ҷ��ۼ�ֵ
  for(int y = 0; y < height; y++)
  {
    for(int x = 0; x < width; x++)
    {
        int up_acc, left_acc, leftup_acc; // �Ϸ������㡢���Ϸ���ĻҶ��ۼ�ֵ
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
      // ����ֲ���ֵ
      int thres = 0;
      thres = grey_acc[y + half_block][x + half_block] - grey_acc[y - half_block - 1][x + half_block] - grey_acc[y + half_block][x - half_block - 1] + grey_acc[y - half_block - 1][x - half_block - 1];
      thres = thres / (block * block) - clip_value;
      // ���ж�ֵ��
      output_data[x+y*width] = img_data[x+y*width]>thres ? 255 : 0;
    }
  }
}

//-------------------------------------------------------------------------------------------------------------------
// �������     ͼ������ǰ���ȣ�ȫͼ��ֵ����
// ����˵��     void
// ���ز���     void
// ʹ��ʾ��     imag_proc();
// ��ע��Ϣ
//-------------------------------------------------------------------------------------------------------------------
void imag_proc()
{
    // ����Ӧ��ֵ����ÿ������Ĵ�СΪ7 * 7������ǿ�зָ����ֵΪ4
    imag_adaptiveThreshold((const uint8 *)mt9v03x_image, binary_imag, MT9V03X_W, MT9V03X_H, 7, 4);
}
