#include "stdafx.h"
#include "AppInitialize.h"
#include "THFaceImage_i.h"
#include "THFeature_i.h"
#include <Poco/Exception.h>
#include <Poco/Process.h>

using Poco::Process;
using Poco::ProcessHandle;

CAppInitialize::CAppInitialize()
{
	HRESULT Hr = ::CoInitialize(NULL);
	if (FAILED(Hr))
		throw Poco::Exception("CoInitialize is failed!");

	AttatchSDK();
	_commit.start();
	LaunchMonitorServer();
}

CAppInitialize::~CAppInitialize()
{
	DetachedSDK();
	_commit.stop();
	::CoUninitialize();
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