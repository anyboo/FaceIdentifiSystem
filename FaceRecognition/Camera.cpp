#include "stdafx.h"
#include "Camera.h"
#include "TiCapture2.h"
#include <Poco/Buffer.h>
#include <Poco/NotificationCenter.h>
#include "CaptureNotification.h"

using Poco::NotificationCenter;
using Poco::Buffer;

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

void Camera::GetFrame()
{
	poco_check_ptr(_camera);

	long len = _width* _height * 3;
	char* data = new char[len];
	
	bool ret = _camera->GetFrame((BYTE*)data, len);
	poco_assert(ret);

	Picture::Ptr pic(new Picture(data, len));
	poco_check_ptr(pic);
	pic->SetHeight(_height);
	pic->SetWidth(_width);
	NotificationCenter::defaultCenter().postNotification(new CaptureNotification(pic));
}