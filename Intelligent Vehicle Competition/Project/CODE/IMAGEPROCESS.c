#include "headfile.h"
#include "IMAGEPROCESS.h"
#include  <math.h>

// 下面是基本的图像二值化和边界特征提取

const uint8 Standard_Road_Wide[MT9V03X_CSI_H]=//标准赛宽
{8,9,10,11,12,13,14,15,16,17,18,19,20,21,22,23,24,25,26,27,28,29,30,31,32,33,34,35,36,37,38,39,40,41,42,43,44,45,46,47,48,49,50,51,52,53,54,55,56,57,58,59,60,61,62,63,64,65,66,
67,68,69,70,71,72,73,74,75,76,
77,78,79,80,81,82,83,84,85,86,
87,88,89,90,91,92,93,94,95,96,
97,98,99,100,101,102,103,104,105,106,
107,108,109,110,111,112,113,114,115,116,
117,118,119,120,121,122,123,124,125,126,127};

uint8 image_two_value[MT9V03X_CSI_H][MT9V03X_CSI_W];//二值化后的原数组
volatile int Left_Line[MT9V03X_CSI_H]; //?????????
volatile int Right_Line[MT9V03X_CSI_H];//?????????
volatile int Mid_Line[MT9V03X_CSI_H];  //????????
volatile int Road_Wide[MT9V03X_CSI_H]; //????????
volatile int White_Column[MT9V03X_CSI_W];    //???????????
volatile int Search_Stop_Line;     //?????????,????????????????????????????????
volatile int Boundry_Start_Left;   //???????????
volatile int Boundry_Start_Right;  //???????????,???????????
volatile int Left_Lost_Time;       //???P????
volatile int Right_Lost_Time;
volatile int Both_Lost_Time;//????????????
int Longest_White_Column_Left[2]; //??????,[0]???????????????????Search_Stop_Line???????????[1????????
int Longest_White_Column_Right[2];//??????,[0]???????????????????Search_Stop_Line???????????[1????????
int Left_Lost_Flag[MT9V03X_CSI_H] ; //??????????????1?????????0
int Right_Lost_Flag[MT9V03X_CSI_H]; //???????????????1?????????0

int start_column=5;//最长白列的搜索区间
int end_column=MT9V03X_CSI_W-5;

//下面是赛段判断程序部分

//直道
volatile uint8 Straight_Flag;// 直道标记

//坡道
volatile int Ramp_Flag=0;//坡道标志

//十字
volatile int Cross_Flag=0;
volatile int Left_Down_Find=0; //十字使用，找到被置行数，没找到就是0
volatile int Left_Up_Find=0;   //四个拐点标志
volatile int Right_Down_Find=0;
volatile int Right_Up_Find=0;

//三叉
volatile int Junction_Flag=0;
volatile int Left_Down_Find_Junction=0; //三叉使用，找到被置行数，没找到就是0
volatile int Right_Down_Find_Junction=0;
volatile int Mid_Junction=0;

//环岛变量
volatile int Island_State=0;     //环岛状态标志
volatile int Left_Island_Flag=0; //左右环岛标志
volatile int Right_Island_Flag=0;//左右环岛标志
volatile int Left_Up_Guai[2];    //四个拐点的坐标存储，[0]存y，第某行，{1}存x，第某列
volatile int Right_Up_Guai[2];   //四个拐点的坐标存储，[0]存y，第某行，{1}存x，第某列

extern volatile float Err;//中线加权误差

/*-------------------------------------------------------------------------------------------------------------------
  @brief     图像二值化处理函数
  @param     二值化阈值
  @return    NULL
  Sample     Image_Binarization(Threshold);//图像二值化
  @note      二值化后直接访问image_two_value[i][j]这个数组即可
-------------------------------------------------------------------------------------------------------------------*/
void Image_Binarization(int threshold)//图像二值化
{
    uint16 i,j;
    for(i=0;i<MT9V03X_CSI_H;i++)
    {
        for(j=0;j<MT9V03X_CSI_W;j++)//灰度图的数据只做判断，不进行更改，二值化图像放在了新数组中
        {
            if(mt9v03x_csi_image[i][j]>=threshold )
                image_two_value[i][j]=IMG_WHITE;//白
            else
                image_two_value[i][j]=IMG_BLACK;//黑
        }
    }
}

/*-------------------------------------------------------------------------------------------------------------------
  @brief     普通大津求阈值
  @param     image       图像数组
             width       列 ，宽度
             height      行，长度
  @return    threshold   返回int类型的的阈值
  Sample     threshold=my_adapt_threshold(mt9v03x_image[0],MT9V03X_W, MT9V03X_H);//普通大津
  @note      据说没有山威大津快，我感觉两个区别不大
-------------------------------------------------------------------------------------------------------------------*/
int my_adapt_threshold(uint8 *image,uint16 width, uint16 height)   //大津算法，注意计算阈值的一定要是原图像
{
    #define GrayScale 256
    int pixelCount[GrayScale];
    float pixelPro[GrayScale];
    int i, j;
    int pixelSum = width * height/4;
    int  threshold = 0;
    uint8* data = image;  //指向像素数据的指针
    for (i = 0; i < GrayScale; i++)
    {
        pixelCount[i] = 0;
        pixelPro[i] = 0;
    }
    uint32 gray_sum=0;
    for (i = 0; i < height; i+=2)//统计灰度级中每个像素在整幅图像中的个数
    {
        for (j = 0; j <width; j+=2)
        {
            pixelCount[(int)data[i * width + j]]++;  //将当前的点的像素值作为计数数组的下标
            gray_sum+=(int)data[i * width + j];       //灰度值总和
        }
    }
    for (i = 0; i < GrayScale; i++) //计算每个像素值的点在整幅图像中的比例
    {
        pixelPro[i] = (float)pixelCount[i] / pixelSum;
    }
    float w0, w1, u0tmp, u1tmp, u0, u1, u, deltaTmp, deltaMax = 0;
    w0 = w1 = u0tmp = u1tmp = u0 = u1 = u = deltaTmp = 0;
    for (j = 0; j < GrayScale; j++)//遍历灰度级[0,255]
    {
        w0 += pixelPro[j];  //背景部分每个灰度值的像素点所占比例之和   即背景部分的比例
        u0tmp += j * pixelPro[j];  //背景部分 每个灰度值的点的比例 *灰度值
        w1=1-w0;
        u1tmp=gray_sum/pixelSum-u0tmp;
        u0 = u0tmp / w0;              //背景平均灰度
        u1 = u1tmp / w1;              //前景平均灰度
        u = u0tmp + u1tmp;            //全局平均灰度
        deltaTmp = w0 * pow((u0 - u), 2) + w1 * pow((u1 - u), 2);//平方
        if (deltaTmp > deltaMax)
        {
            deltaMax = deltaTmp;//最大类间方差法
            threshold = j;
        }
        if (deltaTmp < deltaMax)
        {
            break;
        }
    }
    if(threshold>255)
        threshold=255;
    if(threshold<0)
        threshold=0;
  return threshold;
}

/*-------------------------------------------------------------------------------------------------------------------
  @brief     双最长白列巡线
  @param     null
  @return    null
  Sample     Longest_White_Column_Left();
  @note      最长白列巡线，寻找初始边界，丢线，最长白列等基础元素，后续读取这些变量来进行赛道识别
-------------------------------------------------------------------------------------------------------------------*/
void Longest_White_Column(void)//最长白列巡线
{
    int i, j;
    start_column=20;//最长白列的搜索区间
    end_column=MT9V03X_CSI_W-20;
    int left_border = 0, right_border = 0;//临时存储赛道位置
    Longest_White_Column_Left[0] = 0;//最长白列,[0]是最长白列的长度，[1】是第某列
    Longest_White_Column_Left[1] = 0;//最长白列,[0]是最长白列的长度，[1】是第某列
    Longest_White_Column_Right[0] = 0;//最长白列,[0]是最长白列的长度，[1】是第某列
    Longest_White_Column_Right[1] = 0;//最长白列,[0]是最长白列的长度，[1】是第某列
    Right_Lost_Time = 0;    //边界丢线数
    Left_Lost_Time  = 0;
    Boundry_Start_Left  = 0;//第一个非丢线点,常规边界起始点
    Boundry_Start_Right = 0;
    Both_Lost_Time = 0;//两边同时丢线数
 
    for (i = 0; i <=MT9V03X_CSI_H-1; i++)//数据清零
    {
        Right_Lost_Flag[i] = 0;
        Left_Lost_Flag[i] = 0;
        Left_Line[i] = 0;
        Right_Line[i] = MT9V03X_CSI_W-1;
    }
    for(i=0;i<=MT9V03X_CSI_W-1;i++)
    {
        White_Column[i] = 0;
    }

    
    

//环岛需要对最长白列范围进行限定
    //环岛3状态需要改变最长白列寻找范围
    if(Right_Island_Flag==1)//右环
    {
        if(Island_State==3)
        {
            start_column=40;
            end_column=MT9V03X_CSI_W-20;
        }
    }
    else if(Left_Island_Flag==1)//左环
    {
        if(Island_State==3)
        {
            start_column=20;
            end_column=MT9V03X_CSI_W-40;
        }
    }



    //从左到右，从下往上，遍历全图记录范围内的每一列白点数量
    for (j =start_column; j<=end_column; j++)
    {
        for (i = MT9V03X_CSI_H - 1; i >= 0; i--)
        {
            if(image_two_value[i][j] == IMG_BLACK)
                break;
            else
                White_Column[j]++;
        }
    }
 
    //从左到右找左边最长白列
    Longest_White_Column_Left[0] =0;
    for(i=start_column;i<=end_column;i++)
    {
        if (Longest_White_Column_Left[0] < White_Column[i])//找最长的那一列
        {
            Longest_White_Column_Left[0] = White_Column[i];//【0】是白列长度
            Longest_White_Column_Left[1] = i;              //【1】是下标，第j列
        }
    }
    //从右到左找右左边最长白列
    Longest_White_Column_Right[0] = 0;//【0】是白列长度
    for(i=end_column;i>=start_column;i--)//从右往左，注意条件，找到左边最长白列位置就可以停了
    {
        if (Longest_White_Column_Right[0] < White_Column[i])//找最长的那一列
        {
            Longest_White_Column_Right[0] = White_Column[i];//【0】是白列长度
            Longest_White_Column_Right[1] = i;              //【1】是下标，第j列
        }
    }
 
    Search_Stop_Line = Longest_White_Column_Left[0];//搜索截止行选取左或者右区别不大，他们两个理论上是一样的
    for (i = MT9V03X_CSI_H - 1; i >=MT9V03X_CSI_H-Search_Stop_Line; i--)//常规巡线!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
    {
        for (j = Longest_White_Column_Right[1]; j <= MT9V03X_CSI_W - 1 - 2; j++)
        {
            if (image_two_value[i][j] ==IMG_WHITE && image_two_value[i][j + 1] == IMG_BLACK && image_two_value[i][j + 2] == IMG_BLACK)//白黑黑，找到右边界
            {
                right_border = j;
                Right_Lost_Flag[i] = 0; //右丢线数组，丢线置1，不丢线置0
                break;
            }
            else if(j>=MT9V03X_CSI_W-1-2)//没找到右边界，把屏幕最右赋值给右边界
            {
                right_border = j;
                Right_Lost_Flag[i] = 1; //右丢线数组，丢线置1，不丢线置0
                break;
            }
            //lcd_drawpoint(i,j,RED);
        }
        for (j = Longest_White_Column_Left[1]; j >= 0 + 2; j--)//往左边扫描
        {
            if (image_two_value[i][j] ==IMG_WHITE && image_two_value[i][j - 1] == IMG_BLACK && image_two_value[i][j - 2] == IMG_BLACK)//黑黑白认为到达左边界
            {
                left_border = j;
                Left_Lost_Flag[i] = 0; //左丢线数组，丢线置1，不丢线置0
                break;
            }
            else if(j<=0+2)
            {
                left_border = j;//找到头都没找到边，就把屏幕最左右当做边界
                Left_Lost_Flag[i] = 1; //左丢线数组，丢线置1，不丢线置0
                break;
            }
            //lcd_drawpoint(i,j,BLUE);
        }
        Left_Line [i] = left_border;       //左边线线数组
        Right_Line[i] = right_border;      //右边线线数组
        
        
        // 临时画出中线和边线，若影响判断，后续可以删除
        //image_two_value[i][left_border+1]=128;
        //image_two_value[i][right_border-1]=128;
        //image_two_value[i][(left_border+right_border)/2]=IMG_BLACK;
        
    }
 
    for (i = MT9V03X_CSI_H - 1; i >= 0; i--)//赛道数据初步分析
    {
        if (Left_Lost_Flag[i]  == 1)//单边丢线数
            Left_Lost_Time++;
        if (Right_Lost_Flag[i] == 1)
            Right_Lost_Time++;
        if (Left_Lost_Flag[i] == 1 && Right_Lost_Flag[i] == 1)//双边丢线数
            Both_Lost_Time++;
        if (Boundry_Start_Left ==  0 && Left_Lost_Flag[i]  != 1)//记录第一个非丢线点，边界起始点
            Boundry_Start_Left = i;
        if (Boundry_Start_Right == 0 && Right_Lost_Flag[i] != 1)
            Boundry_Start_Right = i;
        Road_Wide[i]=Right_Line[i]-Left_Line[i];
    }
 
    
/*
    //环岛3状态改变边界，看情况而定，我认为理论上的最优情况是不需要这些处理的
    if(Island_State==3||Island_State==4)
    {
        if(Right_Island_Flag==1)//右环
        {
            for (i = MT9V03X_CSI_H - 1; i >= 0; i--)//右边直接写在边上
            {
                Right_Line[i]=MT9V03X_CSI_W-1;
            }
        }
        else if(Left_Island_Flag==1)//左环
        {
            for (i = MT9V03X_CSI_H - 1; i >= 0; i--)//左边直接写在边上
            {
                Left_Line[i]=0;      //右边线线数组
            }
        }
    }
*/ 
    
    
    //debug使用，屏幕显示相关参数
//    ips200_showint16(0,0, Longest_White_Column_Right[0]);//【0】是白列长度
//    ips200_showint16(0,1, Longest_White_Column_Right[1]);//【1】是下标，第j列)
//    ips200_showint16(0,2, Longest_White_Column_Left[0]);//【0】是白列长度
//    ips200_showint16(0,3, Longest_White_Column_Left[1]);//【1】是下标，第j列)
}

/*-------------------------------------------------------------------------------------------------------------------
  @brief     自定义的一个在二值图上画边线和中线的函数
  @param     null
  @return    null
  Sample     Left_Add_Line(int x1,int y1,int x2,int y2);
  @note      补的直接是边界，点最好是可信度高的,不要乱补
-------------------------------------------------------------------------------------------------------------------*/
void Show_Boundry(void)
{
    int i;
    for (i = MT9V03X_CSI_H - 1; i >=MT9V03X_CSI_H-Search_Stop_Line; i--)
    {
        image_two_value[i][Left_Line[i]+1]=IMG_BLACK;
        image_two_value[i][Right_Line[i]-1]=IMG_BLACK;
        image_two_value[i][(Left_Line[i]+Right_Line[i])/2]=IMG_BLACK;
    }
}


//下面是补线函数部分

/*-------------------------------------------------------------------------------------------------------------------
  @brief     左补线
  @param     补线的起点，终点
  @return    null
  Sample     Left_Add_Line(int x1,int y1,int x2,int y2);
  @note      补的直接是边界，点最好是可信度高的,不要乱补
-------------------------------------------------------------------------------------------------------------------*/
void Left_Add_Line(int x1,int y1,int x2,int y2)//左补线,补的是边界
{
    int i,max,a1,a2;
    int hx;
    if(x1>=MT9V03X_CSI_W-1)//起始点位置校正，排除数组越界的可能
       x1=MT9V03X_CSI_W-1;
    else if(x1<=0)
        x1=0;
     if(y1>=MT9V03X_CSI_H-1)
        y1=MT9V03X_CSI_H-1;
     else if(y1<=0)
        y1=0;
     if(x2>=MT9V03X_CSI_W-1)
        x2=MT9V03X_CSI_W-1;
     else if(x2<=0)
             x2=0;
     if(y2>=MT9V03X_CSI_H-1)
        y2=MT9V03X_CSI_H-1;
     else if(y2<=0)
             y2=0;
    a1=y1;
    a2=y2;
    if(a1>a2)//坐标互换
    {
        max=a1;
        a1=a2;
        a2=max;
    }
    for(i=a1;i<=a2;i++)//根据斜率补线即可
    {
        hx=(i-y1)*(x2-x1)/(y2-y1)+x1;
        if(hx>=MT9V03X_CSI_W)
            hx=MT9V03X_CSI_W;
        else if(hx<=0)
            hx=0;
        Left_Line[i]=hx;
    }
}

/*-------------------------------------------------------------------------------------------------------------------
  @brief     右补线
  @param     补线的起点，终点
  @return    null
  Sample     Right_Add_Line(int x1,int y1,int x2,int y2);
  @note      补的直接是边界，点最好是可信度高的，不要乱补
-------------------------------------------------------------------------------------------------------------------*/
void Right_Add_Line(int x1,int y1,int x2,int y2)//右补线,补的是边界
{
    int i,max,a1,a2;
    int hx;
    if(x1>=MT9V03X_CSI_W-1)//起始点位置校正，排除数组越界的可能
       x1=MT9V03X_CSI_W-1;
    else if(x1<=0)
        x1=0;
    if(y1>=MT9V03X_CSI_H-1)
        y1=MT9V03X_CSI_H-1;
    else if(y1<=0)
        y1=0;
    if(x2>=MT9V03X_CSI_W-1)
        x2=MT9V03X_CSI_W-1;
    else if(x2<=0)
        x2=0;
    if(y2>=MT9V03X_CSI_H-1)
        y2=MT9V03X_CSI_H-1;
    else if(y2<=0)
         y2=0;
    a1=y1;
    a2=y2;
    if(a1>a2)//坐标互换
    {
        max=a1;
        a1=a2;
        a2=max;
    }
    for(i=a1;i<=a2;i++)//根据斜率补线即可
    {
        hx=(i-y1)*(x2-x1)/(y2-y1)+x1;
        if(hx>=MT9V03X_CSI_W)
            hx=MT9V03X_CSI_W;
        else if(hx<=0)
            hx=0;
        Right_Line[i]=hx;
    }
}

/*-------------------------------------------------------------------------------------------------------------------
  @brief     左边界延长
  @param     延长起始行数，延长到某行
  @return    null
  Sample     Stop_Detect(void)
  @note      从起始点向上找5个点，算出斜率，向下延长，直至结束点
-------------------------------------------------------------------------------------------------------------------*/
void Lengthen_Left_Boundry(int start,int end)
{
    int i,t;
    float k=0;
    if(start>=MT9V03X_CSI_H-1)//起始点位置校正，排除数组越界的可能
        start=MT9V03X_CSI_H-1;
    else if(start<=0)
        start=0;
    if(end>=MT9V03X_CSI_H-1)
        end=MT9V03X_CSI_H-1;
    else if(end<=0)
        end=0;
    if(end<start)//++访问，坐标互换
    {
        t=end;
        end=start;
        start=t;
    }

    if(start<=5)//因为需要在开始点向上找3个点，对于起始点过于靠上，不能做延长，只能直接连线
    {
         Left_Add_Line(Left_Line[start],start,Left_Line[end],end);
    }

    else
    {
        k=(float)(Left_Line[start]-Left_Line[start-4])/5.0;//这里的k是1/斜率
        for(i=start;i<=end;i++)
        {
            Left_Line[i]=(int)(i-start)*k+Left_Line[start];//(x=(y-y1)*k+x1),点斜式变形
            if(Left_Line[i]>=MT9V03X_CSI_W-1)
            {
                Left_Line[i]=MT9V03X_CSI_W-1;
            }
            else if(Left_Line[i]<=0)
            {
                Left_Line[i]=0;
            }
        }
    }
}

/*-------------------------------------------------------------------------------------------------------------------
  @brief     右左边界延长
  @param     延长起始行数，延长到某行
  @return    null
  Sample     Stop_Detect(void)
  @note      从起始点向上找3个点，算出斜率，向下延长，直至结束点
-------------------------------------------------------------------------------------------------------------------*/
void Lengthen_Right_Boundry(int start,int end)
{
    int i,t;
    float k=0;
    if(start>=MT9V03X_CSI_H-1)//起始点位置校正，排除数组越界的可能
        start=MT9V03X_CSI_H-1;
    else if(start<=0)
        start=0;
    if(end>=MT9V03X_CSI_H-1)
        end=MT9V03X_CSI_H-1;
    else if(end<=0)
        end=0;
    if(end<start)//++访问，坐标互换
    {
        t=end;
        end=start;
        start=t;
    }

    if(start<=5)//因为需要在开始点向上找3个点，对于起始点过于靠上，不能做延长，只能直接连线
    {
        Right_Add_Line(Right_Line[start],start,Right_Line[end],end);
    }
    else
    {
        k=(float)(Right_Line[start]-Right_Line[start-4])/5.0;//这里的k是1/斜率
        for(i=start;i<=end;i++)
        {
            Right_Line[i]=(int)(i-start)*k+Right_Line[start];//(x=(y-y1)*k+x1),点斜式变形
            if(Right_Line[i]>=MT9V03X_CSI_W-1)
            {
                Right_Line[i]=MT9V03X_CSI_W-1;
            }
            else if(Right_Line[i]<=0)
            {
                Right_Line[i]=0;
            }
        }
    }
}


/*-------------------------------------------------------------------------------------------------------------------
  @brief     直道检测
  @param     null
  @return    null
  Sample     Straight_Detect()；
  @note      利用最长白列，边界起始点，中线起始点，
-------------------------------------------------------------------------------------------------------------------*/
void Straight_Detect(void)
{
    Straight_Flag=0;
    if(Search_Stop_Line>=65)//截止行很远
    {
        if(Boundry_Start_Left>=68&&Boundry_Start_Right>=65)//起始点靠下
        {
            if(-5<=Err&&Err<=5)//误差很小
            {
                Straight_Flag=1;//认为是直道
            }
        }
    }
}

/*-------------------------------------------------------------------------------------------------------------------
  @brief     找下面的两个拐点，供十字使用
  @param     搜索的范围起点，终点
  @return    修改两个全局变量
             Right_Down_Find=0;
             Left_Down_Find=0;
  Sample     Find_Down_Point(int start,int end)
  @note      运行完之后查看对应的变量，注意，没找到时对应变量将是0
-------------------------------------------------------------------------------------------------------------------*/
void Find_Down_Point(int start,int end)
{
    int i,t;
    Right_Down_Find=0;
    Left_Down_Find=0;
    if(start<end)
    {
        t=start;
        start=end;
        end=t;
    }
    if(start>=MT9V03X_CSI_H-1-5)//下面5行数据不稳定，不能作为边界点来判断，舍弃
        start=MT9V03X_CSI_H-1-5;
    if(end<=MT9V03X_CSI_H-Search_Stop_Line)
        end=MT9V03X_CSI_H-Search_Stop_Line;
    if(end<=5)
       end=5;
    for(i=start;i>=end;i--)
    {
        if(Left_Down_Find==0&&//只找第一个符合条件的点
           abs(Left_Line[i]-Left_Line[i+1])<=5&&//角点的阈值可以更改
           abs(Left_Line[i+1]-Left_Line[i+2])<=5&&
           abs(Left_Line[i+2]-Left_Line[i+3])<=5&&
              (Left_Line[i]-Left_Line[i-2])>=8&&
              (Left_Line[i]-Left_Line[i-3])>=15&&
              (Left_Line[i]-Left_Line[i-4])>=15)
        {
            Left_Down_Find=i;//获取行数即可
        }
        if(Right_Down_Find==0&&//只找第一个符合条件的点
           abs(Right_Line[i]-Right_Line[i+1])<=5&&//角点的阈值可以更改
           abs(Right_Line[i+1]-Right_Line[i+2])<=5&&
           abs(Right_Line[i+2]-Right_Line[i+3])<=5&&
              (Right_Line[i]-Right_Line[i-2])<=-8&&
              (Right_Line[i]-Right_Line[i-3])<=-15&&
              (Right_Line[i]-Right_Line[i-4])<=-15)
        {
            Right_Down_Find=i;
        }
        if(Left_Down_Find!=0&&Right_Down_Find!=0)//两个找到就退出
        {
            break;
        }
    }
}

/*-------------------------------------------------------------------------------------------------------------------
  @brief     找上面的两个拐点，供十字使用
  @param     搜索的范围起点，终点
  @return    修改两个全局变量
             Left_Up_Find=0;
             Right_Up_Find=0;
  Sample     Find_Up_Point(int start,int end)
  @note      运行完之后查看对应的变量，注意，没找到时对应变量将是0
-------------------------------------------------------------------------------------------------------------------*/
void Find_Up_Point(int start,int end)
{
    int i,t;
    Left_Up_Find=0;
    Right_Up_Find=0;
    if(start<end)
    {
        t=start;
        start=end;
        end=t;
    }
    if(end<=MT9V03X_CSI_H-Search_Stop_Line)
        end=MT9V03X_CSI_H-Search_Stop_Line;
    if(end<=5)//及时最长白列非常长，也要舍弃部分点，防止数组越界
        end=5;
    if(start>=MT9V03X_CSI_H-1-5)//下面5行数据不稳定，不能作为边界点来判断，舍弃
        start=MT9V03X_CSI_H-1-5;
    for(i=start;i>=end;i--)
    {
        if(Left_Up_Find==0&&//只找第一个符合条件的点
           abs(Left_Line[i]-Left_Line[i-1])<=5&&
           abs(Left_Line[i-1]-Left_Line[i-2])<=5&&
           abs(Left_Line[i-2]-Left_Line[i-3])<=5&&
              (Left_Line[i]-Left_Line[i+2])>=8&&
              (Left_Line[i]-Left_Line[i+3])>=15&&
              (Left_Line[i]-Left_Line[i+4])>=15)
        {
            Left_Up_Find=i;//获取行数即可
        }
        if(Right_Up_Find==0&&//只找第一个符合条件的点
           abs(Right_Line[i]-Right_Line[i-1])<=5&&//下面两行位置差不多
           abs(Right_Line[i-1]-Right_Line[i-2])<=5&&
           abs(Right_Line[i-2]-Right_Line[i-3])<=5&&
              (Right_Line[i]-Right_Line[i+2])<=-8&&
              (Right_Line[i]-Right_Line[i+3])<=-15&&
              (Right_Line[i]-Right_Line[i+4])<=-15)
        {
            Right_Up_Find=i;//获取行数即可
        }
        if(Left_Up_Find!=0&&Right_Up_Find!=0)//下面两个找到就出去
        {
            break;
        }
    }
    if(abs(Right_Up_Find-Left_Up_Find)>=30)//纵向撕裂过大，视为误判
    {
        Right_Up_Find=0;
        Left_Up_Find=0;
    }
}

/*-------------------------------------------------------------------------------------------------------------------
  @brief     十字检测
  @param     null
  @return    null
  Sample     Cross_Detect(void);
  @note      利用四个拐点判别函数，查找四个角点，根据找到拐点的个数决定是否补线
-------------------------------------------------------------------------------------------------------------------*/
void Cross_Detect(void)
{
    int down_search_start=0;//下点搜索开始行
    Cross_Flag=0;
    if(Island_State==0&& Junction_Flag == 0 &&Ramp_Flag==0)//与环岛互斥开
    {
        Left_Up_Find=0;
        Right_Up_Find=0;
        if(Both_Lost_Time>=10)//十字必定有双边丢线，在有双边丢线的情况下再开始找角点
        {
            Find_Up_Point( MT9V03X_CSI_H-1, 0 );
            if(Left_Up_Find==0&&Right_Up_Find==0)//只要没有同时找到两个上点，直接结束
            {
                return;
            }
        }
        if(Left_Up_Find!=0&&Right_Up_Find!=0)//找到两个上点，就找到十字了
        {
            //lcd_showint8(1,0,1);
            Cross_Flag=1;//对应标志位，便于各元素互斥掉
            down_search_start=Left_Up_Find>Right_Up_Find?Left_Up_Find:Right_Up_Find;//用两个上拐点坐标靠下者作为下点的搜索上限
            Find_Down_Point(MT9V03X_CSI_H-5,down_search_start+2);//在上拐点下2行作为下点的截止行
            if(Left_Down_Find<=Left_Up_Find)
            {
                Left_Down_Find=0;//下点不可能比上点还靠上
            }
            if(Right_Down_Find<=Right_Up_Find)
            {
                Right_Down_Find=0;//下点不可能比上点还靠上
            }
            if(Left_Down_Find!=0&&Right_Down_Find!=0)
            {//四个点都在，无脑连线，这种情况显然很少
                Left_Add_Line (Left_Line [Left_Up_Find ],Left_Up_Find ,Left_Line [Left_Down_Find ] ,Left_Down_Find);
                Right_Add_Line(Right_Line[Right_Up_Find],Right_Up_Find,Right_Line[Right_Down_Find],Right_Down_Find);
            }
            else if(Left_Down_Find==0&&Right_Down_Find!=0)//11//这里使用的都是斜率补线
            {//三个点                                     //01
                Lengthen_Left_Boundry(Left_Up_Find-1,MT9V03X_CSI_H-1);
                Right_Add_Line(Right_Line[Right_Up_Find],Right_Up_Find,Right_Line[Right_Down_Find],Right_Down_Find);
            }
            else if(Left_Down_Find!=0&&Right_Down_Find==0)//11
            {//三个点                                     //10
                Left_Add_Line (Left_Line [Left_Up_Find ],Left_Up_Find ,Left_Line [Left_Down_Find ] ,Left_Down_Find);
                Lengthen_Right_Boundry(Right_Up_Find-1,MT9V03X_CSI_H-1);
            }
            else if(Left_Down_Find==0&&Right_Down_Find==0)//11
            {//就俩上点                                   //00
                Lengthen_Left_Boundry (Left_Up_Find-1,MT9V03X_CSI_H-1);
                Lengthen_Right_Boundry(Right_Up_Find-1,MT9V03X_CSI_H-1);
            }
        }
        else
        {
            Cross_Flag=0;
        }
    }
    //角点相关变量，debug使用
    //ips200_showuint8(0,12,Cross_Flag);
//    ips200_showuint8(0,13,Island_State);
//    ips200_showuint8(50,12,Left_Up_Find);
//    ips200_showuint8(100,12,Right_Up_Find);
//    ips200_showuint8(50,13,Left_Down_Find);
//    ips200_showuint8(100,13,Right_Down_Find);
}


volatile int Ding_Point = 0;

/*-------------------------------------------------------------------------------------------------------------------
  @brief     找下面的两个拐点，供三叉使用
  @param     搜索的范围起点，终点
  @return    修改两个全局变量
             Right_Down_Find_Junction=0;
             Left_Down_Find_Junction=0;
  Sample     Find_Down_Point_Junction(int start,int end)
  @note      运行完之后查看对应的变量，注意，没找到时对应变量将是0
-------------------------------------------------------------------------------------------------------------------*/
void Find_Down_Point_Junction(int start,int end)
{
    int i,t;
    Right_Down_Find_Junction=0;
    Left_Down_Find_Junction=0;
    for(i=start_column;i<=Ding_Point;i++)
    {
      if(White_Column[i+1]-White_Column[i]>30 && Cross_Flag!=1)
      {
        Left_Down_Find_Junction=MT9V03X_CSI_H - White_Column[i];
        break;
      }
    }
    for(i=end_column;i>=Ding_Point;i--)
    {
      if(White_Column[i-1]-White_Column[i]>30 && Cross_Flag!=1)
      {
        Right_Down_Find_Junction=MT9V03X_CSI_H - White_Column[i];
        break;
      }
    }
    Mid_Junction = (Left_Line[Left_Down_Find_Junction]+Right_Line[Right_Down_Find_Junction])/2;
}


/*-------------------------------------------------------------------------------------------------------------------
  @brief     找顶点，供三叉使用
  @param     None
  @return    顶点所在列数
-------------------------------------------------------------------------------------------------------------------*/
void Find_Ding_Point(void)
{
    int i;
    Ding_Point = 0;
    for (i = start_column; i <= end_column; i++)
    {
      if(White_Column[i] >= 5 && White_Column[i - 3] >= White_Column[i] && White_Column[i+3] >= White_Column[i]
         && White_Column[i - 5] > White_Column[i] && White_Column[i+5] > White_Column[i]
           && White_Column[i - 7] > White_Column[i] && White_Column[i+7] > White_Column[i]
           && abs((White_Column[i - 5] - White_Column[i]) / 5 - (White_Column[i - 7] - White_Column[i]) / 7) <= 2
           && abs((White_Column[i + 5] - White_Column[i]) / 5 - (White_Column[i + 7] - White_Column[i]) / 7) <= 2)
      {
        Ding_Point = i;
      }
    }
    //lcd_showint8(5,5,Ding_Point);
    //lcd_showint8(6,6,White_Column[Ding_Point]);
}

/*-------------------------------------------------------------------------------------------------------------------
  @brief     三叉检测
  @param     null
  @return    null
  Sample     Cross_Detect(void);
-------------------------------------------------------------------------------------------------------------------*/
void Junction_Detect(void)
{ 
  //Find_Down_Point_Junction(MT9V03X_CSI_H,Search_Stop_Line);
  //if(Left_Down_Find_Junction)lcd_showint8(1,1,1);
  //if(Right_Down_Find_Junction)lcd_showint8(2,2,2);
  Junction_Flag = 0;
  Find_Ding_Point();
  if(Ding_Point!=0 && White_Column[Ding_Point]>=60 &&Cross_Flag!=1 && Island_State == 0)
  {
    Search_Stop_Line = White_Column[Ding_Point];
    Junction_Flag = 1;
    //lcd_showint8(1,1,Ding_Point);
    Find_Down_Point_Junction(MT9V03X_CSI_H,Search_Stop_Line);
    //if(Left_Down_Find_Junction)lcd_showint8(2,2,1);//
    //if(Right_Down_Find_Junction)lcd_showint8(3,3,2);//
    if(Left_Down_Find_Junction)
    {
      Left_Add_Line(Ding_Point, MT9V03X_CSI_H - White_Column[Ding_Point],Left_Line[Left_Down_Find_Junction] ,Left_Down_Find_Junction);
    }
    else
    {
      Lengthen_Left_Boundry(MT9V03X_CSI_H - White_Column[Ding_Point]-1,MT9V03X_CSI_H-1);
    }
    
  }
}

//////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////
//环岛相关
/*-------------------------------------------------------------------------------------------------------------------
  @brief     左赛道连续性检测
  @param     起始点，终止点
  @return    连续返回0，不连续返回断线出行数
  Sample     Continuity_Change_Left(int start,int end);
  @note      连续性的阈值设置为5，可更改
-------------------------------------------------------------------------------------------------------------------*/
int Continuity_Change_Left(int start,int end)//连续性阈值设置为5
{
    int i;
    int t;
    int continuity_change_flag=0;
    if(Left_Lost_Time>=0.9*MT9V03X_CSI_H)//大部分都丢线，没必要判断了
       return 1;
    if(Search_Stop_Line<=5)//搜所截止行很矮
       return 1;
    if(start>=MT9V03X_CSI_H-1-5)//数组越界保护
        start=MT9V03X_CSI_H-1-5;
    if(end<=5)
       end=5;
    if(start<end)//都是从下往上计算的，反了就互换一下
    {
       t=start;
       start=end;
       end=t;
    }
    
    for(i=start;i>=end;i--)
    {
       if(abs(Left_Line[i]-Left_Line[i-1])>=5)//连续判断阈值是5,可更改
       {
            continuity_change_flag=i;
            break;
       }
    }
    return continuity_change_flag;
}

/*-------------------------------------------------------------------------------------------------------------------
  @brief     右赛道连续性检测
  @param     起始点，终止点
  @return    连续返回0，不连续返回断线出行数
  Sample     continuity_change_flag=Continuity_Change_Right(int start,int end)
  @note      连续性的阈值设置为5，可更改
-------------------------------------------------------------------------------------------------------------------*/
int Continuity_Change_Right(int start,int end)
{
    int i;
    int t;
    int continuity_change_flag=0;
    if(Right_Lost_Time>=0.9*MT9V03X_CSI_H)//大部分都丢线，没必要判断了
       return 1;
    if(start>=MT9V03X_CSI_H-5)//数组越界保护
        start=MT9V03X_CSI_H-5;
    if(end<=5)
       end=5;
    if(start<end)//都是从下往上计算的，反了就互换一下
    {
       t=start;
       start=end;
       end=t;
    }

    for(i=start;i>=end;i--)
    {
        if(abs(Right_Line[i]-Right_Line[i-1])>=5)//连续性阈值是5，可更改
       {
            continuity_change_flag=i;
            break;
       }
    }
    return continuity_change_flag;
}

/*-------------------------------------------------------------------------------------------------------------------
  @brief     单调性突变检测
  @param     起始点，终止行
  @return    点所在的行数，找不到返回0
  Sample     Find_Right_Up_Point(int start,int end);
  @note      前5后5它最大（最小），那他就是角点
-------------------------------------------------------------------------------------------------------------------*/
int Monotonicity_Change_Left(int start,int end)//单调性改变，返回值是单调性改变点所在的行数
{
    int i;
    int monotonicity_change_line=0;
    if(Left_Lost_Time>=0.9*MT9V03X_CSI_H)//大部分都丢线，没有单调性判断的意义
       return monotonicity_change_line;
    if(start>=MT9V03X_CSI_H-1-5)//数组越界保护，在判断第i个点时
       start=MT9V03X_CSI_H-1-5; //要访问它前后5个点，数组两头的点要不能作为起点终点
    if(end<=5)
        end=5;
    if(start<=end)//递减计算，入口反了，直接返回0
      return monotonicity_change_line;
    for(i=start;i>=end;i--)//会读取前5后5数据，所以前面对输入范围有要求
    {
        if(Left_Line[i]==Left_Line[i+5]&&Left_Line[i]==Left_Line[i-5]&&
        Left_Line[i]==Left_Line[i+4]&&Left_Line[i]==Left_Line[i-4]&&
        Left_Line[i]==Left_Line[i+3]&&Left_Line[i]==Left_Line[i-3]&&
        Left_Line[i]==Left_Line[i+2]&&Left_Line[i]==Left_Line[i-2]&&
        Left_Line[i]==Left_Line[i+1]&&Left_Line[i]==Left_Line[i-1])
        {//一堆数据一样，显然不能作为单调转折点
            continue;
        }
        else if(Left_Line[i]>=Left_Line[i+5]&&Left_Line[i]>=Left_Line[i-5]&&
        Left_Line[i]>=Left_Line[i+4]&&Left_Line[i]>=Left_Line[i-4]&&
        Left_Line[i]>=Left_Line[i+3]&&Left_Line[i]>=Left_Line[i-3]&&
        Left_Line[i]>=Left_Line[i+2]&&Left_Line[i]>=Left_Line[i-2]&&
        Left_Line[i]>=Left_Line[i+1]&&Left_Line[i]>=Left_Line[i-1])
        {//就很暴力，这个数据是在前5，后5中最大的（可以取等），那就是单调突变点
            monotonicity_change_line=i;
            break;
        }
    }
    return monotonicity_change_line;
}

/*-------------------------------------------------------------------------------------------------------------------
  @brief     单调性突变检测
  @param     起始点，终止行
  @return    点所在的行数，找不到返回0
  Sample     Find_Right_Up_Point(int start,int end);
  @note      前5后5它最大（最小），那他就是角点
-------------------------------------------------------------------------------------------------------------------*/
int Monotonicity_Change_Right(int start,int end)//单调性改变，返回值是单调性改变点所在的行数
{
    int i;
    int monotonicity_change_line=0;

    if(Right_Lost_Time>=0.9*MT9V03X_CSI_H)//大部分都丢线，没有单调性判断的意义
        return monotonicity_change_line;
    if(start>=MT9V03X_CSI_H-1-5)//数组越界保护
        start=MT9V03X_CSI_H-1-5;
    if(end<=5)
        end=5;
    if(start<=end)
        return monotonicity_change_line;
    for(i=start;i>=end;i--)//会读取前5后5数据，所以前面对输入范围有要求
    {
        if(Right_Line[i]==Right_Line[i+5]&&Right_Line[i]==Right_Line[i-5]&&
        Right_Line[i]==Right_Line[i+4]&&Right_Line[i]==Right_Line[i-4]&&
        Right_Line[i]==Right_Line[i+3]&&Right_Line[i]==Right_Line[i-3]&&
        Right_Line[i]==Right_Line[i+2]&&Right_Line[i]==Right_Line[i-2]&&
        Right_Line[i]==Right_Line[i+1]&&Right_Line[i]==Right_Line[i-1])
        {//一堆数据一样，显然不能作为单调转折点
            continue;
        }
        else if(Right_Line[i]<=Right_Line[i+5]&&Right_Line[i]<=Right_Line[i-5]&&
        Right_Line[i]<=Right_Line[i+4]&&Right_Line[i]<=Right_Line[i-4]&&
        Right_Line[i]<=Right_Line[i+3]&&Right_Line[i]<=Right_Line[i-3]&&
        Right_Line[i]<=Right_Line[i+2]&&Right_Line[i]<=Right_Line[i-2]&&
        Right_Line[i]<=Right_Line[i+1]&&Right_Line[i]<=Right_Line[i-1])
        {//就很暴力，这个数据是在前5，后5中最大的，那就是单调突变点
            monotonicity_change_line=i;
            break;
        }
    }
    return monotonicity_change_line;
}

/*-------------------------------------------------------------------------------------------------------------------
  @brief     左下角点检测
  @param     起始点，终止点
  @return    返回角点所在的行数，找不到返回0
  Sample     Find_Left_Down_Point(int start,int end);
  @note      角点检测阈值可根据实际值更改
-------------------------------------------------------------------------------------------------------------------*/
int Find_Left_Down_Point(int start,int end)//找四个角点，返回值是角点所在的行数
{
    int i,t;
    int left_down_line=0;
    if(Left_Lost_Time>=0.9*MT9V03X_CSI_H)//大部分都丢线，没有拐点判断的意义
       return left_down_line;
    if(start<end)
    {
        t=start;
        start=end;
        end=t;
    }
    if(start>=MT9V03X_CSI_H-1-5)//下面5行数据不稳定，不能作为边界点来判断，舍弃
        start=MT9V03X_CSI_H-1-5;
    if(end<=MT9V03X_CSI_H-Search_Stop_Line)
        end=MT9V03X_CSI_H-Search_Stop_Line;
    if(end<=5)
       end=5;
    for(i=start;i>=end;i--)
    {
        if(left_down_line==0&&//只找第一个符合条件的点
           abs(Left_Line[i]-Left_Line[i+1])<=5&&//角点的阈值可以更改
           abs(Left_Line[i+1]-Left_Line[i+2])<=5&&  
           abs(Left_Line[i+2]-Left_Line[i+3])<=5&&
              (Left_Line[i]-Left_Line[i-2])>=5&&
              (Left_Line[i]-Left_Line[i-3])>=10&&
              (Left_Line[i]-Left_Line[i-4])>=10)
        {
            left_down_line=i;//获取行数即可
            break;
        }
    }
    return left_down_line;
}

/*-------------------------------------------------------------------------------------------------------------------
  @brief     右下角点检测
  @param     起始点，终止点
  @return    返回角点所在的行数，找不到返回0
  Sample     Find_Right_Down_Point(int start,int end);
  @note      角点检测阈值可根据实际值更改
-------------------------------------------------------------------------------------------------------------------*/
int Find_Right_Down_Point(int start,int end)//找四个角点，返回值是角点所在的行数
{
    int i,t;
    int right_down_line=0;
    if(Right_Lost_Time>=0.9*MT9V03X_CSI_H)//大部分都丢线，没有拐点判断的意义
        return right_down_line;
    if(start<end)
    {
        t=start;
        start=end;
        end=t;
    }
    if(start>=MT9V03X_CSI_H-1-5)//下面5行数据不稳定，不能作为边界点来判断，舍弃
        start=MT9V03X_CSI_H-1-5;
    if(end<=MT9V03X_CSI_H-Search_Stop_Line)
        end=MT9V03X_CSI_H-Search_Stop_Line;
    if(end<=5)
       end=5;
    for(i=start;i>=end;i--)
    {
        if(right_down_line==0&&//只找第一个符合条件的点
           abs(Right_Line[i]-Right_Line[i+1])<=5&&//角点的阈值可以更改
           abs(Right_Line[i+1]-Right_Line[i+2])<=5&&  
           abs(Right_Line[i+2]-Right_Line[i+3])<=5&&
              (Right_Line[i]-Right_Line[i-2])<=-5&&
              (Right_Line[i]-Right_Line[i-3])<=-10&&
              (Right_Line[i]-Right_Line[i-4])<=-10)
        {
            right_down_line=i;//获取行数即可
            break;
        }
    }
    return right_down_line;
}



/*-------------------------------------------------------------------------------------------------------------------
  @brief     画线
  @param     输入起始点，终点坐标，补一条宽度为2的黑线
  @return    null
  Sample     Draw_Line(0, 0,MT9V03X_CSI_W-1,MT9V03X_CSI_H-1);
             Draw_Line(MT9V03X_CSI_W-1, 0,0,MT9V03X_CSI_H-1);
                                    画一个大×
  @note     补的就是一条线，需要重新扫线
-------------------------------------------------------------------------------------------------------------------*/
void Draw_Line(int startX, int startY, int endX, int endY)
{
    int i,x,y;
    int start=0,end=0;
    if(startX>=MT9V03X_CSI_W-1)//限幅处理
        startX=MT9V03X_CSI_W-1;
    else if(startX<=0)
        startX=0;
    if(startY>=MT9V03X_CSI_H-1)
        startY=MT9V03X_CSI_H-1;
    else if(startY<=0)
        startY=0;
    if(endX>=MT9V03X_CSI_W-1)
        endX=MT9V03X_CSI_W-1;
    else if(endX<=0)
        endX=0;
    if(endY>=MT9V03X_CSI_H-1)
        endY=MT9V03X_CSI_H-1;
    else if(endY<=0)
        endY=0;
    if(startX==endX)//一条竖线
    {
        if (startY > endY)//互换
        {
            start=endY;
            end=startY;
        }
        for (i = start; i <= end; i++)
        {
            if(i<=1)
                i=1;
            image_two_value[i][startX]=IMG_BLACK;
            image_two_value[i-1][startX]=IMG_BLACK;
            image_two_value[i+1][startX]=IMG_BLACK;
        }
    }
    else if(startY == endY)//补一条横线
    {
        if (startX > endX)//互换
        {
            start=endX;
            end=startX;
        }
        for (i = start; i <= end; i++)
        {
            if(startY<=1)
                startY=1;
            image_two_value[startY][i]=IMG_BLACK;
            image_two_value[startY-1][i]=IMG_BLACK;
            image_two_value[startY+1][i]=IMG_BLACK;
        }
    }
    else //上面两个是水平，竖直特殊情况，下面是常见情况
    {
        if(startY>endY)//起始点矫正
        {
            start=endY;
            end=startY;
        }
        else
        {
            start=startY;
            end=endY;
        }
        for (i = start; i <= end; i++)//纵向补线，保证每一行都有黑点
        {
            x =(int)(startX+(endX-startX)*(i-startY)/(endY-startY));//两点式变形
            if(x>=MT9V03X_CSI_W-1)
                x=MT9V03X_CSI_W-1;
            else if (x<=1)
                x=1;
            image_two_value[i][x] = IMG_BLACK;
            image_two_value[i][x-1] = IMG_BLACK;
        }
        if(startX>endX)
        {
            start=endX;
            end=startX;
        }
        else
        {
            start=startX;
            end=endX;
        }
        for (i = start; i <= end; i++)//横向补线，保证每一列都有黑点
        {
 
            y =(int)(startY+(endY-startY)*(i-startX)/(endX-startX));//两点式变形
            if(y>=MT9V03X_CSI_H-1)
                y=MT9V03X_CSI_H-1;
            else if (y<=0)
                y=0;
            image_two_value[y][i] = IMG_BLACK;
        }
    }
}



/*-------------------------------------------------------------------------------------------------------------------
  @brief     根据斜率划线
  @param     输入斜率，定点，画一条黑线
  @return    null
  Sample     K_Draw_Line(k, 20,MT9V03X_CSI_H-1 ,0)
  @note      补的就是一条线，需要重新扫线
-------------------------------------------------------------------------------------------------------------------*/
void K_Draw_Line(float k, int startX, int startY,int endY)
{
    int endX=0;

    if(startX>=MT9V03X_CSI_W-1)//限幅处理
        startX=MT9V03X_CSI_W-1;
    else if(startX<=0)
        startX=0;
    if(startY>=MT9V03X_CSI_H-1)
        startY=MT9V03X_CSI_H-1;
    else if(startY<=0)
        startY=0;
    if(endY>=MT9V03X_CSI_H-1)
        endY=MT9V03X_CSI_H-1;
    else if(endY<=0)
        endY=0;
    endX=(int)((endY-startY)/k+startX);//(y-y1)=k(x-x1)变形，x=(y-y1)/k+x1
    Draw_Line(startX,startY,endX,endY);
}

/*-------------------------------------------------------------------------------------------------------------------
  @brief     通过斜率，定点补线--
  @param     k       输入斜率
             startY  输入起始点纵坐标
             endY    结束点纵坐标
  @return    null
  Sample     K_Add_Boundry_Right(float k,int startY,int endY);
  @note      补得线直接贴在边线上
-------------------------------------------------------------------------------------------------------------------*/
void K_Add_Boundry_Right(float k,int startX,int startY,int endY)
{
    int i,t;
    if(startY>=MT9V03X_CSI_H-1)
        startY=MT9V03X_CSI_H-1;
    else if(startY<=0)
        startY=0;
    if(endY>=MT9V03X_CSI_H-1)
        endY=MT9V03X_CSI_H-1;
    else if(endY<=0)
        endY=0;
    if(startY<endY)//--操作，start需要大
    {
        t=startY;
        startY=endY;
        endY=t;
    }

    for(i=startY;i>=endY;i--)
    {
        Right_Line[i]=(int)((i-startY)/k+startX);//(y-y1)=k(x-x1)变形，x=(y-y1)/k+x1
        if(Right_Line[i]>=MT9V03X_CSI_W-1)
        {
            Right_Line[i]=MT9V03X_CSI_W-1;
        }
        else if(Right_Line[i]<=0)
        {
            Right_Line[i]=0;
        }
    }
}

/*-------------------------------------------------------------------------------------------------------------------
  @brief     左上角点检测
  @param     起始点，终止点
  @return    返回角点所在的行数，找不到返回0
  Sample     Find_Left_Up_Point(int start,int end);
  @note      角点检测阈值可根据实际值更改
-------------------------------------------------------------------------------------------------------------------*/
int Find_Left_Up_Point(int start,int end)//找四个角点，返回值是角点所在的行数
{
    int i,t;
    int left_up_line=0;
    if(Left_Lost_Time>=0.9*MT9V03X_CSI_H)//大部分都丢线，没有拐点判断的意义
       return left_up_line;
    if(start<end)
    {
        t=start;
        start=end;
        end=t;
    }
    if(end<=MT9V03X_CSI_H-Search_Stop_Line)//搜索截止行往上的全都不判
        end=MT9V03X_CSI_H-Search_Stop_Line;
    if(end<=5)//及时最长白列非常长，也要舍弃部分点，防止数组越界
        end=5;
    if(start>=MT9V03X_CSI_H-1-5)
        start=MT9V03X_CSI_H-1-5;
    for(i=start;i>=end;i--)
    {
        if(left_up_line==0&&//只找第一个符合条件的点
           abs(Left_Line[i]-Left_Line[i-1])<=5&&
           abs(Left_Line[i-1]-Left_Line[i-2])<=5&&  
           abs(Left_Line[i-2]-Left_Line[i-3])<=5&&
              (Left_Line[i]-Left_Line[i+2])>=8&& 
              (Left_Line[i]-Left_Line[i+3])>=15&&
              (Left_Line[i]-Left_Line[i+4])>=15)
        {
            left_up_line=i;//获取行数即可
            break;
        }
    }
    return left_up_line;//如果是MT9V03X_H-1，说明没有这么个拐点
}

/*-------------------------------------------------------------------------------------------------------------------
  @brief     右上角点检测
  @param     起始点，终止点
  @return    返回角点所在的行数，找不到返回0
  Sample     Find_Right_Up_Point(int start,int end);
  @note      角点检测阈值可根据实际值更改
-------------------------------------------------------------------------------------------------------------------*/
int Find_Right_Up_Point(int start,int end)//找四个角点，返回值是角点所在的行数
{
    int i,t;
    int right_up_line=0;
    if(Right_Lost_Time>=0.9*MT9V03X_CSI_H)//大部分都丢线，没有拐点判断的意义
        return right_up_line;
    if(start<end)
    {
        t=start;
        start=end;
        end=t;
    }    
    if(end<=MT9V03X_CSI_H-Search_Stop_Line)//搜索截止行往上的全都不判
        end=MT9V03X_CSI_H-Search_Stop_Line;
    if(end<=5)//及时最长白列非常长，也要舍弃部分点，防止数组越界
        end=5;
    if(start>=MT9V03X_CSI_H-1-5)
        start=MT9V03X_CSI_H-1-5;
    for(i=start;i>=end;i--)
    {
        if(right_up_line==0&&//只找第一个符合条件的点
           abs(Right_Line[i]-Right_Line[i-1])<=5&&//下面两行位置差不多
           abs(Right_Line[i-1]-Right_Line[i-2])<=5&&  
           abs(Right_Line[i-2]-Right_Line[i-3])<=5&&
              (Right_Line[i]-Right_Line[i+2])<=-8&&
              (Right_Line[i]-Right_Line[i+3])<=-15&&
              (Right_Line[i]-Right_Line[i+4])<=-15)
        {
            right_up_line=i;//获取行数即可
            break;
        }
    }
    return right_up_line;
}

void Island_Detect(void)
{ 
    static float k=0;//3和5状态的k
    static int island_state_5_down[2]={0};//状态5时即将离开环岛，左右边界边最低点，[0]存y，第某行，{1}存x，第某列
    static int island_state_3_up[2]={0};//状态3时即将进入环岛用，左右上面角点[0]存y，第某行，{1}存x，第某列
    static int left_down_guai[2]={0};//四个拐点的坐标存储，[0]存y，第某行，{1}存x，第某列
    static int right_down_guai[2]={0};//四个拐点的坐标存储，[0]存y，第某行，{1}存x，第某列
    int monotonicity_change_line[2];//单调性改变点坐标，[0]寸某行，[1]寸某列
    int monotonicity_change_left_flag=0;//不转折是0
    int monotonicity_change_right_flag=0;//不转折是0
    int continuity_change_right_flag=0; //连续是0
    int continuity_change_left_flag=0;  //连续是0
    //以下是常规判断法
    continuity_change_left_flag=Continuity_Change_Left(MT9V03X_CSI_H-1-5,30);//连续性判断
    continuity_change_right_flag=Continuity_Change_Right(MT9V03X_CSI_H-1-5,30);
    monotonicity_change_right_flag=Monotonicity_Change_Right(MT9V03X_CSI_H-1-10,30);
    monotonicity_change_left_flag=Monotonicity_Change_Left(MT9V03X_CSI_H-1-10,30);
    
   // lcd_showint8(1,1,monotonicity_change_right_flag);
   // lcd_showint8(2,2,  continuity_change_left_flag);
   // lcd_showint8(3,3,continuity_change_right_flag);
   // lcd_showint8(4,4,Left_Lost_Time);
   //lcd_showint8(5,5,Right_Lost_Time);
   
    
   // lcd_showint8(7,7,
    
    if(Junction_Flag == 0 && Cross_Flag==0&&Island_State==0&&Ramp_Flag==0)//&&Ramp_Flag==0
    {
        /*continuity_change_left_flag=Continuity_Change_Left(MT9V03X_CSI_H-1-5,10);//连续性判断
        continuity_change_right_flag=Continuity_Change_Right(MT9V03X_CSI_H-1-5,10);*/
        if(Left_Island_Flag==0)//左环
        {
          
            if(monotonicity_change_right_flag==0&&//右边是单调的
               continuity_change_left_flag!=0//左边是不连续的
               &&continuity_change_right_flag==0//左环岛右边是连续的
               &&Left_Lost_Time>=10&& //左边丢线很多
               Left_Lost_Time<=50&& //也不能全丢了
               Right_Lost_Time<=10&&//右边丢线较少*/
               Search_Stop_Line>=MT9V03X_CSI_H*0.8&&//搜索截止行看到很远
               Boundry_Start_Left>=MT9V03X_CSI_H-20&&Boundry_Start_Right>=MT9V03X_CSI_H-20&&//边界起始点靠下
               Both_Lost_Time<=10)//双边丢线少
            {
                left_down_guai[0]=Find_Left_Down_Point(MT9V03X_CSI_H-1,20);//找左下角点
                if(left_down_guai[0]>=30)//条件1很松，在这里判断拐点，位置不对，则是误判，跳出
                {
                    Island_State=1;
                    Left_Island_Flag=1;
                }
                else//误判，归零
                {
                    Island_State=0;
                    Left_Island_Flag=0;
                }
            }
        }
        if(Right_Island_Flag==0)//右环
        {
            if(monotonicity_change_left_flag==0&&
               continuity_change_left_flag==0&&//右环岛左边是连续的
               continuity_change_right_flag!=1&&//右边是不连续的
               Right_Lost_Time>=10&&           //右丢线多
               Right_Lost_Time<=50&&           //右丢线不能太多
               Left_Lost_Time<=10&&            //左丢线少
               Search_Stop_Line>=MT9V03X_CSI_H*0.95&&//搜索截止行看到很远
               Boundry_Start_Left>=MT9V03X_CSI_H-20&&Boundry_Start_Right>=MT9V03X_CSI_H-20&&//边界起始点靠下
               Both_Lost_Time<=10)
            {
                right_down_guai[0]=Find_Right_Down_Point(MT9V03X_CSI_H-1,20);//右下点
                if(right_down_guai[0]>=30)//条件1很松，在这里加判拐点，位置不对，则是误判，跳出
                {
                    Island_State=1;
                    Right_Island_Flag=1;
                }
                else
                {
                    Island_State=0;
                    Right_Island_Flag=0;
                }
            }
        }
    }
    if(Left_Island_Flag==1)//1状态下拐点还在，没丢线
    {
        if(Island_State==1)
        {
            monotonicity_change_line[0]=Monotonicity_Change_Left(30,5);//寻找单调性改变点
            monotonicity_change_line[1]=Left_Line[monotonicity_change_line[0]];
            Left_Add_Line((int)(monotonicity_change_line[1]*0.15),MT9V03X_CSI_H-1,monotonicity_change_line[1],monotonicity_change_line[0]);
            if((Island_State==1)&&(Boundry_Start_Left<65))//下方当丢线时候进2
            {
                Island_State=2;
            }
        }
        else if(Island_State==2)//下方角点消失，2状态时下方应该是丢线，上面是弧线
        {
            monotonicity_change_line[0]=Monotonicity_Change_Left(90,10);//寻找单调性改变点
            monotonicity_change_line[1]=Left_Line[monotonicity_change_line[0]];
            Left_Add_Line((int)(monotonicity_change_line[1]*0.1),MT9V03X_CSI_H-1,monotonicity_change_line[1],monotonicity_change_line[0]);
            lcd_showint8(5,5,Boundry_Start_Left);
            lcd_showint8(4,4,monotonicity_change_line[0]);
            if(Island_State==2 && monotonicity_change_line[0]>65)//左下角再不丢线进3
            {
                Island_State=3;//下方丢线，说明大弧线已经下来了
            }
        }
        
        else if(Island_State==3)//3状态准备进环，寻找上拐点，连线
        {
            int flag_out_state3 = 0;
            if(k!=0)
            {
               // int max = 0;
                for(int i = MT9V03X_CSI_H - 5; i >= MT9V03X_CSI_H - 50; i --)
                {
                  if(abs(Right_Line[i]-Right_Line[i-1])<=5&&//下面两行位置差不多
           abs(Right_Line[i-1]-Right_Line[i-2])<=5&&
           abs(Right_Line[i-2]-Right_Line[i-3])<=5&&
              (Right_Line[i]-Right_Line[i+2])<=-3&&
              (Right_Line[i]-Right_Line[i+3])<=-3&&
              (Right_Line[i]-Right_Line[i+4])<=-3)flag_out_state3 =1;
                }
                if(Right_Lost_Time<=3 && Left_Lost_Time<=3)flag_out_state3 =1;
                //lcd_showint8(1,1,max);
                K_Draw_Line(k,MT9V03X_CSI_W-30,MT9V03X_CSI_H-1,0);//k是刚刚算出来的，静态变量存着
                Longest_White_Column();//刷新边界数据
            }
            else
            {
                Left_Up_Guai[0]=Find_Left_Up_Point(60,5);//找左上拐点!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
                Left_Up_Guai[1]=Left_Line[Left_Up_Guai[0]];
                /*if (Left_Up_Guai[0]<5)//这里改过啊!!!!
                {
                    Island_State=0;
                    Left_Island_Flag=0;
                }*/
                //lcd_showint8(4,4,Left_Up_Guai[1]);
                //lcd_showint8(5,5,Left_Up_Guai[0]);
                if(k==0&&(40<=Left_Up_Guai[0]&&Left_Up_Guai[0]<60)&&(50<Left_Up_Guai[1]&&Left_Up_Guai[1]<110))//拐点出现在一定范围内，认为是拐点出现!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
                {
                    island_state_3_up[0]= Left_Up_Guai[0];
                    island_state_3_up[1]= Left_Up_Guai[1];
                    k=(float)((float)(MT9V03X_CSI_H-island_state_3_up[0])/(float)(MT9V03X_CSI_W-20-island_state_3_up[1]));
                    K_Draw_Line(k,MT9V03X_CSI_W-30,MT9V03X_CSI_H-1,0);//记录下第一次上点出现时位置，针对这个环岛拉一条死线，入环
                    Longest_White_Column();//刷新边界数据
                }
            }
            if((Island_State==3)&&(k!=0)&&flag_out_state3==1)
            {
               k=0;//斜率清零
               Island_State=33;//这一步时候顺便调整了最长白列的搜索范围
               Longest_White_Column();//最长白列
            }
        }
        else if(Island_State == 33)
        {
          lcd_showint8(2,2,Right_Lost_Time);
          lcd_showint8(3,3,Left_Lost_Time);
          if(Right_Lost_Time==0 && Left_Lost_Time>=20)Island_State =4;
        }
        else if(Island_State==4)//4状态完全进去环岛了
        {
          if(Right_Lost_Time >= 8)
          {
             monotonicity_change_line[0]=Monotonicity_Change_Right(MT9V03X_CSI_H-10,10);//单调性改变
             monotonicity_change_line[1]=Right_Line[monotonicity_change_line[0]];
             if((Island_State==4)&&(MT9V03X_CSI_H - 65<=monotonicity_change_line[0]&&monotonicity_change_line[0]<=MT9V03X_CSI_H - 10&&monotonicity_change_line[1]>=MT9V03X_CSI_H - 40))//单调点靠下，进去5
             {//monotonicity_change_line[1]>=90&&
                 island_state_5_down[0]=MT9V03X_CSI_H-1;
                 island_state_5_down[1]=Right_Line[MT9V03X_CSI_H-1];
                 k=(float)((float)(MT9V03X_CSI_H-monotonicity_change_line[0])/(float)(island_state_5_down[1]-monotonicity_change_line[1]));
                 K_Add_Boundry_Right(k,island_state_5_down[1],island_state_5_down[0],0);//和状态3一样，记住斜率
                 Island_State=5;
             }
          }
             
        }

        else if(Island_State==5)//出环
        {
            monotonicity_change_line[0]=Monotonicity_Change_Right(MT9V03X_CSI_H-10,10);//单调性改变
            monotonicity_change_line[1]=Right_Line[monotonicity_change_line[0]];
            K_Add_Boundry_Right(k,island_state_5_down[1],island_state_5_down[0],0);
            lcd_showint8(5,5,monotonicity_change_line[0]);
            if((Island_State==5)&&(monotonicity_change_line[0]>MT9V03X_CSI_H-20))//右边先丢线
            {
                Island_State=6;
            }
        }
         else if(Island_State==6)//还在出
        {
            K_Add_Boundry_Right(k,island_state_5_down[1],island_state_5_down[0],0);
            if((Island_State==6)&&(Boundry_Start_Right>MT9V03X_CSI_H-10))//右边不丢线
            {//
                k=0;
                Island_State=7;
            }
        }
        else if(Island_State==7)
        {
          for(int i = MT9V03X_CSI_H - 1; i>=MT9V03X_CSI_H - Search_Stop_Line;i--)
          {
              Left_Line[i] = Right_Line[i] - Standard_Road_Wide[i];
           }
          if(Right_Lost_Time <= 3 && Left_Lost_Time <= 3)
          {
            Island_State = 9;
          }
        }
        /*else if(Island_State==7)//基本出去了，在寻找拐点，准备离开环岛状态
        {
            Left_Up_Guai[0]=Find_Left_Up_Point(MT9V03X_CSI_H-10,0);//获取左上点坐标，坐标点合理去8
            Left_Up_Guai[1]=Left_Line[Left_Up_Guai[0]];
            if((Island_State==7)&&(Left_Up_Guai[1]<=100)&&(5<=Left_Up_Guai[0]&&Left_Up_Guai[0]<=MT9V03X_CSI_H-20))//注意这里，对横纵坐标都有要求
            {
                Island_State=8;//基本上找到拐点就去8
            }
        }
        else if(Island_State==8)//连线，出环最后一步
        {
            Left_Up_Guai[0]=Find_Left_Up_Point(MT9V03X_CSI_H-1,10);//获取左上点坐标
            Left_Up_Guai[1]=Left_Line[Left_Up_Guai[0]];
            Lengthen_Left_Boundry(Left_Up_Guai[0]-1,MT9V03X_CSI_H-1);
            if((Island_State==8)&&(Left_Up_Guai[0]>=MT9V03X_CSI_H-20))//当拐点靠下时候，认为出环了，环岛结束
            {//要么拐点靠下，要么拐点丢了，切下方不丢线，认为环岛结束了
                Island_State=9;//8时候环岛基本结束了，为了防止连续判环，8后会进9，大概几十毫秒后归零，
                
            }
        }*/
        else if(Island_State==9)
        {
          if(Right_Lost_Time <= 2 && Left_Lost_Time <= 2)
          {
            Island_State = 0;
            Left_Island_Flag=0;
          }
        }
    }
}
