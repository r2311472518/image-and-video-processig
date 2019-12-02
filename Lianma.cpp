#include"ImageProcess.h"
void Lianma:: caulculate(BMP *p){
    
    unsigned char *pBmpBuf=p->getPBmpBuf();
    int bmpHeight=p->getHeight();
    int bmpWidth=p->getWidth();
    int biBitCount=p->getBiBitCount();
    int linebyte1=(bmpWidth*biBitCount/8+3)/4*4;

    if(biBitCount == 24)
    {
        int x[8]={0,-1,-1,-1,0,1,1,1};
        int y[8]={1,1,0,-1,-1,-1,0,1};
        //找到起始点
        unsigned gray;
        gray=pBmpBuf[0];
        int i,j;
        for(i=0;i<bmpHeight;i++)
        {
            for(j=0;j<bmpWidth;j++){
                gray=pBmpBuf[i*linebyte1+j*3];
                if(gray==0) break;
                //cout<<(gray==0?0:1)<<" ";
            }
            if(gray==0) break;
            //cout<<","<<i<<endl;
        }
        point[0]=i;
        point[1]=j;

        //计算链码
        int stop=0;int find=0;
        int L[10000]={0};
        while(stop==0){
            find=0;
            int k=0;int f=0;//f代表该点是否已经被遍历过
            //寻找下一个点，找到find=1，找不到find=0
            for(f=0,k=0;k<=7;k++){
                gray=pBmpBuf[(i+x[k])*linebyte1+(j+y[k])*3];
                if (gray==0) {
                   int a=point[0],b=point[1];
                   int n=0;
                   for(f=0;n<number;n++){
                       a=a+x[L[n]];b=b+y[L[n]];
                       if(a==(i+x[k])&&b==(j+y[k])) {f=1;break;}
                    }
                }
                if (gray==0&&f==0) {find=1;break;}
            }
            if (find==1) {
                    i=i+x[k];
                    j=j+y[k];
                    L[number]=k;
                    number++;
                }
            if (find==0&&k>7) stop=1;
            if ((i==point[0])&&(j==point[1])) stop=1;
        }
        //设置链码
        lm=new int[number];
        for(int k=0;k<number;k++){
            lm[k]=L[k];
        }
        cout<<"Lianma suc"<<endl;
    }
}

void Lianma:: show(){
    cout<<"起始点坐标：("<<point[0]<<","<<point[1]<<")"<<endl;
    cout<<"周长："<<number+1<<endl;
    cout<<"链码：";
    if(number!=0){
        for(int k=0;k<number;k++){
        cout<<lm[k]<<" ";
        }
    }
    cout<<endl;
}