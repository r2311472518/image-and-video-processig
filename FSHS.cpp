#include "ImageProcess.h"
 void FSHS_Gray(BMP *p)
{
    unsigned char *pBmpBuf=p->getPBmpBuf();
    int bmpHeight=p->getHeight();
    int bmpWidth=p->getWidth();
    int biBitCount=p->getBiBitCount();
    int linebyte1=(bmpWidth*biBitCount/8+3)/4*4;
    if(biBitCount == 24)
    {
        //找到最小和最大灰度值
        unsigned char grayMin,grayMax,gray;
        grayMin=pBmpBuf[0];grayMax=pBmpBuf[0];
        for(int i=0;i<bmpHeight;i++)
        {
            for(int j=0;j<bmpWidth;j++)
            {
                gray=pBmpBuf[i*linebyte1+j*3];
                grayMin=(gray<grayMin)?gray:grayMin;
                grayMax=(gray>grayMax)?gray:grayMax;    
            }
        }
        //进行FSHS
        float K=(float)255/(float)(grayMax-grayMin);
        for(int i=0;i<bmpHeight;i++)
        {
            for(int j=0;j<bmpWidth;j++)
            {
                int n = i*linebyte1+j*3;
                gray=K*(float)(pBmpBuf[i*linebyte1+j*3]-grayMin);
                pBmpBuf[n]=gray;
                pBmpBuf[n+1]=gray;
                pBmpBuf[n+2]=gray;
            }
        }
    }
    cout<<"FSHS suc"<<endl;
 }

void FSHS(BMP *p)
{
    unsigned char *pBmpBuf=p->getPBmpBuf();
    int bmpHeight=p->getHeight();
    int bmpWidth=p->getWidth();
    int biBitCount=p->getBiBitCount();
    int linebyte1=(bmpWidth*biBitCount/8+3)/4*4;//每行的字节数，由于存在4字节补齐
    if(biBitCount == 24)
    {
        //找到最小和最大值
        unsigned char rmin,rmax,gmin,gmax,bmin,bmax,r,g,b;
        bmin=pBmpBuf[0];bmax=pBmpBuf[0];
        gmin=pBmpBuf[1];gmax=pBmpBuf[1];
        rmin=pBmpBuf[2];rmax=pBmpBuf[2];
        for(int i=0;i<bmpHeight;i++)
        {
            for(int j=0;j<bmpWidth;j++)
            {
                b=pBmpBuf[i*linebyte1+j*3];
                bmin=(b<bmin)?b:bmin;
                bmax=(b>bmax)?b:bmax;
                g=pBmpBuf[i*linebyte1+j*3+1];
                gmin=(g<gmin)?g:gmin;
                gmax=(g>gmax)?g:gmax;
                r=pBmpBuf[i*linebyte1+j*3+2];
                rmin=(r<rmin)?r:rmin;
                rmax=(r>rmax)?r:rmax;
            }
        }
        //进行FSHS
        float Kb=(float)255/(float)(bmax-bmin);
        float Kg=(float)255/(float)(gmax-gmin);
        float Kr=(float)255/(float)(rmax-rmin);
        for(int i=0;i<bmpHeight;i++)
        {
            for(int j=0;j<bmpWidth;j++)
            {
                int n = i*linebyte1+j*3;
                b=Kb*(float)(pBmpBuf[i*linebyte1+j*3]-bmin);
                g=Kg*(float)(pBmpBuf[i*linebyte1+j*3+1]-gmin);
                r=Kr*(float)(pBmpBuf[i*linebyte1+j*3+2]-rmin);
                pBmpBuf[n]=b;
                pBmpBuf[n+1]=g;
                pBmpBuf[n+2]=r;
            }
        }
        cout<<"FSHS suc"<<endl;
    }
 }