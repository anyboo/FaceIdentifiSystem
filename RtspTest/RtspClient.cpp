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
	//��ʼ��AVFormatContext, ����һ���ڴ棬������Ƶ������Ϣ
	AVFormatContext * pFormatCtx = avformat_alloc_context();
	if (avformat_open_input(&pFormatCtx, pURL, NULL, NULL) != 0)
	{
		TRACE("open  input  file  faile !");
		return false;
	}

	//��������Ϣ
	if (avformat_find_stream_info(pFormatCtx, NULL) < 0)
	{
		TRACE("Couldn't find stream information.\n");
		return false;
	}

	int videoStream;
	//��ȡ��Ƶ��ID
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
	//���ҽ�����
	AVCodec *pCodec = avcodec_find_decoder(pCodecCtx->codec_id);
	if (pCodec == NULL)
	{
		TRACE("could not find decoder");
		return false;
	}

	//�򿪽�����
	if (avcodec_open2(pCodecCtx, pCodec, NULL) < 0)
	{
		TRACE("could not open decoder");
		return false;
	}

	av_dump_format(pFormatCtx, 0, pURL, 0);
	//�洢�����AVFrame
	AVFrame *pFrame = av_frame_alloc();
	//�洢ת����AVFrame
	AVFrame *pFrameRGB = av_frame_alloc();
	AVPacket *packet = (AVPacket *)av_malloc(sizeof(AVPacket));
	if (pFrameRGB == NULL)
	{
		TRACE("open mem faile");
		return false;
	}

	//����AVFrame�����ڴ�
	int numBytes = avpicture_get_size(AV_PIX_FMT_RGB24, pCodecCtx->width, pCodecCtx->height);
	BYTE *buffer = (BYTE *)av_malloc(numBytes*sizeof(BYTE));
	////���AVFrame
	avpicture_fill((AVPicture *)pFrameRGB, buffer, AV_PIX_FMT_RGB24, pCodecCtx->width, pCodecCtx->height);

	IplImage* motion = 0;
	int frameFinished = 0;
	while (av_read_frame(pFormatCtx, packet) >= 0)
	{
		if (packet->stream_index != videoStream)
		{
			//Sleep(50);
			av_packet_unref(packet);
			continue;
		}
		

		avcodec_decode_video2(pCodecCtx, pFrame, &frameFinished, packet);
		if (!frameFinished)
		{
			TRACE("frameFinished is false");
			//Sleep(50);
			av_packet_unref(packet);
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
		av_packet_unref(packet);
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
	// 指向源图像的指针
	LPSTR	lpSrc;
	// 指向要复制区域的指针
	LPSTR	lpDst;
	// 指向复制图像的指�?
	LPSTR	lpBits;
	HLOCAL	hBits;
	// 循环变量
	LONG	i;
	LONG	j;
	int nBits;//每像素占的位�?
	// 图像每行的字节数
	LONG lLineBytes;
	// 计算图像每行的字节数
	lLineBytes = WIDTHBYTES(lWidth *nImageBits);
	// 暂时分配内存，以保存一行图�?
	hBits = LocalAlloc(LHND, lLineBytes);
	if (hBits == NULL)
	{
		// 分配内存失败
		return FALSE;
	}
	// 锁定内存
	lpBits = (char *)LocalLock(hBits);
	int nStep = nImageBits / 8;
	long lCenter = lWidth / 2 * nStep;
	// 判断镜像方式
	if (bDirection)
	{
		// 水平镜像
		// 针对图像每行进行操作
		for (i = 0; i < lHeight; i++)
		{
			// 针对每行图像左半部分进行操作
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
		// 垂直镜像
		// 针对上半图像进行操作
		for (i = 0; i < lHeight / 2; i++)
		{
			// 指向倒数第i行象素起点的指针
			lpSrc = (char *)lpDIBBits + lLineBytes * i;
			// 指向第i行象素起点的指针
			lpDst = (char *)lpDIBBits + lLineBytes * (lHeight - i - 1);
			// 备份一行，宽度为lWidth
			memcpy(lpBits, lpDst, lLineBytes);
			// 将倒数第i行象素复制到第i�?
			memcpy(lpDst, lpSrc, lLineBytes);
			// 将第i行象素复制到倒数第i�?
			memcpy(lpSrc, lpBits, lLineBytes);

		}
	}
	// 释放内存
	LocalUnlock(hBits);
	LocalFree(hBits);
	// 返回
	return TRUE;
}

