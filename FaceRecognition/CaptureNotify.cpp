#include "stdafx.h"
#include "CaptureNotification.h"
#include "CaptureNotify.h"
#include <Poco/AutoPtr.h>
#include <Poco/Observer.h>
#include <Poco/NotificationCenter.h>

using Poco::AutoPtr;
using Poco::Observer;
using Poco::Notification;
using Poco::NotificationCenter;

CaptureNotify::CaptureNotify()
:nc(NotificationCenter::defaultCenter())
{
}


CaptureNotify::~CaptureNotify()
{
}

void CaptureNotify::handle1(Poco::Notification* pNf)
{
}

void CaptureNotify::handle1(Poco::Notification* pNf, Picture **pImg)
{
}
