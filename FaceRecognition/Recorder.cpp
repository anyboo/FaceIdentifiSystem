#include "stdafx.h"
#include "Recorder.h"
#include "Camera.h"

Recorder::Recorder(ICamera::Ptr camera)
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
		poco_check_ptr(_camera.get());
		_camera->Open();
		t.start(tc);
		sw.start();
		_running = true;
	}
}

void Recorder::stop()
{
	if (_running)
	{
		poco_check_ptr(_camera.get());
		_camera->Close();
		sw.stop();
		sw.reset();
		_running = false;
	}
}

void Recorder::onTimer(Poco::Timer& timer)
{
	poco_check_ptr(_camera.get());
	_camera->GetFrame();
	
	//FrameBuffer data(100);
	/*sw.elapsed();
	if (!bcompare)
		comparePhoto();*/
}
