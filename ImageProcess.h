#include "BMP.h"

#ifndef MYIMAGEPROCESS_H_
#define MYIMAGEPROCESS_H_
//直方图全尺度拉伸
void FSHS_Gray(BMP *p);
void FSHS(BMP *p);
void HistogramEqualization_gray(BMP *p);
void HistogramShaping_gray(BMP *p,double *shaping);//shaping[256]是归一化的面积为1的曲线
void Fourier(BMP *p);
class Lianma{
    private:
      int point[2]={0};
      int *lm;
      int number=0;
    public:
      void show();
      void caulculate(BMP *p);
      //~Lianma(){delete lm;}
};

#endif