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
, _channelNum(4)
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
	PrepareChannel();

	while (!_activity.isStopped())
	{
		Thread::sleep(50);
		//
		Notification::Ptr pNf(_queue.waitDequeueNotification());
		if (pNf)
		{
			/*
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

						FaceMatch::AddArgs args = { userpic };
						ActiveResult<bool> result = example.activeMatch(args);
						result.wait();
						bool ret = result.data();
						if (ret)
						{
							_serial = i;
						}
						t_end = GetTickCount();
						poco_information_f2(logger_handle, "compare result:%d, time:%d", (int)ret, (int)(t_end - t_start));
						std::stringstream ostr;
						ostr << "result:" << ret << std::endl;
						OutputDebugStringA(ostr.str().c_str());
						commitResult(ret);
					}					
					FaceMatch::AddArgs args = { pic };
					FaceMatch example;
					Result result = example.activeMatch(args);
					//result.wait();
					_resultSet.push_back(result);
				}
				
			}*/
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


int ActivityDispatcher::queryPerson()
{
	return _serial;
}

bool ActivityDispatcher::queryResult()
{
//	return false;

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

//int ActivityDispatcher::queryPerson()
//{
//	return _serial;
//}

