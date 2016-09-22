/*
* Copyright (c) 2014, 深圳市飞瑞斯科技有限公司
* All rights reserved.
* 
* 文件名称：FrImageFunc.cpp
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

#include "stdafx.h"
#include "FrImageFunc.h"
#include <atlimage.h>
#include <algorithm>

///////////////////////////////////////////////////////////////////////////


void WriteBmpFile(LPCTSTR szFile, LPBYTE pRgb24Data, int nWidth, int nHeight)
{
#if 1

	DWORD dwBytesLine = NBYTES_RGB24LINE(nWidth);

	BITMAPFILEHEADER bmheader;
	memset(&bmheader, 0, sizeof(BITMAPFILEHEADER));
	bmheader.bfType = 0x4d42;
	bmheader.bfOffBits = sizeof(BITMAPFILEHEADER) + sizeof(BITMAPINFO);
	bmheader.bfSize = dwBytesLine * (std::abs)(nHeight) + bmheader.bfOffBits;

	BITMAPINFO bmInfo;
	memset(&bmInfo, 0, sizeof(BITMAPINFO));
	bmInfo.bmiHeader.biSize = sizeof(BITMAPINFOHEADER);
	bmInfo.bmiHeader.biWidth = nWidth;
	bmInfo.bmiHeader.biHeight = nHeight;
	bmInfo.bmiHeader.biPlanes = 1;
	bmInfo.bmiHeader.biBitCount = 24;
	bmInfo.bmiHeader.biCompression = BI_RGB;

	CFile file;
	if (file.Open(szFile, CFile::modeCreate | CFile::modeWrite))
	{
		file.Write(&bmheader, sizeof(BITMAPFILEHEADER));
		file.Write(&bmInfo, sizeof(BITMAPINFO));
		file.Write(pRgb24Data, dwBytesLine * (std::abs)(nHeight));
		file.Flush();
		file.Close();
	}
#else
	TCHAR szRFile[_MAX_PATH];
	_stprintf(szRFile, TEXT("%s%s"), szFile, TEXT(".raw"));
	std::string szfile = CStringTostringAnsi(szRFile);
	FILE *file = fopen(szfile.c_str(), "wb");
	if (NULL != file)
	{
		fwrite(pRgb24Data, 1, nWidth * nHeight * 3, file);
		fflush(file);
		fclose(file);
	}
#endif
}

BOOL RGB2JPG(unsigned char* rgb24_bl,int w, int h, const char * filename)
{
	if(w <2 || std::abs(h) < 2)
	{
		return FALSE;
	}
	
	CImage desImg;
	long nbytesline = NBYTES_RGB24LINE(w);
	if(h>0)
	{	
		desImg.Create(w,h,24);
		unsigned char * pTmpBuf = (unsigned char *)desImg.GetPixelAddress(0, h - 1);//倒首地址
		for (int k = h - 1 ; k >=0; k--, pTmpBuf += nbytesline) 
		{
			memcpy(pTmpBuf, (rgb24_bl + k * nbytesline), nbytesline);
		}
	}else
	{
		h = std::abs(h);
		desImg.Create(w,h,24);
		unsigned char * pTmpBuf = (unsigned char *)desImg.GetPixelAddress(0,0);//倒首地址
		for (int k = 0 ; k > h; k++, pTmpBuf += nbytesline) 
		{
			memcpy(pTmpBuf, (rgb24_bl + k * nbytesline), nbytesline);
		}
	}

	if(desImg.Save(filename, Gdiplus::ImageFormatJPEG) == S_OK)
	{
		return TRUE;
	}

	return FALSE;
}

BOOL RGB2JPG1(unsigned char* rgb24_bl,int w, int h, const char * filename)
{
	h = std::abs(h);
	if(w <2 || h < 2)
	{
		return FALSE;
	}

	long nbytesline = NBYTES_RGB24LINE(w);

	CImage desImg;
	desImg.Create(w,h,24);
	unsigned char * pTmpBuf = (unsigned char *)desImg.GetPixelAddress(0, h - 1);//倒首地址
	for (int k = 0 ; k < h; k++, pTmpBuf += nbytesline) 
	{
		memcpy(pTmpBuf, (rgb24_bl + k * nbytesline), nbytesline);
	}

	if(desImg.Save(filename, Gdiplus::ImageFormatJPEG) == S_OK)
	{
		return TRUE;
	}

	return FALSE;
}

BOOL FlipV_RGB(BYTE * ptRgb24Image, long lwidth, long lheight, BYTE * ptSwapline, long nbytesLine)
{
	long lhalfheight = (lheight >> 1);         //右移1位相当于除以2 即 240/2 => 120

	long nbytesline = NBYTES_RGB24LINE(lwidth);//宽是4的倍数*3
	if (nbytesLine < nbytesline)
	{
		return FALSE;
	}

	for (long line = 0; line < lhalfheight; ++line)
	{
		BYTE *srcbytes = (BYTE *)ptRgb24Image + line * nbytesline;
		BYTE *dstbytes = (BYTE *)ptRgb24Image + (lheight - 1 - line) * nbytesline;

		memcpy(ptSwapline, srcbytes, nbytesline);
		memcpy(srcbytes, dstbytes, nbytesline);
		memcpy(dstbytes, ptSwapline, nbytesline);
	}

	return TRUE;
}

BOOL Rgb24_To_Rgb8(BYTE * rgb24, BYTE * rgb8, int width, int height)
{
	long nbytesline = NBYTES_RGB24LINE(width);//宽是4的倍数*3
	for (int i = 0; i < height; ++i)	
	{
		BYTE * ptLine = rgb24 + i * nbytesline;
		for (int j = 0; j < width; ++j)
		{
			rgb8[i * width + j] = (ptLine[j * 3 + 0] + ptLine[j * 3 + 1] + ptLine[j * 3 + 2]) / 3;
		}
	}

	return TRUE;
}

void ReverseRgbToBgr(BYTE * pRgb, int nWidth, int nHeight)
{
	int x;
	int y;
	int pos;
	BYTE temp;

	for (y = 0; y < nHeight; ++y)
	{
		for (x = 0; x < nWidth; ++x)
		{
			pos = (y * nWidth * 3) + (x * 3);

			temp = pRgb[pos + 0];
			pRgb[pos + 0] = pRgb[pos + 2];
			pRgb[pos + 2] = temp;
		}
	}
}

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
void ResizeImage(unsigned char *src,int nSrcWidth, int nSrcHeight, unsigned char *dst,int nDstWidth, int nDstHeight,int nChannel)
{

	int step1= nSrcWidth*nChannel;
	int step2= nDstWidth*nChannel;
	int j;
	int i;
	int k;

	unsigned char* data1= (unsigned char*)src;
	unsigned char* data2= (unsigned char*)dst;

	long xrIntFloat_16=((nSrcWidth)<<16)/nDstWidth+1; 
	long yrIntFloat_16=((nSrcHeight)<<16)/nDstHeight+1;
	const long csDErrorX=-(1<<15)+(xrIntFloat_16>>1);
	const long csDErrorY=-(1<<15)+(yrIntFloat_16>>1);

	long srcy_16=csDErrorY;

	for (j = 0; j < nDstHeight; j++)
	{

		long srcx_16=csDErrorX;
		unsigned long v_8=(srcy_16 & 0xFFFF)>>8;
		long kq=srcy_16>>16;
		kq = max_int(0,kq);
		kq = min_int(kq,nSrcHeight-2);

		for (i = 0;i < nDstWidth; i++)
		{ 	

			unsigned long u_8=(srcx_16 & 0xFFFF)>>8;
			unsigned long pm3_16=(u_8*v_8);
			unsigned long pm2_16=(u_8*(unsigned long)(256-v_8));
			unsigned long pm1_16=(v_8*(unsigned long)(256-u_8));
			unsigned long pm0_16=((256-u_8)*(256-v_8));

			long kp=srcx_16>>16;
			kp = max_int(0,kp);
			kp = min_int(kp,nSrcWidth-2);

			for (k =0;k<3;k++)
			{
				data2[j*step2+i*3+k] =(unsigned char) ( (pm0_16*data1[kq*step1+kp*3+k] + pm1_16*data1[(kq+1)*step1+kp*3+k] +
					pm2_16*data1[kq*step1+(kp+1)*3+k] +pm3_16*data1[(kq+1)*step1+(kp+1)*3+k])>>16 ) ;

			}
			srcx_16+=xrIntFloat_16;

		}
		srcy_16+=yrIntFloat_16;

	}

}


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
void ZoomImage(unsigned char *src, int nSrcWidth, int nSrcHeight, unsigned char *dst, int nDstWidth, int nDstHeight, int nChannel)
{
	if (nSrcHeight>=nDstHeight&&nSrcWidth>=nDstWidth)
	{
		int xx = nSrcWidth-nDstWidth;
		int yy = nSrcHeight-nDstHeight;
		int left = xx/2;
		int top = yy/2;
		for (int i =0;i<nDstHeight;i++)
		{
			memcpy(dst+i*nDstWidth*nChannel, src+(top+i)*nSrcWidth*nChannel+left*nChannel, nDstWidth*nChannel);
		}
	}
	else if (nSrcHeight<nDstHeight&&nSrcWidth<nDstWidth)
	{
		memset(dst,0,nDstWidth*nDstHeight*3);
		int xx = nDstWidth-nSrcWidth;
		int yy = nDstHeight-nSrcHeight;
		int left = xx/2;
		int top = yy/2;
		for (int i =0;i<nSrcHeight;i++)
		{
			memcpy(dst+(i+top)*nDstWidth*nChannel+left*nChannel, src+i*nSrcWidth*nChannel, nSrcWidth*nChannel);
		}
	}
	else if (nSrcHeight>=nDstHeight&&nSrcWidth<nDstWidth)
	{
		memset(dst,0,nDstWidth*nDstHeight*3);

		int xx = nDstWidth-nSrcWidth;
		int yy = nSrcHeight - nDstHeight;
		int left = xx/2;
		int top = yy/2;
		for (int i =0;i<nDstHeight;i++)
		{
			memcpy(dst+i*nDstWidth*nChannel+left*nChannel, src+(i+top)*nSrcWidth*nChannel, nSrcWidth*nChannel);
		}
	}
	else// nSrcHeight<=nDstHeight&&nSrcWidth>nDstWidth
	{
		memset(dst,0,nDstWidth*nDstHeight*3);

		int xx = nSrcWidth-nDstWidth;
		int yy = nDstHeight - nSrcHeight;
		int left = xx/2;
		int top = yy/2;
		for (int i =0;i<nSrcHeight;i++)
		{
			memcpy(dst+(i+top)*nDstWidth*nChannel, src+i*nSrcWidth*nChannel+left*nChannel, nDstWidth*nChannel);
		}
	}

}

BOOL ReadJpgGetFeature(CString path,BYTE* feature)
{
	/*
	HRESULT iRet;
	CImage desImg;
	iRet=desImg.Load(path);
	if(S_OK != iRet)
		return FALSE;
	int nWidth=desImg.GetWidth();
	int nHeight=desImg.GetHeight();
	BYTE* pRgb8=new BYTE[nWidth*nHeight];
	BYTE* pRgbL=new BYTE[4*nWidth];
	memset(pRgb8,0,nWidth*nHeight);

	BYTE * pImageData =	pImageData=(BYTE *)desImg.GetBits()+(desImg.GetPitch()*(desImg.GetHeight()-1));
    FlipV_RGB(pImageData, nWidth, nHeight, pRgbL,4*nWidth);
	Rgb24_To_Rgb8(pImageData, pRgb8,nWidth,nHeight);

	BOOL bRet=FALSE;
	int s_nFeatureSize =  THFaceGetFeatureSize();
	BYTE* pFeartures=new BYTE[s_nFeatureSize*100];
	ThFacePos ptfp[100];
	int nFace=THFaceDetectMT(pRgb8,nWidth,nHeight,ptfp,pFeartures);//多线程检测，并输入对应的特征
	if(nFace)
	{
		bRet = TRUE;
		memset(feature,0,s_nFeatureSize);
		memcpy(feature,pFeartures,s_nFeatureSize);
	}
	delete []pFeartures;
	delete []pRgb8;
	delete []pRgbL;
	return bRet;
	*/
	return NULL;
}


///////////////////////////////////////////////////////////////////////////
// bits = width * depthbits, (depthbits: 8, 16, 24, 32)
int FI_NBYTES_SCANLINE(int bits)
{
	return (int)(((bits) + 31) / 32 * 4);
}

void CutPicByBuf(BYTE* pRgb24Buf,int w, int h, RECT rc,BYTE*pRgb24CutBuf)
{
	int x = rc.right - rc.left;
	int y = rc.bottom - rc.top;
	BYTE * pTmpBuf = pRgb24CutBuf;

	int nbytesline = FI_NBYTES_SCANLINE(x * 24);

	for (int k = rc.top; k < rc.bottom; k++, pTmpBuf += nbytesline) 
	{
		memcpy(pTmpBuf, (pRgb24Buf + k * w * 3 + rc.left * 3), nbytesline);
	}
}