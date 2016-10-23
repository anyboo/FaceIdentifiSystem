#pragma once
#include <string>
#include <Poco/Pipe.h>
#include <Poco/Process.h>

class psftp
{
public:
	psftp();
	~psftp();

	void connect();
	void disconnect();
	void upload(const std::string filname);
private:
	std::string _exec_name;
	Poco::ProcessHandle* _pph;
	Poco::Pipe _inPipe;
	Poco::Pipe _outPipe;
};

