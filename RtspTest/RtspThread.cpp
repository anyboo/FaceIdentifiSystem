#include "stdafx.h"
#include "RtspThread.h"




RtspThread::RtspThread(
	ThreadCallback call,
	AVFormatContext *pFormatCtx,
	int videoStream,
	AVPacket *packet,
	AVCodecContext *pCodecCtx,
	AVFrame *pFrame,
	AVFrame *pFrameRGB,
	BYTE *buffer,
	PVOID pDlg) :	
	_call((call)),
	_FormatCtx(pFormatCtx),
	_videoStream(videoStream),
	_packet(packet),
	_CodecCtx(pCodecCtx),
	_Frame(pFrame),
	_FrameRGB(pFrameRGB),
	_buffer(buffer),
	_dlg(pDlg)
{
	_break = FALSE;
}


RtspThread::~RtspThread()
{
}

void RtspThread::onEvent(const void* pSender, bool& arg)
{	
	_break = arg;
}

void RtspThread::run()
{	
	int frameFinished = 0;
	while (av_read_frame(_FormatCtx, _packet) >= 0)
	{	
		if (_break)
		{
			av_packet_unref(_packet);
			break;
		}else		
		{
			if (_packet->stream_index != _videoStream)
			{				
				av_packet_unref(_packet);
				continue;
			}


			avcodec_decode_video2(_CodecCtx, _Frame, &frameFinished, _packet);
			if (!frameFinished)
			{
				TRACE("frameFinished is false");				
				av_packet_unref(_packet);
				continue;
			}

			struct SwsContext * img_convert_ctx;
			img_convert_ctx = sws_getContext(_CodecCtx->width, _CodecCtx->height, _CodecCtx->pix_fmt, _CodecCtx->width, _CodecCtx->height, AV_PIX_FMT_RGB24, SWS_BICUBIC, NULL, NULL, NULL);
			if (img_convert_ctx == NULL)
			{
				TRACE("Cannot initialize the conversion context !");
				continue;
			}

			static BYTE *p = NULL;
			p = _Frame->data[1];
			_Frame->data[1] = _Frame->data[2];
			_Frame->data[2] = p;

			sws_scale(img_convert_ctx,
				_Frame->data, _Frame->linesize, 0, _CodecCtx->height,
				_FrameRGB->data, _FrameRGB->linesize);

			if (img_convert_ctx)
			{
				sws_freeContext(img_convert_ctx);
				img_convert_ctx = NULL;
			}

			_call((const char *)_buffer, _CodecCtx->width * _CodecCtx->height * 3, _CodecCtx->width, _CodecCtx->height, (UINT)_dlg);		
			av_packet_unref(_packet);
		}		
		
	}
}