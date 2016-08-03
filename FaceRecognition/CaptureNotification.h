#pragma once

#include "Picture.h"
#include <Poco/Notification.h>

using Poco::Notification;

class CaptureNotification
	:public Poco::Notification
{
public:

	CaptureNotification(const Picture::Ptr data);
	~CaptureNotification();

	Picture* data();
private:
	Picture::Ptr _data;
};

