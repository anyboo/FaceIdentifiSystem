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
_unit_id(0),
_userinfo_id(0)
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
	if (!isValidInformation())
	{
		std::string str = LangueConfig::GetShowText(1);
		return _prompt->SetText(str.c_str());
	}

	if (!bAlreadyTaked)
	{
		std::string str = LangueConfig::GetShowText(2);
		return _prompt->SetText(str.c_str());
	}

	GetDataFromUI();
	if (IsDuplicate())
	{
		std::string str = LangueConfig::GetShowText(10);
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
		DUITRACE("ConstraintViolationException: class_name %s, display: %s", e.className(), e.displayText().c_str());
		std::string str = LangueConfig::GetShowText(11);
		_prompt->SetText(str.c_str());
	}

}

bool RegisterUI::IsDuplicate()
{
	struct Person
	{
		int id;
		std::string  name;
		std::string  code;
	};
	Person people = {0, "", ""};

	try
	{
		Session session("SQLite", "D:\\workstation\\code\\GitHub\\FaceIdentifiSystem\\bin\\facerecog.db");
		Statement duplicate(session);
		duplicate << "SELECT identify.id, identify.code, identify.filepath, UserInfo.name \
				 				 		FROM identify INNER JOIN UserInfo ON identify.code \
																		= ?",
																		into(people.id),
																		into(people.name),
																		into(people.code),
																		use(_value_identify_code),
																		now;
	}
	catch (Poco::Data::DataException& e)
	{
		DUITRACE(e.displayText().c_str());
	}
	

	return !people.name.empty();
}

void RegisterUI::GetDataFromUI()
{
	_value_unit = "lm_tech";
	_value_name = _name->GetText();
	_value_birthday = _birth->GetText();
	_value_address = _address->GetText();
	_value_cellphone = _phone->GetText();
	_value_license_code = _license_code->GetText();
	_value_issue_date = _issue_date->GetText();
	_value_identify_code = _identify->GetText();
	_value_photo_path = "D:/workstation/code/GitHub/FaceIdentifiSystem/bin/photo/20161023234543.jpg";
	_value_sex = _sex->GetCurSel();
	_value_level = _license_level->GetCurSel() + 1;
}

void RegisterUI::TakePhoto(TNotifyUI& msg)
{
	if (!isValidInformation())
	{
		std::string str = LangueConfig::GetShowText(1);
		return _prompt->SetText(str.c_str());
	}

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

	commit_to_server();
}

#include "ActiveUploader.h"
#include "ActiveReporter.h"
#include "prettywriter.h"
#include "stringbuffer.h"
#include <Poco/Path.h>

using Poco::Path;
using namespace rapidjson;

void RegisterUI::commit_to_server()
{
	FTPClientSession _ftp("202.103.191.73", FTPClientSession::FTP_PORT, "ftpalert", "1");
	ActiveUploader ur(_ftp);
	ActiveResult<bool> upload_result = ur.upload(_value_photo_path);

	HTTPClientSession session("202.103.191.73");
	ActiveReporter rp(session);

	/*
	body={"api":"user_add","name":"yuk","sex":1,"birthday":"19830214","address":"fz","phone":"13950424413",
	"license_code":"li0001", "unit":"lm_tech","identify_code":"352225198215250075",
	"photo_path":"http://202.103.191.73/api/3870861421055882.jpg", "issue_date":"20110101", "level":5}
	*/
	
	Document d;
	d.SetObject();

	std::string server_dir("http://202.103.191.73/var/www/sisec_website/photos/");
	Path p(_value_photo_path);
	server_dir += p.getFileName();

	d.AddMember("api", "user_add", d.GetAllocator());
	d.AddMember("name", StringRef(_value_name.c_str()), d.GetAllocator());
	d.AddMember("sex", _value_sex, d.GetAllocator());
	d.AddMember("birthday", StringRef(_value_birthday.c_str()), d.GetAllocator());
	d.AddMember("address", StringRef(_value_address.c_str()), d.GetAllocator());
	d.AddMember("phone", StringRef(_value_cellphone.c_str()), d.GetAllocator());
	d.AddMember("license_code", StringRef(_value_license_code.c_str()), d.GetAllocator());
	d.AddMember("unit", StringRef(_value_unit.c_str()), d.GetAllocator());
	d.AddMember("identify_code", StringRef(_value_identify_code.c_str()), d.GetAllocator());
	d.AddMember("photo_path", StringRef(server_dir.c_str()), d.GetAllocator());
	d.AddMember("issue_date", StringRef(_value_issue_date.c_str()), d.GetAllocator());
	d.AddMember("level", _value_level, d.GetAllocator());

	StringBuffer sb;
	Writer<StringBuffer> writer(sb);
	d.Accept(writer);
	
	ActiveResult<std::string> result = rp.report(sb.GetString());
	result.wait();
	std::string received = result.data();
	DUITRACE("HTTP RECEIVED : %s", received);

	if (!received.empty())
	{
		Document document;
		received = received.substr(3, std::string::npos);
		if (!document.Parse(received.c_str()).HasParseError())
		{
			if (document.HasMember("id"))
			{
				std::string server_id = document["id"].GetString();

				Session session("SQLite", "D:\\workstation\\code\\GitHub\\FaceIdentifiSystem\\bin\\facerecog.db");
				Statement update(session);
				update << "UPDATE UserInfo SET id = ? WHERE id = ?",
					bind(std::stoi(server_id)),
					bind(_userinfo_id),
					now;
			}
		}
	}
	
	upload_result.wait();
	_ftp.close();
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

	license li =
	{
		_license_id,
		"testimg",//_license_image->GetText(),
		_value_license_code,
		_value_issue_date,
		_value_level,
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
		_value_identify_code,
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

	_userinfo_id = ++maxid;

	UserInfo user_info =
	{
		_userinfo_id,
		_value_name,
		_value_sex,
		_value_birthday,
		_value_address,
		_value_cellphone,
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
