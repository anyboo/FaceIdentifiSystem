#include "stdafx.h"
#include "Recorder.h"
#include "Camera.h"

Recorder::Recorder()
:t(100, 50), tc(*this, &Recorder::onTimer),
_camera(new Camera)
{
	_camera->Open();
	t.start(tc);
}


Recorder::~Recorder()
{
	_camera->Close();
}

void Recorder::onTimer(Poco::Timer& timer)
{
	_camera->GetFrame();
}
