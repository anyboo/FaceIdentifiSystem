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
}

CAppInitialize::~CAppInitialize()
{
	DetachedSDK();
}

void CAppInitialize::AttatchSDK()
{
	//THFaceImageSDK();
	//TiCapture2SDK();
}

void CAppInitialize::DetachedSDK()
{
	//THFI_Release();
	//EF_Release();
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
	/*QFileSqlite *pDb = QFileSqlite::getInstance();
	pDb->createTable(CREATE_USER_INFO_TABLE);
	RegUserInfo::init();*/
}

void CAppInitialize::LoadUserConfig()
{

}


