#include "stdafx.h"
#include "ActivityCommit.h"
#include "ActiveReporter.h"
#include "ActiveUploader.h"
#include "Poco/Data/Session.h"
#include "Poco/Data/SQLite/Connector.h"
#include "document.h" 
#include "prettywriter.h"
#include "stringbuffer.h"

using namespace rapidjson;
using Poco::Data::Session;
using Poco::Data::Statement;

using namespace Poco::Data::Keywords;

struct alert_table
{
	int id;
	std::string filepath;
	int type;
	std::string timestamp;
	int userid;
	int deviceid;
};

ActivityCommit::ActivityCommit():
_activity(this, &ActivityCommit::runActivity)
{
}


ActivityCommit::~ActivityCommit()
{
}

void ActivityCommit::start()
{
	_activity.start();
}

void ActivityCommit::stop()
{
	_activity.stop();
	_activity.wait();
//	_ftp.close();
}

void ActivityCommit::runActivity()
{
	while (!_activity.isStopped())
	{
		Session session("SQLite", "D:\\workstation\\code\\GitHub\\FaceIdentifiSystem\\bin\\facerecog.db");
		Statement select(session);
		alert_table alert;
		select << "SELECT id, filepath, type, timestamp, UserInfoID, DeviceID FROM alert WHERE hasUpload = 0",
			into(alert.id),
			into(alert.filepath),
			into(alert.type),
			into(alert.timestamp),
			into(alert.userid),
			into(alert.deviceid),
			range(1, 1);

		while (!select.done())
		{
			try 
			{ 
				select.execute(); 
				FTPClientSession _ftp("202.103.191.73", FTPClientSession::FTP_PORT, "ftpalert", "1");
				ActiveUploader ur(_ftp);
				ActiveResult<bool> result = ur.upload(alert.filepath);
				post_alert_data(alert);

				Statement update(session);
				update << "UPDATE alert SET hasUpload = 1 WHERE id = ?",
					use(alert.id),
					now;

				result.wait();
				_ftp.close();
			}
			catch (Poco::Exception& e)
			{
				DUITRACE(e.displayText().c_str());
			}
		}

		Thread::sleep(2000);
	}
}

#include <Poco/Path.h>
using Poco::Path;

void ActivityCommit::post_alert_data(alert_table& alert)
{

	HTTPClientSession session("202.103.191.73");
	ActiveReporter rp(session);

	/*
	std::string body("body={\"api\":\"wn_add\", \"filepath\":\"http://202.103.191.73/api/3870861421055882.jpg\",\"type\":1,\"timestamp\":\"20161019221808\",\"UserInfoID\":1,\"DeviceID\":1,\"summary\":\"test\"}");
	request.setContentLength((int)body.length());
	*/
	std::string server_dir("http://202.103.191.73/var/www/sisec_website/photos/");
	Path p(alert.filepath);
	server_dir += p.getFileName();

	Document d;
	d.SetObject();
	d.AddMember("api", "wn_add", d.GetAllocator());
	d.AddMember("filepath", StringRef(server_dir.c_str()), d.GetAllocator());
	d.AddMember("type", alert.type, d.GetAllocator());
	d.AddMember("timestamp", StringRef(alert.timestamp.c_str()), d.GetAllocator());
	d.AddMember("UserInfoID", alert.userid, d.GetAllocator());
	d.AddMember("DeviceID", alert.deviceid, d.GetAllocator());
	d.AddMember("summary", "test", d.GetAllocator());
	StringBuffer sb;
	Writer<StringBuffer> writer(sb);
	d.Accept(writer);

	ActiveResult<std::string> result = rp.report(sb.GetString());
	result.wait();
	std::string received = result.data();
	DUITRACE("HTTP RECEIVED : %s", received);
}