#pragma once

#include "Poco/ThreadPool.h"
#include "Poco/Runnable.h"
#include "RtspClient.h"


class RtspThread : public Poco::Runnable
{
public:	
	RtspThread();
	RtspThread(ThreadCallback call,
		AVFormatContext *pFormatCtx,
		int videoStream,
		AVPacket *packet,
		AVCodecContext *pCodecCtx,
		AVFrame *pFrame,
		AVFrame *pFrameRGB,
		BYTE *buffer,
		PVOID pDlg);
	~RtspThread();
	void run();
	void onEvent(const void* pSender, bool& arg);
private:		
	ThreadCallback			 _call;
	AVFormatContext *		 _FormatCtx;
	int						 _videoStream;
	AVPacket *				 _packet;
	AVCodecContext *		 _CodecCtx;
	AVFrame *                _Frame;
	AVFrame *				 _FrameRGB;
	BYTE *                   _buffer;
	PVOID                    _dlg;
	bool                     _break;
};

