#ifndef FACE_ATTEND_SERVER_PUBLIC_H
#define FACE_ATTEND_SERVER_PUBLIC_H
#include <math.h>

#include "ijl15\\IjlLib.h"
#include "ThFacerCap.h"

#include "ClrButton.h"

#include <cv.h>
#include <cxcore.h>
#include <highgui.h>
using namespace cv;

#pragma warning(disable:4786 4305 4101)
#include <fstream>
#include <iterator>
#include <algorithm>
#include <string>
#include <vector>
#include "dib.h"
using namespace std;

#include "../../include-mt/THFaceImage_i.h"
#pragma comment (lib, "../../lib-mt/THFaceImage.lib")

#include "../../include-mt/THFeature_i.h"
#pragma comment(lib,"../../lib-mt/THFeature.lib")

#define WM_MY_TRAY_NOTIFICATION WM_USER+105
#define WM_PROCESS_LOG_PACKET WM_USER+110

#define APP_BACK_COLOR RGB(228,228,228)
#define EDIT_CTRL_COLOR RGB(36,100,144)
#define BUTTON_COLOR RGB(255,0,0)
///
#define BTN_INACTIVE_BG_COLOR RGB(215,215,215)//(228,228,215)
#define BTN_INACTIVE_FG_COLOR RGB(0,0,0)
#define BTN_ACTIVE_BG_COLOR RGB(237,233,207)
#define BTN_ACTIVE_FG_COLOR RGB(0,0,0)

#define Faceprint_Feature_Num  13
#define ShowImageWidth  56
#define ShowImageHeight 56
#define FaceImageWidth  160
#define FaceImageHeight 160

#define FRAME_WIDTH 640
#define FRAME_HEIGHT 480

#define WIDTHBYTES(bits)    (((bits) + 31) / 32 * 4)



#define GetRandom(min,max) rand()*((float)((max)-(min)))/(float)(RAND_MAX)+(min)

struct PersonInfo
{
	long nPersonBbID;
	
	char sStuffID[32];
	int nGender;

	char sStuffName[32];

	long nFeatureNum;
	BYTE* pFeatureBuf;

	PersonInfo()
	{
		pFeatureBuf=NULL;
	};
	~PersonInfo()
	{
		delete []pFeatureBuf;
		pFeatureBuf=NULL;
	};

};

struct IndentifyInfo
{
	PersonInfo* ppi;
	int nIndex;
	float fScore;
	bool bIndetify;
};

struct SysConfig
{
	UINT nFalseAccept;
	UINT nVerifyFalseAccept;
	UINT nServerPort;
	UINT nUpdateFeatures;
	UINT nSimilarity;
};

struct EnrollBmpBufInfo
{
	BYTE* m_pBmpBuf;
	long m_nWidth;
	long m_nHeight;
	EnrollBmpBufInfo(long nWidth,long nHeight)
	{
		m_nWidth=nWidth;
		m_nHeight=nHeight;
		m_pBmpBuf=new BYTE[m_nWidth*m_nHeight*3];
	};
	~EnrollBmpBufInfo()
	{
		delete []m_pBmpBuf;
		m_pBmpBuf=NULL;
	};
};
struct GrayBufInfo
{
	BYTE* pGrayBuf;
	long m_nWidth;
	long m_nHeight;
	GrayBufInfo(long nWidth,long nHeight)
	{
		m_nWidth=nWidth;
		m_nHeight=nHeight;
		pGrayBuf=new BYTE[m_nWidth*m_nHeight];
	}
	~GrayBufInfo()
	{
		delete []pGrayBuf;
		pGrayBuf=NULL;
		m_nWidth=0;
		m_nHeight=0;
	}
};

struct FaceFrameInfo
{
	BYTE* pRgb24Buf;
	long m_nWidth;
	long m_nHeight;
	THFI_FacePos* pFaceData;

	FaceFrameInfo(long nWidth,long nHeight,THFI_FacePos* ptfp)
	{
		m_nWidth=nWidth;
		m_nHeight=nHeight;
		pRgb24Buf=new BYTE[m_nWidth*m_nHeight*3];
		pFaceData=new THFI_FacePos;
		memcpy(pFaceData,ptfp,sizeof(THFI_FacePos));
	}
	~FaceFrameInfo()
	{
		delete []pRgb24Buf;
		pRgb24Buf=NULL;
		m_nWidth=0;
		m_nHeight=0;
		delete [](BYTE*)pFaceData->dwReserved;
		delete pFaceData;pFaceData=NULL;
	}
};

struct Log_Info
{
	char sName[32];
	char sSex[32];
	char sNation[32];
	char sBirthday[32];
	char sAddress[256];
	char sId[32];
	char sStartDate[32];
	char sEndDate[32];
	char sDepartment[128];
	double fScore;
	SYSTEMTIME logTime;
	short nType;
};

extern CDWordArray g_SelGrayBufInfoArray;
extern EnrollBmpBufInfo* g_pIrEnrollBmpBufInfo;
extern EnrollBmpBufInfo* g_pPopEnrollBmpBufInfo;
extern BYTE* g_pEnrollFeatureBuf;

extern SysConfig g_SysConfig;
extern long g_nFeatureSize;
extern long g_nFeatureNum;
extern CPtrList g_PersonInfoList;
extern vector<BOOL> g_PersonTemplateEnable;

extern CString g_strAppPath;
extern CBrush g_BackBrush;

extern BOOL g_bDevelopMode;
extern HWND g_hMainWnd;
extern int g_nVideoMode;

extern	MCapture* m_pCap[1];
extern	SCapParam m_capParam[1];

extern HMODULE g_hModule;
extern bool g_bGoodCrop;
extern BOOL g_bHorizontalImage;
//////////////////////////////////////////


extern BYTE* Rotate(BYTE* pRgbBuf,long& nWidth,long& nHeight,float angle);
extern BOOL BGRFromRGB(BYTE *buf, UINT widthPix, UINT height);
extern BOOL MirrorDIB(LPSTR lpDIBBits, LONG lWidth, LONG lHeight, BOOL bDirection,int nImageBits);
extern void DrawBmpBuf(BITMAPINFOHEADER& bih,BYTE* pDataBuf,HWND hShowWnd,BOOL bFitWnd);
extern void DrawBmpBuf1(BITMAPINFOHEADER& bih,BYTE* pDataBuf,HWND hShowWnd,BOOL bFitWnd,const char* pTopText,COLORREF clTopText);
extern void ContructBih(int nWidth,int nHeight,BITMAPINFOHEADER& bih);
extern void DrawFaceRects(BYTE* pRgbBuf,int nBufWidth,int nBufHeight,
				   RECT* pFaceRects,int nRectNum,
				   COLORREF color,int nPenWidth);
extern BOOL GetCBitmap(BITMAPINFOHEADER* pBih, BYTE* pDataBuf,CBitmap &bmp);
extern BOOL ResampleBmp(BITMAPINFOHEADER& bih, BYTE* inBuf,int newx,int newy,BYTE* outBuf);
extern void GetPartImage( BYTE* image, int imageWidth, int imageHeight, BYTE* part,  int left,int top, int right ,int bottom );
extern BYTE* LoadBmpFromFile(LPCTSTR strFileName,BITMAPINFOHEADER& bih);
extern BOOL SaveBufToBmpFile( BYTE* buf, LPBITMAPINFOHEADER lpBih, LPCSTR strFileName );
extern HBITMAP DrawCrossEyes(BYTE* image, POINT LEye,POINT REye,int nWidth,int nHeight,COLORREF color,int nPenWidth,int nCrossSize);
extern void DeleteDirFile(CString sPath);
extern BOOL DebugInfo(const char* pDebugInfo);
extern double ScoreMap(double nSourceScore,double nFAR);
extern void DoNetSend(CString strComputer, CString strMessage);
extern int SynCheck(BYTE *Image, int Width, int Height);
extern void SaveBmp8File(CString szFileName, BYTE * srcImage, int Width, int Height);
extern void _DebugInfo(LPCSTR szLogFile,LPCSTR szInfo, ...);
extern LPVOID LoadDataResource(LPCTSTR lpResName,const char *name);
extern void RotateRgb(unsigned char* pRgbBuf,int nWidth,int nHeight,short nRgbChannel,int angle);
extern BOOL WriteLog(Log_Info li,Mat* pMatLog);
extern BOOL WriteIDPhoto(Mat* pMatID,const char* sID);

extern  bool IsExpired();
#endif