/****************************************************************************************
 * QuadTree
 * Function:Compression and uncompression of 8-bit bmp image file using quadtree coding method
 * by: Xu Zhiyuan, xzy_1108@163.com
 * 2015/05/06
******************************************************************************************/

#include "Compress.h"
#include "unCompress.h"

int main()
{
    int num, n;
    char bmpname[50], datname[50], colorname[50];

    printf("#注：此程序的作用是为8bit(256色位)bmp图像进行四叉树编码压缩\n#所有的数据均在.\\QuadTree\\QuadTree目录下操作\n");
    printf("\n请输入 1-压缩BMP 或者 2-解压缩BMP  ");
    int x;
    scanf("%d", &x);
    if (x == 1)
    {
        printf("\n请输入图像的名称及后缀名：");
        scanf("%s", bmpname);
        printf("\n请输入正方形图像的边长(确保图像边长大小为2的次幂)：");
        scanf("%d", &num);
        if (num & (num - 1))	//num不为2的幂次方
        {
            printf("\n不可进行四叉树编码，请确保输入图像边长大小为2的幂次方！\n");
            return 0;
        }
        else
            n = log2(num);

        printf("\n请输入图像压缩后压缩文件的名称及后缀名：");		//如何解决路径输出问题
        scanf("%s", datname);
        printf("\n请输入图像压缩后生成颜色表文件的名称及后缀名：");		//如何解决路径输出问题
        scanf("%s", colorname);

        printf("\n共压缩为 %d 项数据\n", Compress(num, datname, colorname, bmpname));
    }
    else if (x == 2)
    {
        printf("\n请输入压缩文件的名称及后缀名：");		//如何解决路径输出问题
        scanf("%s", datname);
        printf("\n请输入颜色表文件的名称及后缀名：");
        scanf("%s", colorname);
        printf("\n请输入正方形图像的边长(确保图像边长大小为2的次幂)：");
        scanf("%d", &num);
        if (num & (num - 1))	//num不为2的幂次方
        {
            printf("\n不可进行四叉树编码，请确保输入图像边长大小为2的幂次方！\n");
            return 0;
        }
        else
            n = log2(num);
        printf("\n请输入解压后图像的名称及后缀名：");
        scanf("%s", bmpname);

        unCompress(num, datname, colorname, bmpname);
        printf("\n解压缩完毕！\n");
    }
    else
        printf("\n请输入正确的编号！\n");

    system("pause");
    return 0;
}