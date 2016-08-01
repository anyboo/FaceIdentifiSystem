#pragma once

#include <Poco/Timer.h>

class ICamera;

class Recorder
{
public:
	Recorder(ICamera& camera);
	~Recorder();

	void start();
	void stop();

protected:
	void onTimer(Poco::Timer& timer);
private:
	Poco::Timer t;
	Poco::TimerCallback<Recorder> tc;
	ICamera& _camera;
};

