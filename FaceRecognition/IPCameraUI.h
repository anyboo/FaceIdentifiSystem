#pragma once

#include <DuiLib/UIlib.h>

#define  DUI_CTR_IPCameraUI                         (_T("IPCameraUI"))
#define  EVENT_IPCAMER_TIEM_ID  1011

#ifdef __cplusplus
extern "C"{
#endif

#include "libavformat/avformat.h"
#include "libavcodec/avcodec.h"
#include "libswscale/swscale.h"
#include "libavutil/avutil.h"
#include "libavutil/imgutils.h"

#ifdef __cplusplus
}
#endif

class IPCameraUI : public CControlUI
{
public:
	IPCameraUI();
	~IPCameraUI();

	LPCTSTR	GetClass() const;
	LPVOID	GetInterface(LPCTSTR pstrName);
	virtual void SetVisible(bool bVisible = true);
	virtual bool DoPaint(HDC hDC, const RECT& rcPaint, CControlUI* pStopControl);
	virtual void DoEvent(TEventUI& event);

private:
	void OnTimer(UINT_PTR idEvent);
	void DrawFrame(HDC hDC);	
	void StartCapture();
	void StopCapture();
	void Prepare();
	void Release();
	void load();

	bool _captured;
	bool _paint;

	std::string _url;
	int video_stream_index;
	/*int _width;
	int _height;*/

	AVFormatContext * _FormatCtx;
	AVFrame *_Frame;
	AVFrame *_FrameRGB;
	AVPacket *packet;
	AVCodecContext* context;
	AVPixelFormat pixfmt;
	struct SwsContext * img_convert_ctx;
};