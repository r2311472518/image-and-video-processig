#include<iostream>
#include<fstream>
#include<cstdlib>
#include<cstdio>
#include<cmath>
#include<iomanip>
#include<windows.h>
using namespace std;

unsigned char *pBmpBuf;//读入图像数据的指针
int bmpWidth;//图像的宽
int bmpHeight;//图像的高
RGBQUAD *pColorTable;//颜色表指针
int biBitCount;//图像类型，每像素位数


int main()
{
    convertGray("girl2.bmp");
    FSHS_Gray("gray.bmp");
    FSHS("girl2.bmp");
    return 0;
}


//-----------------------------------------------------------------------------------------
//给定一个图像位图数据、宽、高、颜色表指针及每像素所占的位数等信息,将其写到指定文件中
bool readBmp(char *bmpName) 
{
    FILE *fp=fopen(bmpName,"rb");//二进制读方式打开指定的图像文件
    if(fp==0) return 0;
    
    fseek(fp, sizeof(BITMAPFILEHEADER),0);//跳过位图文件头结构BITMAPFILEHEADER

    //定义位图信息头结构变量，读取位图信息头进内存，存放在变量head中
    BITMAPINFOHEADER head; 
    fread(&head, sizeof(BITMAPINFOHEADER), 1,fp); //获取图像宽、高、每像素所占位数等信息
    bmpWidth = head.biWidth;
    bmpHeight = head.biHeight;
    biBitCount = head.biBitCount;//定义变量，计算图像每行像素所占的字节数（必须是4的倍数）

    int lineByte=(bmpWidth * biBitCount/8+3)/4*4;//灰度图像有颜色表，且颜色表表项为256
    if(biBitCount==8)
    {
        //申请颜色表所需要的空间，读颜色表进内存
        pColorTable=new RGBQUAD[256];
        fread(pColorTable,sizeof(RGBQUAD),256,fp);
    }

    //申请位图数据所需要的空间，读位图数据进内存
    pBmpBuf=new unsigned char[lineByte * bmpHeight];
    fread(pBmpBuf,1,lineByte * bmpHeight,fp);
    
    fclose(fp);//关闭文件
    return 1;//读取文件成功
}

bool saveBmp(char *bmpName, unsigned char *imgBuf, int width, int height, int biBitCount, RGBQUAD *pColorTable)
{

    //如果位图数据指针为0，则没有数据传入，函数返回

    if(!imgBuf)
        return 0;

    //颜色表大小，以字节为单位，灰度图像颜色表为1024字节，彩色图像颜色表大小为0

    int colorTablesize=0;

    if(biBitCount==8)
        colorTablesize=1024;

    //待存储图像数据每行字节数为4的倍数

    int lineByte=(width * biBitCount/8+3)/4*4;

    //以二进制写的方式打开文件

    FILE *fp=fopen(bmpName,"wb");

    if(fp==0)
        return 0;

    //申请位图文件头结构变量，填写文件头信息

    BITMAPFILEHEADER fileHead;

    fileHead.bfType = 0x4D42;//bmp类型

    //bfSize是图像文件4个组成部分之和

    fileHead.bfSize= sizeof(BITMAPFILEHEADER) + sizeof(BITMAPINFOHEADER) + colorTablesize + lineByte*height;

    fileHead.bfReserved1 = 0;

    fileHead.bfReserved2 = 0;

    //bfOffBits是图像文件前3个部分所需空间之和

    fileHead.bfOffBits=54+colorTablesize;

    //写文件头进文件

    fwrite(&fileHead, sizeof(BITMAPFILEHEADER),1, fp);

    //申请位图信息头结构变量，填写信息头信息

    BITMAPINFOHEADER head; 

    head.biBitCount=biBitCount;

    head.biClrImportant=0;

    head.biClrUsed=0;

    head.biCompression=0;

    head.biHeight=height;

    head.biPlanes=1;

    head.biSize=40;

    head.biSizeImage=lineByte*height;

    head.biWidth=width;

    head.biXPelsPerMeter=0;

    head.biYPelsPerMeter=0;

    //写位图信息头进内存

    fwrite(&head, sizeof(BITMAPINFOHEADER),1, fp);

    //如果灰度图像，有颜色表，写入文件 

    if(biBitCount==8)
        fwrite(pColorTable, sizeof(RGBQUAD),256, fp);

    //写位图数据进文件

    fwrite(imgBuf, height*lineByte, 1, fp);

    //关闭文件

    fclose(fp);

    return 1;

}
 void convertGray(char *ImagePath)
{
    if(readBmp(ImagePath)) cout<<"read successfully"<<endl;
    // 输出整体图像信息
    cout << "width=" << bmpWidth << ",height=" << bmpHeight << ",biBitCount=" << biBitCount <<endl;
    int linebyte1=(bmpWidth*biBitCount/8+3)/4*4;//每行的字节数，由于存在4字节补齐
     //转换为灰度图
    if(biBitCount == 24)
    {
        for(int i=0;i<bmpHeight;i++)
        {
            for(int j=0;j<bmpWidth;j++)
            {
                unsigned char blue,green,red,gray;
                blue=pBmpBuf[i*linebyte1+j*3];
                green=pBmpBuf[i*linebyte1+j*3+1];
                red=pBmpBuf[i*linebyte1+j*3+2];
                gray = (float)red*0.299 + (float)green*0.587 + (float)blue*0.114;
                pBmpBuf[i*linebyte1+j*3]=gray;
                pBmpBuf[i*linebyte1+j*3+1]=gray;
                pBmpBuf[i*linebyte1+j*3+2]=gray;
            }
        }
    }

    // 将灰度图写入文件
    char *writePath = "gray.bmp";
    saveBmp(writePath, pBmpBuf, bmpWidth, bmpHeight, biBitCount, pColorTable);

    cout<<"-------------------convert successfully---------------------"<<endl;

    //清除缓冲区，pBmpBuf和pColorTable是全局变量，在文件读入时申请的空间
    delete []pBmpBuf;
    if(biBitCount==8) delete []pColorTable;

 }

 void FSHS_Gray(char *ImagePath)
{
    if(readBmp(ImagePath)) cout<<"read successfully"<<endl;
    // 输出整体图像信息
    cout << "width=" << bmpWidth << ",height=" << bmpHeight << ",biBitCount=" << biBitCount <<endl;
    int linebyte1=(bmpWidth*biBitCount/8+3)/4*4;//每行的字节数，由于存在4字节补齐
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
                gray=K*(float)(pBmpBuf[i*linebyte1+j*3]-grayMin);
                pBmpBuf[i*linebyte1+j*3]=gray;
                pBmpBuf[i*linebyte1+j*3+1]=gray;
                pBmpBuf[i*linebyte1+j*3+2]=gray;
            }
        }
    }

    // 将FSHS后的图像写入文件
    char *writePath = "FSHS_gray.bmp";
    saveBmp(writePath, pBmpBuf, bmpWidth, bmpHeight, biBitCount, pColorTable);

    cout<<"-------------------FSHS Gray successfully---------------------"<<endl;

    //清除缓冲区，pBmpBuf和pColorTable是全局变量，在文件读入时申请的空间
    delete []pBmpBuf;
    if(biBitCount==8) delete []pColorTable;
 }

  void FSHS(char *ImagePath)
{
    if(readBmp(ImagePath)) cout<<"read successfully"<<endl;
    // 输出整体图像信息
    cout << "width=" << bmpWidth << ",height=" << bmpHeight << ",biBitCount=" << biBitCount <<endl;
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
                b=Kb*(float)(pBmpBuf[i*linebyte1+j*3]-bmin);
                g=Kg*(float)(pBmpBuf[i*linebyte1+j*3+1]-gmin);
                r=Kr*(float)(pBmpBuf[i*linebyte1+j*3+2]-rmin);
                pBmpBuf[i*linebyte1+j*3]=b;
                pBmpBuf[i*linebyte1+j*3+1]=g;
                pBmpBuf[i*linebyte1+j*3+2]=r;
            }
        }
    }

    // 将FSHS后的图像写入文件
    char *writePath = "FSHS.bmp";
    saveBmp(writePath, pBmpBuf, bmpWidth, bmpHeight, biBitCount, pColorTable);

    cout<<"-------------------FSHS successfully---------------------"<<endl;

    //清除缓冲区，pBmpBuf和pColorTable是全局变量，在文件读入时申请的空间
    delete []pBmpBuf;
    if(biBitCount==8) delete []pColorTable;
 }
