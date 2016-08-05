#pragma once
#include <Poco/Activity.h>
#include <Poco/NotificationQueue.h>
#include <Poco/Notification.h>
#include <Poco/Mutex.h>
#include <set>

class ActivityDispatcher
{
public:
	ActivityDispatcher();

	void start();
	void stop();

	void enqueueNotification(Poco::Notification::Ptr pNotification);
	bool queryResult();
protected:
	void runActivity();
	void commitResult(bool result);
private:
	Poco::Activity<ActivityDispatcher> _activity;
	Poco::NotificationQueue _queue;
	static Poco::FastMutex  _mutex;
	bool _results;
};

