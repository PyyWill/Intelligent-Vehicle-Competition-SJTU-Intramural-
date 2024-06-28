#include "zf_common_headfile.h"
#include "imagprocess.h"

uint8 binary_imag[MT9V03X_H][MT9V03X_W];                                                // 定义二值化后的图像
uint8 Mid = MT9V03X_W / 2;                                                              // 初始化扫线的中点为图像中点
uint8 Lost_Row = 0;                                                                     // 中线丢失的行坐标(扫线到赛道外)
uint8 LostNum_LeftLine = 0,LostNum_RightLine = 0;                                       // 记录左右边界丢线数
int LeftLine[MT9V03X_H] = {0}, CentreLine[MT9V03X_H] = {0}, RightLine[MT9V03X_H] = {0}; // 扫线处理左中右三线
Point InflectionL,InflectionR;                                                          // 左右下拐点

//-------------------------------------------------------------------------------------------------------------------
// 函数简介     大津法二值化程序
// 参数说明     *image ：图像地址
// 参数说明     width:  图像宽
// 参数说明     height：图像高
// 返回参数     void
// 使用示例     image_threshold = otsuThreshold(mt9v03x_image[0],MT9V03X_W,MT9V03X_H);//使用大津法得到二值化阈值
// 备注信息
//-------------------------------------------------------------------------------------------------------------------
uint8 otsuThreshold(uint8 *image, uint16 width, uint16 height)
{
    int pixelCount[GrayScale] = {0}; // 每个灰度值所占像素个数
    float pixelPro[GrayScale] = {0}; // 每个灰度值所占总像素比例
    int Sumpix = width * height; // 总像素点
    uint8 threshold = 0;
    uint8* data = image; // 指向像素数据的指针

    // 统计灰度级中每个像素在整幅图像中的个数
    for(int i = 0; i < height; i++)
    {
        for (int j = 0; j < width; j++)
        {
            pixelCount[(int)data[i * width + j]]++; // 统计各个灰度值
        }
    }
    float u = 0;
    for(int i = 0; i < GrayScale; i++)
    {
        pixelPro[i] = (float)pixelCount[i] / Sumpix; // 计算每个像素在整幅图像中的比例
        u += i * pixelPro[i]; // 总平均灰度
    }

    float maxVariance=0.0; // 最大类间方差
    float w0 = 0, avgValue0  = 0, w1 = 0, avgValue1 = 0;  // w0 背景比例 ，avgValue0 背景平均灰度，w1 前景比例 ，avgValue1 前景平均灰度
    for(int i = 0; i < 256; i++) // 每一次循环都是一次完整类间方差计算 (两个for叠加为1个)
    {
        w0 += pixelPro[i]; // 假设当前灰度i为阈值, 0~i 灰度像素所占整幅图像的比例即背景比例
        avgValue0 += i * pixelPro[i];
        w1 = 1 - w0;
        avgValue1 = u - avgValue0;

        float variance = w0 * pow((avgValue0 / w0 - u), 2) + w1 * pow((avgValue1 / w1 - u), 2); // 类间方差
        if(variance > maxVariance)
        {
            maxVariance = variance;
            threshold = (uint8)i;
        }
    }
    return threshold;
}

//-------------------------------------------------------------------------------------------------------------------
// 函数简介     图像处理（当前进度：全图二值化）
// 参数说明     void
// 返回参数     void
// 使用示例     imag_binary();
// 备注信息
//-------------------------------------------------------------------------------------------------------------------
void imag_binary()
{
    uint8 image_threshold = otsuThreshold(mt9v03x_image[0], MT9V03X_W, MT9V03X_H); // 使用大津法得到二值化阈值
    for(int i = 0; i < MT9V03X_H; i++)
    {
        for(int j = 0; j < MT9V03X_W; j++)
        {
            if(mt9v03x_image[i][j] <= image_threshold) // 进行二值化之前只是得到阈值
                binary_imag[i][j] = IMAGE_BLACK; // 0是黑色  //图像原点不变
            else
                binary_imag[i][j] = IMAGE_WHITE; // 1是白色  //图像原点不变
        }
    }
}

//-------------------------------------------------------------------------------------------------------------------
// 函数简介     对二值图找左右边界和中线
// 参数说明     *LeftLine 左边线数组指针
// 参数说明     *CentreLine 中线数组指针
// 参数说明     *RightLine 右边线数组指针
// 参数说明     path 当前中间为背景（黑色）时，默认先往左右哪个方向找赛道（白色），可以取为'L':向左，'R':向右
// 返回参数     void
// 使用示例     imag_getline(LeftLine, CentreLine, RightLine, 'L');
// 备注信息
//-------------------------------------------------------------------------------------------------------------------
void imag_getline(int *LeftLine, int *CentreLine, int *RightLine ,char path)
{
    uint8 row,cloum; // 行,列
    uint8 flag_l = 0,flag_r = 0; // 记录是否丢线flag，flag=0：丢线
    uint8 num = 0; // 记录中线连续丢失的次数
    LostNum_LeftLine = 0;LostNum_RightLine = 0; //把丢线的计数变量清零

    // 开始扫线(从下往上,从中间往两边),为了扫线的严谨性,我们做BORDER_BIAS的误差处理，即扫线范围会小于图像大小
    for(row = MT9V03X_H - 1; row > 0; row--) //从下往上，遍历整幅图像
    {
        // 在赛道外，优先按path扫线方向寻找赛道
        if(binary_imag[row][Mid]== IMAGE_BLACK)  // 扫线中点是黑色的（中点在赛道外）
        {
            num++;    //中线连续丢失，+1
            if(path == 'L')   //默认向左扫线
            {
                // 先向左边扫线，寻找右边界点
                for(cloum = Mid; cloum - BORDER_BIAS > 0; cloum--)    //向左扫
                {
                    if(binary_imag[row][cloum] == IMAGE_WHITE && binary_imag[row][cloum - BORDER_BIAS] == IMAGE_WHITE) // 找到白点（从赛道外扫到赛道内：黑-白）
                    {
                        RightLine[row] = cloum;    //记录右边界点（向左找到的是右边界点）
                        flag_r = 1;               //flag做无丢线标记
                        break;
                    }
                }
                //根据上面扫线的结果判断丢失的赛道是在左边还是右边从而决定继续向哪边扫线
                if(flag_r == 1)   //找到了右边界（丢失的赛道在左边）
                {
                    //继续向左寻找左边界
                    for(; cloum-BORDER_BIAS>0; cloum--)    //继续向左扫
                    {
                        if(binary_imag[row][cloum] == IMAGE_BLACK && binary_imag[row][cloum - BORDER_BIAS] == IMAGE_BLACK)    //找到黑点：（从赛道内扫到赛道外：白-黑）
                        {
                            LeftLine[row] = cloum;   //记录左边界点
                            flag_l = 1;              //flag做无丢线标记
                            break;
                        }
                    }
                }
                else            //没有找到右边界（丢失的赛道在右边）
                {
                    for(cloum = Mid; cloum + BORDER_BIAS < MT9V03X_W - 1; cloum++)    //向右扫
                    {
                        if(binary_imag[row][cloum] == IMAGE_WHITE && binary_imag[row][cloum + BORDER_BIAS] == IMAGE_WHITE)
                        {
                            LeftLine[row] = cloum;   //记录左边界点（向右找到的是左边界点）
                            flag_l = 1;              //flag做无丢线标记
                            break;
                        }
                    }
                    if(flag_l == 1)   //找到了左边界（丢失的赛道在右边）
                    {
                        for(; cloum+BORDER_BIAS < MT9V03X_W - 1; cloum++) //继续向右扫
                        {
                            if(binary_imag[row][cloum] == IMAGE_BLACK && binary_imag[row][cloum + BORDER_BIAS] == IMAGE_BLACK)
                            {
                                RightLine[row] = cloum;   //记录右边界点
                                flag_r = 1;               //flag做无丢线标记
                                break;
                            }
                        }
                    }
                }
            }
            else if(path == 'R')  //默认向右扫线
            {
                //先向右边扫线，寻找左边界点
                for(cloum = Mid; cloum + BORDER_BIAS < MT9V03X_W - 1; cloum++)    //向右扫
                {
                    //判断左边界点
                    if(binary_imag[row][cloum] == IMAGE_WHITE && binary_imag[row][cloum + BORDER_BIAS] == IMAGE_WHITE)  //找到白点（从赛道外扫到赛道内：黑-白）
                    {
                        LeftLine[row] = cloum;    //记录左边界点（向右找到的是左边界点）
                        flag_l = 1;               //flag做无丢线标记
                        break;
                    }
                }
                //根据上面扫线的结果判断丢失的赛道是在左边还是右边从而决定继续向哪边扫线
                if(flag_l == 1)   //找到了左边界（丢失的赛道在右边）
                {
                    //继续向右寻找右边界
                    for(; cloum + BORDER_BIAS < MT9V03X_W - 1; cloum++) //继续向右扫
                    {
                        //判断右边界点
                        if(binary_imag[row][cloum] == IMAGE_BLACK && binary_imag[row][cloum + BORDER_BIAS] == IMAGE_BLACK)    //找到黑点（从赛道内扫到赛道外：白-黑）
                        {
                            RightLine[row] = cloum;   //记录左边界点
                            flag_r = 1;               //flag做无丢线标记
                            break;
                        }
                    }
                }
                else            //没有找到左边界（丢失的赛道在左边）
                {
                    for(cloum = Mid; cloum - BORDER_BIAS > 0; cloum--)    //向左扫
                    {
                        //判断右边界点
                        if(binary_imag[row][cloum] == IMAGE_WHITE && binary_imag[row][cloum-BORDER_BIAS] == IMAGE_WHITE)
                        {
                            RightLine[row] = cloum;   //记录右边界点（向左找到的是右边界点）
                            flag_r = 1;               //flag做无丢线标记
                            break;
                        }
                    }
                    if(flag_r == 1)   //找到了右边界（丢失的赛道在左边）
                    {
                        //继续向左寻找左边界
                        for(; cloum-BORDER_BIAS > 0; cloum--)   //继续向左扫
                        {
                            //判断左边界点
                            if(binary_imag[row][cloum] == IMAGE_BLACK && binary_imag[row][cloum - BORDER_BIAS] == IMAGE_BLACK)  //判断右边界点
                            {
                                LeftLine[row] = cloum;    //记录左边界点
                                flag_l = 1;               //flag做无丢线标记
                                break;
                            }
                        }
                    }
                }
            }
        }
        //在赛道中，正常扫线
        else
        {
            num = 0;  //打断中线连续丢失，=0
            //左边扫线
            for(cloum=Mid; cloum - BORDER_BIAS > 0; cloum--)              //向左扫
            {
                if(binary_imag[row][cloum] == IMAGE_BLACK && binary_imag[row][cloum - BORDER_BIAS] == IMAGE_BLACK)  //判断左边界点（BORDER_BIAS防偶然因素）
                {
                    LeftLine[row] = cloum;    //记录左边界点
                    flag_l = 1;               //flag做无丢线标记
                    break;
                }
            }
            //右边扫线
            for(cloum=Mid; cloum+BORDER_BIAS < MT9V03X_W - 1; cloum++)    //向右扫
            {
                if(binary_imag[row][cloum] == IMAGE_BLACK && binary_imag[row][cloum + BORDER_BIAS] == IMAGE_BLACK)  //判断右边界点（BORDER_BIAS防偶然因素）
                {
                    RightLine[row] = cloum;   //记录右边界点
                    flag_r = 1;               //flag做无丢线标记
                    break;
                }
            }
        }
        if(flag_l == 0)   //左边界丢线
        {
            LeftLine[row] = 0;            //左边界点等于图像的左边界
            LostNum_LeftLine++;         //左丢线数+1
        }
        if(flag_r == 0)   //右边界丢线
        {
            RightLine[row] = MT9V03X_W-1; //右边界点等于图像的右边界
            LostNum_RightLine++;        //右丢线数+1
        }
        CentreLine[row] = (LeftLine[row] + RightLine[row]) / 2;   //计算中线点
        //为下一次扫线做准备
        Mid = CentreLine[row];    //以上一次的中线值为下一次扫线的中间点
        flag_l = 0;               //左边界丢线flag置0
        flag_r = 0;               //右边界丢线flag置0
    }
}

//-------------------------------------------------------------------------------------------------------------------
// 函数简介     根据左右边界线来得到下拐点（十字、三岔、环岛的判断会用上）
// 参数说明     starline 起始行
// 参数说明     endline 结束行
// 参数说明     *InflectionL 左边拐点
// 参数说明     *InflectionR 右边拐点
// 返回参数     void
// 使用示例     imag_GetDownInflection(110,45,&InflectionL,&InflectionR);
// 备注信息
//-------------------------------------------------------------------------------------------------------------------
void imag_GetDownInflection(int startline,int endline,Point *InflectionL,Point *InflectionR)
{
    if(startline < endline) // 保证起始行大于终止行，从下往上搜索
    {
        startline += endline;
        endline = startline - endline;
        startline = startline - endline;
    }
    InflectionL->X = 0;InflectionL->Y = 0;InflectionR->X = 0;InflectionR->Y = 0;//左右拐点置零
    for(int i = startline; i > endline; i--)
    {
        // 遍历左线，求出先变小大后变小的点，多加三个点的判断是为了误判，左边丢线为0
        // 注意：这里如果判断条件是和相邻的1,3的值对比的话，会区间太小从而如果有相等的列数存在的话，会影响判断，所以需要改大比较的区间
        // 加了个判断InflectionL->Y==0是为了从下往上遍历，找到了之后就不再继续往上找了，这样遍历时候的截距图片就不用刚刚好了
        if(InflectionL->Y == 0 && LeftLine[i] >= LeftLine[i-1] && LeftLine[i] >= LeftLine[i-3] && LeftLine[i] >= LeftLine[i+1] && LeftLine[i] >= LeftLine[i+3]
           && LeftLine[i] > LeftLine[i-5] && LeftLine[i] > LeftLine[i-7] && LeftLine[i] > LeftLine[i+5] && LeftLine[i] > LeftLine[i+7])
        {
            InflectionL->X = LeftLine[i];//存入拐点的（x,y）坐标
            InflectionL->Y = i;
        }

        // 遍历右线，求出列数最小的点就是右边的拐点，右边线丢线为MT9V03X_W-1
        // 加了个判断InflectionR->Y==0是为了从下往上遍历，找到了之后就不再继续往上找了，这样遍历时候的截距图片就不用刚刚好了
        if(InflectionR->Y==0 && RightLine[i]<=RightLine[i-1] && RightLine[i]<=RightLine[i-3] && RightLine[i]<=RightLine[i+1] && RightLine[i]<=RightLine[i+3]
           && RightLine[i]<RightLine[i-5] && RightLine[i]<RightLine[i-7] && RightLine[i]<RightLine[i+5] && RightLine[i]<RightLine[i+7])
        {
            InflectionR->X=RightLine[i];//存入拐点的（x,y）坐标
            InflectionR->Y=i;
        }
    }
    /*debug*/
    // ips114_draw_line(InflectionR->X, InflectionR->Y,InflectionL->X, InflectionL->Y, RGB565_PINK);
}

//-------------------------------------------------------------------------------------------------------------------
// 函数简介     根据左右边界得到上拐点
// 参数说明     Choose 选择遍历左线还是右线
// 参数说明     startline 起始行
// 参数说明     endline 结束行
// 参数说明     *UpInflection 上拐点
// 返回参数     void
// 使用示例     imag_GetUpInflection('L', 45, 110, &UpInflectionL);
// 备注信息
//-------------------------------------------------------------------------------------------------------------------
void imag_GetUpInflection(char Choose,int startline,int endline,Point *UpInflection)
{
    const int UPINFLECTION_DOWM_MIN_THRESHOLD = 15; // 上拐点列坐标与下面行数的差值最小阈值
    const int UPINFLECTION_UP_MAX_THRESHOLD = 5; // 上拐点列坐标与上面行数的差值最大阈值
    const int UPINFLECTION_COMPARE_INTERVAL = 3; // 上拐点两点之间比较间隔
    if(startline > endline) // 保证起始行小于终止行，从上往下搜索
    {
        startline += endline;
        endline = startline - endline;
        startline = startline - endline;
    }
    switch(Choose)
    {
        case 'L':
        {
            // 从上往下遍历
            for (int row = startline; row < endline; row++)
            {
                // 下三行的列坐标-这行列坐标大于阈值，不用ABS是为了速度更快
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
                // 这行列坐标-下三行的列坐标大于阈值，不用ABS是为了速度更快
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
// 函数简介     图像滤波
// 参数说明     void
// 返回参数     void
// 使用示例     image_Filter();
// 备注信息
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
// 函数简介     赛道边线和中线绘制
// 参数说明     void
// 返回参数     void
// 使用示例     imag_drawline();
// 备注信息
//-------------------------------------------------------------------------------------------------------------------
void imag_drawline()
{
    for(uint8 row = MT9V03X_H - 1; row > 0; row--) //从下往上，遍历整幅图像
    {
        ips114_draw_point(LeftLine[row], row, RGB565_GREEN);
        ips114_draw_point(LeftLine[row] + 1, row, RGB565_GREEN);
        ips114_draw_point(CentreLine[row],row,RGB565_RED);
        ips114_draw_point(RightLine[row],row,RGB565_BLUE);
        ips114_draw_point(RightLine[row] - 1,row,RGB565_BLUE);
    }
}

//-------------------------------------------------------------------------------------------------------------------
// 函数简介     图像基础处理：二值化、找边线、找下面两个拐点并显示在屏幕上
// 参数说明     void
// 返回参数     void
// 使用示例     imag_process();
// 备注信息
//-------------------------------------------------------------------------------------------------------------------
void imag_process()
{
    // 先对图像进行二值化、滤波、边线提取、找下面两个拐点
    imag_binary();
    image_Filter();
    imag_getline(LeftLine, CentreLine, RightLine, 'R');
    imag_GetDownInflection(110, 45, &InflectionL, &InflectionR);

    // 屏幕上图像，并显示边线和中线
    ips114_displayimage03x((const uint8 *)binary_imag, MT9V03X_W, MT9V03X_H);
    imag_drawline();
}
