#pragma once
#include <Poco/Buffer.h>
#include "Picture.h"

class CCapture;
struct SCapQuality;

class ICamera
{
public:
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

	size_t width() const;
	size_t height() const;

protected:
	void SetQuality(SCapQuality* pQuality);
	void SaveBmp(char* pDataBuf);
private:
	CCapture* _camera;
	size_t _width;
	size_t _height;
	bool isRunning;
};

