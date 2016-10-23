#include "stdafx.h"
#include "RegisterUI.h"
#include "CaptureNotification.h"
#include "Camera.h"
#include "Util.h"
#include "SettingConfig.h"
#include "ClipUI.h"


#include "Mmsystem.h"

#include "Employee.h"

#include "Poco/Data/Session.h"
#include "Poco/Data/SQLite/Connector.h"
#include <Poco/Data/SQLite/SQLiteException.h>

using Poco::Data::Session;
using Poco::Data::Statement;
using namespace Poco::Data::Keywords;


RegisterUI::RegisterUI()
:m_photo_agin(false)
, r(new Camera)
, bAlreadyTaked(false),
_license_id(0),
_identify_id(0),
_photo_id(0),
_unit_id(0)
{
	m_closeApp = true;
}

RegisterUI::~RegisterUI()
{

}

DUI_BEGIN_MESSAGE_MAP(RegisterUI, WindowImplBase)

DUI_ON_CLICK_CTRNAME(BT_CLOSERWND, OnCloseRWnd)
//DUI_ON_CLICK_CTRNAME(BT_GETPHOTO, OnGetPhoto)
//DUI_ON_CLICK_CTRNAME(BT_OK_REGISTER, OnRegister)

DUI_ON_CLICK_CTRNAME(BT_CLOSERWND, Backward)
DUI_ON_CLICK_CTRNAME(BT_GETPHOTO, TakePhoto)
DUI_ON_CLICK_CTRNAME(BT_OK_REGISTER, SignUp)

DUI_END_MESSAGE_MAP()

LPCTSTR RegisterUI::GetWindowClassName() const
{
	return _T("RegisterUI");
}

CDuiString RegisterUI::GetSkinFolder()
{
	return _T("skin");
}

CDuiString RegisterUI::GetSkinFile()
{
	return _T("xml\\RegisterUI.xml");
}

CControlUI* RegisterUI::CreateControl(LPCTSTR pstrClass)
{
	if (_tcscmp(pstrClass, DUI_CTR_CLIP) == 0)
		return new CClipUI;
	return NULL;
}

void RegisterUI::OnFinalMessage(HWND hWnd)
{
	removeObserver(*this);
	r.stop();

	WindowImplBase::OnFinalMessage(hWnd);
}

void RegisterUI::Notify(TNotifyUI& msg)
{
	WindowImplBase::Notify(msg);
}


void RegisterUI::OnCloseRWnd(TNotifyUI& msg)
{
	m_closeApp = false;
	Close();
}


void RegisterUI::InitWindow()
{
	addObserver(*this);
	r.start();
	BandingSubControl();
}


void RegisterUI::BandingSubControl()
{
	_name = dynamic_cast<CEditUI*>(m_PaintManager.FindControl(_T("Edit_Name")));
	_age = dynamic_cast<CEditUI*>(m_PaintManager.FindControl(_T("Edit_Age")));
	_sex = dynamic_cast<CComboUI*>(m_PaintManager.FindControl(_T("combo_sex")));
	_birth = dynamic_cast<CEditUI*>(m_PaintManager.FindControl(_T("Edit_Birth")));
	_address = dynamic_cast<CEditUI*>(m_PaintManager.FindControl(_T("Edit_Address")));
	_phone = dynamic_cast<CEditUI*>(m_PaintManager.FindControl(_T("Edit_Phone")));
	_identify = dynamic_cast<CEditUI*>(m_PaintManager.FindControl(_T("Edit_Identify")));
	_license_code = dynamic_cast<CEditUI*>(m_PaintManager.FindControl(_T("Edit_license_code")));
	_license_level = dynamic_cast<CComboUI*>(m_PaintManager.FindControl(_T("Edit_license_level")));
	_issue_date = dynamic_cast<CEditUI*>(m_PaintManager.FindControl(_T("Edit_issue_date")));
	_license_image = dynamic_cast<CEditUI*>(m_PaintManager.FindControl(_T("Edit_license_image")));
	_prompt = dynamic_cast<CLabelUI*>(m_PaintManager.FindControl(_T("lab_Prompt")));

	_shutter = dynamic_cast<CButtonUI*>(m_PaintManager.FindControl(_T("photo")));
}

void RegisterUI::Backward(TNotifyUI& msg)
{
	Close();
}

void RegisterUI::SignUp(TNotifyUI& msg)
{
	if (!bAlreadyTaked)
	{
		std::string str = LangueConfig::GetShowText(2);
		return _prompt->SetText(str.c_str());
	}

	if (!isValidInformation())
	{
		std::string str = LangueConfig::GetShowText(1);
		return _prompt->SetText(str.c_str());
	}


	//RegUserInfo::addUserInfo(m_userInfo);
	try
	{
		SaveRegisterInformation();

		PlaySoundA(_T("ZC.wav"), NULL, SND_FILENAME | SND_ASYNC);
		m_closeApp = false;

		Close();
	}
	catch (Poco::Data::SQLite::ConstraintViolationException& e)
	{
		e.displayText();
		_prompt->SetText("register failed!");
	}

}

void RegisterUI::TakePhoto(TNotifyUI& msg)
{
	if (bAlreadyTaked)
	{
		std::string str = LangueConfig::GetShowText(3);
		_shutter->SetText(str.c_str());
		bAlreadyTaked = false;
	}
	else
	{
		std::string str = LangueConfig::GetShowText(4);
		_shutter->SetText(str.c_str());
		bAlreadyTaked = true;
	}
}

bool RegisterUI::isValidInformation()
{

	if (_name->GetText().IsEmpty() || _sex->GetText().IsEmpty() || _birth->GetText().IsEmpty()
		|| _address->GetText().IsEmpty() || _phone->GetText().IsEmpty() || _identify->GetText().IsEmpty())
	{
		return false;
	}

	return true;
}

void RegisterUI::SaveRegisterInformation()
{
	add_user_license();
	add_user_identify();
	add_user_info();
}

void RegisterUI::add_user_license()
{
	struct license
	{
		int id;
		std::string filepath;
		std::string code;
		std::string issue_date;
		int level;
	};

	int maxid = 0;
	Session session("SQLite", "D:\\workstation\\code\\GitHub\\FaceIdentifiSystem\\bin\\facerecog.db");
	session << "SELECT max(id) FROM license",
		into(maxid),
		now;

	_license_id = ++maxid;

	int level = _license_level->GetCurSel() + 1;

	license li =
	{
		maxid,
		"testimg",//_license_image->GetText(),
		_license_code->GetText(),
		_issue_date->GetText(),
		level,
	};

	Statement insert(session);

	insert << "INSERT INTO license VALUES(?, ?, ?, ?, ?)",
		use(li.id),
		use(li.filepath),
		use(li.code),
		use(li.issue_date),
		use(li.level);

	insert.execute();
}

void RegisterUI::add_user_identify()
{
	struct identify
	{
		int id;
		std::string filepath;
		std::string code;
	};

	int maxid = 0;
	Session session("SQLite", "D:\\workstation\\code\\GitHub\\FaceIdentifiSystem\\bin\\facerecog.db");
	session << "SELECT max(id) FROM identify",
		into(maxid),
		now;

	_identify_id = ++maxid;

	identify ident =
	{
		_identify_id,
		"xxx",
		_identify->GetText(),
	};

	Statement insert(session);

	insert << "INSERT INTO identify VALUES(?, ?, ?)",
		use(ident.id),
		use(ident.filepath),
		use(ident.code);

	insert.execute();
}

void RegisterUI::add_user_info()
{
	struct UserInfo
	{
		int id;
		std::string name;
		int sex;
		std::string birthday;
		std::string address;
		std::string cellphone;
		int license_id;
		int unit_id;
		int identify_id;
		int photo_id;
		bool authorized;
	};


	int maxid = 0;
	Session session("SQLite", "D:\\workstation\\code\\GitHub\\FaceIdentifiSystem\\bin\\facerecog.db");
	session << "SELECT max(id) FROM UserInfo",
		into(maxid),
		now;

	maxid++;

	UserInfo user_info =
	{
		maxid,
		_name->GetText(),
		_sex->GetCurSel(),
		_birth->GetText(),
		_address->GetText(),
		_phone->GetText(),
		_license_id,
		_unit_id,
		_identify_id,
		_photo_id,
		false,
	};

	Statement insert(session);

	insert << "INSERT INTO UserInfo VALUES(?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?)",
		use(user_info.id),
		use(user_info.name),
		use(user_info.sex),
		use(user_info.birthday),
		use(user_info.address),
		use(user_info.cellphone),
		use(user_info.license_id),
		use(user_info.unit_id),
		use(user_info.identify_id),
		use(user_info.photo_id),
		use(user_info.authorized);

	insert.execute();
}

void RegisterUI::handle1(Poco::Notification* pNf)
{
	poco_check_ptr(pNf);
	if (!bAlreadyTaked)
	{
		CaptureNotification* nf = dynamic_cast<CaptureNotification*>(pNf);
		if (nf)
		{
			/*Poco::Data::CLOB saveImage((const char*)pic->data(), pic->len());
			m_userInfo.set<8>(saveImage);*/

			CControlUI* Image = m_PaintManager.FindControl(_T("photo_wnd"));
			Util::DrawSomething(nf->data(), Image, GetHWND());
		}
	}

	pNf->release();
}

LRESULT RegisterUI::HandleCustomMessage(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
	bHandled = FALSE;
	return 0;
}
