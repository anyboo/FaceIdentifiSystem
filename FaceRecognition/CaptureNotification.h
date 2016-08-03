#pragma once

#include "Picture.h"
#include <Poco/Notification.h>

using Poco::Notification;

class CaptureNotification
	:public Poco::Notification
{
public:
	typedef Poco::AutoPtr<CaptureNotification> Ptr;

	CaptureNotification(Picture* data);
	~CaptureNotification();

	Picture* data();
private:
	Picture::Ptr _data;
};

