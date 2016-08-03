#pragma once

#include <Poco/NotificationCenter.h>

class CaptureNotify
{
public:
	CaptureNotify();
	virtual ~CaptureNotify();

private:
	Poco::NotificationCenter &center;
};

