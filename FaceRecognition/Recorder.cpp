#include "stdafx.h"
#include "Recorder.h"
#include "Camera.h"

Recorder::Recorder(ICamera& camera)
:t(100, 50), tc(*this, &Recorder::onTimer),
_camera(camera)
{
	
}

Recorder::~Recorder()
{
}

void Recorder::start()
{
	_camera.Open();
	t.start(tc);
}

void Recorder::stop()
{
	_camera.Close();
}

void Recorder::onTimer(Poco::Timer& timer)
{
	_camera.GetFrame();
}
