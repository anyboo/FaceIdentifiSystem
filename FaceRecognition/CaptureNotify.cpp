#include "stdafx.h"
#include "CaptureNotify.h"

using Poco::NotificationCenter;

CaptureNotify::CaptureNotify()
:center(NotificationCenter::defaultCenter())
{
}


CaptureNotify::~CaptureNotify()
{
}
