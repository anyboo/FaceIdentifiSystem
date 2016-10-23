#include "SceneTestSuite.h"
#include "SFTPClientTest.h"
#include "psftpClientTest.h"

CppUnit::Test* SceneTestSuite::suite()
{
	CppUnit::TestSuite* pSuite = new CppUnit::TestSuite("SceneTestSuite");

	pSuite->addTest(psftpClientTest::suite());

	return pSuite;
}
