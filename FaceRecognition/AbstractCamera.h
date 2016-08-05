#pragma once
#include <Poco/Buffer.h>
#include "Picture.h"
#include <Poco/RefCountedObject.h>

class CCapture;
struct SCapQuality;

class AbstractCamera
	:public Poco::RefCountedObject
{
public:
	typedef Poco::AutoPtr<AbstractCamera> Ptr;

	virtual void Open() = 0;
	virtual void Close() = 0;
	virtual void GetFrame() = 0;
};

