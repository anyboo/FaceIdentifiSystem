#pragma once

#include "Picture.h"
#include <Poco/Notification.h>

class CaptureNotification
	:public Poco::Notification
{
public:

	CaptureNotification(const Picture::Ptr data);
	~CaptureNotification();
private:
	Picture::Ptr _data;
};

