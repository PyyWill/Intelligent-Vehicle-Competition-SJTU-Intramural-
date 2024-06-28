#include "zf_common_headfile.h"
#include "imagprocess.h"

uint8 binary_imag[MT9V03X_H][MT9V03X_W];                                                // �����ֵ�����ͼ��
uint8 Mid = MT9V03X_W / 2;                                                              // ��ʼ��ɨ�ߵ��е�Ϊͼ���е�
uint8 Lost_Row = 0;                                                                     // ���߶�ʧ��������(ɨ�ߵ�������)
uint8 LostNum_LeftLine = 0,LostNum_RightLine = 0;                                       // ��¼���ұ߽綪����
int LeftLine[MT9V03X_H] = {0}, CentreLine[MT9V03X_H] = {0}, RightLine[MT9V03X_H] = {0}; // ɨ�ߴ�������������
Point InflectionL,InflectionR;                                                          // �����¹յ�

//-------------------------------------------------------------------------------------------------------------------
// �������     ��򷨶�ֵ������
// ����˵��     *image ��ͼ���ַ
// ����˵��     width:  ͼ���
// ����˵��     height��ͼ���
// ���ز���     void
// ʹ��ʾ��     image_threshold = otsuThreshold(mt9v03x_image[0],MT9V03X_W,MT9V03X_H);//ʹ�ô�򷨵õ���ֵ����ֵ
// ��ע��Ϣ
//-------------------------------------------------------------------------------------------------------------------
uint8 otsuThreshold(uint8 *image, uint16 width, uint16 height)
{
    int pixelCount[GrayScale] = {0}; // ÿ���Ҷ�ֵ��ռ���ظ���
    float pixelPro[GrayScale] = {0}; // ÿ���Ҷ�ֵ��ռ�����ر���
    int Sumpix = width * height; // �����ص�
    uint8 threshold = 0;
    uint8* data = image; // ָ���������ݵ�ָ��

    // ͳ�ƻҶȼ���ÿ������������ͼ���еĸ���
    for(int i = 0; i < height; i++)
    {
        for (int j = 0; j < width; j++)
        {
            pixelCount[(int)data[i * width + j]]++; // ͳ�Ƹ����Ҷ�ֵ
        }
    }
    float u = 0;
    for(int i = 0; i < GrayScale; i++)
    {
        pixelPro[i] = (float)pixelCount[i] / Sumpix; // ����ÿ������������ͼ���еı���
        u += i * pixelPro[i]; // ��ƽ���Ҷ�
    }

    float maxVariance=0.0; // �����䷽��
    float w0 = 0, avgValue0  = 0, w1 = 0, avgValue1 = 0;  // w0 �������� ��avgValue0 ����ƽ���Ҷȣ�w1 ǰ������ ��avgValue1 ǰ��ƽ���Ҷ�
    for(int i = 0; i < 256; i++) // ÿһ��ѭ������һ��������䷽����� (����for����Ϊ1��)
    {
        w0 += pixelPro[i]; // ���赱ǰ�Ҷ�iΪ��ֵ, 0~i �Ҷ�������ռ����ͼ��ı�������������
        avgValue0 += i * pixelPro[i];
        w1 = 1 - w0;
        avgValue1 = u - avgValue0;

        float variance = w0 * pow((avgValue0 / w0 - u), 2) + w1 * pow((avgValue1 / w1 - u), 2); // ��䷽��
        if(variance > maxVariance)
        {
            maxVariance = variance;
            threshold = (uint8)i;
        }
    }
    return threshold;
}

//-------------------------------------------------------------------------------------------------------------------
// �������     ͼ������ǰ���ȣ�ȫͼ��ֵ����
// ����˵��     void
// ���ز���     void
// ʹ��ʾ��     imag_binary();
// ��ע��Ϣ
//-------------------------------------------------------------------------------------------------------------------
void imag_binary()
{
    uint8 image_threshold = otsuThreshold(mt9v03x_image[0], MT9V03X_W, MT9V03X_H); // ʹ�ô�򷨵õ���ֵ����ֵ
    for(int i = 0; i < MT9V03X_H; i++)
    {
        for(int j = 0; j < MT9V03X_W; j++)
        {
            if(mt9v03x_image[i][j] <= image_threshold) // ���ж�ֵ��֮ǰֻ�ǵõ���ֵ
                binary_imag[i][j] = IMAGE_BLACK; // 0�Ǻ�ɫ  //ͼ��ԭ�㲻��
            else
                binary_imag[i][j] = IMAGE_WHITE; // 1�ǰ�ɫ  //ͼ��ԭ�㲻��
        }
    }
}

//-------------------------------------------------------------------------------------------------------------------
// �������     �Զ�ֵͼ�����ұ߽������
// ����˵��     *LeftLine ���������ָ��
// ����˵��     *CentreLine ��������ָ��
// ����˵��     *RightLine �ұ�������ָ��
// ����˵��     path ��ǰ�м�Ϊ��������ɫ��ʱ��Ĭ�����������ĸ���������������ɫ��������ȡΪ'L':����'R':����
// ���ز���     void
// ʹ��ʾ��     imag_getline(LeftLine, CentreLine, RightLine, 'L');
// ��ע��Ϣ
//-------------------------------------------------------------------------------------------------------------------
void imag_getline(int *LeftLine, int *CentreLine, int *RightLine ,char path)
{
    uint8 row,cloum; // ��,��
    uint8 flag_l = 0,flag_r = 0; // ��¼�Ƿ���flag��flag=0������
    uint8 num = 0; // ��¼����������ʧ�Ĵ���
    LostNum_LeftLine = 0;LostNum_RightLine = 0; //�Ѷ��ߵļ�����������

    // ��ʼɨ��(��������,���м�������),Ϊ��ɨ�ߵ��Ͻ���,������BORDER_BIAS��������ɨ�߷�Χ��С��ͼ���С
    for(row = MT9V03X_H - 1; row > 0; row--) //�������ϣ���������ͼ��
    {
        // �������⣬���Ȱ�pathɨ�߷���Ѱ������
        if(binary_imag[row][Mid]== IMAGE_BLACK)  // ɨ���е��Ǻ�ɫ�ģ��е��������⣩
        {
            num++;    //����������ʧ��+1
            if(path == 'L')   //Ĭ������ɨ��
            {
                // �������ɨ�ߣ�Ѱ���ұ߽��
                for(cloum = Mid; cloum - BORDER_BIAS > 0; cloum--)    //����ɨ
                {
                    if(binary_imag[row][cloum] == IMAGE_WHITE && binary_imag[row][cloum - BORDER_BIAS] == IMAGE_WHITE) // �ҵ��׵㣨��������ɨ�������ڣ���-�ף�
                    {
                        RightLine[row] = cloum;    //��¼�ұ߽�㣨�����ҵ������ұ߽�㣩
                        flag_r = 1;               //flag���޶��߱��
                        break;
                    }
                }
                //��������ɨ�ߵĽ���ж϶�ʧ������������߻����ұߴӶ������������ı�ɨ��
                if(flag_r == 1)   //�ҵ����ұ߽磨��ʧ����������ߣ�
                {
                    //��������Ѱ����߽�
                    for(; cloum-BORDER_BIAS>0; cloum--)    //��������ɨ
                    {
                        if(binary_imag[row][cloum] == IMAGE_BLACK && binary_imag[row][cloum - BORDER_BIAS] == IMAGE_BLACK)    //�ҵ��ڵ㣺����������ɨ�������⣺��-�ڣ�
                        {
                            LeftLine[row] = cloum;   //��¼��߽��
                            flag_l = 1;              //flag���޶��߱��
                            break;
                        }
                    }
                }
                else            //û���ҵ��ұ߽磨��ʧ���������ұߣ�
                {
                    for(cloum = Mid; cloum + BORDER_BIAS < MT9V03X_W - 1; cloum++)    //����ɨ
                    {
                        if(binary_imag[row][cloum] == IMAGE_WHITE && binary_imag[row][cloum + BORDER_BIAS] == IMAGE_WHITE)
                        {
                            LeftLine[row] = cloum;   //��¼��߽�㣨�����ҵ�������߽�㣩
                            flag_l = 1;              //flag���޶��߱��
                            break;
                        }
                    }
                    if(flag_l == 1)   //�ҵ�����߽磨��ʧ���������ұߣ�
                    {
                        for(; cloum+BORDER_BIAS < MT9V03X_W - 1; cloum++) //��������ɨ
                        {
                            if(binary_imag[row][cloum] == IMAGE_BLACK && binary_imag[row][cloum + BORDER_BIAS] == IMAGE_BLACK)
                            {
                                RightLine[row] = cloum;   //��¼�ұ߽��
                                flag_r = 1;               //flag���޶��߱��
                                break;
                            }
                        }
                    }
                }
            }
            else if(path == 'R')  //Ĭ������ɨ��
            {
                //�����ұ�ɨ�ߣ�Ѱ����߽��
                for(cloum = Mid; cloum + BORDER_BIAS < MT9V03X_W - 1; cloum++)    //����ɨ
                {
                    //�ж���߽��
                    if(binary_imag[row][cloum] == IMAGE_WHITE && binary_imag[row][cloum + BORDER_BIAS] == IMAGE_WHITE)  //�ҵ��׵㣨��������ɨ�������ڣ���-�ף�
                    {
                        LeftLine[row] = cloum;    //��¼��߽�㣨�����ҵ�������߽�㣩
                        flag_l = 1;               //flag���޶��߱��
                        break;
                    }
                }
                //��������ɨ�ߵĽ���ж϶�ʧ������������߻����ұߴӶ������������ı�ɨ��
                if(flag_l == 1)   //�ҵ�����߽磨��ʧ���������ұߣ�
                {
                    //��������Ѱ���ұ߽�
                    for(; cloum + BORDER_BIAS < MT9V03X_W - 1; cloum++) //��������ɨ
                    {
                        //�ж��ұ߽��
                        if(binary_imag[row][cloum] == IMAGE_BLACK && binary_imag[row][cloum + BORDER_BIAS] == IMAGE_BLACK)    //�ҵ��ڵ㣨��������ɨ�������⣺��-�ڣ�
                        {
                            RightLine[row] = cloum;   //��¼��߽��
                            flag_r = 1;               //flag���޶��߱��
                            break;
                        }
                    }
                }
                else            //û���ҵ���߽磨��ʧ����������ߣ�
                {
                    for(cloum = Mid; cloum - BORDER_BIAS > 0; cloum--)    //����ɨ
                    {
                        //�ж��ұ߽��
                        if(binary_imag[row][cloum] == IMAGE_WHITE && binary_imag[row][cloum-BORDER_BIAS] == IMAGE_WHITE)
                        {
                            RightLine[row] = cloum;   //��¼�ұ߽�㣨�����ҵ������ұ߽�㣩
                            flag_r = 1;               //flag���޶��߱��
                            break;
                        }
                    }
                    if(flag_r == 1)   //�ҵ����ұ߽磨��ʧ����������ߣ�
                    {
                        //��������Ѱ����߽�
                        for(; cloum-BORDER_BIAS > 0; cloum--)   //��������ɨ
                        {
                            //�ж���߽��
                            if(binary_imag[row][cloum] == IMAGE_BLACK && binary_imag[row][cloum - BORDER_BIAS] == IMAGE_BLACK)  //�ж��ұ߽��
                            {
                                LeftLine[row] = cloum;    //��¼��߽��
                                flag_l = 1;               //flag���޶��߱��
                                break;
                            }
                        }
                    }
                }
            }
        }
        //�������У�����ɨ��
        else
        {
            num = 0;  //�������������ʧ��=0
            //���ɨ��
            for(cloum=Mid; cloum - BORDER_BIAS > 0; cloum--)              //����ɨ
            {
                if(binary_imag[row][cloum] == IMAGE_BLACK && binary_imag[row][cloum - BORDER_BIAS] == IMAGE_BLACK)  //�ж���߽�㣨BORDER_BIAS��żȻ���أ�
                {
                    LeftLine[row] = cloum;    //��¼��߽��
                    flag_l = 1;               //flag���޶��߱��
                    break;
                }
            }
            //�ұ�ɨ��
            for(cloum=Mid; cloum+BORDER_BIAS < MT9V03X_W - 1; cloum++)    //����ɨ
            {
                if(binary_imag[row][cloum] == IMAGE_BLACK && binary_imag[row][cloum + BORDER_BIAS] == IMAGE_BLACK)  //�ж��ұ߽�㣨BORDER_BIAS��żȻ���أ�
                {
                    RightLine[row] = cloum;   //��¼�ұ߽��
                    flag_r = 1;               //flag���޶��߱��
                    break;
                }
            }
        }
        if(flag_l == 0)   //��߽綪��
        {
            LeftLine[row] = 0;            //��߽�����ͼ�����߽�
            LostNum_LeftLine++;         //������+1
        }
        if(flag_r == 0)   //�ұ߽綪��
        {
            RightLine[row] = MT9V03X_W-1; //�ұ߽�����ͼ����ұ߽�
            LostNum_RightLine++;        //�Ҷ�����+1
        }
        CentreLine[row] = (LeftLine[row] + RightLine[row]) / 2;   //�������ߵ�
        //Ϊ��һ��ɨ����׼��
        Mid = CentreLine[row];    //����һ�ε�����ֵΪ��һ��ɨ�ߵ��м��
        flag_l = 0;               //��߽綪��flag��0
        flag_r = 0;               //�ұ߽綪��flag��0
    }
}

//-------------------------------------------------------------------------------------------------------------------
// �������     �������ұ߽������õ��¹յ㣨ʮ�֡������������жϻ����ϣ�
// ����˵��     starline ��ʼ��
// ����˵��     endline ������
// ����˵��     *InflectionL ��߹յ�
// ����˵��     *InflectionR �ұ߹յ�
// ���ز���     void
// ʹ��ʾ��     imag_GetDownInflection(110,45,&InflectionL,&InflectionR);
// ��ע��Ϣ
//-------------------------------------------------------------------------------------------------------------------
void imag_GetDownInflection(int startline,int endline,Point *InflectionL,Point *InflectionR)
{
    if(startline < endline) // ��֤��ʼ�д�����ֹ�У�������������
    {
        startline += endline;
        endline = startline - endline;
        startline = startline - endline;
    }
    InflectionL->X = 0;InflectionL->Y = 0;InflectionR->X = 0;InflectionR->Y = 0;//���ҹյ�����
    for(int i = startline; i > endline; i--)
    {
        // �������ߣ�����ȱ�С����С�ĵ㣬�����������ж���Ϊ�����У���߶���Ϊ0
        // ע�⣺��������ж������Ǻ����ڵ�1,3��ֵ�ԱȵĻ���������̫С�Ӷ��������ȵ��������ڵĻ�����Ӱ���жϣ�������Ҫ�Ĵ�Ƚϵ�����
        // ���˸��ж�InflectionL->Y==0��Ϊ�˴������ϱ������ҵ���֮��Ͳ��ټ����������ˣ���������ʱ��Ľؾ�ͼƬ�Ͳ��øոպ���
        if(InflectionL->Y == 0 && LeftLine[i] >= LeftLine[i-1] && LeftLine[i] >= LeftLine[i-3] && LeftLine[i] >= LeftLine[i+1] && LeftLine[i] >= LeftLine[i+3]
           && LeftLine[i] > LeftLine[i-5] && LeftLine[i] > LeftLine[i-7] && LeftLine[i] > LeftLine[i+5] && LeftLine[i] > LeftLine[i+7])
        {
            InflectionL->X = LeftLine[i];//����յ�ģ�x,y������
            InflectionL->Y = i;
        }

        // �������ߣ����������С�ĵ�����ұߵĹյ㣬�ұ��߶���ΪMT9V03X_W-1
        // ���˸��ж�InflectionR->Y==0��Ϊ�˴������ϱ������ҵ���֮��Ͳ��ټ����������ˣ���������ʱ��Ľؾ�ͼƬ�Ͳ��øոպ���
        if(InflectionR->Y==0 && RightLine[i]<=RightLine[i-1] && RightLine[i]<=RightLine[i-3] && RightLine[i]<=RightLine[i+1] && RightLine[i]<=RightLine[i+3]
           && RightLine[i]<RightLine[i-5] && RightLine[i]<RightLine[i-7] && RightLine[i]<RightLine[i+5] && RightLine[i]<RightLine[i+7])
        {
            InflectionR->X=RightLine[i];//����յ�ģ�x,y������
            InflectionR->Y=i;
        }
    }
    /*debug*/
    // ips114_draw_line(InflectionR->X, InflectionR->Y,InflectionL->X, InflectionL->Y, RGB565_PINK);
}

//-------------------------------------------------------------------------------------------------------------------
// �������     �������ұ߽�õ��Ϲյ�
// ����˵��     Choose ѡ��������߻�������
// ����˵��     startline ��ʼ��
// ����˵��     endline ������
// ����˵��     *UpInflection �Ϲյ�
// ���ز���     void
// ʹ��ʾ��     imag_GetUpInflection('L', 45, 110, &UpInflectionL);
// ��ע��Ϣ
//-------------------------------------------------------------------------------------------------------------------
void imag_GetUpInflection(char Choose,int startline,int endline,Point *UpInflection)
{
    const int UPINFLECTION_DOWM_MIN_THRESHOLD = 15; // �Ϲյ������������������Ĳ�ֵ��С��ֵ
    const int UPINFLECTION_UP_MAX_THRESHOLD = 5; // �Ϲյ������������������Ĳ�ֵ�����ֵ
    const int UPINFLECTION_COMPARE_INTERVAL = 3; // �Ϲյ�����֮��Ƚϼ��
    if(startline > endline) // ��֤��ʼ��С����ֹ�У�������������
    {
        startline += endline;
        endline = startline - endline;
        startline = startline - endline;
    }
    switch(Choose)
    {
        case 'L':
        {
            // �������±���
            for (int row = startline; row < endline; row++)
            {
                // �����е�������-���������������ֵ������ABS��Ϊ���ٶȸ���
                if (LeftLine[row] - LeftLine[row+UPINFLECTION_COMPARE_INTERVAL] >= UPINFLECTION_DOWM_MIN_THRESHOLD
                 && LeftLine[row - UPINFLECTION_COMPARE_INTERVAL] - LeftLine[row + UPINFLECTION_COMPARE_INTERVAL] >= UPINFLECTION_DOWM_MIN_THRESHOLD
                 && LeftLine[row-1]-LeftLine[row]<=UPINFLECTION_UP_MAX_THRESHOLD)
                {
                    UpInflection->X = LeftLine[row]; UpInflection->Y = row;
                    break;
                }
            }
            break;
        }
        case 'R':
        {
            for(int row = startline; row < endline; row++)
            {
                // ����������-�����е������������ֵ������ABS��Ϊ���ٶȸ���
                if (RightLine[row+UPINFLECTION_COMPARE_INTERVAL] - RightLine[row] >= UPINFLECTION_DOWM_MIN_THRESHOLD
                 && RightLine[row + UPINFLECTION_COMPARE_INTERVAL] - RightLine[row - UPINFLECTION_COMPARE_INTERVAL] >= UPINFLECTION_DOWM_MIN_THRESHOLD
                 && RightLine[row]-RightLine[row-1]<=UPINFLECTION_UP_MAX_THRESHOLD)
                {
                    UpInflection->X=RightLine[row];UpInflection->Y=row;
                    break;
                }
            }
            break;
        }
        default:break;
    }
}

//-------------------------------------------------------------------------------------------------------------------
// �������     ͼ���˲�
// ����˵��     void
// ���ز���     void
// ʹ��ʾ��     image_Filter();
// ��ע��Ϣ
//-------------------------------------------------------------------------------------------------------------------
void image_Filter()
{
    for (int nr=1; nr < MT9V03X_H-1; nr++)
    {
        for (int nc=1; nc < MT9V03X_W-1; nc++)
        {
            if ((binary_imag[nr][nc] == IMAGE_BLACK)
                    &&(binary_imag[nr-1][nc]+binary_imag[nr+1][nc]+binary_imag[nr][nc+1]+binary_imag[nr][nc-1]>510))
            {
                binary_imag[nr][nc] = IMAGE_WHITE;
            }
            else if ((binary_imag[nr][nc] == IMAGE_WHITE)
                    && (binary_imag[nr-1][nc]+binary_imag[nr+1][nc]+binary_imag[nr][nc+1]+binary_imag[nr][nc-1]<510))
            {
                binary_imag[nr][nc] = IMAGE_BLACK;
            }
        }
    }
}

//-------------------------------------------------------------------------------------------------------------------
// �������     �������ߺ����߻���
// ����˵��     void
// ���ز���     void
// ʹ��ʾ��     imag_drawline();
// ��ע��Ϣ
//-------------------------------------------------------------------------------------------------------------------
void imag_drawline()
{
    for(uint8 row = MT9V03X_H - 1; row > 0; row--) //�������ϣ���������ͼ��
    {
        ips114_draw_point(LeftLine[row], row, RGB565_GREEN);
        ips114_draw_point(LeftLine[row] + 1, row, RGB565_GREEN);
        ips114_draw_point(CentreLine[row],row,RGB565_RED);
        ips114_draw_point(RightLine[row],row,RGB565_BLUE);
        ips114_draw_point(RightLine[row] - 1,row,RGB565_BLUE);
    }
}

//-------------------------------------------------------------------------------------------------------------------
// �������     ͼ�����������ֵ�����ұ��ߡ������������յ㲢��ʾ����Ļ��
// ����˵��     void
// ���ز���     void
// ʹ��ʾ��     imag_process();
// ��ע��Ϣ
//-------------------------------------------------------------------------------------------------------------------
void imag_process()
{
    // �ȶ�ͼ����ж�ֵ�����˲���������ȡ�������������յ�
    imag_binary();
    image_Filter();
    imag_getline(LeftLine, CentreLine, RightLine, 'R');
    imag_GetDownInflection(110, 45, &InflectionL, &InflectionR);

    // ��Ļ��ͼ�񣬲���ʾ���ߺ�����
    ips114_displayimage03x((const uint8 *)binary_imag, MT9V03X_W, MT9V03X_H);
    imag_drawline();
}
