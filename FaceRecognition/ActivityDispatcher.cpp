#include "stdafx.h"
#include "ActivityDispatcher.h"
#include <Poco/Thread.h>
#include "CaptureNotification.h"
#include "FaceMatch.h"
#include <Poco/ActiveResult.h>
#include "RegUserInfo.h"

using Poco::ActiveResult;
using Poco::Thread;
using Poco::FastMutex;
using Poco::Notification;

FastMutex ActivityDispatcher::_mutex;

ActivityDispatcher::ActivityDispatcher(size_t channelNum) :
_activity(this, &ActivityDispatcher::runActivity), _results(false)
, _channelNum(channelNum)
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
	PrepareChannel();

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
					FaceMatch::AddArgs args = { pic };
					FaceMatch example;
					Result result = example.activeMatch(args);
					//result.wait();
					_resultSet.push_back(result);
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
	return false;

	Poco::Stopwatch sw;
	sw.start();
	for (auto var : _resultSet)
	{
		if (var.available())
		{
			_resultSet.pop_back();
			if (var.data())
				return true;
		}
		else
			var.wait(10);
	}

	sw.stop();
	std::stringstream ss;
	ss << "queryResult time" << sw.elapsed() << std::endl;
	OutputDebugStringA(ss.str().c_str());
	return false;
}

#include "THFaceImage_i.h"
#include "THFeature_i.h"

void ActivityDispatcher::PrepareChannel()
{
	//init face
	THFI_Param param;
	param.nMinFaceSize = 150;
	param.nRollAngle = 145;
	param.bOnlyDetect = true;

	THFI_Create(_channelNum, &param);

	short ret = EF_Init(_channelNum);
	if (ret <= 0)
	{
		throw Poco::IOException("THFaceImageSDK Failed!", ret);
	}
}
