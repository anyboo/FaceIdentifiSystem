#include "stdafx.h"
#include "ClipUI.h"

CClipUI::CClipUI()
{
	
}


CClipUI::~CClipUI()
{
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

bool CClipUI::DoPaint(HDC hDC, const RECT& rcPaint, CControlUI* pStopControl)
{
	if (_picture.isNull()) return true;

	poco_check_ptr(_picture.get());
	_picture->MirrorDIB(true);

	//PAINTSTRUCT ps;
	//::BeginPaint(hwnd, &ps);


	::SetStretchBltMode(hDC, COLORONCOLOR);

	BITMAPINFOHEADER bih;

	bih.biSize = 40;
	bih.biWidth = _picture->width();
	bih.biHeight = _picture->height();
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

	::StretchDIBits(hDC, GetX(), GetY(), GetWidth(), GetHeight(),
		0, 0, _picture->width(), _picture->height(), _picture->data(), &bi,
		DIB_RGB_COLORS, SRCCOPY);

	/*::EndPaint(hwnd, &ps);
	::ReleaseDC(hwnd, hDC);*/
	return true;
}