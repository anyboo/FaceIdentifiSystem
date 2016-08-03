#include "stdafx.h"
#include "CaptureNotification.h"

CaptureNotification::CaptureNotification(Picture* data)
:_data(data,true)
{
}


CaptureNotification::~CaptureNotification()
{
}


Picture* CaptureNotification::data()
{
	poco_check_ptr(_data.get());
	return  _data.duplicate();
}