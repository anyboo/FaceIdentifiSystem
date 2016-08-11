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


					//压缩图像大小，提升人脸检测的速度					
					DWORD start, stop;
					start = GetTickCount();
					Mat srcMat(pic->height(), pic->width(), CV_8UC3, (char *)pic->data());
					cv::Mat dstMat;					
					cv::resize(srcMat, dstMat, cv::Size(srcMat.cols / 2, srcMat.rows / 2));	
					Picture::Ptr pic1(new Picture((const char *)dstMat.data, dstMat.total() * 3));
					pic1->SetWidth(320);
					pic1->SetHeight(240);
					stop = GetTickCount();
					poco_information_f1(logger_handle, "resize 1 tiem: %lu", stop - start);

					int i;
					vector<readUserInfo> userinfo = RegUserInfo::getUserInfo();
					for (i = 0; i < userinfo.size(); i++)
					{
						//压缩图像大小，提升人脸检测的速度
						Mat srcMat1(640, 480, CV_8UC3, (char *)userinfo[i].get<9>().rawContent());
						cv::Mat dstMat1;
						cv::resize(srcMat1, dstMat1, cv::Size(srcMat.cols / 2, srcMat.rows / 2));
						Picture::Ptr userpic(new Picture((const char *)dstMat1.data, dstMat1.total() * 3));
						userpic->SetWidth(320);
						userpic->SetHeight(240);						

						DWORD start, stop;
						start = GetTickCount();

						FaceMatch example;
						FaceMatch::AddArgs args = { pic1, userpic };
						ActiveResult<bool> result = example.activeMatch(args);
						result.wait();
						bool ret = result.data();
						stop = GetTickCount();
						poco_information_f2(logger_handle, "ret :%d, time: %lu", (int)ret, stop - start);
						std::stringstream ostr;
						ostr << "result:" << ret << std::endl;
						OutputDebugStringA(ostr.str().c_str());
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