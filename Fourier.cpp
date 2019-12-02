#include"ImageProcess.h"
#include<math.h>
#include<complex>

void Fourier(BMP *p){
    unsigned char *pBmpBuf=p->getPBmpBuf();
    int bmpHeight=p->getHeight();
    int bmpWidth=p->getWidth();
    int biBitCount=p->getBiBitCount();
    int linebyte1=(bmpWidth*biBitCount/8+3)/4*4;
    if(biBitCount == 24)
    {
        int m=bmpHeight;int n=bmpWidth;
        int masku=m/4;int maskv=n/4;
        //傅里叶变换
        complex<double> *F;
        F=new complex<double>[m*n];
        double f,pi=3.1415926;
        double kux=-2*pi/m;
        double kvy=-2*pi/n;
        //计算频谱F(u,v),同时乘上mask
        for (int u=0;u<m;u++){
            for(int v=0;v<n;v++){
                //计算F(u,v)
                F[u*n+v]=0;
                if(abs(u-m/2)>masku&&abs(v-n/2)>maskv){
                    for(int x=0;x<m;x++){
                        for(int y=0;y<n;y++){
                            f=pBmpBuf[x*linebyte1+y*3];
                            complex<double> temp(f*cos(u*x*kux+v*y*kvy),f*sin(u*x*kux+v*y*kvy));
                            F[u*n+v]=F[u*n+v]+temp;
                        }
                    }
                }
                
                
            }
        }
        cout<<"fourier suc"<<endl;
        //进行反变换
        double fre=0,fim=0;
        int f2[m][n];
        kux=-kux;kvy=-kvy;
        for (int x=0;x<m;x++){
            for(int y=0;y<n;y++){
                //计算f(x,y)
                fre=0;fim=0;
                for(int u=0;u<m;u++){
                    for(int v=0;v<n;v++)
                    {
                         fre=F[u*n+v].real()*cos(u*x*kux+v*y*kvy)-F[u*n+v].imag()*sin(u*x*kux+v*y*kvy)+fre;
                         fim=F[u*n+v].real()*sin(u*x*kux+v*y*kvy)+F[u*n+v].imag()*cos(u*x*kux+v*y*kvy)+fim;
                    }
                }
                complex<double> temp(fre,fim);
                f2[x][y]=abs(temp)/m/n;
            }
        }
        cout<<"ifourier suc"<<endl;
        delete F;
        //进行FSHS
        unsigned char grayMin,grayMax,gray;
        grayMin=pBmpBuf[0];grayMax=pBmpBuf[0];
        for(int i=0;i<bmpHeight;i++)
        {
            for(int j=0;j<bmpWidth;j++)
            {
                gray=f2[i][j];
                grayMin=(gray<grayMin)?gray:grayMin;
                grayMax=(gray>grayMax)?gray:grayMax;    
            }
        }
        //重新设置图像
        float K=(float)255/(float)(grayMax-grayMin);
        for(int i=0;i<bmpHeight;i++)
        {
            for(int j=0;j<bmpWidth;j++)
            {
                int n = i*linebyte1+j*3;
                int gray;
                gray=K*(float)(f2[i][j]-grayMin);
                pBmpBuf[n]=gray;
                pBmpBuf[n+1]=gray;
                pBmpBuf[n+2]=gray;
            }
        }
    }
}