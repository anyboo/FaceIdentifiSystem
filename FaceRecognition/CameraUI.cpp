#include "stdafx.h"
#include "CameraUI.h"
#include "TiCapture2.h"
#include <cv.h>  
#include <cxcore.h>  
#include <highgui.h>
#include <Poco/Path.h>
#include <Poco/Exception.h>
#include <Poco/TemporaryFile.h>
#include <Poco/FileStream.h>

using Poco::TemporaryFile;
using Poco::FileOutputStream;
using Poco::Path;
/*
优化：将摄像头的捕获采用opencv
*/

CameraUI::CameraUI() :
_camera(0),
_width(640),
_height(480),
_captured(false)
{
	Prepare();
	_bitmapSize = _width * _height * 3;
}


CameraUI::~CameraUI()
{
	if (_camera)
	{
		delete _camera;
	}
}

LPCTSTR	CameraUI::GetClass() const
{
	return DUI_CTR_CAMERAUI;
}

LPVOID	CameraUI::GetInterface(LPCTSTR pstrName)
{
	if (_tcscmp(pstrName, DUI_CTR_CAMERAUI) == 0) return static_cast<CameraUI*>(this);
	return CControlUI::GetInterface(pstrName);
}

void CameraUI::SetVisible(bool bVisible)
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

long CameraUI::GetBitmapSize()
{
	return _bitmapSize;
}

void CameraUI::StartCapture()
{
	if (_captured || _camera == NULL)
	{
		return;
	}

	m_pManager->SetTimer(this, EVENT_CAMER_TIEM_ID, 100);

	_captured = _camera->Play();
}

void CameraUI::StopCapture()
{
	if (!_captured || _camera == NULL)
	{
		return;
	}

	m_pManager->KillTimer(this, EVENT_CAMER_TIEM_ID);
	this->Invalidate();

	_camera->Stop();
	_captured = false;
}

void CameraUI::Init()
{

}

void CameraUI::Prepare()
{
	SCapParam cameraParam;
	memset(&cameraParam, 0, sizeof(SCapParam));

	cameraParam.szCap.cx = _width;
	cameraParam.szCap.cy = _height;
	cameraParam.eType = CAP_WDM;
	cameraParam.lIndex = 0;

	_camera = CCapture::Create(&cameraParam);
	assert(_camera);
	if (_camera)
	{
		_camera->InitCapture();
	}
}

bool CameraUI::DoPaint(HDC hDC, const RECT& rcPaint, CControlUI* pStopControl)
{
	DrawFrame(hDC);
	return true;
}

void CameraUI::OnTimer(UINT_PTR idEvent)
{
	if (idEvent != EVENT_CAMER_TIEM_ID)
		return;
	this->Invalidate();
}

void CameraUI::DoEvent(TEventUI& event)
{
	if (event.Type == UIEVENT_TIMER)
		OnTimer((UINT_PTR)event.wParam);
	/*if (event.Type == UIEVENT_DBLCLICK)
		ScreenSnapshot();*/
}

bool CameraUI::ScreenSnapshot(const std::string& jpegPath)
{
	if (!_captured || _camera == NULL)
	{
		return false;
	}

	long len = _width*_height * 3;
	BYTE* data = new BYTE[len];
	_camera->GetFrame(data, len);

	/*Path p(Path::current());
	std::string temp(p.parent().toString());*/
	
	std::string temp(Path::current().append("photoshop\\TemporaryFile.bmp"));
	SaveBmp((char*)data, _width, _height, temp);
	
	IplImage *image = cvLoadImage(temp.c_str());
	cvSaveImage(jpegPath.c_str(), image);
	cvReleaseImage(&image);

	delete[] data;

	return true;
}

BYTE* CameraUI::beginSnapshot()
{
	if (_captured || _camera == NULL)
	{
		return NULL;
	}

	long len = _width*_height * 3;
	_buffer = new BYTE[len];

	if (!_camera->GetFrame(_buffer, len))
	{
		delete[]_buffer;
		_buffer = 0;
	}

	return _buffer;
}

void CameraUI::endSnapshot()
{
	if (_buffer)
		delete _buffer;
}

void CameraUI::ScreenSnapshot(BYTE* framebuffer, long len)
{
	if (_captured || _camera == NULL)
	{
		return;
	}

	if(len < (_width*_height * 3))
		throw Poco::Exception("framebuffer is too small!");

	if (!_camera->GetFrame(framebuffer, len))
	{
		throw Poco::Exception("ScreenSnapshot failed!");
	}
}

void CameraUI::SaveBmp(char* data, int width, int height, const std::string& path)
{
	BITMAPINFOHEADER bih;
	bih.biSize = 40; 						// header size
	bih.biWidth = width;
	bih.biHeight = height;
	bih.biPlanes = 1;
	bih.biBitCount = 24;					// RGB encoded, 24 bit
	bih.biCompression = BI_RGB;			// no compression
	bih.biSizeImage = width*height * 3;
	bih.biXPelsPerMeter = 0;
	bih.biYPelsPerMeter = 0;
	bih.biClrUsed = 0;
	bih.biClrImportant = 0;

	BITMAPINFO bi;
	memset(&bi, 0, sizeof(bi));
	memcpy(&(bi.bmiHeader), &bih, sizeof(BITMAPINFOHEADER));
	int iWidth = bih.biWidth;
	int iHeight = bih.biHeight;

	DWORD word = 0;
	std::string Path = path;

	TemporaryFile file(Path);
	FileOutputStream fs(Path);

	//fileheader
	BITMAPFILEHEADER bmfh;
	int nBitsOffset = sizeof(BITMAPFILEHEADER)+bi.bmiHeader.biSize;
	LONG lImageSize = bi.bmiHeader.biSizeImage;
	LONG lFileSize = nBitsOffset + lImageSize;
	bmfh.bfType = 'B' + ('M' << 8);
	bmfh.bfOffBits = nBitsOffset;
	bmfh.bfSize = lFileSize;
	bmfh.bfReserved1 = bmfh.bfReserved2 = 0;
	fs.write((char*)&bmfh, sizeof(BITMAPFILEHEADER));
	//header
	fs.write((char*)&bi.bmiHeader, sizeof(BITMAPINFOHEADER));
	//data
	fs.write((char*)data, bih.biSizeImage);
	fs.close();
}
void CameraUI::DrawFrame(HDC PaintDC)
{
	if (!_captured || _camera == NULL)
	{
		return;
	}

	long len = _width*_height * 3;
	BYTE* data = new BYTE[len];
	bool ret = _camera->GetFrame(data, len);

	::SetStretchBltMode(PaintDC, COLORONCOLOR);

	BITMAPINFOHEADER bih;

	bih.biSize = 40;
	bih.biWidth = _width;
	bih.biHeight = _height;
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

	::StretchDIBits(PaintDC, GetX(), GetY(), GetWidth(), GetHeight(),
		0, 0, _width, _height, data, &bi,
		DIB_RGB_COLORS, SRCCOPY);

	delete[] data;
}
