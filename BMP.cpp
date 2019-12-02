#include"BMP.h" 
//-----------------------------------------------------------------------------------------
//给定一个图像位图数据、宽、高、颜色表指针及每像素所占的位数等信息,将其写到指定文件中
bool BMP::readBmp(char *bmpName) 
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
    cout<<"-------------------open successfully---------------------"<<endl;
    return 1;//读取文件成功
}

bool BMP::saveBmp(char *bmpName)
{
    //如果位图数据指针为0，则没有数据传入，函数返回
    if(!pBmpBuf)
        return 0;

    //颜色表大小，以字节为单位，灰度图像颜色表为1024字节，彩色图像颜色表大小为0
    int colorTablesize=0;
    if(biBitCount==8)
        colorTablesize=1024;

    //待存储图像数据每行字节数为4的倍数
    int lineByte=(bmpWidth * biBitCount/8+3)/4*4;

    //以二进制写的方式打开文件
    FILE *fp=fopen(bmpName,"wb");
    if(fp==0)
        return 0;

    //申请位图文件头结构变量，填写文件头信息
    BITMAPFILEHEADER fileHead;
    fileHead.bfType = 0x4D42;//bmp类型
    //bfSize是图像文件4个组成部分之和
    fileHead.bfSize= sizeof(BITMAPFILEHEADER) + sizeof(BITMAPINFOHEADER) + colorTablesize + lineByte*bmpHeight;
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
    head.biHeight=bmpHeight;
    head.biPlanes=1;
    head.biSize=40;
    head.biSizeImage=lineByte*bmpHeight;
    head.biWidth=bmpWidth;
    head.biXPelsPerMeter=0;
    head.biYPelsPerMeter=0;

    //写位图信息头进内存
    fwrite(&head, sizeof(BITMAPINFOHEADER),1, fp);

    //如果灰度图像，有颜色表，写入文件 
    if(biBitCount==8)
        fwrite(pColorTable, sizeof(RGBQUAD),256, fp);

    //写位图数据进文件
    fwrite(pBmpBuf, bmpHeight*lineByte, 1, fp);

    //关闭文件
    fclose(fp);
    cout<<"-------------------save successfully---------------------"<<endl;
    return 1;
}

void BMP::convertGray(char *bmpName)
{
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
    saveBmp(bmpName);
    cout<<"-------------------convert successfully---------------------"<<endl;
}

void BMP::binary(char *bmpName){
    int linebyte1=(bmpWidth*biBitCount/8+3)/4*4;//每行的字节数，由于存在4字节补齐
     //转换二值图像
    if(biBitCount == 24)
    {
        for(int i=0;i<bmpHeight;i++)
        {
            for(int j=0;j<bmpWidth;j++)
            {
                unsigned char gray;
                gray=pBmpBuf[i*linebyte1+j*3];
                gray=(gray>100)?255:0;                
                pBmpBuf[i*linebyte1+j*3]=gray;
                pBmpBuf[i*linebyte1+j*3+1]=gray;
                pBmpBuf[i*linebyte1+j*3+2]=gray;
            }
        }
    }
    // 将灰度图写入文件
    saveBmp(bmpName);
    cout<<"-------------------binary successfully---------------------"<<endl;
}