#pragma once
#include <Poco/Buffer.h>
#include "Picture.h"
#include <Poco/RefCountedObject.h>

class CCapture;
struct SCapQuality;

class ICamera
	:public Poco::RefCountedObject
{
public:
	typedef Poco::AutoPtr<ICamera> Ptr;
	virtual void Open() = 0;
	virtual void Close() = 0;
	virtual void GetFrame() = 0;
};

class Camera 
	: public ICamera
{
public:

	Camera();
	~Camera();

	virtual void Open();
	virtual void Close();
	virtual void GetFrame();
	virtual void Prepare();

	size_t width() const;
	size_t height() const;

protected:
	void SetQuality(SCapQuality* pQuality);
private:
	CCapture* _camera;
	size_t _width;
	size_t _height;
	bool isRunning;
};

