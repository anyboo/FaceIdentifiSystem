
#ifndef _FIIMAGE_H_
#define _FIIMAGE_H_

#include "StdDefEx.h"

#ifdef __cplusplus
extern "C" {
#endif 
	
#if 0
	#ifdef FIIMAGE_EXPORTS
	#define FIIMAGE_API __declspec(dllexport)
	#else
	#define FIIMAGE_API __declspec(dllimport)
	#endif
#else
	#define FIIMAGE_API
#endif


// �Ҷ�ͼ��
typedef struct CImgGray
{
	int				nWidth;		// ͼ����
	int				nHeight;	// ͼ��߶�
	unsigned char	*pData;		// Gray��ʽ��8 λ
}CImgGray;

// RGB ��ɫͼ��
typedef struct CImgRGB
{
	int				nWidth;		// ͼ����
	int				nHeight;	// ͼ��߶�
	unsigned char	*pData;		// RGB��ʽ��24 λ
}CImgRGB;

// YUV420 ��ɫͼ��
typedef struct CImgYUV420
{
	int				nWidth;		// ͼ����
	int				nHeight;	// ͼ��߶�
	unsigned char	*pData;		// YUV420��ʽ��24 λ
}CImgYUV420;

//
void itYUV2RGB(	unsigned char  y, unsigned char  u, unsigned char  v, 
							unsigned char *r, unsigned char *g, unsigned char *b);

void itRGB2YUV(	unsigned char  r, unsigned char  g, unsigned char  b,
							unsigned char *y, unsigned char *u, unsigned char *v);

void itImageYUV2RGB(unsigned char *pBufYUV420, int nWidth, int nHeight, unsigned char *pBufRGB);
void ImgYUV420p_2_RGB565(unsigned char* psrc, int width, int height, unsigned char* pDst);

// �Ҷ�ͼ������ӿ�
FIIMAGE_API CImgGray *	ImgGray_New(int width, int height);
FIIMAGE_API CImgGray *	ImgGray_Create(int width, int height, unsigned char *pData);
FIIMAGE_API CImgGray *	ImgGray_Clone(CImgGray imgSrc);
FIIMAGE_API CImgGray *	ImgGray_CutIcon(CImgGray imgSrc, RECT rtIcon);
FIIMAGE_API void		ImgGray_Destroy(CImgGray **pImage);
FIIMAGE_API BOOL		ImgGray_IsValid(CImgGray imgSrc);

FIIMAGE_API CImgGray *	ImgGray_FromImgRGB(CImgRGB imgSrc);

// RGB ��ɫͼ������ӿ�
FIIMAGE_API CImgRGB *	ImgRGB_New(int width, int height);
FIIMAGE_API CImgRGB *	ImgRGB_Create(int width, int height, unsigned char *pData);
FIIMAGE_API CImgRGB *	ImgRGB_Clone(CImgRGB imgSrc);
FIIMAGE_API CImgRGB *	ImgRGB_CutIcon(CImgRGB imgSrc, RECT rtIcon);
FIIMAGE_API void		ImgRGB_Destroy(CImgRGB **pImage);
FIIMAGE_API BOOL		ImgRGB_IsValid(CImgRGB imgSrc);

FIIMAGE_API CImgRGB *	ImgRGB_FromImgYUV420(CImgYUV420 imgSrc);

// YUV420 ��ɫͼ������ӿ�
FIIMAGE_API CImgYUV420 *	ImgYUV420_New(int width, int height);
FIIMAGE_API CImgYUV420 *	ImgYUV420_Create(int width, int height, unsigned char *pData);
FIIMAGE_API CImgYUV420 *	ImgYUV420_Clone(CImgYUV420 imgSrc);
FIIMAGE_API CImgYUV420 *	ImgYUV420_CutIcon(CImgYUV420 imgSrc, RECT rtIcon);
FIIMAGE_API void		ImgYUV420_Destroy(CImgYUV420 **pImage);
FIIMAGE_API BOOL		ImgYUV420_IsValid(CImgYUV420 imgSrc);

#ifdef __cplusplus
}
#endif  

#endif // _FIIMAGE_H_