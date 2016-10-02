#ifndef THFACEIMAGE_I_H
#define THFACEIMAGE_I_H

/*
* ============================================================================
*  Name     : THFaceImage_i.h
*  Part of  : Face Recognition (THFaceImage) SDK
*  Created  : 11.1.2012 by XXX
*  Description:
*     THFaceImage_i.h -  Face Recognition (THFaceImage) SDK header file
*  Version  : 1.0.0
*  Copyright: All Rights Reserved by XXXX
*  Revision:
* ============================================================================
*/

#define THFACEIMAGE_API

//////Struct define//////

struct FaceAngle
{
	int   yaw;//angle of yaw,from -90 to +90,left is negative,right is postive
	int   pitch;//angle of pitch,from -90 to +90,up is negative,down is postive
	int   roll;//angle of roll,from -90 to +90,left is negative,right is postive
};

struct THFI_FacePos
{
    RECT		rcFace;//coordinate of face
   	POINT		ptLeftEye;//coordinate of left eye
	POINT		ptRightEye;//coordinate of right eye
	POINT		ptMouth;//coordinate of mouth
	POINT		ptNose;//coordinate of nose								
	FaceAngle	fAngle;//value of face angle
	int			nQuality;//quality of face(from 0 to 100)
	DWORD		dwReserved;//reserved value,must be NULL
	THFI_FacePos()
	{
		memset(&rcFace,0,sizeof(RECT));
		memset(&ptLeftEye,0,sizeof(POINT));
		memset(&ptRightEye,0,sizeof(POINT));
		memset(&ptMouth,0,sizeof(POINT));
		memset(&ptNose,0,sizeof(POINT));
		memset(&fAngle,0,sizeof(FaceAngle));
		nQuality=0;
		dwReserved=NULL;
	}
};

#define FACE_DATA_SIZE 80*1024
struct THFI_FaceData
{
	BYTE* pFaceData;//face binrary data for feature extraction
	THFI_FaceData()
	{
		pFaceData=new BYTE[FACE_DATA_SIZE];
	}
	~THFI_FaceData()
	{
		delete []pFaceData;pFaceData=NULL;
	}

};

struct THFI_Param
{
	int nMinFaceSize;//min face width size can be detected,default is 50 pixels
	int nRollAngle;//max face roll angle,default is 30(degree)
	bool bOnlyDetect;//only detect face or not(if it's true,only Detect API is enable,Feature API is disable),defaule is false(Detect API is enable,and Feature API is enable)
	DWORD dwReserved;//reserved value,must be NULL
	THFI_Param()
	{
		nMinFaceSize=50;
		nRollAngle=30;
		bOnlyDetect=false;
		dwReserved=NULL;
	}
};


//////API define//////


/******Create API******/

THFACEIMAGE_API int		THFI_Create(short nChannelNum,THFI_Param* pParam);
/*
 The THFI_Create function will initialize the algorithm engine module

 Parameters:
	nChannelNum[intput],algorithm channel num,for multi-thread mode,one thread uses one channel
	pParam[input],algorithm engine parameter.
 Return Values:
	If the function succeeds, the return value is valid channel number.
	If the function fails, the return value is zero or negative;
 Remarks: 
	This function only can be called one time at program initialization.
*/

/******Detect API(only detect face)******/

THFACEIMAGE_API int		THFI_DetectFace(short nChannelID,BYTE* pImage,int bpp,int nWidth,int nHeight,THFI_FacePos* pfps,int nMaxFaceNums);
/*
 The THFI_DetectFace function execute face detection only.

 Parameters:
	nChannelID[input],channel ID(from 0 to nChannelNum-1)
	pImage[input],image data buffer,RGB24 or grayscale format.
	bpp[input],bits per pixel(8-grayscale image, 24-RGB24 image). 
	nWidth[input],image width.
	nHeight[input],image height.
	pfps[output],the facial position information.
	nMaxFaceNums[input],max face nums that you want
 Return Values:
	If the function succeeds, the return value is face number.
	If the function fails, the return value is negative.
 Remarks:
	1.image data buffer(pImage)	size must be nWidth*(bpp/8)*nHeight.
	2.pfps must be allocated by caller,the memory size is nMaxFaceNums*sizeof(THFI_FacePos).
	3.if image has face(s),face number less than or equal to nMaxFaceNums
*/


/******Feature API(detect face and feature extract)******/

THFACEIMAGE_API int		THFI_GetFeatureSize();
/*
 The THFI_GetFeatureSize function will return feature size.

 Parameters:
	No parameter.
 Return Values:
	If the function succeeds, the return value is feature size.
	If the function fails, the return value is negative;
 Remarks: 
	No remark.
*/

THFACEIMAGE_API int		THFI_DetectFaceFeature(short nChannelID,BYTE* pImage,int bpp,int nWidth,int nHeight,THFI_FacePos* pfps,BYTE* pFeatures,int nMaxFaceNums);
/*
 The THFI_DetectFaceFeature function execute face detection and feature extraction.

 Parameters:
	nChannelID[input],channel ID(from 0 to nChannelNum-1)
	pImage[input],image data buffer,RGB24 or grayscale format.
	bpp[input],bits per pixel(8-grayscale image, 24-RGB24 image). 
	nWidth[input],image width.
	nHeight[input],image height.
	pfps[output],the face position information.
	pFeatures[output],the face feature buffer.
	nMaxFaceNums[input],max face nums that wanted.
 Return Values:
	If the function succeeds, the return value is face number.
	If the function fails, the return value is negative.
 Remarks:
	1.image data buffer(pImage)	size must be nWidth*(bpp/8)*nHeight.
	2.pfps must be allocated by caller,the memory size is nMaxFaceNums*sizeof(THFI_FacePos).
	3.pFeatures must be allocated by caller,the memory size is nMaxFaceNums*THFI_GetFeatureSize().
	4.if image has face(s),face number less than or equal to nMaxFaceNums.
*/

THFACEIMAGE_API int		THFI_DetectFaceData(short nChannelID,BYTE* pImage,int bpp,int nWidth,int nHeight,THFI_FacePos* pfps,THFI_FaceData* pfds,int nMaxFaceNums);
/*
 The THFI_DetectFaceData function execute face detection and return face data.

 Parameters:
	nChannelID[input],channel ID(from 0 to nChannelNum-1)
	pImage[input],image data buffer,RGB24 or grayscale format.
	bpp[input],bits per pixel(8-grayscale image, 24-RGB24 image). 
	nWidth[input],image width.
	nHeight[input],image height.
	pfps[output],the face position information.
	pfds[output],the face data buffer.
	nMaxFaceNums[input],max face nums that wanted.
 Return Values:
	If the function succeeds, the return value is face number.
	If the function fails, the return value is negative.
 Remarks:
	1.image data buffer(pImage)	size must be nWidth*(bpp/8)*nHeight.
	2.pfps must be allocated by caller,the memory size is nMaxFaceNums*sizeof(THFI_FacePos).
	3.pfds must be allocated int heap memory by caller,the memory size is nMaxFaceNums*sizeof(THFI_FaceData).
	4.if image has face(s),face number less than or equal to nMaxFaceNums.
*/

THFACEIMAGE_API int		THFI_ExtractFeatureByFaceData(short nChannelID,THFI_FaceData* pfd,BYTE* pFeature);
/*
 The THFI_ExtractFeatureByFaceData function execute feature extraction by face data.

 Parameters:
	nChannelID[input],channel ID(from 0 to nChannelNum-1)
	pfd[input],one face data information,returned by THFI_DetectFaceData.
	pFeatures[output],one face feature buffer.
 Return Values:
	If the function succeeds, the return value is 1.
	If the function fails, the return value is negative.
 Remarks:
	1.pFeature must be allocated by caller,the memory size is THFI_GetFeatureSize().
*/

THFACEIMAGE_API int		THFI_ExtractFeatureByFacePos(short nChannelID,BYTE* pImage,int bpp,int nWidth,int nHeight,THFI_FacePos* pfp,BYTE* pFeature);
/*
 The THFI_ExtractFeatureByFacePos function execute feature extraction by face position.

 Parameters:
	nChannelID[input],channel ID(from 0 to nChannelNum-1)
	pImage[input],image data buffer,RGB24 or grayscale format.
	bpp[input],bits per pixel(8-grayscale image, 24-RGB24 image). 
	nWidth[input],image width.
	nHeight[input],image height.
	pfp[input],one face position information.
	pFeatures[output],one face feature buffer.
 Return Values:
	If the function succeeds, the return value is 1.
	If the function fails, the return value is negative.
 Remarks:
	1.image data buffer(pImage)	size must be nWidth*(bpp/8)*nHeight.
	2.pFeature must be allocated by caller,the memory size is THFI_GetFeatureSize().
*/

THFACEIMAGE_API float	THFI_CompareFeature(short nChannelID,BYTE* pFeature1,BYTE* pFeature2);
/*
 The THFI_CompareFeature function execute two face features comparison.

 Parameters:
	nChannelID[input],channel ID(from 0 to nChannelNum-1)
	pFeature1[input],point to one face feature buffer.
	pFeature2[input],point to another face feature buffer.
 Return Values:
	If the function succeeds, the return value is the two face features's similarity(0 to 1).
	If the function fails, the return value is negative.
 Remarks:
	No remark.
 */


/******Release API******/

THFACEIMAGE_API void	THFI_Release();
/*
 The THFI_Release function will release the algorithm engine module

 Parameters:
	No parameter.
 Return Values:
	No return value.
 Remarks:
	This function only can be called one time at program exit.
*/

#endif