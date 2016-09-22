#pragma once
#include <Poco/Activity.h>
#include <Poco/NotificationQueue.h>
#include <Poco/Notification.h>
#include <Poco/Mutex.h>
#include <list>

#include <Poco/Stopwatch.h>
#include <Poco/ActiveResult.h>

class ActivityDispatcher
{
public:
	ActivityDispatcher();

	void start();
	void stop();

	void enqueueNotification(Poco::Notification::Ptr pNotification);
	bool queryResult();
	int queryPerson();
protected:
	void runActivity();
	void commitResult(bool result);
	void PrepareChannel();
private:
	typedef Poco::ActiveResult<bool> Result;
	Poco::Activity<ActivityDispatcher> _activity;
	Poco::NotificationQueue _queue;
	static Poco::FastMutex  _mutex;
	bool _results;

	int _serial;

	std::list<Result> _resultSet;
	size_t _channelNum;

};

