
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


// 灰度图像
typedef struct CImgGray
{
	int				nWidth;		// 图像宽度
	int				nHeight;	// 图像高度
	unsigned char	*pData;		// Gray格式，8 位
}CImgGray;

// RGB 彩色图像
typedef struct CImgRGB
{
	int				nWidth;		// 图像宽度
	int				nHeight;	// 图像高度
	unsigned char	*pData;		// RGB格式，24 位
}CImgRGB;

// YUV420 彩色图像
typedef struct CImgYUV420
{
	int				nWidth;		// 图像宽度
	int				nHeight;	// 图像高度
	unsigned char	*pData;		// YUV420格式，24 位
}CImgYUV420;

//
void itYUV2RGB(	unsigned char  y, unsigned char  u, unsigned char  v, 
							unsigned char *r, unsigned char *g, unsigned char *b);

void itRGB2YUV(	unsigned char  r, unsigned char  g, unsigned char  b,
							unsigned char *y, unsigned char *u, unsigned char *v);

void itImageYUV2RGB(unsigned char *pBufYUV420, int nWidth, int nHeight, unsigned char *pBufRGB);
void ImgYUV420p_2_RGB565(unsigned char* psrc, int width, int height, unsigned char* pDst);

// 灰度图像操作接口
FIIMAGE_API CImgGray *	ImgGray_New(int width, int height);
FIIMAGE_API CImgGray *	ImgGray_Create(int width, int height, unsigned char *pData);
FIIMAGE_API CImgGray *	ImgGray_Clone(CImgGray imgSrc);
FIIMAGE_API CImgGray *	ImgGray_CutIcon(CImgGray imgSrc, RECT rtIcon);
FIIMAGE_API void		ImgGray_Destroy(CImgGray **pImage);
FIIMAGE_API BOOL		ImgGray_IsValid(CImgGray imgSrc);

FIIMAGE_API CImgGray *	ImgGray_FromImgRGB(CImgRGB imgSrc);

// RGB 彩色图像操作接口
FIIMAGE_API CImgRGB *	ImgRGB_New(int width, int height);
FIIMAGE_API CImgRGB *	ImgRGB_Create(int width, int height, unsigned char *pData);
FIIMAGE_API CImgRGB *	ImgRGB_Clone(CImgRGB imgSrc);
FIIMAGE_API CImgRGB *	ImgRGB_CutIcon(CImgRGB imgSrc, RECT rtIcon);
FIIMAGE_API void		ImgRGB_Destroy(CImgRGB **pImage);
FIIMAGE_API BOOL		ImgRGB_IsValid(CImgRGB imgSrc);

FIIMAGE_API CImgRGB *	ImgRGB_FromImgYUV420(CImgYUV420 imgSrc);

// YUV420 彩色图像操作接口
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