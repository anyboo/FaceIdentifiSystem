#pragma once
#include <Poco/Net/FTPClientSession.h>
#include <Poco/ActiveMethod.h>
#include <Poco/ActiveResult.h>

using Poco::ActiveMethod;
using Poco::ActiveResult;
using Poco::Net::FTPClientSession;

class ActiveUploader
{
public:
	ActiveUploader(FTPClientSession& session);
	~ActiveUploader();

	ActiveMethod<bool, std::string, ActiveUploader> upload;
protected:
	bool uploadImp(const std::string& file);
private:
	FTPClientSession& _session;
};

