#include<iostream>
#include<fstream>
#include<cstdlib>
#include<cstdio>
#include<cmath>
#include<iomanip>
#include<windows.h>
using namespace std;

#ifndef MYBMP_H_
#define MYBMP_H_
class BMP{
    private:
      unsigned char *pBmpBuf=NULL;//读入图像数据的指针
      int bmpWidth=0;//图像的宽
      int bmpHeight=0;//图像的高
      RGBQUAD *pColorTable=NULL;//颜色表指针
      int biBitCount=0;//图像类型，每像素位数
    public:
      bool readBmp(char *bmpName);
      bool saveBmp(char *bmpName);
      void convertGray(char *bmpName);
      void binary(char *bmpName);
      unsigned char *getPBmpBuf(){return pBmpBuf;}
      int getWidth(){return bmpWidth;}
      int getHeight(){return bmpHeight;}
      int getBiBitCount(){return biBitCount;}
      ~BMP(){
          //清除缓冲区,在文件读入时申请的空间
          if(pBmpBuf!=NULL) delete []pBmpBuf;
          if(biBitCount==8) delete []pColorTable;
      }
};
#endif