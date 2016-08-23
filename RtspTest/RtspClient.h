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

#include "RtspThread.h"
#include "Poco/BasicEvent.h"
#include "Poco/Delegate.h"

using Poco::ThreadPool;
using Poco::Thread;
using Poco::Runnable;


class RtspThread;

class RtspClient
{

public:	
	
	RtspClient();	
	~RtspClient();

	bool start(const char* pURL, ThreadCallback callback, PVOID pDlg);

	bool stop();

	static void init();

	static void uninit();

	static void DrawBmpBuf(cv::Mat srcMat, HWND hShowWnd);

	static BOOL MirrorDIB(LPSTR lpDIBBits, LONG lWidth, LONG lHeight, BOOL bDirection, int nImageBits);
private:
	void fireEvent(bool n)
	{
		_theEvent(this, n);
	}
private:
	RtspThread *		 _thread;
	AVFormatContext *    _FormatCtx;
	AVFrame *			 _Frame;
	AVFrame *			 _FrameRGB;
	AVPacket *           _packet;
	BYTE *				 _buffer;
	Poco::BasicEvent<bool> _theEvent;
};


