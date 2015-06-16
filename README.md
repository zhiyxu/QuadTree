# QuadTree
compression and uncompression of BMP with QuadTree algorithm

本项目使用线性四叉树快速动态压缩编码的方法，实现了对8位bmp图像文件的压缩和解压缩
各文件意义如下：

QuadTree.cpp
	main函数

bmp.h
	bmp图像文件头、信息头、颜色表的结构定义

ComFun.h
	公用函数log2、MortonToRow、MortonToCol的定义

stackFun.h
	栈节点以及栈的Push和Pop操作描述

Compress.h
	bmp图像四叉树压缩方法的描述

unCompress.h
	通过压缩文件以及颜色表对bmp图像进行解压缩方法的描述
