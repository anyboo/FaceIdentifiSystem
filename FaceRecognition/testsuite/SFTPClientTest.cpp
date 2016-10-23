#include "SFTPClientTest.h"
#include <iostream>
#include <fcntl.h>
#include <Poco/CppUnit/TestSuite.h>
#include <Poco/CppUnit/TestCaller.h>

#define	S_IRWXU	0000700	

ssh_session SFTPClientTest::ssh = nullptr;
sftp_session SFTPClientTest::sftp = nullptr;

SFTPClientTest::SFTPClientTest(const std::string& name)
	:CppUnit::TestCase(name)
{
}


SFTPClientTest::~SFTPClientTest()
{
}

void SFTPClientTest::testConnect()
{
	ssh = ssh_new();
	assertNotNullPtr(ssh);
	ssh_options_set(ssh, SSH_OPTIONS_HOST, "www.sisec.com.cn");

	long timeout = 1000;
	ssh_options_set(ssh, SSH_OPTIONS_TIMEOUT, &timeout);
	ssh_options_set(ssh, SSH_OPTIONS_SSH_DIR, "/home/sisec_alert");

	assertEqual(ssh_connect(ssh), SSH_OK);

	assertEqual(ssh_userauth_password(ssh, "sisec_alert", "alert"), SSH_AUTH_SUCCESS);
}

void SFTPClientTest::testDisconnect()
{
	if (ssh)
	{
		ssh_disconnect(ssh);
		ssh_free(ssh);
	}
}

void SFTPClientTest::testOpen()
{
	sftp = sftp_new(ssh);
	assertNotNullPtr(sftp);
	assertEqual(sftp_init(sftp), SSH_OK);
}

void SFTPClientTest::testUpload()
{
	// execute pwd
	int access_type = O_WRONLY | O_CREAT | O_TRUNC;
	sftp_file file;
	const char *helloworld = "Hello, World!\n";
	int length = strlen(helloworld);
	int rc, nwritten;
	//std::string local("D:/workstation/code/GitHub/FaceIdentifiSystem/bin/photo/20161020114211.jpg");
	std::string remote("/home/sisec_alert/alert_photo/20161020114211.jpg");
	file = sftp_open(sftp, remote.c_str(), access_type, S_IRWXU);
	assertNotNullPtr(file);
	if (file == NULL)
	{
		std::cerr << "cerr - Can't open file for writing: " << ssh_get_error(ssh);
		fprintf(stderr, "Can't open file for writing: %s\n",
			ssh_get_error(ssh));
	}
	nwritten = sftp_write(file, helloworld, length);
	if (nwritten != length)
	{
		fprintf(stderr, "Can't write data to file: %s\n",
			ssh_get_error(ssh));
		sftp_close(file);
	}
	rc = sftp_close(file);
	assertEqual(rc, SSH_OK);
	if (rc != SSH_OK)
	{
		fprintf(stderr, "Can't close the written file: %s\n",
			ssh_get_error(ssh));
	}
}

void SFTPClientTest::testClose()
{
	if (sftp)
	{
		sftp_free(sftp);
	}
}

#include "psftp.h"

void SFTPClientTest::test()
{
	std::cout << "start" << std::endl;
}

CppUnit::Test* SFTPClientTest::suite()
{
	CppUnit::TestSuite* pSuite = new CppUnit::TestSuite("SFTPClientTest");

	CppUnit_addTest(pSuite, SFTPClientTest, test);
	CppUnit_addTest(pSuite, SFTPClientTest, testConnect);
	CppUnit_addTest(pSuite, SFTPClientTest, testOpen);
	CppUnit_addTest(pSuite, SFTPClientTest, testUpload);
	CppUnit_addTest(pSuite, SFTPClientTest, testClose);
	CppUnit_addTest(pSuite, SFTPClientTest, testDisconnect);

	return pSuite;
}