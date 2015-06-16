#ifndef COMFUN_H_
#define COMFUN_H_

#include "stackFun.h"

//求出num为2^n
int log2(int num)
{
    if (num == 1)
        return 0;
    else
        return 1 + log2(num >> 1);
}

//通过十进制Morton码求出行号
int MortonToRow(int n, WORD M)
{
    //分别使用n位和2*n位二进制数表示行列号和Morton码
    int *binaryR = new int[n];		//存储Row的二进制值
    int *binaryC = new int[n];		//存储Col的二进制值
    int *binaryM = new int[2 * n];	//存储Morton的二进制值
    int dRow, dCol;		//Row和Col的十进制值
    int rest, Mcopy;

    //通过Morton码的值求出行列号的值
    Mcopy = M;
    for (int k = 2 * n - 1; k >= 0; k--)	//将Morton码转变为二进制
    {
        rest = Mcopy % 2;
        Mcopy = Mcopy / 2;
        binaryM[k] = rest;
    }
    for (int k = 0; k < 2 * n; k++)	//通过Morton码求出行列号
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

    return dRow;
}

//通过十进制Morton码求出列号
int MortonToCol(int n, WORD M)
{
    //分别使用n位和2*n位二进制数表示行列号和Morton码
    int *binaryR = new int[n];		//存储Row的二进制值
    int *binaryC = new int[n];		//存储Col的二进制值
    int *binaryM = new int[2 * n];	//存储Morton的二进制值
    int dRow, dCol;		//Row和Col的十进制值
    int rest, Mcopy;

    //通过Morton码的值求出行列号的值
    Mcopy = M;
    for (int k = 2 * n - 1; k >= 0; k--)	//将Morton码转变为二进制
    {
        rest = Mcopy % 2;
        Mcopy = Mcopy / 2;
        binaryM[k] = rest;
    }
    for (int k = 0; k < 2 * n; k++)	//通过Morton码求出行列号
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

    return dCol;
}

#endif