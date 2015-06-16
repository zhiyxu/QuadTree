#include "ComFun.h"

void unCompress(int num, char *datname, char *colorname, char *bmpname)
{
    int n = log2(num);

    FILE *inptr = fopen(datname, "rb");		//必须使用“rb”方式打开文件！！！不然0x0A会被判定为EOF
    if (inptr == NULL)
    {
        printf("Open Error!");
        return;
    }
    FILE *colorptr = fopen(colorname, "r");
    if (colorptr == NULL)
    {
        printf("Open Error!");
        return;
    }
    FILE *imgptr = fopen(bmpname, "w");
    if (imgptr == NULL)
    {
        printf("Open Error!");
        return;
    }

    /************************************************************************/
    /* 写入文件头信息                                                       */
    /************************************************************************/
    BITMAPFILEHEADER bf;
    bf.bfType = 19778;
    bf.bfReserved1 = 0;
    bf.bfReserved2 = 0;
    bf.bfOffBits = 1078;
    bf.bfSize = num * num + bf.bfOffBits;

    fwrite(&bf.bfType, sizeof(bf.bfType), 1, imgptr);
    fwrite(&bf.bfSize, sizeof(bf.bfSize), 1, imgptr);
    fwrite(&bf.bfReserved1, sizeof(bf.bfReserved1), 1, imgptr);
    fwrite(&bf.bfReserved2, sizeof(bf.bfReserved2), 1, imgptr);
    fwrite(&bf.bfOffBits, sizeof(bf.bfOffBits), 1, imgptr);

    /************************************************************************/
    /* 写入信息头                                                       */
    /************************************************************************/
    BITMAPINFOHEADER bi;
    bi.biSize = 40;
    bi.biWidth = num;
    bi.biHeight = num;
    bi.biPlanes = 1;
    bi.biBitCount = 8;
    bi.biCompression = 0;
    bi.biSizeImage = num * num;
    bi.biXPelsPerMeter = 3780;
    bi.biYPelsPerMeter = 3780;
    bi.biClrImportant = 0;
    bi.biClrUsed = 0;
    fwrite(&bi, sizeof(bi), 1, imgptr);

    /************************************************************************/
    /* 读取颜色表信息并写入                                                 */
    /************************************************************************/
    RGBQUAD quad[256];
    fread(quad, sizeof(RGBQUAD), 256, colorptr);
    fwrite(quad, sizeof(RGBQUAD), 256, imgptr);

    /************************************************************************/
    /* 关键数据的读取，边读边写                                             */
    /************************************************************************/
    InfoStruct Info;
    WORD end = num * num;
    int Row, Col;

    while(fread(&Info.Mvalue, sizeof(Info.Mvalue), 1, inptr) == 1)		//读取数据，必须使用rb方式
    {
        fread(&Info.DIB, sizeof(Info.DIB), 1, inptr);
        for (WORD i = Info.Mvalue; i < end; i++)
        {
            Row = MortonToRow(n, i);
            Col = MortonToCol(n, i);

            long offset = bf.bfOffBits + Row * num + Col;				//通过行列号寻找到写入数据的位置
            fseek(imgptr, offset, SEEK_SET);
            fwrite(&Info.DIB, sizeof(Info.DIB), 1, imgptr);
        }
        end = Info.Mvalue;
    }

    fclose(inptr);
    fclose(colorptr);
    fclose(imgptr);

    return;
}
