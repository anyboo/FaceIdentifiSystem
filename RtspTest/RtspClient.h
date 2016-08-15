#pragma once



#ifdef __cplusplus
extern "C"{
#endif

#include "libavformat/avformat.h"
#include "libavcodec/avcodec.h"
#include "libswscale/swscale.h"
#include "libavutil/avutil.h"

#ifdef __cplusplus
}

#endif 

#pragma comment(lib, "avformat.lib")
#pragma comment(lib, "avcodec.lib")
#pragma comment(lib, "swscale.lib")
#pragma comment(lib, "avutil.lib")


#include "opencv2/core/core.hpp"  
#include "opencv2/highgui/highgui.hpp" 

#ifdef _DEBUG
#pragma comment(lib, "opencv_core2410d.lib")
#pragma comment(lib, "opencv_highgui2410d.lib")
#else
#pragma comment(lib, "opencv_core2410.lib")
#pragma comment(lib, "opencv_highgui2410.lib")
#endif




typedef void(__stdcall *DownloadCallback)(const char* LPBYTE, UINT nSize, int nWidth, int nHeight, UINT nUser);

class RtspClient
{

public:	
	
	RtspClient();	
	~RtspClient();

	bool start(const char* pURL, DownloadCallback callback, PVOID pDlg);

	bool stop();

	static void init();

	static void uninit();

	static void DrawBmpBuf(cv::Mat srcMat, HWND hShowWnd);

	static BOOL MirrorDIB(LPSTR lpDIBBits, LONG lWidth, LONG lHeight, BOOL bDirection, int nImageBits);
protected:
	
	
};

