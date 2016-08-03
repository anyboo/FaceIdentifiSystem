#include "stdafx.h"
#include "CaptureNotification.h"

CaptureNotification::CaptureNotification(const Picture::Ptr data)
:_data(data)
{
}


CaptureNotification::~CaptureNotification()
{
}


Picture* CaptureNotification::data()
{
	poco_check_ptr(_data.get());
	return _data.get();
}