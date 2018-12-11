#include <iostream>
#include <windows.h>
#include <stdio.h>

using namespace std;
int bmpwidth,bmpheight,linebyte;
int colornum;
unsigned char *newBmpBuf;
unsigned char *pBmpBuf;  //�洢ͼ������
unsigned char *pColorBuf;  //�洢ͼ������
BITMAPFILEHEADER fileheader;
BITMAPINFOHEADER infoHead;
void printDetial();
bool readBmp(char *bmpName) {

    FILE *fp;
    if( (fp = fopen(bmpName,"rb")) == NULL){
        cout<<"The file "<<bmpName<<"was not opened"<<endl;
        return FALSE;
    }
    if(!fread(&fileheader,sizeof(BITMAPFILEHEADER),1,fp)){
        cout<<"��תʧ��"<<endl;
        return FALSE;
    }

    fread(&infoHead,sizeof(BITMAPINFOHEADER),1,fp);   //��fp�ж�ȡBITMAPINFOHEADER��Ϣ��infoHead��,ͬʱfp��ָ���ƶ�

    if((sizeof(infoHead)+ sizeof(fileheader))!=fileheader.bfOffBits){
//        cout<<fileheader.bfOffBits<<" "<<infoHead.biSize<<"  "<< sizeof(fileheader)<<"  "<< sizeof(char);
        colornum = (fileheader.bfOffBits-(infoHead.biSize+ sizeof(fileheader)))/ sizeof(char);
        pColorBuf = new unsigned char[colornum];
        fread(pColorBuf,sizeof(char),colornum,fp);
    }
    bmpwidth = infoHead.biWidth;
    bmpheight = infoHead.biHeight;
    linebyte = (bmpwidth*infoHead.biBitCount/8+3)/4*4; //����ÿ�е��ֽ�����24����ͼƬ��24λ��bmpͼ��3��ȷ������ʧ����
//    cout<<infoHead.biBitCount;
//    linebyte = 512; //����ÿ�е��ֽ�����24����ͼƬ��24λ��bmpͼ��3��ȷ������ʧ����
//    cout<<bmpwidth<<" "<<bmpheight<<"  "<< sizeof(fileheader)<<"  "<< sizeof(infoHead)<<"   "<<
//    fileheader.bfOffBits<<infoHead.biCompression<<endl;
    pBmpBuf = new unsigned char[linebyte*bmpheight];
    fread(pBmpBuf,sizeof(char),linebyte*bmpheight,fp);
    fclose(fp);   //�ر��ļ�
    printDetial();
    return TRUE;
}
void printDetial(){
    cout<<"bfType:"<<fileheader.bfType<<endl;
    cout<<"bfSize:"<<fileheader.bfSize<<endl;
    cout<<"bfOffBits:"<<fileheader.bfOffBits<<endl;

    cout<<"bfSize:"<<fileheader.bfSize<<endl;
    cout<<"biBitCount:"<<infoHead.biBitCount<<endl;
    cout<<"biWidth:"<<infoHead.biWidth<<endl;
    cout<<"biHeight:"<<infoHead.biHeight<<endl;
    cout<<"biSize:"<<infoHead.biSize<<endl;
    cout<<"biSizeImage:"<<infoHead.biSizeImage<<endl;
}
void writeBmp(){
    char writeFileName[] = "..\\lena_C_cut.bmp";
    BITMAPFILEHEADER fileheader_cut = fileheader;
    BITMAPINFOHEADER infoHead_cut = infoHead;
    fileheader_cut.bfSize = (fileheader_cut.bfSize - sizeof(infoHead)- sizeof(fileheader))/4;
    infoHead_cut.biHeight /= 2;
    infoHead_cut.biWidth /= 2;
    infoHead_cut.biSizeImage /= 4;
    FILE *fp;
    fp=fopen("..\\lena_C_cut.bmp","wb");
    fwrite(&fileheader_cut,sizeof(fileheader_cut),1,fp);
    fwrite(&infoHead_cut,sizeof(infoHead_cut),1,fp);
    newBmpBuf = new unsigned char[linebyte*bmpheight/4];
//    cout<<bmpheight<<linebyte;
    if((sizeof(infoHead)+ sizeof(fileheader))!=fileheader.bfOffBits){
        fwrite(pColorBuf, sizeof(char),colornum,fp);
    }
//    cout<<linebyte<<"sxa"<<bmpheight<<endl;
    for(int i=bmpheight/2;i<bmpheight;i++){
        for(int j=0;j<linebyte/2;j++){
//            cout<<(int)pBmpBuf[i*linebyte+j];
//            cout<<i*linebyte+j<<endl;
             newBmpBuf[(i-bmpheight/2)*linebyte/2+j] = pBmpBuf[i*linebyte+j];
        }
    }
//    cout<<(int)pBmpBuf[0];
    if(!fwrite(newBmpBuf, sizeof(char),linebyte*bmpheight/4,fp)){
        cout<<"д��ʧ��";
    }
}
void solve()
{
    char readFileName[] = "..\\lena.bmp";
    if(FALSE == readBmp(readFileName))
        cout<<"readfile error!"<<endl;
    writeBmp();
}

int main()
{
    solve();

    return 0;
}
