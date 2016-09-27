#ifndef THFACE_I_H
#define THFACE_I_H

#define THFACE_API

struct ThFacePos
{
    RECT  rcFace;
    POINT ptLeftEye;
	POINT ptRightEye;
	POINT ptMouth;
	POINT ptAngle;
	DWORD dwReserved;
};

THFACE_API int		THFaceInit(BYTE nInitFlag);

THFACE_API int		THFaceGetFeatureSize();
THFACE_API int		THFaceDetect(BYTE* pGray,int nWidth,int nHeight,ThFacePos** pptfp,BYTE* pFeatures=NULL);
THFACE_API int		THFaceDetectMT(BYTE* pGray,int nWidth,int nHeight,ThFacePos* pptfp,BYTE* pFeatures=NULL);//muti-thread interface
THFACE_API float	THFaceCompare(BYTE* pFeature1,BYTE* pFeature2);

//fast compare interface,begin
THFACE_API int		THFaceCreateFeatureDB(int dbID,int maxFeatureNum);
THFACE_API int		THFaceAddFeatureToDB(int dbID,BYTE* pFeatures,int nNum);
THFACE_API int		THFaceFeatureDBSize(int dbID);
THFACE_API int		THFaceCompareFeatureDB(int dbID,BYTE* pFeature,float* fScores);
THFACE_API void		THFaceReleaseFeatureDB(int dbID);
//fast compare interface,end

THFACE_API void		THFaceUnInit();

#endif