// THFacialT.h: interface for the THFacialT class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_THFACIALT_H__2B6D4EA3_866A_436B_AEFF_BE62EB0A0E41__INCLUDED_)
#define AFX_THFACIALT_H__2B6D4EA3_866A_436B_AEFF_BE62EB0A0E41__INCLUDED_

#include "FiImage.h"

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#ifdef __cplusplus
extern "C" {
#endif  

#ifdef THFACIALX_EXPORTS
#define THFACIALEX_API __declspec(dllexport)
#else
#define THFACIALEX_API __declspec(dllimport)
#endif


////
typedef void * THFACIALT_HANDLE;				// ���

#define MAX_FACE_NUM					50		// �����������
#define MAX_TRACK_POINT_NUM				256		// �����ٹ켣��

#define THFACIALT_DETECT_FACE			0x01	// �������
#define THFACIALT_DETECT_EYE			0x02	// ����۾�
#define THFACIALT_DETECT_MOUTH			0x04	// ������
#define THFACIALT_DETECT_EYE_STATUS		0x08	// ����۾�״̬

// �������
const int THFACIALT_DETECT_FACE_ONLY = (THFACIALT_DETECT_FACE);

// ƣ�ͼ��
const int THFACIALT_DETECT_FATIGUE = (THFACIALT_DETECT_FACE|THFACIALT_DETECT_EYE|THFACIALT_DETECT_EYE_STATUS);

// �ڵ����
const int THFACIALT_DETECT_SHELTER = (THFACIALT_DETECT_FACE|THFACIALT_DETECT_MOUTH);

// ȫ�����
const int THFACIALT_DETECT_ALL = (THFACIALT_DETECT_FACE|THFACIALT_DETECT_EYE|THFACIALT_DETECT_MOUTH|THFACIALT_DETECT_EYE_STATUS);



// ������
typedef struct CTHFacialT_DetectParam
{
	CImgRGB imgSrc;					//�������ͼ��
	int nMinFaceWidth;				//  �޶�������С������ȣ���λ�����أ��������0������
	int nMaxFaceWidth;				//  �޶��������������ȣ���λ�����أ��������0������
}CTHFacialT_DetectParam;

// ������
// ��CTHFacialT_DetectParam�Ļ�����������3������������֧�ָ�����Ӧ��
typedef struct CTHFacialT_DetectParam2
{
	CImgRGB imgSrc;					//�������ͼ��
//	float fScaleRatioForDetect;		//��ԭʼͼ�����ű��������ź�ͼ�񹩼���ã�Ŀǰ��֧��2���Ŵ�1������4���Ŵ�2������0.5����С1������0.25����С2�����ȱ���
//	float fScaleRatioForTrack;		//��ԭʼͼ�����ű��������ź�ͼ�񹩸����ã�Ŀǰ��֧��2���Ŵ�1������4���Ŵ�2������0.5����С1������0.25����С2�����ȱ���
	float fScaleRatioForCutting;	//��������С���ű��������ź������ңţã����ڲü�
									// fScaleRatioForCutting Ϊ����뾶�������������������
									// if (fScaleRatioForCutting > 0.0)
									// {
									// 	rtFace.left -= (int) (width * fScaleRatioForCutting * 0.8);
									// 	rtFace.top -= (int) (height * fScaleRatioForCutting * 1.5);
									// 	rtFace.right += (int) (width * fScaleRatioForCutting * 0.8);
									// 	rtFace.bottom += (int) (height * fScaleRatioForCutting * 0.5);
									// }
									// else
									// {
									// 	rtFace.left -= (int) (width * 0.2);
									// 	rtFace.top -= (int) (height * 0.3);
									// 	rtFace.right += (int) (width * 0.2);
									// 	rtFace.bottom += (int) (height * 0.1);
									// }
	int nMinFaceWidth;				//  �޶�������С������ȣ���λ�����أ��������0������
	int nMaxFaceWidth;				//  �޶��������������ȣ���λ�����أ��������0������
}CTHFacialT_DetectParam2;

// ����������Ϣ
typedef struct CTHFacialT_FaceInfo
{
    RECT  rtFace;		// ��������
    POINT ptLeftEye;	// ����λ��
	POINT ptRightEye;	// ����λ��
	POINT ptMouth;		// ���λ��
	BOOL  bEyeOpen;		// �۾��պ�״̬
}CTHFacialT_FaceInfo;

/////////////////////////////////////////////////////////////////////////////////////////////////////
//                        ��ʼ��/����ʼ��������
/////////////////////////////////////////////////////////////////////////////////////////////////////
// ȫ�ֽ���Ҫ����һ�Σ�����ϵͳ��ʼ���ǵ���һ��THFacialT_InitContext����ϵͳ�˳�ʱ����һ��THFacialT_UninitContext
THFACIALEX_API BOOL THFacialT_InitContext();
THFACIALEX_API void THFacialT_UninitContext();

/////////////////////////////////////////////////////////////////////////////////////////////////////
//                        ��/�ر�ͨ���ӿ�
/////////////////////////////////////////////////////////////////////////////////////////////////////
// ÿ��ͨ������һ�Σ�������һ��ͨ��ʱ����һ��THFacialT_OpenChannel���ر�һ��ͨ��ʱ����һ��THFacialT_CloseChannel

///////////////////////////////////////////////////////////
// �������ƣ�THFacialT_OpenChannel
// ������������һ�����ͨ��
// ���������
//				bTrack			�Ƿ�򿪸���ͨ��
//								==TRUE���򿪸���ͨ����nDetectFlag ������Ч������ THFacialT_TrackFrame ����֡����
//								==FALSE����ʵʱͨ����nDetectFlag ������Ч������ THFacialT_DetectFrame ����֡���
//				nDetectFlag		�����Ŀ��ʶ����THFACIALT_DETECT_FACE��THFACIALT_DETECT_EYE��THFACIALT_DETECT_MOUTH
//								��THFACIALT_DETECT_EYE_STATUS������ϣ�
// ���������
// ���ؽ����
//				ͨ�������THFACIALT_HANDLE��
//				!=NULL ��ͨ���ɹ�
//				==NULL ��ͨ��ʧ��
// ����˵����
///////////////////////////////////////////////////////////
THFACIALEX_API THFACIALT_HANDLE THFacialT_OpenChannel(BOOL bTrack IN, UINT nDetectFlag IN);

///////////////////////////////////////////////////////////
// �������ƣ�THFacialT_CloseChannel
// �����������ر�һ�����ͨ��
// ���������
//				hChannel		ͨ�������THFACIALT_HANDLE��
// ���������
// ���ؽ����
// ����˵����
///////////////////////////////////////////////////////////
THFACIALEX_API void THFacialT_CloseChannel(THFACIALT_HANDLE hChannel IN);



/////////////////////////////////////////////////////////////////////////////////////////////////////
//                        ʵʱ���ӿ�
/////////////////////////////////////////////////////////////////////////////////////////////////////

// ����������Ϣ
typedef struct CTHFacialT_BaseFace
{
    RECT  rtFace;		// ��������
    POINT ptLeftEye;	// ����λ��
	POINT ptRightEye;	// ����λ��
	POINT ptMouth;		// ���λ��
	BOOL  bEyeOpen;		// �۾��պ�״̬
}CTHFacialT_BaseFace;

// �����
typedef struct CTHFacialT_DetectResult
{
	int nFaceNum;									// �����������
	CTHFacialT_BaseFace arrFaceInfo[MAX_FACE_NUM];	// ���ٹ켣������
}CTHFacialT_DetectResult;

///////////////////////////////////////////////////////////
// �������ƣ�THFacialT_DetectFrame
// ��������������ʵʱ���
// ���������
//				hChannel		ͨ�������THFACIALT_HANDLE��
//				detectParam		������
//				pFaces			����һ����������
//				nFaceNum		���������������
// ���������
//				pResult			�������CTHFacialT_DetectResult ʵ����ַ
// ���ؽ����
//				����Ƿ�ɹ���==TRUE �ɹ���==FALSE ʧ�ܣ�
// ����˵����
//		�������һ����Ч�������꣬�� THFacialT_DetectFrame2 �ӿڲ����������ֱ��ʹ����������������Ϊ�����������Ȼ������������������������
//		�������һ����Ч�������꣬�� THFacialT_DetectFrame2 �ӿ��ȼ���������꣬Ȼ���ټ����������������������
///////////////////////////////////////////////////////////
THFACIALEX_API BOOL THFacialT_DetectFrame2(THFACIALT_HANDLE hChannel IN, 
										  CTHFacialT_DetectParam detectParam IN, 
										  const RECT* pFaces IN, int nFaceNum IN,
										  CTHFacialT_DetectResult *pResult OUT);


//// �ɽӿڣ����ݾɰ汾
INLINE BOOL THFacialT_DetectFrame(THFACIALT_HANDLE hChannel IN, 
										  CTHFacialT_DetectParam detectParam IN, 
										  CTHFacialT_DetectResult *pResult OUT)
{
	return THFacialT_DetectFrame2(hChannel, detectParam, NULL, 0, pResult);
}

/////////////////////////////////////////////////////////////////////////////////////////////////////
//                        ���ټ��ӿ�
/////////////////////////////////////////////////////////////////////////////////////////////////////

// ����������Ϣ
typedef struct CTHFacialT_TrackFace
{
	int id;						// ����ID
	RECT rect;					// ��������
	BOOL bAlarm;				// bAlarm==TRUE��	��������������������ʧ�٣�����ʱ��ȡ���������������Ƭ
								// bAlarm==FALSE��	û�о�����������������٣�������������...

	int   nTrackPointNum;						// ���ٹ켣�����
	POINT arrTrackPoint[MAX_TRACK_POINT_NUM];	// ���ٹ켣������

}CTHFacialT_TrackFace;

// ���ٽ��
typedef struct CTHFacialT_TrackResult
{
	int nFaceNum;									// �����������
	CTHFacialT_TrackFace arrFaceInfo[MAX_FACE_NUM];	// ������������
}CTHFacialT_TrackResult;

///////////////////////////////////////////////////////////
// �������ƣ�THFacialT_TrackFrame
// �����������������ټ��
// ���������
//				hChannel		ͨ�������THFACIALT_HANDLE��
//				detectParam		������
//				pFaces			����һ����������
//				nFaceNum		���������������
// ���������
//				pResult			�������CTHFacialT_TrackResult ʵ����ַ
// ���ؽ����
//				����Ƿ�ɹ���==TRUE �ɹ���==FALSE ʧ�ܣ�
// ����˵����
//		�������һ����Ч�������꣬�� THFacialT_TrackFrame3 �ӿڲ����������ֱ��ʹ����������������Ϊ�����������Ȼ������������������������
//		�������һ����Ч�������꣬�� THFacialT_TrackFrame3 �ӿ��ȼ���������꣬Ȼ���ټ����������������������
///////////////////////////////////////////////////////////

THFACIALEX_API BOOL THFacialT_TrackFrame3(THFACIALT_HANDLE hChannel IN, 
										CTHFacialT_DetectParam2 detectParam IN, 
										const RECT* pFaces IN, int nFaceNum IN,
										CTHFacialT_TrackResult *pResult OUT);

//// �ɽӿڣ����ݾɰ汾
INLINE BOOL THFacialT_TrackFrame(THFACIALT_HANDLE hChannel IN, 
						  CTHFacialT_DetectParam detectParam IN, 
						  CTHFacialT_TrackResult *pResult OUT)
{
	CTHFacialT_DetectParam2 detectParam2;

	detectParam2.imgSrc.pData = detectParam.imgSrc.pData;
	detectParam2.imgSrc.nWidth = detectParam.imgSrc.nWidth;
	detectParam2.imgSrc.nHeight = detectParam.imgSrc.nHeight;
//	detectParam2.fScaleRatioForDetect = 0.0;
//	detectParam2.fScaleRatioForTrack = 0.0;
	detectParam2.fScaleRatioForCutting = 0.0;

	return THFacialT_TrackFrame3(hChannel, detectParam2, NULL, 0, pResult);
}

//// �ɽӿڣ����ݾɰ汾
INLINE BOOL THFacialT_TrackFrame2(THFACIALT_HANDLE hChannel IN, 
						  CTHFacialT_DetectParam2 detectParam IN, 
						  CTHFacialT_TrackResult *pResult OUT)
{
	return THFacialT_TrackFrame3(hChannel, detectParam, NULL, 0, pResult);
}

///////////////////////////////////////////////////////////
// �������ƣ�THFacialT_GetNewlyPhoto
// ������������ȡ��������������Ƭ
// ���������
//				hChannel		ͨ�������THFACIALT_HANDLE��
//				nObjID			����ID
// ���������
//				pImageData		�����Ƭ���ݣ�Ϊ RGB 24 ��ʽ���ռ��ɵ����߷�����ͷţ�����ǰ��������㹻��С�Ŀռ�
//				nImageWidth		�����Ƭ���
//				nImageHeight	�����Ƭ�߶�
// ���ؽ����
//				�Ƿ�ɹ���==TRUE �ɹ���==FALSE ʧ�ܣ�
// ����˵����
///////////////////////////////////////////////////////////
THFACIALEX_API BOOL THFacialT_GetNewlyPhoto(THFACIALT_HANDLE hChannel IN, 
									   int nObjID IN,
									   BYTE *pImageData OUT,
									   int *nImageWidth OUT,
									   int *nImageHeight OUT);

///////////////////////////////////////////////////////////
// �������ƣ�THFacialT_GetBestPhotoCount
// ������������ȡ�������������Ƭ����
// ���������
//				hChannel		ͨ�������THFACIALT_HANDLE��
//				nObjID			����ID
// ���������
// ���ؽ����
//				<0 ��ȡʧ�ܣ�>=0 �����Ƭ������
// ����˵����
///////////////////////////////////////////////////////////
THFACIALEX_API long THFacialT_GetBestPhotoCount(THFACIALT_HANDLE hChannel IN, int nObjID IN);

///////////////////////////////////////////////////////////
// �������ƣ�THFacialT_GetBestPhoto
// ������������ȡ�������������Ƭ
// ���������
//				hChannel		ͨ�������THFACIALT_HANDLE��
//				nObjID			����ID
//				nBestPhotoIndex ָ��Ҫ��ȡ�������Ƭ��ţ���Ŵ�1��ʼ������һ�����Ƭ��1���ڶ��������Ƭ��2��һ������
// ���������
//				pFaceInfo		�����Ƭ��������Ϣ
//				pImageData		�����Ƭ���ݣ�Ϊ RGB 24 ��ʽ���ռ��ɵ����߷�����ͷţ�����ǰ��������㹻��С�Ŀռ�
//				nImageWidth		�����Ƭ���
//				nImageHeight	�����Ƭ�߶�
// ���ؽ����
//				�Ƿ�ɹ���==TRUE �ɹ���==FALSE ʧ�ܣ�
// ����˵����
//				THFacialT_GetBestPhoto �����ص�һ�������Ƭ
//				THFacialT_GetBestPhoto2������ָ���������Ƭ
///////////////////////////////////////////////////////////
THFACIALEX_API BOOL THFacialT_GetBestPhoto(THFACIALT_HANDLE hChannel IN, 
									   int nObjID IN,
									   BYTE *pImageData OUT,
									   int *nImageWidth OUT,
									   int *nImageHeight OUT);

THFACIALEX_API BOOL THFacialT_GetBestPhoto2(THFACIALT_HANDLE hChannel IN, 
									   int nObjID IN,
									   int nBestPhotoIndex IN,
									   CTHFacialT_FaceInfo* pFaceInfo,
									   BYTE *pImageData OUT,
									   int *nImageWidth OUT,
									   int *nImageHeight OUT);

///////////////////////////////////////////////////////////
// �������ƣ�THFacialT_GetBestPhotoBackground
// ������������ȡ�������������Ƭ����ͼ��
// ���������
//				hChannel		ͨ�������THFACIALT_HANDLE��
//				nObjID			����ID
//				nBestPhotoIndex ָ��Ҫ��ȡ�������Ƭ��ţ���Ŵ�1��ʼ������һ�����Ƭ��1���ڶ��������Ƭ��2��һ������
// ���������
//				pFaceInfo		�����Ƭ��������Ϣ
//				pImageData		�����Ƭ���ݣ�Ϊ RGB 24 ��ʽ���ռ��ɵ����߷�����ͷţ�����ǰ��������㹻��С�Ŀռ�
//				nImageWidth		�����Ƭ���
//				nImageHeight	�����Ƭ�߶�
// ���ؽ����
//				�Ƿ�ɹ���==TRUE �ɹ���==FALSE ʧ�ܣ�
// ����˵����
//				THFacialT_GetBestPhotoBackground �����ص�һ�������ƬĸƬ��ȫ��ͼ��
//				THFacialT_GetBestPhotoBackground2������ָ���������ƬĸƬ��ȫ��ͼ��
///////////////////////////////////////////////////////////
THFACIALEX_API BOOL THFacialT_GetBestPhotoBackground(THFACIALT_HANDLE hChannel IN, 
									   int nObjID IN,
									   BYTE *pImageData OUT,
									   int *nImageWidth OUT,
									   int *nImageHeight OUT);

THFACIALEX_API BOOL THFacialT_GetBestPhotoBackground2(THFACIALT_HANDLE hChannel IN, 
									   int nObjID IN,
									   int nBestPhotoIndex IN,
									   CTHFacialT_FaceInfo* pFaceInfo,
									   BYTE *pImageData OUT,
									   int *nImageWidth OUT,
									   int *nImageHeight OUT);

/****************************************************************************
	
	<�������ʾ��>

	// ȫ�ֵ���һ�γ�ʼ��������
	THFacialT_InitContext();

	//// ��ͨ��
	THFACIALT_HANDLE hChannel = THFacialT_OpenChannel(FALSE, THFACIALT_DETECT_FATIGUE);
	if (hChannel!=NULL)
	{
		//// ѭ����׽�ͼ��
		while (1)
		{
			//// ��׽��Ƶ֡
			...

			//// �������
			CTHFacialT_DetectParam detectParam;
			CTHFacialT_DetectResult detectResult;
			if (THFacialT_DetectFrame(hChannel, detectParam, &detectResult))
			{
				int i;
				for (i=0; i<detectResult.nFaceNum; i++)
				{
					CTHFacialT_BaseFace faceInfo = detectResult.arrFaceInfo[i];

					//// �ж�������Ч�ԣ��������������������Ч��
					if (faceInfo.rtFace.left>0 && faceInfo.ptLeftEye.x>0 && faceInfo.ptRightEye.x>0)
					{
						// ��Ч����
					}
				}
			}
		}
	
		//// �ر�ͨ��
		THFacialT_CloseChannel(hChannel);
	}

	// ȫ�ֵ���һ�η���ʼ��������
	THFacialT_UninitContext();

 ****************************************************************************
*/


/****************************************************************************
	
	<��������ʵ��>

	// ȫ�ֵ���һ�γ�ʼ��������
	THFacialT_InitContext();

	CTHFacialT_TrackResult detectResult;
	BOOL ret = THFacialT_TrackFrame(m_hDetector, detectParam, &detectResult);
	if (ret && detectResult.nFaceNum>0)
	{			
		for (int face_index=0; face_index<detectResult.nFaceNum; face_index++)
		{
			CTHFacialT_TrackFace faceInfo = detectResult.arrFaceInfo[face_index];
			if (faceInfo.bAlarm)
			{
				// �����Ѿ�ʧ�٣�������
				// ��ȡ�����Ƭ��Ŀǰ���ٵ�����õ�������
				THFacialT_GetBestPhoto(...);
			}
			else
			{
				// ���������...
				// ��ȡʵʱ��Ƭ����ǰ֡������
				THFacialT_GetNewlyPhoto(...);
			}
		}
	}

	// ȫ�ֵ���һ�η���ʼ��������
	THFacialT_UninitContext();

 ****************************************************************************
*/

#ifdef __cplusplus
}
#endif  

#endif // !defined(AFX_THFACIALT_H__2B6D4EA3_866A_436B_AEFF_BE62EB0A0E41__INCLUDED_)
