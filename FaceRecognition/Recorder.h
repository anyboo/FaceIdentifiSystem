#pragma once

#include <Poco/Timer.h>
#include <Poco/Stopwatch.h>
#include "Camera.h"

class Recorder
{
public:
	Recorder(ICamera::Ptr camera);
	~Recorder();

	void start();
	void stop();

protected:
	void onTimer(Poco::Timer& timer);
private:
	Poco::Stopwatch sw;
	Poco::Timer t;
	Poco::TimerCallback<Recorder> tc;
	ICamera::Ptr _camera;
	bool _running;
};

