#ifndef _BMP_H
#define _BMP_H

typedef unsigned short WORD;
typedef unsigned long DWORD;
typedef unsigned char BYTE;
typedef long LONG;

struct BITMAPFILEHEADER
{
    WORD  bfType;	//恒为BM
    DWORD bfSize;	//图像总大小
    WORD  bfReserved1;
    WORD  bfReserved2;
    DWORD bfOffBits;	//偏移量，即文件头、信息头、颜色表的字节数之和
};

struct BITMAPINFOHEADER
{
    DWORD biSize;		//信息头大小
    LONG  biWidth;		//宽度大小（像素）
    LONG  biHeight;		//高度大小（像素）
    WORD  biPlanes;
    WORD  biBitCount;	//图像位数
    DWORD biCompression;
    DWORD biSizeImage;	//DIB数据区大小
    LONG  biXPelsPerMeter;
    LONG  biYPelsPerMeter;
    DWORD biClrUsed;
    DWORD biClrImportant;
};

struct RGBQUAD		//颜色表
{
    BYTE rgbBlue;
    BYTE rgbGreen;
    BYTE rgbRed;
    BYTE rgbReserved;
};

#endif
