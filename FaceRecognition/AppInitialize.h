#pragma once
#include "ActivityCommit.h"
class CAppInitialize
{
public:
	CAppInitialize();
	~CAppInitialize();

protected:
	void TiCapture2SDK();
	void THFaceImageSDK();
	void AttatchSDK();
	void DetachedSDK();
	void LaunchMonitorServer();

private:
	ActivityCommit _commit;
};

