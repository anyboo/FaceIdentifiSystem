#pragma once

#include "libssh/sftp.h"
#include "libssh/libssh.h"
#include "Poco/CppUnit/TestCase.h"

class SFTPClientTest :public CppUnit::TestCase
{
public:
	SFTPClientTest(const std::string& name);
	~SFTPClientTest();

	void testConnect();
	void testDisconnect();
	void testOpen();
	void testClose();
	void testUpload();
	void test();

	static CppUnit::Test* suite();
private:
	static ssh_session ssh;
	static sftp_session sftp;
};

