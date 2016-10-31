#pragma once
#include <Poco/Activity.h>
#include <Poco/Thread.h>
#include <Poco/Net/FTPClientSession.h>
#include <Poco/Data/Session.h>

using Poco::Activity;
using Poco::Thread;
using Poco::Net::FTPClientSession;
using Poco::Data::Session;

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
	void update_alert_upload_status(int uid);
	bool check_network_status();
private:
	Activity<ActivityCommit> _activity;
	Session _dbSession;
	std::string _server_addr;
};

