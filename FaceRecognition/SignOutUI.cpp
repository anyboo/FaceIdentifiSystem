#include "stdafx.h"
#include "SignOutUI.h"
#include <vector>
#include "SettingConfig.h"
#include "Mmsystem.h"
#include "CameraUI.h"
#include <Poco/Data/Session.h>
#include <Poco/Data/SQLite/Connector.h>
#include <Poco/Data/SQLite/SQLiteException.h>
#include <Poco/File.h>
#include <Poco/FileStream.h>
#include <Poco/BinaryReader.h>
#include <Poco/Buffer.h>

#include "THFaceImage_i.h"
#include "THFeature_i.h"

#include <cv.h>  
#include <cxcore.h>  
#include <highgui.h>

using namespace cv;

using Poco::Buffer;
using Poco::File;
using Poco::FileInputStream;
using Poco::BinaryReader;
using Poco::Data::Session;
using Poco::Data::Statement;
using namespace Poco::Data::Keywords;

CSignOutUI::CSignOutUI()
:_watch_dog(200, 2000), 
_callback(*this,&CSignOutUI::onTimer),
_pCameraUI(0),
_StopWatchdog(false),
_id_from_detected(0)
{
	m_closeApp = true;
}


CSignOutUI::~CSignOutUI()
{
}


DUI_BEGIN_MESSAGE_MAP(CSignOutUI, WindowImplBase)
DUI_ON_CLICK_CTRNAME(BT_CLOSE_MatchWnd, OnCloseWnd)
DUI_ON_CLICK_CTRNAME(BT_SIGN_OUT, OnFilishMatch)
DUI_END_MESSAGE_MAP()

LPCTSTR CSignOutUI::GetWindowClassName() const
{
	return _T("CSignOutUI");
}

CDuiString CSignOutUI::GetSkinFolder()
{
	return _T("skin");
}

CDuiString CSignOutUI::GetSkinFile()
{
	return _T("xml\\SignOutUI.xml");
}

CControlUI* CSignOutUI::CreateControl(LPCTSTR pstrClass)
{
	if (_tcscmp(pstrClass, DUI_CTR_CAMERAUI) == 0)
	{
		_pCameraUI = new CameraUI;
		return _pCameraUI;
	}
		
	return NULL;
}

void CSignOutUI::InitWindow()
{
	//从数据库获取注册用户信息，人脸特征文件路径，后面要用
	Feather var;
	
	try
	{
		Session session("SQLite", "facerecog.db");
		Statement select(session);
		select << "SELECT id, facefeatherpath FROM UserInfo",
			into(var.id),
			into(var.path),
			range(0, 1);

		while (!select.done())
		{
			select.execute();

			File file(var.path);
			FileInputStream fin(var.path);
			size_t size = file.getSize();
			var.feather.resize(size);
			fin.read(&var.feather[0], size);
			_fgroup.push_back(var);
		}
	}
	catch (Poco::Data::DataException& e)
	{
		DUITRACE(e.displayText().c_str());
	}

	_watch_dog.start(_callback);
}

void CSignOutUI::OnFinalMessage(HWND hWnd)
{
	_watch_dog.stop();
	WindowImplBase::OnFinalMessage(hWnd);
}

void CSignOutUI::Notify(TNotifyUI& msg)
{

	WindowImplBase::Notify(msg);
}

void CSignOutUI::OnCloseWnd(TNotifyUI& msg)
{
	m_closeApp = false;
	Close();
}

void CSignOutUI::OnFilishMatch(TNotifyUI& msg)
{
	//发送数据到服务端
	update_db_onduty(_id_from_detected);
	m_closeApp = false;
	Close();
}

void CSignOutUI::ShowMatchInfo()
{
	CLabelUI* edit_name = dynamic_cast<CLabelUI*>(m_PaintManager.FindControl(_T("Edit_Name")));
	CLabelUI* edit_sex = dynamic_cast<CLabelUI*>(m_PaintManager.FindControl(_T("Edit_Sex")));
	CLabelUI* edit_birth = dynamic_cast<CLabelUI*>(m_PaintManager.FindControl(_T("Edit_Birth")));
	CLabelUI* edit_address = dynamic_cast<CLabelUI*>(m_PaintManager.FindControl(_T("Edit_Address")));
	CLabelUI* edit_phone = dynamic_cast<CLabelUI*>(m_PaintManager.FindControl(_T("Edit_Phone")));
	CLabelUI* edit_CertID = dynamic_cast<CLabelUI*>(m_PaintManager.FindControl(_T("Edit_IDnumber")));
	CLabelUI* edit_identify = dynamic_cast<CLabelUI*>(m_PaintManager.FindControl(_T("identify_id"))); 
	CLabelUI* edit_level = dynamic_cast<CLabelUI*>(m_PaintManager.FindControl(_T("Edit_grade")));
	CLabelUI* edit_issue_date = dynamic_cast<CLabelUI*>(m_PaintManager.FindControl(_T("Edit_time")));
	CButtonUI* button = dynamic_cast<CButtonUI*>(m_PaintManager.FindControl(_T("Sign_out")));
	button->SetEnabled(true);
	
	edit_name->SetText(_value_name.c_str());
	std::string sex;
	if (_value_sex)
		sex = LangueConfig::GetShowText(12);
	else
		sex = LangueConfig::GetShowText(13);

	edit_sex->SetText(sex.c_str());
	edit_birth->SetText(_value_birthday.c_str());
	edit_address->SetText(_value_address.c_str());
	edit_phone->SetText(_value_cellphone.c_str());
	edit_identify->SetText(_value_identify_code.c_str());
	edit_CertID->SetText(_value_license_code.c_str());
	edit_level->SetText(std::to_string(_value_level).c_str());
	edit_issue_date->SetText(_value_issue_date.c_str());

	_pCameraUI->SetVisible(false);

	CHorizontalLayoutUI* photo = dynamic_cast<CHorizontalLayoutUI*>(m_PaintManager.FindControl(_T("photo_for_empl")));
	photo->SetBkImage(_value_photo_path.c_str());
	PlaySoundA(_T("QT.wav"), NULL, SND_FILENAME | SND_ASYNC);
}

#include <Poco/Path.h>

void CSignOutUI::onTimer(Poco::Timer& timer)
{
	if (_StopWatchdog) return;
	//获取一张摄像头图片，判断是否有人脸，并提取人脸特征
	std::string path(Poco::Path::current().append("signout.jpg"));
	if (!_pCameraUI->ScreenSnapshot(path))
		return;

	Mat im1 = imread(path);
	if (im1.empty())
	{
		DUITRACE("imread");
	}

	THFI_FacePos ptfp[3];
	ptfp[0].dwReserved = (DWORD)new BYTE[512];

	int nFace = THFI_DetectFace(0, im1.data, 24, im1.cols, im1.rows, ptfp, 3);
	if (nFace <= 0)
	{
		DUITRACE("nFace <= 0");
		return;
	}

	//取最大的一张人脸位置图
	int nMaxIndex = 0;
	int nMaxSize = 0;
	RECT* pFace = new RECT[nFace];
	for (int i = 0; i <nFace; ++i)
	{
		pFace[i] = ptfp[i].rcFace;
		int w = pFace[i].right - pFace[i].left;
		if (w > nMaxSize)
		{
			nMaxSize = w;
			nMaxIndex = i;

		}
	}

	delete[]pFace;

	ptfp[nMaxIndex];
	
	int m_nFeatureSize = EF_Size();
	Buffer<BYTE> pFeature(m_nFeatureSize);
	if (EF_Extract(0, im1.data, im1.cols, im1.rows, 3, (DWORD)&ptfp[nMaxIndex], pFeature.begin()))
	{
		DUITRACE("EF_Extract");
		//与内存中的特征组逐一比对，成功则获取ID
		for (auto var : _fgroup)
		{
			DUITRACE("compare");
			double precent = EF_Compare(&pFeature[0], (BYTE*)&(var.feather[0])) * 100 + 20;
			
			if (precent > 70)
			{
				//从数据库通过id获取数据信息，上报注册用户信息到UI，同时停止比较
				std::stringstream ss;
				ss << precent;
				DUITRACE("id : %d ,precent : %s", var.id, ss.str().c_str());
				_StopWatchdog = true;
				_id_from_detected = var.id;
				popData2UI(_id_from_detected);
				break;
			}
		}
		
	}
}

void CSignOutUI::popData2UI(int id)
{
	try
	{
		{
			Session session("SQLite", "facerecog.db");
			Statement select(session);

			select << "SELECT name, sex, birthday, cellphone, address, photo_file_path, license_id, identify_id FROM UserInfo WHERE id = ?",
				into(_value_name),
				into(_value_sex),
				into(_value_birthday),
				into(_value_cellphone),
				into(_value_address),
				into(_value_photo_path),
				into(license_id),
				into(identify_id),
				use(id),
				now;
		}
		
		
		{
			Session session("SQLite", "facerecog.db");
			Statement select(session);

			select << "SELECT issue_date, level, code FROM license WHERE id = ?",
				into(_value_issue_date),
				into(_value_level),
				into(_value_license_code),
				use(license_id),
				now;
		}
		
		{
			Session session("SQLite", "facerecog.db");
			Statement select(session);

			select << "SELECT identify.code FROM identify WHERE id = ?",
				into(_value_identify_code),
				use(identify_id),
				now;
		}
		
		

		ShowMatchInfo();

	}
	catch (Poco::Data::DataException& e)
	{
		DUITRACE(e.displayText().c_str());
	}
}

void CSignOutUI::update_db_onduty(int id)
{
	//更新onduty的id一致的onduty字段状态
	try
	{
		Session session("SQLite", "facerecog.db");
		Statement update(session);
		update << "UPDATE duty SET onduty = 0 WHERE userid = ?",
			use(id),
			now;
	}
	catch (Poco::Data::DataException& e)
	{
		DUITRACE("update_db_onduty : %s", e.displayText().c_str());
	}
}
