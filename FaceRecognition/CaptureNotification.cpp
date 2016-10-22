#include "stdafx.h"
#include "CaptureNotification.h"

CaptureNotification::CaptureNotification(Picture& data)
:_data(data)
{
}

CaptureNotification::~CaptureNotification()
{
}


Picture& CaptureNotification::data()
{
	return  _data;
}