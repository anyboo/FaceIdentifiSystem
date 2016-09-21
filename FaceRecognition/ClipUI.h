#pragma once
#include "Picture.h"
#define  DUI_CTR_CLIP                         (_T("Clip"))

#include <Poco/Timer.h>
#include <Poco/Stopwatch.h>

class CClipUI 
	: public CControlUI
{
public:
	CClipUI();
	~CClipUI();

	LPCTSTR GetClass() const;
	LPVOID GetInterface(LPCTSTR pstrName);
	virtual bool DoPaint(HDC hDC, const RECT& rcPaint, CControlUI* pStopControl);
	void PushImage(Picture::Ptr pic);
protected:
	void onTimer(Poco::Timer& timer);
	void DrawFrame(HDC hDC);
	void DeleteImage();
	Gdiplus::Image* LoadPicFromMemory(LPVOID pBuf, size_t dwSize);
	void CreateBitmapInfo();

private:
	Picture::Ptr _image;
	BITMAPINFO bi;
	Poco::Timer t;
	Poco::TimerCallback<CClipUI> tc;

	Poco::Stopwatch sw;
};

