#include "ImageProcess.h"
#include "BMP.h"
using namespace std;

int main()
{
    BMP *p;
    
    //直方图全尺度拉伸，FSHS
    p=new BMP;
    p->readBmp("girl.bmp");
    FSHS_Gray(p);
    p->saveBmp("girl_FSHS.bmp");
    delete p;

    //直方图均衡化，HistogramEqualization
    p=new BMP;
    p->readBmp("girl.bmp");
    HistogramEqualization_gray(p);
    p->saveBmp("girl_HE.bmp");
    delete p;

    //直方图规定化，HistogramShaping
    double shaping[256]={0};
    for(int i=0;i<=100;i++){shaping[i]=(double)(100-i)/5000;}
    //这段代码块用来设置想要塑造的形状，随便用了个斜的直线
    p=new BMP;
    p->readBmp("girl.bmp");
    HistogramShaping_gray(p,shaping);
    p->saveBmp("girl_HS.bmp");
    delete p;

    //傅里叶变换（含低通滤波），Fourier
    p=new BMP;
    p->readBmp("g100.bmp");
    //由于我自己写的傅里叶变换性能比较差，因此就读个100*100像素的，图像太大处理起来特别慢
    Fourier(p);
    p->saveBmp("g100_Fourier.bmp");
    delete p;

    //链码
    p=new BMP;
    p->readBmp("lianma.bmp");
    Lianma l;
    l.caulculate(p);
    l.show();
    delete p;
    /*p=new BMP;
    p->readBmp("girl.bmp");
    p->convertGray("girl_gray.bmp");
    delete p;
    p=new BMP;
    p->readBmp("g100.bmp");
    p->convertGray("g100_gray.bmp");
    delete p;*/

    return 0;
}

