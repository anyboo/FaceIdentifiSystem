#include "stdafx.h"
#include "AppInitialize.h"
#include "THFaceImage_i.h"
#include "THFeature_i.h"
#include "QMFileSqlite.h"
#include "RegUserInfo.h"
#include "log.h"
/*
#include <windows.h>
#include <objbase.h>
#include <shellapi.h>
*/


CAppInitialize::CAppInitialize()
{
	AttatchSDK();
	
	PrepareCamera();
	PrepareRegisteredFace();

	LoadUserConfig();
	//Show_HideTask(true);
}

CAppInitialize::~CAppInitialize()
{
	DetachedSDK();
	//Show_HideTask(true);
}

void CAppInitialize::AttatchSDK()
{
	THFaceImageSDK();
	TiCapture2SDK();
}

void CAppInitialize::DetachedSDK()
{
	THFI_Release();
	EF_Release();
}

void CAppInitialize::THFaceImageSDK()
{
	//init face
	THFI_Param param;
	param.nMinFaceSize = 150;
	param.nRollAngle = 145;
	param.bOnlyDetect = true;

	THFI_Create(1, &param);

	short ret = EF_Init(1);
	if (ret == 1)
	{
		cout << "Feature init ok" << endl;
	}
}

void CAppInitialize::TiCapture2SDK()
{

}

void CAppInitialize::PrepareCamera()
{

}

void CAppInitialize::PrepareRegisteredFace()
{
	//init database
	QFileSqlite *pDb = QFileSqlite::getInstance();
	pDb->createTable(CREATE_USER_INFO_TABLE);
	RegUserInfo::init();
}

void CAppInitialize::LoadUserConfig()
{

}

/*
void Show_HideTask(bool IsHide)
{
	int nCwdShow = -1;
	LPARAM lParam;
	HWND task = FindWindow(_T("Shell_TrayWnd"), NULL);
	if (IsHide)
	{
		lParam = ABS_AUTOHIDE | ABS_ALWAYSONTOP;
		nCwdShow = SW_HIDE;
	}
	else
	{
		lParam = ABS_ALWAYSONTOP;
		nCwdShow = SW_SHOW;
	}

	::ShowWindow(task, nCwdShow);

	APPBARDATA apBar;
	memset(&apBar, 0, sizeof(apBar));
	apBar.cbSize = sizeof(apBar);
	apBar.hWnd = task;
	if (apBar.hWnd != NULL)
	{
		apBar.lParam = lParam;
		SHAppBarMessage(ABM_SETSTATE, &apBar);
	}
}
*/


