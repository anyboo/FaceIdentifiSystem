#include "stdafx.h"
#include "RtspClient.h"
#include <iostream>


RtspClient::RtspClient()
{
	
}


RtspClient::~RtspClient()
{
	
}



bool RtspClient::start(const char* pURL, DownloadCallback callback, PVOID pDlg)
{
	bool bRet = true;
	//初始化AVFormatContext, 分配一块内存，保存视频属性信息
	AVFormatContext * pFormatCtx = avformat_alloc_context();
	if (avformat_open_input(&pFormatCtx, pURL, NULL, NULL) != 0)
	{
		TRACE("open  input  file  faile !");
		return false;
	}

	//查找流信息
	if (avformat_find_stream_info(pFormatCtx, NULL) < 0)
	{
		TRACE("Couldn't find stream information.\n");
		return false;
	}

	int videoStream;
	//获取视频流ID
	for (int i = 0; i < pFormatCtx->nb_streams; i++)
	{
		if (pFormatCtx->streams[i]->codec->codec_type == AVMEDIA_TYPE_VIDEO)
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

	AVCodecContext *pCodecCtx = pFormatCtx->streams[videoStream]->codec;
	//查找解码器
	AVCodec *pCodec = avcodec_find_decoder(pCodecCtx->codec_id);
	if (pCodec == NULL)
	{
		TRACE("could not find decoder");
		return false;
	}

	//打开解码器
	if (avcodec_open2(pCodecCtx, pCodec, NULL) < 0)
	{
		TRACE("could not open decoder");
		return false;
	}

	av_dump_format(pFormatCtx, 0, pURL, 0);
	//存储解码后AVFrame
	AVFrame *pFrame = av_frame_alloc();
	//存储转换后AVFrame
	AVFrame *pFrameRGB = av_frame_alloc();
	AVPacket *packet = (AVPacket *)av_malloc(sizeof(AVPacket));
	if (pFrameRGB == NULL)
	{
		TRACE("open mem faile");
		return false;
	}

	//分配AVFrame所需内存
	int numBytes = avpicture_get_size(AV_PIX_FMT_RGB24, pCodecCtx->width, pCodecCtx->height);
	BYTE *buffer = (BYTE *)av_malloc(numBytes*sizeof(BYTE));
	////填充AVFrame
	avpicture_fill((AVPicture *)pFrameRGB, buffer, AV_PIX_FMT_RGB24, pCodecCtx->width, pCodecCtx->height);

	IplImage* motion = 0;
	int frameFinished = 0;
	while (av_read_frame(pFormatCtx, packet) >= 0)
	{
		if (packet->stream_index != videoStream)
		{
			//Sleep(50);
			continue;
		}

		avcodec_decode_video2(pCodecCtx, pFrame, &frameFinished, packet);
		if (!frameFinished)
		{
			TRACE("frameFinished is false");
			//Sleep(50);
			continue;
		}

		struct SwsContext * img_convert_ctx;
		img_convert_ctx = sws_getContext(pCodecCtx->width, pCodecCtx->height, pCodecCtx->pix_fmt, pCodecCtx->width, pCodecCtx->height, AV_PIX_FMT_RGB24, SWS_BICUBIC, NULL, NULL, NULL);
		if (img_convert_ctx == NULL)
		{
			TRACE("Cannot initialize the conversion context !");
			return false;
		}

		static BYTE *p = NULL;
		p = pFrame->data[1];
		pFrame->data[1] = pFrame->data[2];
		pFrame->data[2] = p;

		sws_scale(img_convert_ctx,
			pFrame->data, pFrame->linesize, 0, pCodecCtx->height,
			pFrameRGB->data, pFrameRGB->linesize);

		if (img_convert_ctx)
		{
			sws_freeContext(img_convert_ctx);
			img_convert_ctx = NULL;
		}

		callback((const char *)buffer, pCodecCtx->width * pCodecCtx->height * 3, pCodecCtx->width, pCodecCtx->height, (UINT)pDlg);

		//Sleep(10);
	}

	return bRet;
}

bool RtspClient::stop()
{
	bool bRet = true;

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
	// 鎸囧悜婧愬浘鍍忕殑鎸囬拡
	LPSTR	lpSrc;
	// 鎸囧悜瑕佸鍒跺尯鍩熺殑鎸囬拡
	LPSTR	lpDst;
	// 鎸囧悜澶嶅埗鍥惧儚鐨勬寚閽?
	LPSTR	lpBits;
	HLOCAL	hBits;
	// 寰幆鍙橀噺
	LONG	i;
	LONG	j;
	int nBits;//姣忓儚绱犲崰鐨勪綅鏁?
	// 鍥惧儚姣忚鐨勫瓧鑺傛暟
	LONG lLineBytes;
	// 璁＄畻鍥惧儚姣忚鐨勫瓧鑺傛暟
	lLineBytes = WIDTHBYTES(lWidth *nImageBits);
	// 鏆傛椂鍒嗛厤鍐呭瓨锛屼互淇濆瓨涓�琛屽浘鍍?
	hBits = LocalAlloc(LHND, lLineBytes);
	if (hBits == NULL)
	{
		// 鍒嗛厤鍐呭瓨澶辫触
		return FALSE;
	}
	// 閿佸畾鍐呭瓨
	lpBits = (char *)LocalLock(hBits);
	int nStep = nImageBits / 8;
	long lCenter = lWidth / 2 * nStep;
	// 鍒ゆ柇闀滃儚鏂瑰紡
	if (bDirection)
	{
		// 姘村钩闀滃儚
		// 閽堝鍥惧儚姣忚杩涜鎿嶄綔
		for (i = 0; i < lHeight; i++)
		{
			// 閽堝姣忚鍥惧儚宸﹀崐閮ㄥ垎杩涜鎿嶄綔
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
		// 鍨傜洿闀滃儚
		// 閽堝涓婂崐鍥惧儚杩涜鎿嶄綔
		for (i = 0; i < lHeight / 2; i++)
		{
			// 鎸囧悜鍊掓暟绗琲琛岃薄绱犺捣鐐圭殑鎸囬拡
			lpSrc = (char *)lpDIBBits + lLineBytes * i;
			// 鎸囧悜绗琲琛岃薄绱犺捣鐐圭殑鎸囬拡
			lpDst = (char *)lpDIBBits + lLineBytes * (lHeight - i - 1);
			// 澶囦唤涓�琛岋紝瀹藉害涓簂Width
			memcpy(lpBits, lpDst, lLineBytes);
			// 灏嗗�掓暟绗琲琛岃薄绱犲鍒跺埌绗琲琛?
			memcpy(lpDst, lpSrc, lLineBytes);
			// 灏嗙i琛岃薄绱犲鍒跺埌鍊掓暟绗琲琛?
			memcpy(lpSrc, lpBits, lLineBytes);

		}
	}
	// 閲婃斁鍐呭瓨
	LocalUnlock(hBits);
	LocalFree(hBits);
	// 杩斿洖
	return TRUE;
}

