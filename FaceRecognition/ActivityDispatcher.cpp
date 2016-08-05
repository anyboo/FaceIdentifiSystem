#include "stdafx.h"
#include "ActivityDispatcher.h"
#include <Poco/Thread.h>
#include "CaptureNotification.h"
#include "FaceMatch.h"
#include <Poco/ActiveResult.h>

using Poco::ActiveResult;
using Poco::Thread;
using Poco::FastMutex;
using Poco::Notification;

FastMutex ActivityDispatcher::_mutex;

ActivityDispatcher::ActivityDispatcher() :
_activity(this, &ActivityDispatcher::runActivity)
{
}

void ActivityDispatcher::start()
{
	_activity.start();
}

void ActivityDispatcher::stop()
{
	_activity.stop();
	enqueueNotification(new Notification);
	_activity.wait();
}

void ActivityDispatcher::runActivity()
{
	while (!_activity.isStopped())
	{
		Thread::sleep(50);
		//
		Notification::Ptr pNf(_queue.waitDequeueNotification());
		if (pNf)
		{
			CaptureNotification::Ptr pWorkNf = pNf.cast<CaptureNotification>();
			if (pWorkNf)
			{
				{
					FastMutex::ScopedLock lock(_mutex);
					Picture::Ptr pic = pWorkNf->data();

					FaceMatch example;
					FaceMatch::AddArgs args = { pic, pic };
					ActiveResult<bool> result = example.activeMatch(args);
					result.wait();
					bool ret = result.data();
					commitResult(ret);
					OutputDebugStringA((std::string("ActiveResult : ") + std::to_string(ret) + std::string("\n")).c_str());
				}
			}
		}
		else break;
	}
}

void ActivityDispatcher::enqueueNotification(Poco::Notification::Ptr pNotification)
{
	_queue.enqueueNotification(pNotification);
}

void ActivityDispatcher::commitResult(bool result)
{
	_results = result;
}

bool ActivityDispatcher::queryResult()
{
	return _results;
}