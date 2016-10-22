// App.cpp : Defines the entry point for the application.
//
#include "stdafx.h"
#include "resource.h"
#include "MainWnd.h"


#include <windows.h>
#include <objbase.h>
#include <shellapi.h>
#include "QMFileSqlite.h"
#include "RegUserInfo.h"
#include "log.h"
#include "SettingConfig.h"

#include "THFaceImage_i.h"
#include "THFeature_i.h"

#include "AppInitialize.h"


int APIENTRY WinMain(HINSTANCE hInstance, HINSTANCE /*hPrevInstance*/, LPSTR /*lpCmdLine*/, int nCmdShow)
{
	CPaintManagerUI::SetInstance(hInstance);
	CPaintManagerUI::SetResourcePath(CPaintManagerUI::GetInstancePath() + _T("skin"));

	HRESULT Hr = ::CoInitialize(NULL);
	if (FAILED(Hr)) return 0;

	Loggering::Logger_initiation();

	THFI_Param param;
	ValueSetting set;
	param.nMinFaceSize = std::stoi(set.GetFaceSize());
	param.nRollAngle = 145;
	param.bOnlyDetect = true;
	THFI_Create(1, &param);

	short ret = EF_Init(1);
	if (ret == 1)
	{
		cout << "Feature init ok" << endl;
	}


	//init database
	QFileSqlite *pDb = QFileSqlite::getInstance();
	pDb->createTable(CREATE_USER_INFO_TABLE);
	
	RegUserInfo::init();

	CAppInitialize  initial;
	
	std::auto_ptr<CMainWnd> pFrame(new CMainWnd);
	assert(pFrame.get());
	pFrame->Create(NULL, NULL, UI_WNDSTYLE_DIALOG, WS_EX_WINDOWEDGE | WS_EX_ACCEPTFILES);
	pFrame->SetIcon(IDI_ICON1);
	pFrame->CenterWindow();
	pFrame->ShowWindow(true);

	CPaintManagerUI::MessageLoop();

	THFI_Release();
	EF_Release();


	::CoUninitialize();

	return 0;
}
