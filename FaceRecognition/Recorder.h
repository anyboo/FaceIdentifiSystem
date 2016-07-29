#pragma once

#include <Poco/Timer.h>
#include <Poco/AutoPtr.h>

class Camera;
class Recorder
{
public:
	Recorder();
	~Recorder();
protected:
	void onTimer(Poco::Timer& timer);
private:
	Poco::Timer t;
	Poco::TimerCallback<Recorder> tc;
	Poco::AutoPtr<Camera> _camera;
};

