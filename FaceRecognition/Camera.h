#pragma once
#include <Poco/RefCountedObject.h>

class CCapture;
struct SCapQuality;

class Camera 
	: public Poco::RefCountedObject
{
public:
	Camera();
	~Camera();

	void Open();
	void Close();
	void SetQuality(SCapQuality* pQuality);
	void GetFrame();

private:
	CCapture* _camera;
	size_t _width;
	size_t _height;
};

