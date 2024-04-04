#include "headfile.h"
#include "IMAGEPROCESS.h"
#include  <math.h>

// �����ǻ�����ͼ���ֵ���ͱ߽�������ȡ

const uint8 Standard_Road_Wide[MT9V03X_CSI_H]=//��׼����
{8,9,10,11,12,13,14,15,16,17,18,19,20,21,22,23,24,25,26,27,28,29,30,31,32,33,34,35,36,37,38,39,40,41,42,43,44,45,46,47,48,49,50,51,52,53,54,55,56,57,58,59,60,61,62,63,64,65,66,
67,68,69,70,71,72,73,74,75,76,
77,78,79,80,81,82,83,84,85,86,
87,88,89,90,91,92,93,94,95,96,
97,98,99,100,101,102,103,104,105,106,
107,108,109,110,111,112,113,114,115,116,
117,118,119,120,121,122,123,124,125,126,127};

uint8 image_two_value[MT9V03X_CSI_H][MT9V03X_CSI_W];//��ֵ�����ԭ����
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

int start_column=5;//����е���������
int end_column=MT9V03X_CSI_W-5;

//�����������жϳ��򲿷�

//ֱ��
volatile uint8 Straight_Flag;// ֱ�����

//�µ�
volatile int Ramp_Flag=0;//�µ���־

//ʮ��
volatile int Cross_Flag=0;
volatile int Left_Down_Find=0; //ʮ��ʹ�ã��ҵ�����������û�ҵ�����0
volatile int Left_Up_Find=0;   //�ĸ��յ��־
volatile int Right_Down_Find=0;
volatile int Right_Up_Find=0;

//����
volatile int Junction_Flag=0;
volatile int Left_Down_Find_Junction=0; //����ʹ�ã��ҵ�����������û�ҵ�����0
volatile int Right_Down_Find_Junction=0;
volatile int Mid_Junction=0;

//��������
volatile int Island_State=0;     //����״̬��־
volatile int Left_Island_Flag=0; //���һ�����־
volatile int Right_Island_Flag=0;//���һ�����־
volatile int Left_Up_Guai[2];    //�ĸ��յ������洢��[0]��y����ĳ�У�{1}��x����ĳ��
volatile int Right_Up_Guai[2];   //�ĸ��յ������洢��[0]��y����ĳ�У�{1}��x����ĳ��

extern volatile float Err;//���߼�Ȩ���

/*-------------------------------------------------------------------------------------------------------------------
  @brief     ͼ���ֵ��������
  @param     ��ֵ����ֵ
  @return    NULL
  Sample     Image_Binarization(Threshold);//ͼ���ֵ��
  @note      ��ֵ����ֱ�ӷ���image_two_value[i][j]������鼴��
-------------------------------------------------------------------------------------------------------------------*/
void Image_Binarization(int threshold)//ͼ���ֵ��
{
    uint16 i,j;
    for(i=0;i<MT9V03X_CSI_H;i++)
    {
        for(j=0;j<MT9V03X_CSI_W;j++)//�Ҷ�ͼ������ֻ���жϣ������и��ģ���ֵ��ͼ���������������
        {
            if(mt9v03x_csi_image[i][j]>=threshold )
                image_two_value[i][j]=IMG_WHITE;//��
            else
                image_two_value[i][j]=IMG_BLACK;//��
        }
    }
}

/*-------------------------------------------------------------------------------------------------------------------
  @brief     ��ͨ�������ֵ
  @param     image       ͼ������
             width       �� �����
             height      �У�����
  @return    threshold   ����int���͵ĵ���ֵ
  Sample     threshold=my_adapt_threshold(mt9v03x_image[0],MT9V03X_W, MT9V03X_H);//��ͨ���
  @note      ��˵û��ɽ�����죬�Ҹо��������𲻴�
-------------------------------------------------------------------------------------------------------------------*/
int my_adapt_threshold(uint8 *image,uint16 width, uint16 height)   //����㷨��ע�������ֵ��һ��Ҫ��ԭͼ��
{
    #define GrayScale 256
    int pixelCount[GrayScale];
    float pixelPro[GrayScale];
    int i, j;
    int pixelSum = width * height/4;
    int  threshold = 0;
    uint8* data = image;  //ָ���������ݵ�ָ��
    for (i = 0; i < GrayScale; i++)
    {
        pixelCount[i] = 0;
        pixelPro[i] = 0;
    }
    uint32 gray_sum=0;
    for (i = 0; i < height; i+=2)//ͳ�ƻҶȼ���ÿ������������ͼ���еĸ���
    {
        for (j = 0; j <width; j+=2)
        {
            pixelCount[(int)data[i * width + j]]++;  //����ǰ�ĵ������ֵ��Ϊ����������±�
            gray_sum+=(int)data[i * width + j];       //�Ҷ�ֵ�ܺ�
        }
    }
    for (i = 0; i < GrayScale; i++) //����ÿ������ֵ�ĵ�������ͼ���еı���
    {
        pixelPro[i] = (float)pixelCount[i] / pixelSum;
    }
    float w0, w1, u0tmp, u1tmp, u0, u1, u, deltaTmp, deltaMax = 0;
    w0 = w1 = u0tmp = u1tmp = u0 = u1 = u = deltaTmp = 0;
    for (j = 0; j < GrayScale; j++)//�����Ҷȼ�[0,255]
    {
        w0 += pixelPro[j];  //��������ÿ���Ҷ�ֵ�����ص���ռ����֮��   ���������ֵı���
        u0tmp += j * pixelPro[j];  //�������� ÿ���Ҷ�ֵ�ĵ�ı��� *�Ҷ�ֵ
        w1=1-w0;
        u1tmp=gray_sum/pixelSum-u0tmp;
        u0 = u0tmp / w0;              //����ƽ���Ҷ�
        u1 = u1tmp / w1;              //ǰ��ƽ���Ҷ�
        u = u0tmp + u1tmp;            //ȫ��ƽ���Ҷ�
        deltaTmp = w0 * pow((u0 - u), 2) + w1 * pow((u1 - u), 2);//ƽ��
        if (deltaTmp > deltaMax)
        {
            deltaMax = deltaTmp;//�����䷽�
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
  @brief     ˫�����Ѳ��
  @param     null
  @return    null
  Sample     Longest_White_Column_Left();
  @note      �����Ѳ�ߣ�Ѱ�ҳ�ʼ�߽磬���ߣ�����еȻ���Ԫ�أ�������ȡ��Щ��������������ʶ��
-------------------------------------------------------------------------------------------------------------------*/
void Longest_White_Column(void)//�����Ѳ��
{
    int i, j;
    start_column=20;//����е���������
    end_column=MT9V03X_CSI_W-20;
    int left_border = 0, right_border = 0;//��ʱ�洢����λ��
    Longest_White_Column_Left[0] = 0;//�����,[0]������еĳ��ȣ�[1���ǵ�ĳ��
    Longest_White_Column_Left[1] = 0;//�����,[0]������еĳ��ȣ�[1���ǵ�ĳ��
    Longest_White_Column_Right[0] = 0;//�����,[0]������еĳ��ȣ�[1���ǵ�ĳ��
    Longest_White_Column_Right[1] = 0;//�����,[0]������еĳ��ȣ�[1���ǵ�ĳ��
    Right_Lost_Time = 0;    //�߽綪����
    Left_Lost_Time  = 0;
    Boundry_Start_Left  = 0;//��һ���Ƕ��ߵ�,����߽���ʼ��
    Boundry_Start_Right = 0;
    Both_Lost_Time = 0;//����ͬʱ������
 
    for (i = 0; i <=MT9V03X_CSI_H-1; i++)//��������
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

    
    

//������Ҫ������з�Χ�����޶�
    //����3״̬��Ҫ�ı������Ѱ�ҷ�Χ
    if(Right_Island_Flag==1)//�һ�
    {
        if(Island_State==3)
        {
            start_column=40;
            end_column=MT9V03X_CSI_W-20;
        }
    }
    else if(Left_Island_Flag==1)//��
    {
        if(Island_State==3)
        {
            start_column=20;
            end_column=MT9V03X_CSI_W-40;
        }
    }



    //�����ң��������ϣ�����ȫͼ��¼��Χ�ڵ�ÿһ�а׵�����
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
 
    //����������������
    Longest_White_Column_Left[0] =0;
    for(i=start_column;i<=end_column;i++)
    {
        if (Longest_White_Column_Left[0] < White_Column[i])//�������һ��
        {
            Longest_White_Column_Left[0] = White_Column[i];//��0���ǰ��г���
            Longest_White_Column_Left[1] = i;              //��1�����±꣬��j��
        }
    }
    //���ҵ���������������
    Longest_White_Column_Right[0] = 0;//��0���ǰ��г���
    for(i=end_column;i>=start_column;i--)//��������ע���������ҵ���������λ�þͿ���ͣ��
    {
        if (Longest_White_Column_Right[0] < White_Column[i])//�������һ��
        {
            Longest_White_Column_Right[0] = White_Column[i];//��0���ǰ��г���
            Longest_White_Column_Right[1] = i;              //��1�����±꣬��j��
        }
    }
 
    Search_Stop_Line = Longest_White_Column_Left[0];//������ֹ��ѡȡ����������𲻴�����������������һ����
    for (i = MT9V03X_CSI_H - 1; i >=MT9V03X_CSI_H-Search_Stop_Line; i--)//����Ѳ��!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
    {
        for (j = Longest_White_Column_Right[1]; j <= MT9V03X_CSI_W - 1 - 2; j++)
        {
            if (image_two_value[i][j] ==IMG_WHITE && image_two_value[i][j + 1] == IMG_BLACK && image_two_value[i][j + 2] == IMG_BLACK)//�׺ںڣ��ҵ��ұ߽�
            {
                right_border = j;
                Right_Lost_Flag[i] = 0; //�Ҷ������飬������1����������0
                break;
            }
            else if(j>=MT9V03X_CSI_W-1-2)//û�ҵ��ұ߽磬����Ļ���Ҹ�ֵ���ұ߽�
            {
                right_border = j;
                Right_Lost_Flag[i] = 1; //�Ҷ������飬������1����������0
                break;
            }
            //lcd_drawpoint(i,j,RED);
        }
        for (j = Longest_White_Column_Left[1]; j >= 0 + 2; j--)//�����ɨ��
        {
            if (image_two_value[i][j] ==IMG_WHITE && image_two_value[i][j - 1] == IMG_BLACK && image_two_value[i][j - 2] == IMG_BLACK)//�ںڰ���Ϊ������߽�
            {
                left_border = j;
                Left_Lost_Flag[i] = 0; //�������飬������1����������0
                break;
            }
            else if(j<=0+2)
            {
                left_border = j;//�ҵ�ͷ��û�ҵ��ߣ��Ͱ���Ļ�����ҵ����߽�
                Left_Lost_Flag[i] = 1; //�������飬������1����������0
                break;
            }
            //lcd_drawpoint(i,j,BLUE);
        }
        Left_Line [i] = left_border;       //�����������
        Right_Line[i] = right_border;      //�ұ���������
        
        
        // ��ʱ�������ߺͱ��ߣ���Ӱ���жϣ���������ɾ��
        //image_two_value[i][left_border+1]=128;
        //image_two_value[i][right_border-1]=128;
        //image_two_value[i][(left_border+right_border)/2]=IMG_BLACK;
        
    }
 
    for (i = MT9V03X_CSI_H - 1; i >= 0; i--)//�������ݳ�������
    {
        if (Left_Lost_Flag[i]  == 1)//���߶�����
            Left_Lost_Time++;
        if (Right_Lost_Flag[i] == 1)
            Right_Lost_Time++;
        if (Left_Lost_Flag[i] == 1 && Right_Lost_Flag[i] == 1)//˫�߶�����
            Both_Lost_Time++;
        if (Boundry_Start_Left ==  0 && Left_Lost_Flag[i]  != 1)//��¼��һ���Ƕ��ߵ㣬�߽���ʼ��
            Boundry_Start_Left = i;
        if (Boundry_Start_Right == 0 && Right_Lost_Flag[i] != 1)
            Boundry_Start_Right = i;
        Road_Wide[i]=Right_Line[i]-Left_Line[i];
    }
 
    
/*
    //����3״̬�ı�߽磬���������������Ϊ�����ϵ���������ǲ���Ҫ��Щ�����
    if(Island_State==3||Island_State==4)
    {
        if(Right_Island_Flag==1)//�һ�
        {
            for (i = MT9V03X_CSI_H - 1; i >= 0; i--)//�ұ�ֱ��д�ڱ���
            {
                Right_Line[i]=MT9V03X_CSI_W-1;
            }
        }
        else if(Left_Island_Flag==1)//��
        {
            for (i = MT9V03X_CSI_H - 1; i >= 0; i--)//���ֱ��д�ڱ���
            {
                Left_Line[i]=0;      //�ұ���������
            }
        }
    }
*/ 
    
    
    //debugʹ�ã���Ļ��ʾ��ز���
//    ips200_showint16(0,0, Longest_White_Column_Right[0]);//��0���ǰ��г���
//    ips200_showint16(0,1, Longest_White_Column_Right[1]);//��1�����±꣬��j��)
//    ips200_showint16(0,2, Longest_White_Column_Left[0]);//��0���ǰ��г���
//    ips200_showint16(0,3, Longest_White_Column_Left[1]);//��1�����±꣬��j��)
}

/*-------------------------------------------------------------------------------------------------------------------
  @brief     �Զ����һ���ڶ�ֵͼ�ϻ����ߺ����ߵĺ���
  @param     null
  @return    null
  Sample     Left_Add_Line(int x1,int y1,int x2,int y2);
  @note      ����ֱ���Ǳ߽磬������ǿ��Ŷȸߵ�,��Ҫ�Ҳ�
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


//�����ǲ��ߺ�������

/*-------------------------------------------------------------------------------------------------------------------
  @brief     ����
  @param     ���ߵ���㣬�յ�
  @return    null
  Sample     Left_Add_Line(int x1,int y1,int x2,int y2);
  @note      ����ֱ���Ǳ߽磬������ǿ��Ŷȸߵ�,��Ҫ�Ҳ�
-------------------------------------------------------------------------------------------------------------------*/
void Left_Add_Line(int x1,int y1,int x2,int y2)//����,�����Ǳ߽�
{
    int i,max,a1,a2;
    int hx;
    if(x1>=MT9V03X_CSI_W-1)//��ʼ��λ��У�����ų�����Խ��Ŀ���
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
    if(a1>a2)//���껥��
    {
        max=a1;
        a1=a2;
        a2=max;
    }
    for(i=a1;i<=a2;i++)//����б�ʲ��߼���
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
  @brief     �Ҳ���
  @param     ���ߵ���㣬�յ�
  @return    null
  Sample     Right_Add_Line(int x1,int y1,int x2,int y2);
  @note      ����ֱ���Ǳ߽磬������ǿ��Ŷȸߵģ���Ҫ�Ҳ�
-------------------------------------------------------------------------------------------------------------------*/
void Right_Add_Line(int x1,int y1,int x2,int y2)//�Ҳ���,�����Ǳ߽�
{
    int i,max,a1,a2;
    int hx;
    if(x1>=MT9V03X_CSI_W-1)//��ʼ��λ��У�����ų�����Խ��Ŀ���
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
    if(a1>a2)//���껥��
    {
        max=a1;
        a1=a2;
        a2=max;
    }
    for(i=a1;i<=a2;i++)//����б�ʲ��߼���
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
  @brief     ��߽��ӳ�
  @param     �ӳ���ʼ�������ӳ���ĳ��
  @return    null
  Sample     Stop_Detect(void)
  @note      ����ʼ��������5���㣬���б�ʣ������ӳ���ֱ��������
-------------------------------------------------------------------------------------------------------------------*/
void Lengthen_Left_Boundry(int start,int end)
{
    int i,t;
    float k=0;
    if(start>=MT9V03X_CSI_H-1)//��ʼ��λ��У�����ų�����Խ��Ŀ���
        start=MT9V03X_CSI_H-1;
    else if(start<=0)
        start=0;
    if(end>=MT9V03X_CSI_H-1)
        end=MT9V03X_CSI_H-1;
    else if(end<=0)
        end=0;
    if(end<start)//++���ʣ����껥��
    {
        t=end;
        end=start;
        start=t;
    }

    if(start<=5)//��Ϊ��Ҫ�ڿ�ʼ��������3���㣬������ʼ����ڿ��ϣ��������ӳ���ֻ��ֱ������
    {
         Left_Add_Line(Left_Line[start],start,Left_Line[end],end);
    }

    else
    {
        k=(float)(Left_Line[start]-Left_Line[start-4])/5.0;//�����k��1/б��
        for(i=start;i<=end;i++)
        {
            Left_Line[i]=(int)(i-start)*k+Left_Line[start];//(x=(y-y1)*k+x1),��бʽ����
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
  @brief     ����߽��ӳ�
  @param     �ӳ���ʼ�������ӳ���ĳ��
  @return    null
  Sample     Stop_Detect(void)
  @note      ����ʼ��������3���㣬���б�ʣ������ӳ���ֱ��������
-------------------------------------------------------------------------------------------------------------------*/
void Lengthen_Right_Boundry(int start,int end)
{
    int i,t;
    float k=0;
    if(start>=MT9V03X_CSI_H-1)//��ʼ��λ��У�����ų�����Խ��Ŀ���
        start=MT9V03X_CSI_H-1;
    else if(start<=0)
        start=0;
    if(end>=MT9V03X_CSI_H-1)
        end=MT9V03X_CSI_H-1;
    else if(end<=0)
        end=0;
    if(end<start)//++���ʣ����껥��
    {
        t=end;
        end=start;
        start=t;
    }

    if(start<=5)//��Ϊ��Ҫ�ڿ�ʼ��������3���㣬������ʼ����ڿ��ϣ��������ӳ���ֻ��ֱ������
    {
        Right_Add_Line(Right_Line[start],start,Right_Line[end],end);
    }
    else
    {
        k=(float)(Right_Line[start]-Right_Line[start-4])/5.0;//�����k��1/б��
        for(i=start;i<=end;i++)
        {
            Right_Line[i]=(int)(i-start)*k+Right_Line[start];//(x=(y-y1)*k+x1),��бʽ����
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
  @brief     ֱ�����
  @param     null
  @return    null
  Sample     Straight_Detect()��
  @note      ��������У��߽���ʼ�㣬������ʼ�㣬
-------------------------------------------------------------------------------------------------------------------*/
void Straight_Detect(void)
{
    Straight_Flag=0;
    if(Search_Stop_Line>=65)//��ֹ�к�Զ
    {
        if(Boundry_Start_Left>=68&&Boundry_Start_Right>=65)//��ʼ�㿿��
        {
            if(-5<=Err&&Err<=5)//����С
            {
                Straight_Flag=1;//��Ϊ��ֱ��
            }
        }
    }
}

/*-------------------------------------------------------------------------------------------------------------------
  @brief     ������������յ㣬��ʮ��ʹ��
  @param     �����ķ�Χ��㣬�յ�
  @return    �޸�����ȫ�ֱ���
             Right_Down_Find=0;
             Left_Down_Find=0;
  Sample     Find_Down_Point(int start,int end)
  @note      ������֮��鿴��Ӧ�ı�����ע�⣬û�ҵ�ʱ��Ӧ��������0
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
    if(start>=MT9V03X_CSI_H-1-5)//����5�����ݲ��ȶ���������Ϊ�߽�����жϣ�����
        start=MT9V03X_CSI_H-1-5;
    if(end<=MT9V03X_CSI_H-Search_Stop_Line)
        end=MT9V03X_CSI_H-Search_Stop_Line;
    if(end<=5)
       end=5;
    for(i=start;i>=end;i--)
    {
        if(Left_Down_Find==0&&//ֻ�ҵ�һ�����������ĵ�
           abs(Left_Line[i]-Left_Line[i+1])<=5&&//�ǵ����ֵ���Ը���
           abs(Left_Line[i+1]-Left_Line[i+2])<=5&&
           abs(Left_Line[i+2]-Left_Line[i+3])<=5&&
              (Left_Line[i]-Left_Line[i-2])>=8&&
              (Left_Line[i]-Left_Line[i-3])>=15&&
              (Left_Line[i]-Left_Line[i-4])>=15)
        {
            Left_Down_Find=i;//��ȡ��������
        }
        if(Right_Down_Find==0&&//ֻ�ҵ�һ�����������ĵ�
           abs(Right_Line[i]-Right_Line[i+1])<=5&&//�ǵ����ֵ���Ը���
           abs(Right_Line[i+1]-Right_Line[i+2])<=5&&
           abs(Right_Line[i+2]-Right_Line[i+3])<=5&&
              (Right_Line[i]-Right_Line[i-2])<=-8&&
              (Right_Line[i]-Right_Line[i-3])<=-15&&
              (Right_Line[i]-Right_Line[i-4])<=-15)
        {
            Right_Down_Find=i;
        }
        if(Left_Down_Find!=0&&Right_Down_Find!=0)//�����ҵ����˳�
        {
            break;
        }
    }
}

/*-------------------------------------------------------------------------------------------------------------------
  @brief     ������������յ㣬��ʮ��ʹ��
  @param     �����ķ�Χ��㣬�յ�
  @return    �޸�����ȫ�ֱ���
             Left_Up_Find=0;
             Right_Up_Find=0;
  Sample     Find_Up_Point(int start,int end)
  @note      ������֮��鿴��Ӧ�ı�����ע�⣬û�ҵ�ʱ��Ӧ��������0
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
    if(end<=5)//��ʱ����зǳ�����ҲҪ�������ֵ㣬��ֹ����Խ��
        end=5;
    if(start>=MT9V03X_CSI_H-1-5)//����5�����ݲ��ȶ���������Ϊ�߽�����жϣ�����
        start=MT9V03X_CSI_H-1-5;
    for(i=start;i>=end;i--)
    {
        if(Left_Up_Find==0&&//ֻ�ҵ�һ�����������ĵ�
           abs(Left_Line[i]-Left_Line[i-1])<=5&&
           abs(Left_Line[i-1]-Left_Line[i-2])<=5&&
           abs(Left_Line[i-2]-Left_Line[i-3])<=5&&
              (Left_Line[i]-Left_Line[i+2])>=8&&
              (Left_Line[i]-Left_Line[i+3])>=15&&
              (Left_Line[i]-Left_Line[i+4])>=15)
        {
            Left_Up_Find=i;//��ȡ��������
        }
        if(Right_Up_Find==0&&//ֻ�ҵ�һ�����������ĵ�
           abs(Right_Line[i]-Right_Line[i-1])<=5&&//��������λ�ò��
           abs(Right_Line[i-1]-Right_Line[i-2])<=5&&
           abs(Right_Line[i-2]-Right_Line[i-3])<=5&&
              (Right_Line[i]-Right_Line[i+2])<=-8&&
              (Right_Line[i]-Right_Line[i+3])<=-15&&
              (Right_Line[i]-Right_Line[i+4])<=-15)
        {
            Right_Up_Find=i;//��ȡ��������
        }
        if(Left_Up_Find!=0&&Right_Up_Find!=0)//���������ҵ��ͳ�ȥ
        {
            break;
        }
    }
    if(abs(Right_Up_Find-Left_Up_Find)>=30)//����˺�ѹ�����Ϊ����
    {
        Right_Up_Find=0;
        Left_Up_Find=0;
    }
}

/*-------------------------------------------------------------------------------------------------------------------
  @brief     ʮ�ּ��
  @param     null
  @return    null
  Sample     Cross_Detect(void);
  @note      �����ĸ��յ��б����������ĸ��ǵ㣬�����ҵ��յ�ĸ��������Ƿ���
-------------------------------------------------------------------------------------------------------------------*/
void Cross_Detect(void)
{
    int down_search_start=0;//�µ�������ʼ��
    Cross_Flag=0;
    if(Island_State==0&& Junction_Flag == 0 &&Ramp_Flag==0)//�뻷�����⿪
    {
        Left_Up_Find=0;
        Right_Up_Find=0;
        if(Both_Lost_Time>=10)//ʮ�ֱض���˫�߶��ߣ�����˫�߶��ߵ�������ٿ�ʼ�ҽǵ�
        {
            Find_Up_Point( MT9V03X_CSI_H-1, 0 );
            if(Left_Up_Find==0&&Right_Up_Find==0)//ֻҪû��ͬʱ�ҵ������ϵ㣬ֱ�ӽ���
            {
                return;
            }
        }
        if(Left_Up_Find!=0&&Right_Up_Find!=0)//�ҵ������ϵ㣬���ҵ�ʮ����
        {
            //lcd_showint8(1,0,1);
            Cross_Flag=1;//��Ӧ��־λ�����ڸ�Ԫ�ػ����
            down_search_start=Left_Up_Find>Right_Up_Find?Left_Up_Find:Right_Up_Find;//�������Ϲյ����꿿������Ϊ�µ����������
            Find_Down_Point(MT9V03X_CSI_H-5,down_search_start+2);//���Ϲյ���2����Ϊ�µ�Ľ�ֹ��
            if(Left_Down_Find<=Left_Up_Find)
            {
                Left_Down_Find=0;//�µ㲻���ܱ��ϵ㻹����
            }
            if(Right_Down_Find<=Right_Up_Find)
            {
                Right_Down_Find=0;//�µ㲻���ܱ��ϵ㻹����
            }
            if(Left_Down_Find!=0&&Right_Down_Find!=0)
            {//�ĸ��㶼�ڣ��������ߣ����������Ȼ����
                Left_Add_Line (Left_Line [Left_Up_Find ],Left_Up_Find ,Left_Line [Left_Down_Find ] ,Left_Down_Find);
                Right_Add_Line(Right_Line[Right_Up_Find],Right_Up_Find,Right_Line[Right_Down_Find],Right_Down_Find);
            }
            else if(Left_Down_Find==0&&Right_Down_Find!=0)//11//����ʹ�õĶ���б�ʲ���
            {//������                                     //01
                Lengthen_Left_Boundry(Left_Up_Find-1,MT9V03X_CSI_H-1);
                Right_Add_Line(Right_Line[Right_Up_Find],Right_Up_Find,Right_Line[Right_Down_Find],Right_Down_Find);
            }
            else if(Left_Down_Find!=0&&Right_Down_Find==0)//11
            {//������                                     //10
                Left_Add_Line (Left_Line [Left_Up_Find ],Left_Up_Find ,Left_Line [Left_Down_Find ] ,Left_Down_Find);
                Lengthen_Right_Boundry(Right_Up_Find-1,MT9V03X_CSI_H-1);
            }
            else if(Left_Down_Find==0&&Right_Down_Find==0)//11
            {//�����ϵ�                                   //00
                Lengthen_Left_Boundry (Left_Up_Find-1,MT9V03X_CSI_H-1);
                Lengthen_Right_Boundry(Right_Up_Find-1,MT9V03X_CSI_H-1);
            }
        }
        else
        {
            Cross_Flag=0;
        }
    }
    //�ǵ���ر�����debugʹ��
    //ips200_showuint8(0,12,Cross_Flag);
//    ips200_showuint8(0,13,Island_State);
//    ips200_showuint8(50,12,Left_Up_Find);
//    ips200_showuint8(100,12,Right_Up_Find);
//    ips200_showuint8(50,13,Left_Down_Find);
//    ips200_showuint8(100,13,Right_Down_Find);
}


volatile int Ding_Point = 0;

/*-------------------------------------------------------------------------------------------------------------------
  @brief     ������������յ㣬������ʹ��
  @param     �����ķ�Χ��㣬�յ�
  @return    �޸�����ȫ�ֱ���
             Right_Down_Find_Junction=0;
             Left_Down_Find_Junction=0;
  Sample     Find_Down_Point_Junction(int start,int end)
  @note      ������֮��鿴��Ӧ�ı�����ע�⣬û�ҵ�ʱ��Ӧ��������0
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
  @brief     �Ҷ��㣬������ʹ��
  @param     None
  @return    ������������
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
  @brief     ������
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
//�������
/*-------------------------------------------------------------------------------------------------------------------
  @brief     �����������Լ��
  @param     ��ʼ�㣬��ֹ��
  @return    ��������0�����������ض��߳�����
  Sample     Continuity_Change_Left(int start,int end);
  @note      �����Ե���ֵ����Ϊ5���ɸ���
-------------------------------------------------------------------------------------------------------------------*/
int Continuity_Change_Left(int start,int end)//��������ֵ����Ϊ5
{
    int i;
    int t;
    int continuity_change_flag=0;
    if(Left_Lost_Time>=0.9*MT9V03X_CSI_H)//�󲿷ֶ����ߣ�û��Ҫ�ж���
       return 1;
    if(Search_Stop_Line<=5)//������ֹ�кܰ�
       return 1;
    if(start>=MT9V03X_CSI_H-1-5)//����Խ�籣��
        start=MT9V03X_CSI_H-1-5;
    if(end<=5)
       end=5;
    if(start<end)//���Ǵ������ϼ���ģ����˾ͻ���һ��
    {
       t=start;
       start=end;
       end=t;
    }
    
    for(i=start;i>=end;i--)
    {
       if(abs(Left_Line[i]-Left_Line[i-1])>=5)//�����ж���ֵ��5,�ɸ���
       {
            continuity_change_flag=i;
            break;
       }
    }
    return continuity_change_flag;
}

/*-------------------------------------------------------------------------------------------------------------------
  @brief     �����������Լ��
  @param     ��ʼ�㣬��ֹ��
  @return    ��������0�����������ض��߳�����
  Sample     continuity_change_flag=Continuity_Change_Right(int start,int end)
  @note      �����Ե���ֵ����Ϊ5���ɸ���
-------------------------------------------------------------------------------------------------------------------*/
int Continuity_Change_Right(int start,int end)
{
    int i;
    int t;
    int continuity_change_flag=0;
    if(Right_Lost_Time>=0.9*MT9V03X_CSI_H)//�󲿷ֶ����ߣ�û��Ҫ�ж���
       return 1;
    if(start>=MT9V03X_CSI_H-5)//����Խ�籣��
        start=MT9V03X_CSI_H-5;
    if(end<=5)
       end=5;
    if(start<end)//���Ǵ������ϼ���ģ����˾ͻ���һ��
    {
       t=start;
       start=end;
       end=t;
    }

    for(i=start;i>=end;i--)
    {
        if(abs(Right_Line[i]-Right_Line[i-1])>=5)//��������ֵ��5���ɸ���
       {
            continuity_change_flag=i;
            break;
       }
    }
    return continuity_change_flag;
}

/*-------------------------------------------------------------------------------------------------------------------
  @brief     ������ͻ����
  @param     ��ʼ�㣬��ֹ��
  @return    �����ڵ��������Ҳ�������0
  Sample     Find_Right_Up_Point(int start,int end);
  @note      ǰ5��5�������С�����������ǽǵ�
-------------------------------------------------------------------------------------------------------------------*/
int Monotonicity_Change_Left(int start,int end)//�����Ըı䣬����ֵ�ǵ����Ըı�����ڵ�����
{
    int i;
    int monotonicity_change_line=0;
    if(Left_Lost_Time>=0.9*MT9V03X_CSI_H)//�󲿷ֶ����ߣ�û�е������жϵ�����
       return monotonicity_change_line;
    if(start>=MT9V03X_CSI_H-1-5)//����Խ�籣�������жϵ�i����ʱ
       start=MT9V03X_CSI_H-1-5; //Ҫ������ǰ��5���㣬������ͷ�ĵ�Ҫ������Ϊ����յ�
    if(end<=5)
        end=5;
    if(start<=end)//�ݼ����㣬��ڷ��ˣ�ֱ�ӷ���0
      return monotonicity_change_line;
    for(i=start;i>=end;i--)//���ȡǰ5��5���ݣ�����ǰ������뷶Χ��Ҫ��
    {
        if(Left_Line[i]==Left_Line[i+5]&&Left_Line[i]==Left_Line[i-5]&&
        Left_Line[i]==Left_Line[i+4]&&Left_Line[i]==Left_Line[i-4]&&
        Left_Line[i]==Left_Line[i+3]&&Left_Line[i]==Left_Line[i-3]&&
        Left_Line[i]==Left_Line[i+2]&&Left_Line[i]==Left_Line[i-2]&&
        Left_Line[i]==Left_Line[i+1]&&Left_Line[i]==Left_Line[i-1])
        {//һ������һ������Ȼ������Ϊ����ת�۵�
            continue;
        }
        else if(Left_Line[i]>=Left_Line[i+5]&&Left_Line[i]>=Left_Line[i-5]&&
        Left_Line[i]>=Left_Line[i+4]&&Left_Line[i]>=Left_Line[i-4]&&
        Left_Line[i]>=Left_Line[i+3]&&Left_Line[i]>=Left_Line[i-3]&&
        Left_Line[i]>=Left_Line[i+2]&&Left_Line[i]>=Left_Line[i-2]&&
        Left_Line[i]>=Left_Line[i+1]&&Left_Line[i]>=Left_Line[i-1])
        {//�ͺܱ����������������ǰ5����5�����ģ�����ȡ�ȣ����Ǿ��ǵ���ͻ���
            monotonicity_change_line=i;
            break;
        }
    }
    return monotonicity_change_line;
}

/*-------------------------------------------------------------------------------------------------------------------
  @brief     ������ͻ����
  @param     ��ʼ�㣬��ֹ��
  @return    �����ڵ��������Ҳ�������0
  Sample     Find_Right_Up_Point(int start,int end);
  @note      ǰ5��5�������С�����������ǽǵ�
-------------------------------------------------------------------------------------------------------------------*/
int Monotonicity_Change_Right(int start,int end)//�����Ըı䣬����ֵ�ǵ����Ըı�����ڵ�����
{
    int i;
    int monotonicity_change_line=0;

    if(Right_Lost_Time>=0.9*MT9V03X_CSI_H)//�󲿷ֶ����ߣ�û�е������жϵ�����
        return monotonicity_change_line;
    if(start>=MT9V03X_CSI_H-1-5)//����Խ�籣��
        start=MT9V03X_CSI_H-1-5;
    if(end<=5)
        end=5;
    if(start<=end)
        return monotonicity_change_line;
    for(i=start;i>=end;i--)//���ȡǰ5��5���ݣ�����ǰ������뷶Χ��Ҫ��
    {
        if(Right_Line[i]==Right_Line[i+5]&&Right_Line[i]==Right_Line[i-5]&&
        Right_Line[i]==Right_Line[i+4]&&Right_Line[i]==Right_Line[i-4]&&
        Right_Line[i]==Right_Line[i+3]&&Right_Line[i]==Right_Line[i-3]&&
        Right_Line[i]==Right_Line[i+2]&&Right_Line[i]==Right_Line[i-2]&&
        Right_Line[i]==Right_Line[i+1]&&Right_Line[i]==Right_Line[i-1])
        {//һ������һ������Ȼ������Ϊ����ת�۵�
            continue;
        }
        else if(Right_Line[i]<=Right_Line[i+5]&&Right_Line[i]<=Right_Line[i-5]&&
        Right_Line[i]<=Right_Line[i+4]&&Right_Line[i]<=Right_Line[i-4]&&
        Right_Line[i]<=Right_Line[i+3]&&Right_Line[i]<=Right_Line[i-3]&&
        Right_Line[i]<=Right_Line[i+2]&&Right_Line[i]<=Right_Line[i-2]&&
        Right_Line[i]<=Right_Line[i+1]&&Right_Line[i]<=Right_Line[i-1])
        {//�ͺܱ����������������ǰ5����5�����ģ��Ǿ��ǵ���ͻ���
            monotonicity_change_line=i;
            break;
        }
    }
    return monotonicity_change_line;
}

/*-------------------------------------------------------------------------------------------------------------------
  @brief     ���½ǵ���
  @param     ��ʼ�㣬��ֹ��
  @return    ���ؽǵ����ڵ��������Ҳ�������0
  Sample     Find_Left_Down_Point(int start,int end);
  @note      �ǵ�����ֵ�ɸ���ʵ��ֵ����
-------------------------------------------------------------------------------------------------------------------*/
int Find_Left_Down_Point(int start,int end)//���ĸ��ǵ㣬����ֵ�ǽǵ����ڵ�����
{
    int i,t;
    int left_down_line=0;
    if(Left_Lost_Time>=0.9*MT9V03X_CSI_H)//�󲿷ֶ����ߣ�û�йյ��жϵ�����
       return left_down_line;
    if(start<end)
    {
        t=start;
        start=end;
        end=t;
    }
    if(start>=MT9V03X_CSI_H-1-5)//����5�����ݲ��ȶ���������Ϊ�߽�����жϣ�����
        start=MT9V03X_CSI_H-1-5;
    if(end<=MT9V03X_CSI_H-Search_Stop_Line)
        end=MT9V03X_CSI_H-Search_Stop_Line;
    if(end<=5)
       end=5;
    for(i=start;i>=end;i--)
    {
        if(left_down_line==0&&//ֻ�ҵ�һ�����������ĵ�
           abs(Left_Line[i]-Left_Line[i+1])<=5&&//�ǵ����ֵ���Ը���
           abs(Left_Line[i+1]-Left_Line[i+2])<=5&&  
           abs(Left_Line[i+2]-Left_Line[i+3])<=5&&
              (Left_Line[i]-Left_Line[i-2])>=5&&
              (Left_Line[i]-Left_Line[i-3])>=10&&
              (Left_Line[i]-Left_Line[i-4])>=10)
        {
            left_down_line=i;//��ȡ��������
            break;
        }
    }
    return left_down_line;
}

/*-------------------------------------------------------------------------------------------------------------------
  @brief     ���½ǵ���
  @param     ��ʼ�㣬��ֹ��
  @return    ���ؽǵ����ڵ��������Ҳ�������0
  Sample     Find_Right_Down_Point(int start,int end);
  @note      �ǵ�����ֵ�ɸ���ʵ��ֵ����
-------------------------------------------------------------------------------------------------------------------*/
int Find_Right_Down_Point(int start,int end)//���ĸ��ǵ㣬����ֵ�ǽǵ����ڵ�����
{
    int i,t;
    int right_down_line=0;
    if(Right_Lost_Time>=0.9*MT9V03X_CSI_H)//�󲿷ֶ����ߣ�û�йյ��жϵ�����
        return right_down_line;
    if(start<end)
    {
        t=start;
        start=end;
        end=t;
    }
    if(start>=MT9V03X_CSI_H-1-5)//����5�����ݲ��ȶ���������Ϊ�߽�����жϣ�����
        start=MT9V03X_CSI_H-1-5;
    if(end<=MT9V03X_CSI_H-Search_Stop_Line)
        end=MT9V03X_CSI_H-Search_Stop_Line;
    if(end<=5)
       end=5;
    for(i=start;i>=end;i--)
    {
        if(right_down_line==0&&//ֻ�ҵ�һ�����������ĵ�
           abs(Right_Line[i]-Right_Line[i+1])<=5&&//�ǵ����ֵ���Ը���
           abs(Right_Line[i+1]-Right_Line[i+2])<=5&&  
           abs(Right_Line[i+2]-Right_Line[i+3])<=5&&
              (Right_Line[i]-Right_Line[i-2])<=-5&&
              (Right_Line[i]-Right_Line[i-3])<=-10&&
              (Right_Line[i]-Right_Line[i-4])<=-10)
        {
            right_down_line=i;//��ȡ��������
            break;
        }
    }
    return right_down_line;
}



/*-------------------------------------------------------------------------------------------------------------------
  @brief     ����
  @param     ������ʼ�㣬�յ����꣬��һ�����Ϊ2�ĺ���
  @return    null
  Sample     Draw_Line(0, 0,MT9V03X_CSI_W-1,MT9V03X_CSI_H-1);
             Draw_Line(MT9V03X_CSI_W-1, 0,0,MT9V03X_CSI_H-1);
                                    ��һ�����
  @note     ���ľ���һ���ߣ���Ҫ����ɨ��
-------------------------------------------------------------------------------------------------------------------*/
void Draw_Line(int startX, int startY, int endX, int endY)
{
    int i,x,y;
    int start=0,end=0;
    if(startX>=MT9V03X_CSI_W-1)//�޷�����
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
    if(startX==endX)//һ������
    {
        if (startY > endY)//����
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
    else if(startY == endY)//��һ������
    {
        if (startX > endX)//����
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
    else //����������ˮƽ����ֱ��������������ǳ������
    {
        if(startY>endY)//��ʼ�����
        {
            start=endY;
            end=startY;
        }
        else
        {
            start=startY;
            end=endY;
        }
        for (i = start; i <= end; i++)//�����ߣ���֤ÿһ�ж��кڵ�
        {
            x =(int)(startX+(endX-startX)*(i-startY)/(endY-startY));//����ʽ����
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
        for (i = start; i <= end; i++)//�����ߣ���֤ÿһ�ж��кڵ�
        {
 
            y =(int)(startY+(endY-startY)*(i-startX)/(endX-startX));//����ʽ����
            if(y>=MT9V03X_CSI_H-1)
                y=MT9V03X_CSI_H-1;
            else if (y<=0)
                y=0;
            image_two_value[y][i] = IMG_BLACK;
        }
    }
}



/*-------------------------------------------------------------------------------------------------------------------
  @brief     ����б�ʻ���
  @param     ����б�ʣ����㣬��һ������
  @return    null
  Sample     K_Draw_Line(k, 20,MT9V03X_CSI_H-1 ,0)
  @note      ���ľ���һ���ߣ���Ҫ����ɨ��
-------------------------------------------------------------------------------------------------------------------*/
void K_Draw_Line(float k, int startX, int startY,int endY)
{
    int endX=0;

    if(startX>=MT9V03X_CSI_W-1)//�޷�����
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
    endX=(int)((endY-startY)/k+startX);//(y-y1)=k(x-x1)���Σ�x=(y-y1)/k+x1
    Draw_Line(startX,startY,endX,endY);
}

/*-------------------------------------------------------------------------------------------------------------------
  @brief     ͨ��б�ʣ����㲹��--
  @param     k       ����б��
             startY  ������ʼ��������
             endY    ������������
  @return    null
  Sample     K_Add_Boundry_Right(float k,int startY,int endY);
  @note      ������ֱ�����ڱ�����
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
    if(startY<endY)//--������start��Ҫ��
    {
        t=startY;
        startY=endY;
        endY=t;
    }

    for(i=startY;i>=endY;i--)
    {
        Right_Line[i]=(int)((i-startY)/k+startX);//(y-y1)=k(x-x1)���Σ�x=(y-y1)/k+x1
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
  @brief     ���Ͻǵ���
  @param     ��ʼ�㣬��ֹ��
  @return    ���ؽǵ����ڵ��������Ҳ�������0
  Sample     Find_Left_Up_Point(int start,int end);
  @note      �ǵ�����ֵ�ɸ���ʵ��ֵ����
-------------------------------------------------------------------------------------------------------------------*/
int Find_Left_Up_Point(int start,int end)//���ĸ��ǵ㣬����ֵ�ǽǵ����ڵ�����
{
    int i,t;
    int left_up_line=0;
    if(Left_Lost_Time>=0.9*MT9V03X_CSI_H)//�󲿷ֶ����ߣ�û�йյ��жϵ�����
       return left_up_line;
    if(start<end)
    {
        t=start;
        start=end;
        end=t;
    }
    if(end<=MT9V03X_CSI_H-Search_Stop_Line)//������ֹ�����ϵ�ȫ������
        end=MT9V03X_CSI_H-Search_Stop_Line;
    if(end<=5)//��ʱ����зǳ�����ҲҪ�������ֵ㣬��ֹ����Խ��
        end=5;
    if(start>=MT9V03X_CSI_H-1-5)
        start=MT9V03X_CSI_H-1-5;
    for(i=start;i>=end;i--)
    {
        if(left_up_line==0&&//ֻ�ҵ�һ�����������ĵ�
           abs(Left_Line[i]-Left_Line[i-1])<=5&&
           abs(Left_Line[i-1]-Left_Line[i-2])<=5&&  
           abs(Left_Line[i-2]-Left_Line[i-3])<=5&&
              (Left_Line[i]-Left_Line[i+2])>=8&& 
              (Left_Line[i]-Left_Line[i+3])>=15&&
              (Left_Line[i]-Left_Line[i+4])>=15)
        {
            left_up_line=i;//��ȡ��������
            break;
        }
    }
    return left_up_line;//�����MT9V03X_H-1��˵��û����ô���յ�
}

/*-------------------------------------------------------------------------------------------------------------------
  @brief     ���Ͻǵ���
  @param     ��ʼ�㣬��ֹ��
  @return    ���ؽǵ����ڵ��������Ҳ�������0
  Sample     Find_Right_Up_Point(int start,int end);
  @note      �ǵ�����ֵ�ɸ���ʵ��ֵ����
-------------------------------------------------------------------------------------------------------------------*/
int Find_Right_Up_Point(int start,int end)//���ĸ��ǵ㣬����ֵ�ǽǵ����ڵ�����
{
    int i,t;
    int right_up_line=0;
    if(Right_Lost_Time>=0.9*MT9V03X_CSI_H)//�󲿷ֶ����ߣ�û�йյ��жϵ�����
        return right_up_line;
    if(start<end)
    {
        t=start;
        start=end;
        end=t;
    }    
    if(end<=MT9V03X_CSI_H-Search_Stop_Line)//������ֹ�����ϵ�ȫ������
        end=MT9V03X_CSI_H-Search_Stop_Line;
    if(end<=5)//��ʱ����зǳ�����ҲҪ�������ֵ㣬��ֹ����Խ��
        end=5;
    if(start>=MT9V03X_CSI_H-1-5)
        start=MT9V03X_CSI_H-1-5;
    for(i=start;i>=end;i--)
    {
        if(right_up_line==0&&//ֻ�ҵ�һ�����������ĵ�
           abs(Right_Line[i]-Right_Line[i-1])<=5&&//��������λ�ò��
           abs(Right_Line[i-1]-Right_Line[i-2])<=5&&  
           abs(Right_Line[i-2]-Right_Line[i-3])<=5&&
              (Right_Line[i]-Right_Line[i+2])<=-8&&
              (Right_Line[i]-Right_Line[i+3])<=-15&&
              (Right_Line[i]-Right_Line[i+4])<=-15)
        {
            right_up_line=i;//��ȡ��������
            break;
        }
    }
    return right_up_line;
}

void Island_Detect(void)
{ 
    static float k=0;//3��5״̬��k
    static int island_state_5_down[2]={0};//״̬5ʱ�����뿪���������ұ߽����͵㣬[0]��y����ĳ�У�{1}��x����ĳ��
    static int island_state_3_up[2]={0};//״̬3ʱ�������뻷���ã���������ǵ�[0]��y����ĳ�У�{1}��x����ĳ��
    static int left_down_guai[2]={0};//�ĸ��յ������洢��[0]��y����ĳ�У�{1}��x����ĳ��
    static int right_down_guai[2]={0};//�ĸ��յ������洢��[0]��y����ĳ�У�{1}��x����ĳ��
    int monotonicity_change_line[2];//�����Ըı�����꣬[0]��ĳ�У�[1]��ĳ��
    int monotonicity_change_left_flag=0;//��ת����0
    int monotonicity_change_right_flag=0;//��ת����0
    int continuity_change_right_flag=0; //������0
    int continuity_change_left_flag=0;  //������0
    //�����ǳ����жϷ�
    continuity_change_left_flag=Continuity_Change_Left(MT9V03X_CSI_H-1-5,30);//�������ж�
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
        /*continuity_change_left_flag=Continuity_Change_Left(MT9V03X_CSI_H-1-5,10);//�������ж�
        continuity_change_right_flag=Continuity_Change_Right(MT9V03X_CSI_H-1-5,10);*/
        if(Left_Island_Flag==0)//��
        {
          
            if(monotonicity_change_right_flag==0&&//�ұ��ǵ�����
               continuity_change_left_flag!=0//����ǲ�������
               &&continuity_change_right_flag==0//�󻷵��ұ���������
               &&Left_Lost_Time>=10&& //��߶��ߺܶ�
               Left_Lost_Time<=50&& //Ҳ����ȫ����
               Right_Lost_Time<=10&&//�ұ߶��߽���*/
               Search_Stop_Line>=MT9V03X_CSI_H*0.8&&//������ֹ�п�����Զ
               Boundry_Start_Left>=MT9V03X_CSI_H-20&&Boundry_Start_Right>=MT9V03X_CSI_H-20&&//�߽���ʼ�㿿��
               Both_Lost_Time<=10)//˫�߶�����
            {
                left_down_guai[0]=Find_Left_Down_Point(MT9V03X_CSI_H-1,20);//�����½ǵ�
                if(left_down_guai[0]>=30)//����1���ɣ��������жϹյ㣬λ�ò��ԣ��������У�����
                {
                    Island_State=1;
                    Left_Island_Flag=1;
                }
                else//���У�����
                {
                    Island_State=0;
                    Left_Island_Flag=0;
                }
            }
        }
        if(Right_Island_Flag==0)//�һ�
        {
            if(monotonicity_change_left_flag==0&&
               continuity_change_left_flag==0&&//�һ��������������
               continuity_change_right_flag!=1&&//�ұ��ǲ�������
               Right_Lost_Time>=10&&           //�Ҷ��߶�
               Right_Lost_Time<=50&&           //�Ҷ��߲���̫��
               Left_Lost_Time<=10&&            //������
               Search_Stop_Line>=MT9V03X_CSI_H*0.95&&//������ֹ�п�����Զ
               Boundry_Start_Left>=MT9V03X_CSI_H-20&&Boundry_Start_Right>=MT9V03X_CSI_H-20&&//�߽���ʼ�㿿��
               Both_Lost_Time<=10)
            {
                right_down_guai[0]=Find_Right_Down_Point(MT9V03X_CSI_H-1,20);//���µ�
                if(right_down_guai[0]>=30)//����1���ɣ���������йյ㣬λ�ò��ԣ��������У�����
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
    if(Left_Island_Flag==1)//1״̬�¹յ㻹�ڣ�û����
    {
        if(Island_State==1)
        {
            monotonicity_change_line[0]=Monotonicity_Change_Left(30,5);//Ѱ�ҵ����Ըı��
            monotonicity_change_line[1]=Left_Line[monotonicity_change_line[0]];
            Left_Add_Line((int)(monotonicity_change_line[1]*0.15),MT9V03X_CSI_H-1,monotonicity_change_line[1],monotonicity_change_line[0]);
            if((Island_State==1)&&(Boundry_Start_Left<65))//�·�������ʱ���2
            {
                Island_State=2;
            }
        }
        else if(Island_State==2)//�·��ǵ���ʧ��2״̬ʱ�·�Ӧ���Ƕ��ߣ������ǻ���
        {
            monotonicity_change_line[0]=Monotonicity_Change_Left(90,10);//Ѱ�ҵ����Ըı��
            monotonicity_change_line[1]=Left_Line[monotonicity_change_line[0]];
            Left_Add_Line((int)(monotonicity_change_line[1]*0.1),MT9V03X_CSI_H-1,monotonicity_change_line[1],monotonicity_change_line[0]);
            lcd_showint8(5,5,Boundry_Start_Left);
            lcd_showint8(4,4,monotonicity_change_line[0]);
            if(Island_State==2 && monotonicity_change_line[0]>65)//���½��ٲ����߽�3
            {
                Island_State=3;//�·����ߣ�˵�������Ѿ�������
            }
        }
        
        else if(Island_State==3)//3״̬׼��������Ѱ���Ϲյ㣬����
        {
            int flag_out_state3 = 0;
            if(k!=0)
            {
               // int max = 0;
                for(int i = MT9V03X_CSI_H - 5; i >= MT9V03X_CSI_H - 50; i --)
                {
                  if(abs(Right_Line[i]-Right_Line[i-1])<=5&&//��������λ�ò��
           abs(Right_Line[i-1]-Right_Line[i-2])<=5&&
           abs(Right_Line[i-2]-Right_Line[i-3])<=5&&
              (Right_Line[i]-Right_Line[i+2])<=-3&&
              (Right_Line[i]-Right_Line[i+3])<=-3&&
              (Right_Line[i]-Right_Line[i+4])<=-3)flag_out_state3 =1;
                }
                if(Right_Lost_Time<=3 && Left_Lost_Time<=3)flag_out_state3 =1;
                //lcd_showint8(1,1,max);
                K_Draw_Line(k,MT9V03X_CSI_W-30,MT9V03X_CSI_H-1,0);//k�Ǹո�������ģ���̬��������
                Longest_White_Column();//ˢ�±߽�����
            }
            else
            {
                Left_Up_Guai[0]=Find_Left_Up_Point(60,5);//�����Ϲյ�!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
                Left_Up_Guai[1]=Left_Line[Left_Up_Guai[0]];
                /*if (Left_Up_Guai[0]<5)//����Ĺ���!!!!
                {
                    Island_State=0;
                    Left_Island_Flag=0;
                }*/
                //lcd_showint8(4,4,Left_Up_Guai[1]);
                //lcd_showint8(5,5,Left_Up_Guai[0]);
                if(k==0&&(40<=Left_Up_Guai[0]&&Left_Up_Guai[0]<60)&&(50<Left_Up_Guai[1]&&Left_Up_Guai[1]<110))//�յ������һ����Χ�ڣ���Ϊ�ǹյ����!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
                {
                    island_state_3_up[0]= Left_Up_Guai[0];
                    island_state_3_up[1]= Left_Up_Guai[1];
                    k=(float)((float)(MT9V03X_CSI_H-island_state_3_up[0])/(float)(MT9V03X_CSI_W-20-island_state_3_up[1]));
                    K_Draw_Line(k,MT9V03X_CSI_W-30,MT9V03X_CSI_H-1,0);//��¼�µ�һ���ϵ����ʱλ�ã�������������һ�����ߣ��뻷
                    Longest_White_Column();//ˢ�±߽�����
                }
            }
            if((Island_State==3)&&(k!=0)&&flag_out_state3==1)
            {
               k=0;//б������
               Island_State=33;//��һ��ʱ��˳�����������е�������Χ
               Longest_White_Column();//�����
            }
        }
        else if(Island_State == 33)
        {
          lcd_showint8(2,2,Right_Lost_Time);
          lcd_showint8(3,3,Left_Lost_Time);
          if(Right_Lost_Time==0 && Left_Lost_Time>=20)Island_State =4;
        }
        else if(Island_State==4)//4״̬��ȫ��ȥ������
        {
          if(Right_Lost_Time >= 8)
          {
             monotonicity_change_line[0]=Monotonicity_Change_Right(MT9V03X_CSI_H-10,10);//�����Ըı�
             monotonicity_change_line[1]=Right_Line[monotonicity_change_line[0]];
             if((Island_State==4)&&(MT9V03X_CSI_H - 65<=monotonicity_change_line[0]&&monotonicity_change_line[0]<=MT9V03X_CSI_H - 10&&monotonicity_change_line[1]>=MT9V03X_CSI_H - 40))//�����㿿�£���ȥ5
             {//monotonicity_change_line[1]>=90&&
                 island_state_5_down[0]=MT9V03X_CSI_H-1;
                 island_state_5_down[1]=Right_Line[MT9V03X_CSI_H-1];
                 k=(float)((float)(MT9V03X_CSI_H-monotonicity_change_line[0])/(float)(island_state_5_down[1]-monotonicity_change_line[1]));
                 K_Add_Boundry_Right(k,island_state_5_down[1],island_state_5_down[0],0);//��״̬3һ������סб��
                 Island_State=5;
             }
          }
             
        }

        else if(Island_State==5)//����
        {
            monotonicity_change_line[0]=Monotonicity_Change_Right(MT9V03X_CSI_H-10,10);//�����Ըı�
            monotonicity_change_line[1]=Right_Line[monotonicity_change_line[0]];
            K_Add_Boundry_Right(k,island_state_5_down[1],island_state_5_down[0],0);
            lcd_showint8(5,5,monotonicity_change_line[0]);
            if((Island_State==5)&&(monotonicity_change_line[0]>MT9V03X_CSI_H-20))//�ұ��ȶ���
            {
                Island_State=6;
            }
        }
         else if(Island_State==6)//���ڳ�
        {
            K_Add_Boundry_Right(k,island_state_5_down[1],island_state_5_down[0],0);
            if((Island_State==6)&&(Boundry_Start_Right>MT9V03X_CSI_H-10))//�ұ߲�����
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
        /*else if(Island_State==7)//������ȥ�ˣ���Ѱ�ҹյ㣬׼���뿪����״̬
        {
            Left_Up_Guai[0]=Find_Left_Up_Point(MT9V03X_CSI_H-10,0);//��ȡ���ϵ����꣬��������ȥ8
            Left_Up_Guai[1]=Left_Line[Left_Up_Guai[0]];
            if((Island_State==7)&&(Left_Up_Guai[1]<=100)&&(5<=Left_Up_Guai[0]&&Left_Up_Guai[0]<=MT9V03X_CSI_H-20))//ע������Ժ������궼��Ҫ��
            {
                Island_State=8;//�������ҵ��յ��ȥ8
            }
        }
        else if(Island_State==8)//���ߣ��������һ��
        {
            Left_Up_Guai[0]=Find_Left_Up_Point(MT9V03X_CSI_H-1,10);//��ȡ���ϵ�����
            Left_Up_Guai[1]=Left_Line[Left_Up_Guai[0]];
            Lengthen_Left_Boundry(Left_Up_Guai[0]-1,MT9V03X_CSI_H-1);
            if((Island_State==8)&&(Left_Up_Guai[0]>=MT9V03X_CSI_H-20))//���յ㿿��ʱ����Ϊ�����ˣ���������
            {//Ҫô�յ㿿�£�Ҫô�յ㶪�ˣ����·������ߣ���Ϊ����������
                Island_State=9;//8ʱ�򻷵����������ˣ�Ϊ�˷�ֹ�����л���8����9����ż�ʮ�������㣬
                
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
