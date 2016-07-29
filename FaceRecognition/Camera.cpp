#include "stdafx.h"
#include "Camera.h"
#include "TiCapture2.h"
#include <Poco/FIFOBuffer.h>
#include <Poco/FIFOBufferStream.h>

using Poco::FIFOBuffer;
using Poco::FIFOBufferStream;

#pragma warning(disable:4800)

Camera::Camera()
:_width(640), _height(480), isRunning(false)
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
	poco_check_ptr(_camera);
	delete _camera;
	_camera = 0;
}


void Camera::Open()
{
	poco_check_ptr(_camera);
	if (!isRunning)
	{
		isRunning = _camera->Play();
		poco_assert(isRunning);
	}
}

void Camera::Close()
{
	poco_check_ptr(_camera);
	if (isRunning)
	{
		_camera->Stop();
	}
}

void Camera::SetQuality(SCapQuality* pQuality)
{
	poco_check_ptr(_camera);
	bool ret = _camera->SetQuality(pQuality);
	poco_assert(ret);
}

size_t Camera::width() const
{
	return _width;
}

size_t Camera::height() const
{
	return _height;
}

#include <Poco/Buffer.h>
#include <Poco/SharedPtr.h>
#include <Poco/AutoPtr.h>

using Poco::SharedPtr;

void Camera::GetFrame()
{
	poco_check_ptr(_camera);

	long length = _width* _height * 3;
	SharedPtr<char> data(new char[length]);
	//char* data = new char[length];
	
	memset(data,0,sizeof(data));
	bool ret = _camera->GetFrame((BYTE*)data.get(), length);
	poco_assert(ret);
	SaveBmp(data);
	/*FIFOBuffer fb(length);
	FIFOBufferStream iostr(fb);
	iostr.write(data, length);
	assert(iostr.rdbuf()->fifoBuffer().isEmpty());
	iostr.rdbuf()->fifoBuffer*/
}

#include <Poco/TemporaryFile.h>
#include <Poco/FileStream.h>

using Poco::TemporaryFile;
using Poco::FileOutputStream;

void Camera::SaveBmp(char* data)
{
	BITMAPINFOHEADER bih;
	bih.biSize = 40; 						// header size
	bih.biWidth = _width;
	bih.biHeight = _height;
	bih.biPlanes = 1;
	bih.biBitCount = 24;					// RGB encoded, 24 bit
	bih.biCompression = BI_RGB;			// no compression
	bih.biSizeImage = _width*_height * 3;
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
	std::string Path = "d:\\bmp\\";
	std::string name = "test";
	std::string ext = ".bmp";
	name += std::to_string(::GetTickCount()).c_str();
	Path.append(name);
	Path.append(ext);

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