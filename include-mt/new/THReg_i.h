#ifndef THREG_I_H
#define THREG_I_H

#ifdef THREG_EXPORTS
#define THREG_API __declspec(dllexport)
#else
#define THREG_API __declspec(dllimport)
#endif

struct ThImageBuffer
{
	BYTE* pRgbBuffer;
	short nRgbChannel;
	int nWidth;
	int nHeight;
	POINT ptLeftEye;
	POINT ptRightEye;
};

THREG_API BOOL ThRegInit(int nChanNum);
THREG_API BOOL ThRegUnInit();
THREG_API BOOL ThGetFeature(ThImageBuffer* ptib,int nChanID,BYTE* pFeatureBuf,BYTE* pIDFeatureBuf);	
THREG_API int ThGetIDFeatureSize(int nChanID);
THREG_API float ThFeatureCompare(BYTE* pFea1,BYTE* pFea2,int nChanID);	
THREG_API BOOL ThIsGoodFace(BYTE* pGrayBuf,int nWidth,int nHeight,POINT ptLeftEye,POINT ptRightEye,float fThreshold);		

#endif