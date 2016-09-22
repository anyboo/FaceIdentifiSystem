#ifndef TH_FACIALX_I_H
#define TH_FACIALX_I_H

#ifdef THFACIALX_EXPORTS
#define THFACIALX_API __declspec(dllexport)
#else
#define THFACIALX_API __declspec(dllimport)
#endif

#define THFACIALX_FACE_INIT			0x01
#define THFACIALX_EYE_INIT			0x02
#define THFACIALX_MOUTH_INIT		0x04
#define THFACIALX_EYE_STATUS_INIT	0x08
#define THFACIALX_ALL_INIT			0x80

struct ThFacialXFea
{
    RECT  rcFace;
    POINT ptLeftEye;
	POINT ptRightEye;
	POINT ptMouth;
	BYTE  bEyeOpen;
};

THFACIALX_API int ThFacialXInit(BYTE nInitFlag);
THFACIALX_API void ThFacialXUnInit();
THFACIALX_API int ThFacialXDetect(BYTE* pRgb24,int nWidth,int nHeight,ThFacialXFea** tff);
THFACIALX_API int ThFacialXDetectFace(BYTE* pRgb24,int nWidth,int nHeight,ThFacialXFea** tff);
THFACIALX_API int ThFacialXDetectEyeByFace(BYTE* pRgb8,int nWidth,int nHeight,RECT rcFace,POINT* ptLeft,POINT* ptRight);

#endif