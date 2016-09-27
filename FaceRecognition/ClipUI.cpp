#include "stdafx.h"
#include "ClipUI.h"

CClipUI::CClipUI():
t(100, 50), tc(*this, &CClipUI::onTimer)
{
	t.start(tc);
}


CClipUI::~CClipUI()
{
	t.stop();
}

LPCTSTR CClipUI::GetClass() const
{
	return DUI_CTR_CLIP;
}

LPVOID CClipUI::GetInterface(LPCTSTR pstrName)
{
	if (_tcscmp(pstrName, DUI_CTR_CLIP) == 0) return static_cast<CClipUI*>(this);
	return CControlUI::GetInterface(pstrName);
}

void CClipUI::onTimer(Poco::Timer& timer)
{
	Invalidate();
}

#include <sstream>

void debug_msg(std::string tip, unsigned long seconds)
{
	std::stringstream ss;
	ss << tip << "- StopWatch:" << seconds << "ms" << std::endl;
	OutputDebugStringA(ss.str().c_str());
}
bool CClipUI::DoPaint(HDC hDC, const RECT& rcPaint, CControlUI* pStopControl)
{
	::SetStretchBltMode(hDC, COLORONCOLOR);

	::StretchDIBits(hDC, GetX(), GetY(), GetWidth(), GetHeight(),
		0, 0, _image->width(), _image->height(), _image->data(), &bi,
		DIB_RGB_COLORS, SRCCOPY);
	sw.stop();

	return true;
}

void CClipUI::CreateBitmapInfo()
{
	BITMAPINFOHEADER bih;

	bih.biSize = 40;
	bih.biWidth = _image->width();
	bih.biHeight = _image->height();
	bih.biPlanes = 1;
	bih.biBitCount = 24;
	bih.biCompression = BI_RGB;
	bih.biSizeImage = bih.biWidth * bih.biHeight * 3;
	bih.biXPelsPerMeter = 0;
	bih.biYPelsPerMeter = 0;
	bih.biClrUsed = 0;
	bih.biClrImportant = 0;

	
	memset(&bi, 0, sizeof(bi));
	memcpy(&(bi.bmiHeader), &bih, sizeof(BITMAPINFOHEADER));
	int iWidth = bih.biWidth;
	int iHeight = bih.biHeight;
}

void CClipUI::PushImage(Picture::Ptr pic)
{
	_image = pic;
	_image->MirrorDIB(true);
	CreateBitmapInfo();
}

