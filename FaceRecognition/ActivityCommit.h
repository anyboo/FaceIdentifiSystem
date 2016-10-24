#pragma once
#include <Poco/Activity.h>
#include <Poco/Thread.h>
#include <Poco/Net/FTPClientSession.h>

using Poco::Activity;
using Poco::Thread;
using Poco::Net::FTPClientSession;

struct alert_table;

class ActivityCommit
{
public:
	ActivityCommit();
	~ActivityCommit();

	void start();
	void stop();
protected:
	void runActivity();
	void post_alert_data(alert_table& alert);
private:
	Activity<ActivityCommit> _activity;
};

