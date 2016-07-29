#include "stdafx.h"
#include "Camera.h"
#include "TiCapture2.h"
#include <Poco/Foundation.h>
#include <Poco/Buffer.h>

using Poco::Buffer;

#pragma warning(disable:4800)

Camera::Camera()
:_width(640), _height(480)
{
	SCapParam cameraParam;
	memset(&cameraParam, 0, sizeof(SCapParam));

	cameraParam.szCap.cx = _width;
	cameraParam.szCap.cy = _height;
	cameraParam.eType = CAP_WDM;
	cameraParam.lIndex = 0;

	_camera = CCapture::Create(&cameraParam);
	poco_check_ptr(_camera);
	bool ret = _camera->InitCapture();
	poco_assert(ret);
}


Camera::~Camera()
{
	Close();
}


void Camera::Open()
{
	poco_check_ptr(_camera);
	bool ret = _camera->Play();
	poco_assert(ret);
}

void Camera::Close()
{
	if (_camera)
	{
		_camera->Stop();
		delete _camera;
		_camera = 0;
	}
}

void Camera::SetQuality(SCapQuality* pQuality)
{
	poco_check_ptr(_camera);
	bool ret = _camera->SetQuality(pQuality);
	poco_assert(ret);
}

void Camera::GetFrame()
{
	long length = _width* _height * 3;

	Buffer<BYTE> fb(length);
	poco_check_ptr(_camera);
	bool ret = _camera->GetFrame(fb.begin(), length);
	poco_assert(ret);
}

//class Photo
//{
//private:
//	size_t width;
//	size_t height;
//	Buffer<char> fb(length);
//};
//
//void SaveBmp(BITMAPINFO& bi, BYTE* pDataBuf)
//{
//	void ContructBih(int nWidth, int nHeight, BITMAPINFOHEADER& bih)
//	{
//		bih.biSize = 40; 						// header size
//		bih.biWidth = nWidth;
//		bih.biHeight = nHeight;
//		bih.biPlanes = 1;
//		bih.biBitCount = 24;					// RGB encoded, 24 bit
//		bih.biCompression = BI_RGB;			// no compression
//		bih.biSizeImage = nWidth*nHeight * 3;
//		bih.biXPelsPerMeter = 0;
//		bih.biYPelsPerMeter = 0;
//		bih.biClrUsed = 0;
//		bih.biClrImportant = 0;
//	}
//
//	BITMAPINFO bi;
//	memset(&bi, 0, sizeof(bi));
//	memcpy(&(bi.bmiHeader), &bih, sizeof(BITMAPINFOHEADER));
//	int iWidth = bih.biWidth;
//	int iHeight = bih.biHeight;
//
//	DWORD word = 0;
//	CString Path = "d:\\bmp\\";
//	CString name = "test";
//	CString ext = ".bmp";
//	name += std::to_string(::GetTickCount()).c_str();
//	Path.Append(name);
//	Path.Append(ext);
//	::OutputDebugStringA(Path + "\n");
//	HANDLE hfile = CreateFile(Path, GENERIC_WRITE, FILE_SHARE_READ, 0, OPEN_ALWAYS, 0, 0);
//	//fileheader
//	BITMAPFILEHEADER bmfh;                         // Other BMP header
//	int nBitsOffset = sizeof(BITMAPFILEHEADER)+bi.bmiHeader.biSize;
//	LONG lImageSize = bi.bmiHeader.biSizeImage;
//	LONG lFileSize = nBitsOffset + lImageSize;
//	bmfh.bfType = 'B' + ('M' << 8);
//	bmfh.bfOffBits = nBitsOffset;
//	bmfh.bfSize = lFileSize;
//	bmfh.bfReserved1 = bmfh.bfReserved2 = 0;
//	WriteFile(hfile, &bmfh, sizeof(BITMAPFILEHEADER), &word, 0);
//	//header
//	WriteFile(hfile, &bi.bmiHeader, sizeof(BITMAPINFOHEADER), &word, 0);
//	//info
//	//WriteFile(hfile, &bi, sizeof(BITMAPINFO), &word, 0);
//	//data
//	int bit = COLORONCOLOR;
//	WriteFile(hfile, pDataBuf, bi.bmiHeader.biWidth * bi.bmiHeader.biHeight * bit/*bih.biBitCount*/, &word, 0);
//	CloseHandle(hfile);
//}