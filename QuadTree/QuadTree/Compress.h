#include "ComFun.h"

int Compress(int num, char *datname, char *colorname, char *bmpname)	//数据的压缩操作
{
    int n = log2(num);
    int StackCount(0);

    FILE *inptr = fopen(bmpname, "r");
    if (inptr == NULL)
    {
        printf("Open Error！");
        return 0;
    }

    /************************************************************************/
    /*文件头读取                                                            */
    /************************************************************************/
    BITMAPFILEHEADER bf;
    fread(&bf.bfType, sizeof(bf.bfType), 1, inptr);
    printf("\n图像的详细信息如下：\nbf.bfType:%c%c\n", bf.bfType % 256, bf.bfType / 256);
    fread(&bf.bfSize, sizeof(bf.bfSize), 1, inptr);
    printf("bf.bfSize:%d\n", bf.bfSize);
    fread(&bf.bfReserved1, sizeof(bf.bfReserved1), 1, inptr);
    printf("bf.bfReserved1:%d\n", bf.bfReserved1);
    fread(&bf.bfReserved2, sizeof(bf.bfReserved2), 1, inptr);
    printf("bf.bfReserved2:%d\n", bf.bfReserved2);
    fread(&bf.bfOffBits, sizeof(bf.bfOffBits), 1, inptr);
    printf("bf.bfOffBits:%d\n", bf.bfOffBits);

    /************************************************************************/
    /*信息头读取                                                            */
    /************************************************************************/
    BITMAPINFOHEADER bi;
    fread(&bi, sizeof(BITMAPINFOHEADER), 1, inptr);
    printf("bi.biSize:%d\n", bi.biSize);
    printf("bi.biWidth:%d\n", bi.biWidth);
    printf("bi.biHeight:%d\n", bi.biHeight);
    printf("bi.biSizeImage:%d\n", bi.biSizeImage);
    printf("bi.biBitCount:%d\n", bi.biBitCount);

    /************************************************************************/
    /* 1024B颜色表数据的处理                                                */
    /************************************************************************/
    FILE *colorptr = fopen(colorname, "w");

    RGBQUAD quad[256];
    fread(quad, sizeof(RGBQUAD), 256, inptr);

    for (int i = 0; i < 256; i++)
    {
        fprintf(colorptr, "%c%c%c%c", quad[i].rgbBlue, quad[i].rgbGreen, quad[i].rgbRed, quad[i].rgbReserved);
    }

    /************************************************************************/
    /*对DIB栅格属性数据进行处理                                             */
    /************************************************************************/
    Node *header = new Node;	//构建栈结构用以存储Morton编码&属性数据
    header->pred = NULL;
    header->succ = NULL;

    InfoStruct str;
    int M(0), layer(0);		//Morton码及数据层次初始化
    int count(0);			//计算取了多少次2*2栅格

    //分别使用n位和2*n位二进制数表示行列号和Morton码
    int *binaryR = new int[n];		//存储Row的二进制值
    int *binaryC = new int[n];		//存储Col的二进制值
    int *binaryM = new int[2 * n];	//存储Morton的二进制值
    int dRow, dCol;			//行号和列号的十进制值
    int rest, Mcopy;

    while (M < num * num)	//Morton码的结束条件，不是n*n！
    {
        //通过Morton码的值求出行列号的值
        Mcopy = M;
        for (int k = 2 * n - 1; k >= 0; k--)	//将Morton码转变为二进制
        {
            rest = Mcopy % 2;
            Mcopy = Mcopy / 2;
            binaryM[k] = rest;
        }
        for (int k = 0; k < 2 * n; k++)			//通过Morton码求出行列号
        {
            if (k % 2 == 0)
                binaryR[k / 2] = binaryM[k];
            else
                binaryC[k / 2] = binaryM[k];
        }

        dRow = 0;
        dCol = 0;	//低级错误，这个应该放在外面
        for (int k = n - 1, q = 1; k >= 0; k--, q *= 2)		//将行列号的二进制值化为十进制值
        {
            dRow += binaryR[k] * q;
            dCol += binaryC[k] * q;
        }

        //读取2*2大小窗格的DIB栅格属性数据(使用fseek以通过行列号寻找数据)
        BYTE *DIBData = new BYTE[4];
        long offset1 = bf.bfOffBits + dRow * num + dCol;
        fseek(inptr, offset1, SEEK_SET);
        fread(&DIBData[0], sizeof(BYTE), 1, inptr);
        long offset2 = bf.bfOffBits + dRow * num + dCol + 1;
        fseek(inptr, offset2, SEEK_SET);
        fread(&DIBData[1], sizeof(BYTE), 1, inptr);
        long offset3 = bf.bfOffBits + (dRow + 1) * num + dCol;
        fseek(inptr, offset3, SEEK_SET);
        fread(&DIBData[2], sizeof(BYTE), 1, inptr);
        long offset4 = bf.bfOffBits + (dRow + 1) * num + dCol + 1;
        fseek(inptr, offset4, SEEK_SET);
        fread(&DIBData[3], sizeof(BYTE), 1, inptr);

        //比较属性值，确定如何入栈
        if (DIBData[0] == DIBData[1] && DIBData[1] == DIBData[2] && DIBData[2] == DIBData[3])	//所有的值相等，则只取左上角Morton&DIB入栈
        {
            str.Mvalue = M;
            str.DIB = DIBData[0];
            stackPush(header, str);
            M += 4;
        }
        else
        {
            for (int i = 0; i < 4; i ++)		//将四个属性值值均入栈
            {
                str.Mvalue = M++;
                str.DIB = DIBData[i];
                stackPush(header, str);
            }
        }
        count++;

        //判断M值是否到了需要压缩的条件，每当读取了4个2*2窗格，便需判断压缩，重点！
        if (count % 4 == 0)
        {
            InfoStruct *store =  new InfoStruct[4];

            for (int i = 0; i <= layer; i++)				//到达某一层次的数据后，需要按照之前所有层次要求进行检查压缩
            {
                if ((M % (int)pow((double)4, i + 2)) == 0)	//确保不会跨层压缩，关键！！！
                {
                    Node *record = header->succ;
                    if (record != NULL) store[0] = record->info;
                    else break;
                    if (record->succ != NULL) store[1] = record->succ->info;
                    else break;
                    if (record->succ->succ != NULL) store[2] = record->succ->succ->info;
                    else break;
                    if (record->succ->succ->succ != NULL) store[3] = record->succ->succ->succ->info;
                    else break;

                    if (store[0].Mvalue - store[1].Mvalue == pow((double)4, i + 1)			//判断最上面四个节点M值是否相差一定值，判断语句顺序不要弄反
                            && store[1].Mvalue - store[2].Mvalue == pow((double)4, i + 1)
                            && store[2].Mvalue - store[3].Mvalue == pow((double)4, i + 1)
                            && store[3].DIB == store[2].DIB
                            && store[2].DIB == store[1].DIB
                            && store[1].DIB == store[0].DIB)								//首先要求Morton值相差为4^(layer+1)，其次要求属性值全相等
                        stackPop(header, 3);
                    else
                        break;
                    record = NULL;
                }
            }
            delete store;
            store = NULL;
        }
        if (count != 1 && (count & count - 1) == 0 && log2(count) % 2 == 0)		//count为4的次幂时，层次增加。注意:count==1时也满足4^0要求，故去掉
            layer++;
    }
    //至此，所有的栅格数据已读取完毕，四叉树已存储在栈中

    /************************************************************************/
    /* 接下来需要按照行程编码的压缩方式对四叉树编码结果进一步压缩，并写为文件
    /************************************************************************/
    FILE *outptr = fopen(datname, "wb");   //必须使用“wb”方式打开文件！！！不然0x0A会被写为0x0D0A！！！
    if (outptr == NULL)
    {
        printf("Open Error.");
    }

    //第二次压缩——行程编码压缩！并输出！！！采用的是即时输出，并将已输出节点剔出栈的方式
    Node *recorder = header->succ;
    int DIBsame(0);
    while (recorder->succ != NULL)
    {
        BYTE DIBval = recorder->info.DIB;
        while (recorder->succ != NULL && recorder->succ->info.DIB == DIBval)
        {
            DIBsame++;
            recorder = recorder->succ;
        }
        if (DIBsame != 0) stackPop(header, DIBsame);
        if (DIBsame == 0)
        {
            //fprintf(outptr, "%x %x\n", recorder->info.Mvalue, recorder->info.DIB);
            //fprintf(outptr, "%c%c%c", recorder->info.Mvalue % 256, recorder->info.Mvalue / 256, recorder->info.DIB);
            fwrite(&recorder->info.Mvalue, sizeof(recorder->info.Mvalue), 1, outptr);
            fwrite(&recorder->info.DIB, sizeof(recorder->info.DIB), 1, outptr);
            StackCount++;
            recorder = recorder->succ;
            stackPop(header, 1);
            continue;
        }
        DIBsame = 0;

        if (recorder->succ == NULL) break;

        //fprintf(outptr, "%x %x\n", recorder->info.Mvalue, recorder->info.DIB);
        //fprintf(outptr, "%c%c%c", recorder->info.Mvalue % 256, recorder->info.Mvalue / 256, recorder->info.DIB);
        fwrite(&recorder->info.Mvalue, sizeof(recorder->info.Mvalue), 1, outptr);
        fwrite(&recorder->info.DIB, sizeof(recorder->info.DIB), 1, outptr);
        StackCount++;
        recorder = recorder->succ;
        stackPop(header, 1);
    }
    //fprintf(outptr, "%x %x\n", recorder->info.Mvalue, recorder->info.DIB);
    //fprintf(outptr, "%c%c%c", recorder->info.Mvalue % 256, recorder->info.Mvalue / 256, recorder->info.DIB);
    fwrite(&recorder->info.Mvalue, sizeof(recorder->info.Mvalue), 1, outptr);
    fwrite(&recorder->info.DIB, sizeof(recorder->info.DIB), 1, outptr);
    StackCount++;
    stackPop(header, 1);

    delete header;
    header = NULL;

    fclose(colorptr);
    fclose(outptr);
    fclose(inptr);

    return StackCount;
}