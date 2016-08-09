#include "stdafx.h"
#include "ActivityDispatcher.h"
#include <Poco/Thread.h>
#include "CaptureNotification.h"
#include "FaceMatch.h"
#include <Poco/ActiveResult.h>
#include "RegUserInfo.h"
#include "log.h"

using Poco::ActiveResult;
using Poco::Thread;
using Poco::FastMutex;
using Poco::Notification;

FastMutex ActivityDispatcher::_mutex;

ActivityDispatcher::ActivityDispatcher() :
_activity(this, &ActivityDispatcher::runActivity), _results(false)
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

#include <Poco/Stopwatch.h>

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
					int i;
					vector<readUserInfo> userinfo = RegUserInfo::getUserInfo();
					for (i = 0; i < userinfo.size(); i++)
					{
						long t_start, t_end;
						t_start = GetTickCount();
						Picture::Ptr userpic(new Picture(userinfo[i].get<9>().rawContent(), pic->width() * pic->height() * 3));
						userpic->SetWidth(pic->width());
						userpic->SetHeight(pic->height());
						FaceMatch example;

						FaceMatch::AddArgs args = { pic, userpic };
						ActiveResult<bool> result = example.activeMatch(args);
						result.wait();
						bool ret = result.data();
						if (ret)
						{
							_serial = i;
						}
						t_end = GetTickCount();
						poco_information_f2(logger_handle, "compare result:%d, time:%d", (int)ret, (int)(t_end - t_start));
						commitResult(ret);
					}					
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

int ActivityDispatcher::queryPerson()
{
	return _serial;
}