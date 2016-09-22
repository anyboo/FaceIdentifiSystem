/*
* Copyright (c) 2014, �����з���˹�Ƽ����޹�˾
* All rights reserved.
* 
* �ļ����ƣ�FrImageFunc.h
* �������ڣ�2014��5��31��
* �ļ���ʶ��
* �ļ�ժҪ������ RGB ͼ������ӿڡ�
* 
* ��ǰ�汾��1.0.0.0
* ��    �ߣ�
* ������ڣ�2014��5��31��
* �汾ժҪ��
* 
* ȡ���汾��
* ԭ����  ��
* ������ڣ�
* �汾ժҪ��
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
void WriteBmpFile(LPCTSTR szFile, LPBYTE pRgb24Data, int nWidth, int nHeight); //nHeight ֧������ֵ
BOOL FlipV_RGB(BYTE * ptRgb24Image, long lwidth, long lheight, BYTE * ptSwapline, long nbytesLine);
BOOL Rgb24_To_Rgb8(BYTE * rgb24, BYTE * rgb8, int width, int height);
void ReverseRgbToBgr(BYTE * pRgb, int nWidth, int nHeight);

BOOL ReadJpgGetFeature(CString path,BYTE* feature);

/////////////////////////////////////////////////////////////////////////////////////
//  ResizeImage by pengshengfeng
//  ����:ͼ�����ź���
//  ����:
//  src:                ����ͼ������
//  nSrcWidth:     ����ͼ����
//  nSrcHeight:     ����ͼ��߶�
//  dst:                ���ͼ������
//  nDstWidth:     ���ͼ����
//  nDstHeight:     ���ͼ��߶�
//  nChannel:        ͼ��ͨ������ɫΪ3�Ҷ�Ϊ1
/////////////////////////////////////////////////////////////////////////////////////
void ResizeImage(unsigned char *src,int nSrcWidth, int nSrcHeight, unsigned char *dst,int nDstWidth, int nDstHeight,int nChannel);

/////////////////////////////////////////////////////////////////////////////////////
//  ZoomImage by pengshengfeng
//  ����:��ָ����С�ü��������ͼ��
//  ����:
//  src:                ����ͼ������
//  nSrcWidth:     ����ͼ����
//  nSrcHeight:     ����ͼ��߶�
//  dst:                ���ͼ������
//  nDstWidth:     ���ͼ����
//  nDstHeight:     ���ͼ��߶�
//  nChannel:        ͼ��ͨ������ɫΪ3�Ҷ�Ϊ1
/////////////////////////////////////////////////////////////////////////////////////
void ZoomImage(unsigned char *src, int nSrcWidth, int nSrcHeight, unsigned char *dst, int nDstWidth, int nDstHeight, int nChannel);

///////////////////////////////////////////////////////////////////////////

///////////////////////////////////////////////////////////////////////////
void CutPicByBuf(BYTE* pRgb24Buf,int w, int h, RECT rc,BYTE*pRgb24CutBuf);

#endif // __FRIMAGEFUNC_H__
