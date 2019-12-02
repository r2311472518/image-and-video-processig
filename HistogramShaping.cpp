#include "ImageProcess.h"
void HistogramEqualization_gray(BMP *p){
    unsigned char *pBmpBuf=p->getPBmpBuf();
    int bmpHeight=p->getHeight();
    int bmpWidth=p->getWidth();
    int biBitCount=p->getBiBitCount();
    int linebyte1=(bmpWidth*biBitCount/8+3)/4*4;

    int GrayTable[256]={0};
    int GrayNumber[256]={0};

    if(biBitCount == 24)
    {
        //计算Hf(k)，即每个值的点数
        unsigned char gray;
        for(int i=0;i<bmpHeight;i++)
        {
            for(int j=0;j<bmpWidth;j++)
            {
                gray=pBmpBuf[i*linebyte1+j*3];
                GrayNumber[gray]++;
            }
        }
        //计算分布函数同时FSHS,总像素点数最后再除
        int P[256]={0};
        P[0]=GrayNumber[0];
        GrayTable[0]=255*P[0];
        for (int k=1;k<=255;k++){
            P[k]=P[k-1]+GrayNumber[k];
            GrayTable[k]=(float)P[k]*255/bmpHeight/bmpWidth+0.5;//在此处除以了总像素点数
        }
        //按照换算表重置图像
        for(int i=0;i<bmpHeight;i++)
        {
            for(int j=0;j<bmpWidth;j++)
            {
                int k =i*linebyte1+j*3;
                gray=pBmpBuf[i*linebyte1+j*3];
                gray=GrayTable[gray];
                pBmpBuf[k]=gray;
                pBmpBuf[k+1]=gray;
                pBmpBuf[k+2]=gray;
            }
        }
        cout<<"HistogramEqualization suc"<<endl;
    }
}

void HistogramShaping_gray(BMP *p,double *shaping){
    unsigned char *pBmpBuf=p->getPBmpBuf();
    int bmpHeight=p->getHeight();
    int bmpWidth=p->getWidth();
    int biBitCount=p->getBiBitCount();
    int linebyte1=(bmpWidth*biBitCount/8+3)/4*4;

    int GrayTable[256]={0};
    int GrayNumber[256]={0};

    if(biBitCount == 24)
    {
        //计算Hf(k)，即每个值的点数
        unsigned char gray;
        for(int i=0;i<bmpHeight;i++)
        {
            for(int j=0;j<bmpWidth;j++)
            {
                gray=pBmpBuf[i*linebyte1+j*3];
                GrayNumber[gray]++;
            }
        }
        //计算分布函数同时FSHS,总像素点数最后再除。得到映射表1
        double P1[256]={0};
        P1[0]=(double)GrayNumber[0]/bmpHeight/bmpWidth;
        double P2[256];
        P2[0]=shaping[0];
        for (int k=1;k<=255;k++){
            P1[k]=P1[k-1]+(double)GrayNumber[k]/bmpHeight/bmpWidth;
            P2[k]=P2[k-1]+shaping[k];
        }
        //计算映射表-方法1,参考课本
        for(int i=0;i<=255;i++){
            int k;double difmin;
            for(k=0;(P2[k]-P1[i])<0&&(k<255);k++)
            {}
            GrayTable[i]=k;
        }
        //方法2，参考网上
        /*for(int i=0;i<=255;i++){
            int k;double difmin;
            difmin=abs(P1[i]-P2[0]);
            for(int j=1;j<=255;j++){
                if (abs(P1[i]-P2[j])<difmin){
                    difmin=abs(P1[i]-P2[j]);
                    k=j;
                }
            }
            GrayTable[i]=k;
        }*/
        //按照换算表重置图像
        for(int i=0;i<bmpHeight;i++)
        {
            for(int j=0;j<bmpWidth;j++)
            {
                int k =i*linebyte1+j*3;
                gray=pBmpBuf[i*linebyte1+j*3];
                gray=GrayTable[gray];
                pBmpBuf[k]=gray;
                pBmpBuf[k+1]=gray;
                pBmpBuf[k+2]=gray;
            }
        }
        cout<<"HistogramShaping suc"<<endl;
    }
}