#include "stdafx.h"
#include "Recorder.h"
#include "Camera.h"

Recorder::Recorder(ICamera& camera)
:t(100, 50), tc(*this, &Recorder::onTimer),
_camera(camera), _running(false)
{
	
}

Recorder::~Recorder()
{
}

void Recorder::start()
{
	if (!_running)
	{
		_camera.Open();
		t.start(tc);
		sw.start();
		_running = true;
	}
}

void Recorder::stop()
{
	if (_running)
	{
		_camera.Close();
		sw.stop();
		sw.reset();
		_running = false;
	}
}

void Recorder::onTimer(Poco::Timer& timer)
{
	_camera.GetFrame();
	
	//FrameBuffer data(100);
	/*sw.elapsed();
	if (!bcompare)
		comparePhoto();*/
}
