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
typedef void * THFACIALT_HANDLE;				// 句柄

#define MAX_FACE_NUM					50		// 最大人脸个数
#define MAX_TRACK_POINT_NUM				256		// 最大跟踪轨迹点

#define THFACIALT_DETECT_FACE			0x01	// 检测人脸
#define THFACIALT_DETECT_EYE			0x02	// 检测眼睛
#define THFACIALT_DETECT_MOUTH			0x04	// 检测嘴巴
#define THFACIALT_DETECT_EYE_STATUS		0x08	// 检测眼睛状态

// 人脸检测
const int THFACIALT_DETECT_FACE_ONLY = (THFACIALT_DETECT_FACE);

// 疲劳检测
const int THFACIALT_DETECT_FATIGUE = (THFACIALT_DETECT_FACE|THFACIALT_DETECT_EYE|THFACIALT_DETECT_EYE_STATUS);

// 遮挡检测
const int THFACIALT_DETECT_SHELTER = (THFACIALT_DETECT_FACE|THFACIALT_DETECT_MOUTH);

// 全部检测
const int THFACIALT_DETECT_ALL = (THFACIALT_DETECT_FACE|THFACIALT_DETECT_EYE|THFACIALT_DETECT_MOUTH|THFACIALT_DETECT_EYE_STATUS);



// 检测参数
typedef struct CTHFacialT_DetectParam
{
	CImgRGB imgSrc;					//　待检测图像
	int nMinFaceWidth;				//  限定检测的最小人脸宽度，单位是像素，如果输入0则不限制
	int nMaxFaceWidth;				//  限定检测的最大人脸宽度，单位是像素，如果输入0则不限制
}CTHFacialT_DetectParam;

// 检测参数
// 在CTHFacialT_DetectParam的基础上增加了3个参数，用于支持更灵活的应用
typedef struct CTHFacialT_DetectParam2
{
	CImgRGB imgSrc;					//　待检测图像
//	float fScaleRatioForDetect;		//　原始图像缩放比例，缩放后图像供检测用，目前仅支持2（放大1倍）、4（放大2倍）、0.5（缩小1倍）、0.25（缩小2倍）等比例
//	float fScaleRatioForTrack;		//　原始图像缩放比例，缩放后图像供跟踪用，目前仅支持2（放大1倍）、4（放大2倍）、0.5（缩小1倍）、0.25（缩小2倍）等比例
	float fScaleRatioForCutting;	//　人脸大小缩放比例，缩放后人脸ＲＥＣＴ用于裁剪
									// fScaleRatioForCutting 为扩大半径比例，即单边扩大比例
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
	int nMinFaceWidth;				//  限定检测的最小人脸宽度，单位是像素，如果输入0则不限制
	int nMaxFaceWidth;				//  限定检测的最大人脸宽度，单位是像素，如果输入0则不限制
}CTHFacialT_DetectParam2;

// 基本人脸信息
typedef struct CTHFacialT_FaceInfo
{
    RECT  rtFace;		// 人脸区域
    POINT ptLeftEye;	// 左眼位置
	POINT ptRightEye;	// 右眼位置
	POINT ptMouth;		// 嘴巴位置
	BOOL  bEyeOpen;		// 眼睛闭合状态
}CTHFacialT_FaceInfo;

/////////////////////////////////////////////////////////////////////////////////////////////////////
//                        初始化/反初始化上下文
/////////////////////////////////////////////////////////////////////////////////////////////////////
// 全局仅需要调用一次，即在系统初始化是调用一次THFacialT_InitContext，在系统退出时调用一次THFacialT_UninitContext
THFACIALEX_API BOOL THFacialT_InitContext();
THFACIALEX_API void THFacialT_UninitContext();

/////////////////////////////////////////////////////////////////////////////////////////////////////
//                        打开/关闭通道接口
/////////////////////////////////////////////////////////////////////////////////////////////////////
// 每个通道调用一次，即创建一个通道时调用一次THFacialT_OpenChannel，关闭一个通道时调用一次THFacialT_CloseChannel

///////////////////////////////////////////////////////////
// 函数名称：THFacialT_OpenChannel
// 函数描述：打开一个检测通道
// 输入参数：
//				bTrack			是否打开跟踪通道
//								==TRUE，打开跟踪通道，nDetectFlag 参数无效，调用 THFacialT_TrackFrame 进行帧跟踪
//								==FALSE，打开实时通道，nDetectFlag 参数有效，调用 THFacialT_DetectFrame 进行帧检测
//				nDetectFlag		检测项目标识，由THFACIALT_DETECT_FACE、THFACIALT_DETECT_EYE、THFACIALT_DETECT_MOUTH
//								和THFACIALT_DETECT_EYE_STATUS进行组合；
// 输出参数：
// 返回结果：
//				通道句柄（THFACIALT_HANDLE）
//				!=NULL 打开通道成功
//				==NULL 打开通道失败
// 附加说明：
///////////////////////////////////////////////////////////
THFACIALEX_API THFACIALT_HANDLE THFacialT_OpenChannel(BOOL bTrack IN, UINT nDetectFlag IN);

///////////////////////////////////////////////////////////
// 函数名称：THFacialT_CloseChannel
// 函数描述：关闭一个检测通道
// 输入参数：
//				hChannel		通道句柄（THFACIALT_HANDLE）
// 输出参数：
// 返回结果：
// 附加说明：
///////////////////////////////////////////////////////////
THFACIALEX_API void THFacialT_CloseChannel(THFACIALT_HANDLE hChannel IN);



/////////////////////////////////////////////////////////////////////////////////////////////////////
//                        实时检测接口
/////////////////////////////////////////////////////////////////////////////////////////////////////

// 基本人脸信息
typedef struct CTHFacialT_BaseFace
{
    RECT  rtFace;		// 人脸区域
    POINT ptLeftEye;	// 左眼位置
	POINT ptRightEye;	// 右眼位置
	POINT ptMouth;		// 嘴巴位置
	BOOL  bEyeOpen;		// 眼睛闭合状态
}CTHFacialT_BaseFace;

// 检测结果
typedef struct CTHFacialT_DetectResult
{
	int nFaceNum;									// 人脸对象个数
	CTHFacialT_BaseFace arrFaceInfo[MAX_FACE_NUM];	// 跟踪轨迹点数组
}CTHFacialT_DetectResult;

///////////////////////////////////////////////////////////
// 函数名称：THFacialT_DetectFrame
// 函数描述：人脸实时检测
// 输入参数：
//				hChannel		通道句柄（THFACIALT_HANDLE）
//				detectParam		检测参数
//				pFaces			输入一组人脸坐标
//				nFaceNum		输入人脸坐标个数
// 输出参数：
//				pResult			检测结果，CTHFacialT_DetectResult 实例地址
// 返回结果：
//				检测是否成功：==TRUE 成功；==FALSE 失败；
// 附加说明：
//		如果输入一组有效人脸坐标，则 THFacialT_DetectFrame2 接口不检测人脸，直接使用这组人脸坐标作为人脸检测结果，然后继续检测其它的人脸特征；
//		如果输入一组无效人脸坐标，则 THFacialT_DetectFrame2 接口先检测人脸坐标，然后再继续检测其它的人脸特征；
///////////////////////////////////////////////////////////
THFACIALEX_API BOOL THFacialT_DetectFrame2(THFACIALT_HANDLE hChannel IN, 
										  CTHFacialT_DetectParam detectParam IN, 
										  const RECT* pFaces IN, int nFaceNum IN,
										  CTHFacialT_DetectResult *pResult OUT);


//// 旧接口，兼容旧版本
INLINE BOOL THFacialT_DetectFrame(THFACIALT_HANDLE hChannel IN, 
										  CTHFacialT_DetectParam detectParam IN, 
										  CTHFacialT_DetectResult *pResult OUT)
{
	return THFacialT_DetectFrame2(hChannel, detectParam, NULL, 0, pResult);
}

/////////////////////////////////////////////////////////////////////////////////////////////////////
//                        跟踪检测接口
/////////////////////////////////////////////////////////////////////////////////////////////////////

// 人脸跟踪信息
typedef struct CTHFacialT_TrackFace
{
	int id;						// 对象ID
	RECT rect;					// 人脸区域
	BOOL bAlarm;				// bAlarm==TRUE，	发生警报（即人脸对象失踪），此时获取并保存最佳人脸照片
								// bAlarm==FALSE，	没有警报（即人脸对象跟踪），持续跟踪中...

	int   nTrackPointNum;						// 跟踪轨迹点个数
	POINT arrTrackPoint[MAX_TRACK_POINT_NUM];	// 跟踪轨迹点数组

}CTHFacialT_TrackFace;

// 跟踪结果
typedef struct CTHFacialT_TrackResult
{
	int nFaceNum;									// 人脸对象个数
	CTHFacialT_TrackFace arrFaceInfo[MAX_FACE_NUM];	// 人脸对象数组
}CTHFacialT_TrackResult;

///////////////////////////////////////////////////////////
// 函数名称：THFacialT_TrackFrame
// 函数描述：人脸跟踪检测
// 输入参数：
//				hChannel		通道句柄（THFACIALT_HANDLE）
//				detectParam		检测参数
//				pFaces			输入一组人脸坐标
//				nFaceNum		输入人脸坐标个数
// 输出参数：
//				pResult			检测结果，CTHFacialT_TrackResult 实例地址
// 返回结果：
//				检测是否成功：==TRUE 成功；==FALSE 失败；
// 附加说明：
//		如果输入一组有效人脸坐标，则 THFacialT_TrackFrame3 接口不检测人脸，直接使用这组人脸坐标作为人脸检测结果，然后继续检测其它的人脸特征；
//		如果输入一组无效人脸坐标，则 THFacialT_TrackFrame3 接口先检测人脸坐标，然后再继续检测其它的人脸特征；
///////////////////////////////////////////////////////////

THFACIALEX_API BOOL THFacialT_TrackFrame3(THFACIALT_HANDLE hChannel IN, 
										CTHFacialT_DetectParam2 detectParam IN, 
										const RECT* pFaces IN, int nFaceNum IN,
										CTHFacialT_TrackResult *pResult OUT);

//// 旧接口，兼容旧版本
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

//// 旧接口，兼容旧版本
INLINE BOOL THFacialT_TrackFrame2(THFACIALT_HANDLE hChannel IN, 
						  CTHFacialT_DetectParam2 detectParam IN, 
						  CTHFacialT_TrackResult *pResult OUT)
{
	return THFacialT_TrackFrame3(hChannel, detectParam, NULL, 0, pResult);
}

///////////////////////////////////////////////////////////
// 函数名称：THFacialT_GetNewlyPhoto
// 函数描述：获取人脸对象最新照片
// 输入参数：
//				hChannel		通道句柄（THFACIALT_HANDLE）
//				nObjID			对象ID
// 输出参数：
//				pImageData		输出照片数据，为 RGB 24 格式，空间由调用者分配和释放，调用前必须分配足够大小的空间
//				nImageWidth		输出照片宽度
//				nImageHeight	输出照片高度
// 返回结果：
//				是否成功：==TRUE 成功；==FALSE 失败；
// 附加说明：
///////////////////////////////////////////////////////////
THFACIALEX_API BOOL THFacialT_GetNewlyPhoto(THFACIALT_HANDLE hChannel IN, 
									   int nObjID IN,
									   BYTE *pImageData OUT,
									   int *nImageWidth OUT,
									   int *nImageHeight OUT);

///////////////////////////////////////////////////////////
// 函数名称：THFacialT_GetBestPhotoCount
// 函数描述：获取人脸对象最佳照片个数
// 输入参数：
//				hChannel		通道句柄（THFACIALT_HANDLE）
//				nObjID			对象ID
// 输出参数：
// 返回结果：
//				<0 获取失败；>=0 最佳照片个数；
// 附加说明：
///////////////////////////////////////////////////////////
THFACIALEX_API long THFacialT_GetBestPhotoCount(THFACIALT_HANDLE hChannel IN, int nObjID IN);

///////////////////////////////////////////////////////////
// 函数名称：THFacialT_GetBestPhoto
// 函数描述：获取人脸对象最佳照片
// 输入参数：
//				hChannel		通道句柄（THFACIALT_HANDLE）
//				nObjID			对象ID
//				nBestPhotoIndex 指定要提取的最佳照片序号，序号从1开始，即第一张最集照片是1，第二张最佳照片是2，一次类推
// 输出参数：
//				pFaceInfo		输出照片的人脸信息
//				pImageData		输出照片数据，为 RGB 24 格式，空间由调用者分配和释放，调用前必须分配足够大小的空间
//				nImageWidth		输出照片宽度
//				nImageHeight	输出照片高度
// 返回结果：
//				是否成功：==TRUE 成功；==FALSE 失败；
// 附加说明：
//				THFacialT_GetBestPhoto 　返回第一个最佳照片
//				THFacialT_GetBestPhoto2　返回指定的最佳照片
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
// 函数名称：THFacialT_GetBestPhotoBackground
// 函数描述：获取人脸对象最佳照片背景图像
// 输入参数：
//				hChannel		通道句柄（THFACIALT_HANDLE）
//				nObjID			对象ID
//				nBestPhotoIndex 指定要提取的最佳照片序号，序号从1开始，即第一张最集照片是1，第二张最佳照片是2，一次类推
// 输出参数：
//				pFaceInfo		输出照片的人脸信息
//				pImageData		输出照片数据，为 RGB 24 格式，空间由调用者分配和释放，调用前必须分配足够大小的空间
//				nImageWidth		输出照片宽度
//				nImageHeight	输出照片高度
// 返回结果：
//				是否成功：==TRUE 成功；==FALSE 失败；
// 附加说明：
//				THFacialT_GetBestPhotoBackground 　返回第一个最佳照片母片（全景图）
//				THFacialT_GetBestPhotoBackground2　返回指定的最佳照片母片（全景图）
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
	
	<人脸检测示例>

	// 全局调用一次初始化上下文
	THFacialT_InitContext();

	//// 打开通道
	THFACIALT_HANDLE hChannel = THFacialT_OpenChannel(FALSE, THFACIALT_DETECT_FATIGUE);
	if (hChannel!=NULL)
	{
		//// 循环捕捉和检测
		while (1)
		{
			//// 捕捉视频帧
			...

			//// 检测人脸
			CTHFacialT_DetectParam detectParam;
			CTHFacialT_DetectResult detectResult;
			if (THFacialT_DetectFrame(hChannel, detectParam, &detectResult))
			{
				int i;
				for (i=0; i<detectResult.nFaceNum; i++)
				{
					CTHFacialT_BaseFace faceInfo = detectResult.arrFaceInfo[i];

					//// 判断人脸有效性（人脸坐标和人眼坐标有效）
					if (faceInfo.rtFace.left>0 && faceInfo.ptLeftEye.x>0 && faceInfo.ptRightEye.x>0)
					{
						// 有效人脸
					}
				}
			}
		}
	
		//// 关闭通道
		THFacialT_CloseChannel(hChannel);
	}

	// 全局调用一次反初始化上下文
	THFacialT_UninitContext();

 ****************************************************************************
*/


/****************************************************************************
	
	<人脸跟踪实例>

	// 全局调用一次初始化上下文
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
				// 对象已经失踪（死亡）
				// 提取最佳照片（目前跟踪到的最好的人脸）
				THFacialT_GetBestPhoto(...);
			}
			else
			{
				// 对象跟踪中...
				// 提取实时照片（当前帧人脸）
				THFacialT_GetNewlyPhoto(...);
			}
		}
	}

	// 全局调用一次反初始化上下文
	THFacialT_UninitContext();

 ****************************************************************************
*/

#ifdef __cplusplus
}
#endif  

#endif // !defined(AFX_THFACIALT_H__2B6D4EA3_866A_436B_AEFF_BE62EB0A0E41__INCLUDED_)
