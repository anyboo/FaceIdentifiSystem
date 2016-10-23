#pragma once

#include "Poco/CppUnit/TestCase.h"

class psftpClientTest :public CppUnit::TestCase
{
public:
	psftpClientTest(const std::string& name);
	~psftpClientTest();

	void testConnect();
	void testDisconnect();
	void testOpen();
	void testClose();
	void testUpload();
	void test();

	static CppUnit::Test* suite();
private:
	
};

