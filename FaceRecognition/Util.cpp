#include "stdafx.h"
#include "Util.h"

Util::Util()
{
}


Util::~Util()
{
}


void Util::DrawSomething(Picture::Ptr pic, CControlUI* control, HWND hwnd)
{
	Picture::MirrorDIB(pic->data(), pic->width(), pic->height(), true, 24);

	poco_check_ptr(control);
	COLORREF* data = (COLORREF*)pic->data();

	CDuiString name = "carmera";
	HDC PaintDC = ::GetDC(hwnd);
	//HDC hChildMemDC = ::CreateCompatibleDC(PaintDC);
	//HBITMAP hBitmap = CRenderEngine::CreateARGB32Bitmap(hChildMemDC, pic->width(), pic->height(), &data);

	PAINTSTRUCT ps;
	::BeginPaint(hwnd, &ps);


	::SetStretchBltMode(PaintDC, COLORONCOLOR);

	BITMAPINFOHEADER bih;

	bih.biSize = 40;
	bih.biWidth = pic->width();
	bih.biHeight = pic->height();
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

	::StretchDIBits(PaintDC, control->GetX(), control->GetY(), control->GetWidth() - 20, control->GetHeight() -20,
		0, 0, pic->width(), pic->height(), pic->data(), &bi,
		DIB_RGB_COLORS, SRCCOPY);

	::EndPaint(hwnd, &ps);
	::ReleaseDC(hwnd, PaintDC);
}
