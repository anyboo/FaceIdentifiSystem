#pragma once

#include <Poco/Timer.h>
#include <Poco/Stopwatch.h>

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
	Poco::Stopwatch sw;
	Poco::Timer t;
	Poco::TimerCallback<Recorder> tc;
	ICamera& _camera;
	bool _running;
};

