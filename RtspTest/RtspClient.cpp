#include "stdafx.h"
#include "RtspClient.h"
#include <iostream>


RtspClient::RtspClient()
{
	_thread = NULL;
	_FormatCtx = NULL;
	_Frame = NULL;
	_FrameRGB = NULL;
	_packet = NULL;
	_buffer = NULL;
}


RtspClient::~RtspClient()
{
	if (_thread)
	{
		delete _thread;
		_thread = NULL;
	}
}



bool RtspClient::start(const char* pURL, ThreadCallback callback, PVOID pDlg)
{
	bool bRet = true;
	
	//init AVFormatContext, new memory
	AVFormatContext * _FormatCtx = avformat_alloc_context();
	AVDictionary* optionsDict = NULL;	
	av_dict_set(&optionsDict, "stimeout", "2000000", 0);
	if (avformat_open_input(&_FormatCtx, pURL, NULL, &optionsDict) != 0)
	{
		TRACE("open  input  file  faile !");
		return false;
	}

	//find stream info
	if (avformat_find_stream_info(_FormatCtx, NULL) < 0)
	{
		TRACE("Couldn't find stream information.\n");
		return false;
	}

	int videoStream;
	//get video stream ID
	for (int i = 0; i < _FormatCtx->nb_streams; i++)
	{
		if (_FormatCtx->streams[i]->codec->codec_type == AVMEDIA_TYPE_VIDEO)
		{
			videoStream = i;
			break;
		}
	}
	if (videoStream == -1)
	{
		TRACE("could  not find video stream !");
		return false;
	}

	AVCodecContext *pCodecCtx = _FormatCtx->streams[videoStream]->codec;
	//find decoder
	AVCodec *pCodec = avcodec_find_decoder(pCodecCtx->codec_id);
	if (pCodec == NULL)
	{
		TRACE("could not find decoder");
		return false;
	}

	//open decoder
	if (avcodec_open2(pCodecCtx, pCodec, NULL) < 0)
	{
		TRACE("could not open decoder");
		return false;
	}

	av_dump_format(_FormatCtx, 0, pURL, 0);
	//save AVFrame	
	_Frame = av_frame_alloc();
	_FrameRGB = av_frame_alloc();
	_packet = (AVPacket *)av_malloc(sizeof(AVPacket));
	if (_FrameRGB == NULL)
	{
		TRACE("open mem faile");
		return false;
	}

	int numBytes = avpicture_get_size(AV_PIX_FMT_RGB24, pCodecCtx->width, pCodecCtx->height);
	_buffer = (BYTE *)av_malloc(numBytes*sizeof(BYTE));	
	avpicture_fill((AVPicture *)_FrameRGB, _buffer, AV_PIX_FMT_RGB24, pCodecCtx->width, pCodecCtx->height);
	
	_thread = new RtspThread(callback, _FormatCtx, videoStream, _packet, pCodecCtx, _Frame, _FrameRGB, _buffer, pDlg);
	//_thread = pthread;
	Poco::ThreadPool::defaultPool().start(*_thread);

	return bRet;
}

bool RtspClient::stop()
{
	bool bRet = true;	
	
	_theEvent += Poco::delegate(_thread, &RtspThread::onEvent);
	fireEvent(true);
	_theEvent -= Poco::delegate(_thread, &RtspThread::onEvent);
	Poco::ThreadPool::defaultPool().joinAll();

	if (_thread)
	{
		delete _thread;
		_thread = NULL;
	}
	avformat_free_context(_FormatCtx); 
	av_frame_free(&_Frame);
	av_frame_free(&_FrameRGB);
	av_free(_buffer);
	av_free(_packet);
	return bRet;
}

void RtspClient::init()
{
	//Initialize libavformat and register all protocols
	av_register_all();
	//initialization of network components
	avformat_network_init();
}

void RtspClient::uninit()
{
	//Undo the initialization done by avformat_network_init
	avformat_network_deinit();
}

void RtspClient::DrawBmpBuf(cv::Mat srcMat, HWND hShowWnd)
{
	RECT rc;
	::GetWindowRect(hShowWnd, &rc);
	int nWidth = rc.right - rc.left - 2;
	int nHeight = rc.bottom - rc.top - 2;

	BITMAPINFOHEADER bih;

	bih.biSize = 40;
	bih.biWidth = srcMat.rows;
	bih.biHeight = srcMat.cols;
	bih.biPlanes = 1;
	bih.biBitCount = 24;
	bih.biCompression = BI_RGB;
	bih.biSizeImage = bih.biWidth * bih.biHeight * 3;
	bih.biXPelsPerMeter = 0;
	bih.biYPelsPerMeter = 0;
	bih.biClrUsed = 0;
	bih.biClrImportant = 0;

	BITMAPINFO bi;
	memset(&bi, 0, sizeof(bi));
	memcpy(&(bi.bmiHeader), &bih, sizeof(BITMAPINFOHEADER));
	int iWidth = bih.biWidth;
	int iHeight = bih.biHeight;	

	// display bitmap
	HDC hdcStill = ::GetDC(hShowWnd);
	PAINTSTRUCT ps;
	::BeginPaint(hShowWnd, &ps);


	::SetStretchBltMode(hdcStill, COLORONCOLOR);


	::StretchDIBits(hdcStill, 0, 0, nWidth, nHeight,
		0, 0, iWidth, iHeight, srcMat.data, &bi,
		DIB_RGB_COLORS, SRCCOPY);



	::EndPaint(hShowWnd, &ps);
	::ReleaseDC(hShowWnd, hdcStill);

}

#define WIDTHBYTES(bits)    (((bits) + 31) / 32 * 4)
BOOL RtspClient::MirrorDIB(LPSTR lpDIBBits, LONG lWidth, LONG lHeight, BOOL bDirection, int nImageBits)
{
	LPSTR	lpSrc;	
	LPSTR	lpDst;
	
	LPSTR	lpBits;
	HLOCAL	hBits;
	
	LONG	i;
	LONG	j;
	int nBits;
	
	LONG lLineBytes;
	
	lLineBytes = WIDTHBYTES(lWidth *nImageBits);
	
	hBits = LocalAlloc(LHND, lLineBytes);
	if (hBits == NULL)
	{		
		return FALSE;
	}
	
	lpBits = (char *)LocalLock(hBits);
	int nStep = nImageBits / 8;
	long lCenter = lWidth / 2 * nStep;
	
	if (bDirection)
	{
		for (i = 0; i < lHeight; i++)
		{
			
			for (j = 0; j < lCenter; j += nStep)
			{
				for (nBits = 0; nBits<nStep; nBits++)
				{
					lpSrc = (char *)lpDIBBits + lLineBytes * i + lCenter - j + nBits;
					lpDst = (char *)lpDIBBits + lLineBytes * i + lCenter + j + nBits;
					*lpBits = *lpDst;
					*lpDst = *lpSrc;
					*lpSrc = *lpBits;
				}

			}

		}
	}
	else
	{		
		for (i = 0; i < lHeight / 2; i++)
		{			
			lpSrc = (char *)lpDIBBits + lLineBytes * i;			
			lpDst = (char *)lpDIBBits + lLineBytes * (lHeight - i - 1);
			
			memcpy(lpBits, lpDst, lLineBytes);			
			memcpy(lpDst, lpSrc, lLineBytes);			
			memcpy(lpSrc, lpBits, lLineBytes);

		}
	}
	
	LocalUnlock(hBits);
	LocalFree(hBits);
	
	return TRUE;
}

