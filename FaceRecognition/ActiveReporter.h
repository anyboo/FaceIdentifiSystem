#pragma once
#include <Poco/Net/HTTPClientSession.h>
#include <Poco/ActiveMethod.h>
#include <Poco/ActiveResult.h>

using Poco::ActiveMethod;
using Poco::ActiveResult;
using Poco::Net::HTTPClientSession;

class ActiveReporter
{
public:
	ActiveReporter(HTTPClientSession& session);
	~ActiveReporter();

	ActiveMethod<std::string, std::string, ActiveReporter> report;

protected:
	std::string reportImp(const std::string& json);

private:
	HTTPClientSession& _session;
};

