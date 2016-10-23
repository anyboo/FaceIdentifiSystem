#include "psftpClientTest.h"
#include "psftp.h"
#include <Poco/CppUnit/TestSuite.h>
#include <Poco/CppUnit/TestCaller.h>

psftpClientTest::psftpClientTest(const std::string& name)
	:CppUnit::TestCase(name)
{
}


psftpClientTest::~psftpClientTest()
{
}

void psftpClientTest::testConnect()
{
	psftp f;
	f.disconnect();
}

void psftpClientTest::testDisconnect()
{

}

void psftpClientTest::testOpen()
{

}

void psftpClientTest::testUpload()
{

}

void psftpClientTest::testClose()
{

}

void psftpClientTest::test()
{
	std::cout << "start" << std::endl;
}

CppUnit::Test* psftpClientTest::suite()
{
	CppUnit::TestSuite* pSuite = new CppUnit::TestSuite("psftpClientTest");

	CppUnit_addTest(pSuite, psftpClientTest, test);
	CppUnit_addTest(pSuite, psftpClientTest, testConnect);
	CppUnit_addTest(pSuite, psftpClientTest, testOpen);
	CppUnit_addTest(pSuite, psftpClientTest, testUpload);
	CppUnit_addTest(pSuite, psftpClientTest, testClose);
	CppUnit_addTest(pSuite, psftpClientTest, testDisconnect);

	return pSuite;
}