#include "stdafx.h"
#include "AppInitialize.h"
#include "THFaceImage_i.h"
#include "THFeature_i.h"
#include <Poco/Exception.h>
#include <Poco/Process.h>
#include <Poco/Logger.h>
#include <Poco/AutoPtr.h>
#include <Poco/PatternFormatter.h>
#include <Poco/FormattingChannel.h>
#include <Poco/FileChannel.h>
#include <Poco/Message.h>
#include <Poco/Path.h>

using Poco::Path;
using Poco::Logger;
using Poco::AutoPtr;
using Poco::PatternFormatter;
using Poco::FormattingChannel;
using Poco::FileChannel;
using Poco::Message;

using Poco::Process;
using Poco::ProcessHandle;

CAppInitialize::CAppInitialize()
{
	HRESULT Hr = ::CoInitialize(NULL);
	if (FAILED(Hr))
		throw Poco::Exception("CoInitialize is failed!");
	SetupLogger("log\\facerecognition.log");
#ifndef _DEBUG
	AttatchSDK();
#endif
	_commit.start();
	LaunchMonitorServer();
	
}

CAppInitialize::~CAppInitialize()
{
#ifndef _DEBUG
	DetachedSDK();
#endif
	_commit.stop();
	ShutdownLogger();
	::CoUninitialize();
}

void CAppInitialize::SetupLogger(const std::string& path)
{
	Path::current().append(path);
	AutoPtr<PatternFormatter> pPatternFormatter2(new PatternFormatter("%Y-%m-%d %H:%M:%S.%c - %s:%q:%t"));
	AutoPtr<FormattingChannel> pFCFile(new FormattingChannel(pPatternFormatter2));
	AutoPtr<FileChannel> pFileChannel(new FileChannel(path));
	pFileChannel->setProperty("rotation", "10M");
	pFileChannel->setProperty("archive", "timestamp");
	pFCFile->setChannel(pFileChannel);
	pFCFile->open();

	Logger::create("FileLogger", pFCFile, Message::PRIO_INFORMATION);
}

void CAppInitialize::ShutdownLogger()
{
	Logger::shutdown();
}

void CAppInitialize::LaunchMonitorServer()
{
	//启动后台服务进程
	std::string command("faceMonitorServer.exe");
	Process::Args args;
#ifndef _DEBUG
	ProcessHandle ph = Process::launch("faceMonitorServer.exe", args);
	poco_assert(Process::isRunning(ph));
	DUITRACE("faceMonitorServer pid : %d ", ph.id());
#endif
}

void CAppInitialize::AttatchSDK()
{
	THFaceImageSDK();
}

void CAppInitialize::DetachedSDK()
{
	THFI_Release();
	EF_Release();
}

void CAppInitialize::THFaceImageSDK()
{
	THFI_Param param;
	param.nMinFaceSize = 150;
	param.nRollAngle = 145;
	param.bOnlyDetect = true;

	THFI_Create(1, &param);

	short ret = EF_Init(1);
	if (ret == 1)
	{
		DUITRACE("Feature init ok");
	}
}