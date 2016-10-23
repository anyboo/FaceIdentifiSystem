#include "psftp.h"
#include <Poco/PipeStream.h>
#include <cassert>

using Poco::Process;
using Poco::ProcessHandle;
using Poco::Pipe;
using Poco::PipeInputStream;
using Poco::PipeOutputStream;

psftp::psftp():
_exec_name("psftp"),
_pph(0)
{
	connect();
}

psftp::~psftp()
{

}

void psftp::connect()
{
	std::string cmd("psftp -l sisec_alert -pw alert www.sisec.com.cn -v -batch");
	std::vector<std::string> args;
	/*args.push_back("-l sisec_alert");
	args.push_back("-pw alert");
	args.push_back("www.sisec.com.cn");
	args.push_back("-v");
	args.push_back("-batch");*/
	
	ProcessHandle ph = Process::launch(cmd, args, 0, &_outPipe, 0);
	std::cout << "id:" << ph.id();
	assert(Process::isRunning(ph));

	PipeInputStream istr(_outPipe);
	std::string s;
	int c = istr.get();
	while (c != -1) { s += (char)c; c = istr.get(); }
	//assert(!s.empty());
	std::cout <<"result :" << s << std::endl;

	/*PipeOutputStream ostr(_inPipe);
	ostr << "quit";
	ostr.close();
	std::string s2;
	int c2 = istr.get();
	while (c2 != -1) { 
		s2 += (char)c2;
		c2 = istr.get(); 
	}*/
	//printf("result : %s\n",s2);
}

void psftp::disconnect()
{
	if (_pph)
	{
		PipeOutputStream ostr(_inPipe);
		ostr << "quit";
		ostr.close();

		int rc = _pph->wait();
		_pph = 0;
	}
	
}

void psftp::upload(const std::string filname)
{
	if (_pph)
	{
		PipeOutputStream ostr(_inPipe);
		ostr << "cd /home/sisec_alert/sisec_photo";
		ostr.flush();
		ostr << "put D:\\workstation\\code\\GitHub\\FaceIdentifiSystem\\FaceRecognition\\testsuite\\psftpClientTest.cpp";
		ostr.flush();
		ostr << "ls";
		ostr.flush();
	}
}