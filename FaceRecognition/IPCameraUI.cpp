#include "stdafx.h"
#include "IPCameraUI.h"
#include "TiCapture2.h"
#include <cv.h>  
#include <cxcore.h>  
#include <highgui.h>
#include <Poco/Path.h>
#include <Poco/Exception.h>
#include <Poco/TemporaryFile.h>
#include <Poco/FileStream.h>
#include <Poco/Data/Session.h>
#include <Poco/Data/SQLite/Connector.h>
#include <Poco/Data/SQLite/SQLiteException.h>
#include <Poco/Logger.h>
//#pragma comment(lib, "avcodec.lib")
//#pragma comment(lib, "avformat.lib")
//#pragma comment(lib, "avutil.lib")
//#pragma comment(lib, "swscale.lib")

using Poco::Logger;
using Poco::TemporaryFile;
using Poco::FileOutputStream;
using Poco::Path;
using Poco::Data::Session;
using Poco::Data::Statement;
using namespace Poco::Data::Keywords;
using namespace cv;

IPCameraUI::IPCameraUI() :
_captured(false),
_paint(false),
pixfmt(AVPixelFormat::AV_PIX_FMT_BGR24),
_FormatCtx(0),
_Frame(0),
_FrameRGB(0),
packet(0),
context(0)
{
	try
	{
		load();
		Prepare();
		_paint = true;
	}
	catch (Poco::Exception& e)
	{
		poco_information_f1(Poco::Logger::get("FileLogger"), "IPCameraUI : %s", e.displayText());
	}
}

void IPCameraUI::load()
{
	struct ipc
	{
		int id;
		std::string ip;
		int port;
		std::string user;
		std::string passwd;
		int channel;
	};

	ipc config;

	try
	{
		Session session("SQLite", "facerecog.db");
		Statement select(session);

		select << "SELECT id, ip, port, user, passwd, channel FROM ipcamerasetting",
			into(config.id),
			into(config.ip),
			into(config.port),
			into(config.user),
			into(config.passwd),
			into(config.channel),
			now;

		_url = (Poco::format("rtsp://%s:%d/user=%s&password=%s&channel=%d&stream=0.sdp", 
			config.ip, 
			config.port, 
			config.user,
			config.passwd,
			config.channel
			));
#ifdef _DEBUG
		_url = "rtsp://192.168.1.224:554/user=admin&password=&channel=1&stream=0.sdp?";
#endif
		poco_information(Poco::Logger::get("FileLogger"), _url);
	}
	catch (Poco::Exception& e)
	{
		DUITRACE("IPCameraUI :%s", e.displayText());
	}
}

IPCameraUI::~IPCameraUI()
{
	Release();
}

void IPCameraUI::Release()
{
	if (packet)
		av_packet_unref(packet);
	if (context)
		avcodec_close(context);
	if (_Frame)
		av_free(_Frame);
	if (_FrameRGB)
		av_free(_FrameRGB);
	if (_FormatCtx)
		avformat_close_input(&_FormatCtx);
}

LPCTSTR	IPCameraUI::GetClass() const
{
	return DUI_CTR_IPCameraUI;
}

LPVOID	IPCameraUI::GetInterface(LPCTSTR pstrName)
{
	if (_tcscmp(pstrName, DUI_CTR_IPCameraUI) == 0) return static_cast<IPCameraUI*>(this);
	return CControlUI::GetInterface(pstrName);
}

void IPCameraUI::SetVisible(bool bVisible)
{
	CControlUI::SetVisible(bVisible);
	if (bVisible)
	{
		StartCapture();
	}
	else
	{
		StopCapture();
	}	
}

void IPCameraUI::StartCapture()
{
	if (_captured || !_paint)
		return;
	
	m_pManager->SetTimer(this, EVENT_IPCAMER_TIEM_ID, 30);
	_captured = !_captured;
}

void IPCameraUI::StopCapture()
{
	if (!_captured || !_paint)
		return;

	m_pManager->KillTimer(this, EVENT_IPCAMER_TIEM_ID);
	this->Invalidate();

	_captured = !_captured;
}

void IPCameraUI::Prepare()
{
	av_register_all();
	avcodec_register_all();
	avformat_network_init();

	_FormatCtx = avformat_alloc_context();
	AVDictionary* optionsDict = NULL;
	av_dict_set(&optionsDict, "stimeout", "500000", 0);

	if (avformat_open_input(&_FormatCtx, _url.c_str(), NULL, &optionsDict) != 0)
	{
		DUITRACE("%s %s failed!", "avformat_open_input", _url.c_str());
		throw Poco::LogicException("avformat_open_input failed");
	}

	//判断流是否存在stream相关信息
	if (avformat_find_stream_info(_FormatCtx, NULL) < 0)
	{
		DUITRACE("%s %s failed!", "avformat_find_stream_info", _url.c_str());
		throw Poco::LogicException("avformat_find_stream_info failed");
	}

	video_stream_index = av_find_best_stream(_FormatCtx, AVMEDIA_TYPE_VIDEO, -1, -1, NULL, 0);

	//根据codec id找到对应的codec
	context = avcodec_alloc_context3(NULL);
	avcodec_parameters_to_context(context, _FormatCtx->streams[video_stream_index]->codecpar);

	AVCodec *codec = avcodec_find_decoder(context->codec_id);
	if (codec == NULL)
	{
		DUITRACE("%s %s failed!", "avcodec_find_decoder","codec not found!");
		throw Poco::LogicException("avcodec_find_decoder codec not found!");
	}

	if (avcodec_open2(context, codec, NULL) < 0) {
		DUITRACE("%s %s failed!", "avcodec_open2", "open context");
		throw Poco::LogicException("avcodec_open2 open context failed");
	}

	_Frame = av_frame_alloc();
	_FrameRGB = av_frame_alloc();
	int numBytes = av_image_get_buffer_size(pixfmt, context->width, context->height, 1);

	uint8_t * _buffer = (uint8_t*)av_malloc(numBytes*sizeof(uint8_t));
	av_image_fill_arrays(_FrameRGB->data, _FrameRGB->linesize, _buffer, pixfmt, context->width, context->height, 1);

	int y_size = context->width * context->height;
	packet = (AVPacket*)malloc(sizeof(AVPacket));
	av_new_packet(packet, y_size);

	img_convert_ctx =
		sws_getCachedContext(0,
		context->width,
		context->height,
		context->pix_fmt,
		context->width / 3,
		context->height / 3,
		pixfmt,
		SWS_BICUBIC,
		0, 0, 0);

}

bool IPCameraUI::DoPaint(HDC hDC, const RECT& rcPaint, CControlUI* pStopControl)
{
	DrawFrame(hDC);
	return true;
}

void IPCameraUI::OnTimer(UINT_PTR idEvent)
{
	if (idEvent != EVENT_IPCAMER_TIEM_ID)
		return;
	this->Invalidate();
}

void IPCameraUI::DoEvent(TEventUI& event)
{
	if (event.Type == UIEVENT_TIMER)
		OnTimer((UINT_PTR)event.wParam);
	/*if (event.Type == UIEVENT_DBLCLICK)
		ScreenSnapshot();*/
}

void IPCameraUI::DrawFrame(HDC PaintDC)
{
	if (!_captured || !_paint) return;

	int res = 0;
	if (res = av_read_frame(_FormatCtx, packet) >= 0)
	{
		if (packet->stream_index != video_stream_index)
			return;

		if (avcodec_send_packet(context, packet) == 0)
		{
			avcodec_receive_frame(context, _Frame);

			if (_Frame->height <= 0 || _Frame->width <= 0)
				return;
			
			sws_scale(img_convert_ctx,
				_Frame->data,
				_Frame->linesize,
				0,
				context->height,
				_FrameRGB->data,
				_FrameRGB->linesize
				);
		}
	}

	// Mat(int _rows, int _cols, int _type, void* _data, size_t _step=AUTO_STEP);
	/*
	int width = context->width / 3;
	int height = context->height / 3;

	Mat img1(width, height, CV_8UC1, _FrameRGB->data);
	Mat img2(width, height, CV_8UC3);
	*/
	
	//transpose(img1, img2);
	//flip(img1, img2, 0);
	//cvFlip(dest, );
	//cv::Mat img(_Frame->height, _Frame->width, CV_8UC3, _FrameRGB->data[0]);
	//
	::SetStretchBltMode(PaintDC, COLORONCOLOR);

	BITMAPINFOHEADER bih;

	bih.biSize = 40;
	bih.biWidth = _Frame->width;
	bih.biHeight = _Frame->height;
	bih.biPlanes = 1;
	bih.biBitCount = 24;
	bih.biCompression = BI_RGB;
	bih.biSizeImage = bih.biWidth * bih.biHeight;
	bih.biXPelsPerMeter = 0;
	bih.biYPelsPerMeter = 0;
	bih.biClrUsed = 0;
	bih.biClrImportant = 0;

	BITMAPINFO bi;
	memset(&bi, 0, sizeof(bi));
	memcpy(&(bi.bmiHeader), &bih, sizeof(BITMAPINFOHEADER));
	int iWidth = bih.biWidth;
	int iHeight = bih.biHeight;
	
	::StretchDIBits(PaintDC, GetX(), GetY(), GetWidth(), GetHeight(),
		0, 0, _Frame->width, _Frame->height, _FrameRGB, &bi,
		DIB_RGB_COLORS, SRCCOPY);
}
