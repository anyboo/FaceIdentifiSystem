/*
* Copyright (c) 2014, 深圳市飞瑞斯科技有限公司
* All rights reserved.
* 
* 文件名称：FrImageFunc.h
* 创建日期：2014年5月31日
* 文件标识：
* 文件摘要：常规 RGB 图像操作接口。
* 
* 当前版本：1.0.0.0
* 作    者：
* 完成日期：2014年5月31日
* 版本摘要：
* 
* 取代版本：
* 原作者  ：
* 完成日期：
* 版本摘要：
* 
*/

#ifndef __FRIMAGEFUNC_H__
#define __FRIMAGEFUNC_H__

///////////////////////////////////////////////////////////////////////////

inline int NBYTES_RGB24LINE(int widthpix)
{
	return (int)(((widthpix * 24) + 31) / 32 * 4);
}

inline int rgb_nbytes_line(int widthpix, int nbits)
{
	return (int)(((widthpix * nbits) + 31) / 32 * 4);
}

inline int min_int(int x, int y) { return (x <= y ? x : y); }
inline int max_int(int x, int y) { return (x <= y ? y : x); }

//=========================================================================

BOOL RGB2JPG(unsigned char* rgb24,int w, int h, const char * filename);
void WriteBmpFile(LPCTSTR szFile, LPBYTE pRgb24Data, int nWidth, int nHeight); //nHeight 支持正负值
BOOL FlipV_RGB(BYTE * ptRgb24Image, long lwidth, long lheight, BYTE * ptSwapline, long nbytesLine);
BOOL Rgb24_To_Rgb8(BYTE * rgb24, BYTE * rgb8, int width, int height);
void ReverseRgbToBgr(BYTE * pRgb, int nWidth, int nHeight);

BOOL ReadJpgGetFeature(CString path,BYTE* feature);

/////////////////////////////////////////////////////////////////////////////////////
//  ResizeImage by pengshengfeng
//  功能:图像缩放函数
//  参数:
//  src:                输入图像数据
//  nSrcWidth:     输入图像宽度
//  nSrcHeight:     输入图像高度
//  dst:                输出图像数据
//  nDstWidth:     输出图像宽度
//  nDstHeight:     输出图像高度
//  nChannel:        图像通道，彩色为3灰度为1
/////////////////////////////////////////////////////////////////////////////////////
void ResizeImage(unsigned char *src,int nSrcWidth, int nSrcHeight, unsigned char *dst,int nDstWidth, int nDstHeight,int nChannel);

/////////////////////////////////////////////////////////////////////////////////////
//  ZoomImage by pengshengfeng
//  功能:按指定大小裁剪或者填充图像
//  参数:
//  src:                输入图像数据
//  nSrcWidth:     输入图像宽度
//  nSrcHeight:     输入图像高度
//  dst:                输出图像数据
//  nDstWidth:     输出图像宽度
//  nDstHeight:     输出图像高度
//  nChannel:        图像通道，彩色为3灰度为1
/////////////////////////////////////////////////////////////////////////////////////
void ZoomImage(unsigned char *src, int nSrcWidth, int nSrcHeight, unsigned char *dst, int nDstWidth, int nDstHeight, int nChannel);

///////////////////////////////////////////////////////////////////////////

///////////////////////////////////////////////////////////////////////////
void CutPicByBuf(BYTE* pRgb24Buf,int w, int h, RECT rc,BYTE*pRgb24CutBuf);

#endif // __FRIMAGEFUNC_H__
