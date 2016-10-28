#include "stdafx.h"
#include "RegisterUI.h"
//#include "CaptureNotification.h"
//#include "Camera.h"
//#include "Util.h"
#include "SettingConfig.h"
#include "ClipUI.h"
#include "CameraUI.h"

#include "Mmsystem.h"

#include "Employee.h"

#include "ActiveUploader.h"
#include "ActiveReporter.h"
#include "document.h"
#include "prettywriter.h"
#include "stringbuffer.h"
#include "THFaceImage_i.h"
#include "THFeature_i.h"
#include <Poco/Buffer.h>
#include <Poco/BinaryWriter.h>
#include <Poco/Path.h>
#include <Poco/File.h>
#include <Poco/FileStream.h>
#include <Poco/Timestamp.h>
#include <Poco/Data/Session.h>
#include <Poco/Data/SQLite/Connector.h>
#include <Poco/Data/SQLite/SQLiteException.h>


using Poco::Buffer;
using Poco::File;
using Poco::FileOutputStream;
using Poco::BinaryWriter;

using Poco::Buffer;
using Poco::File;
using Poco::Path;
using Poco::Data::Session;
using Poco::Data::Statement;
using Poco::DateTimeFormatter;
using Poco::LocalDateTime;

using namespace Poco::Data::Keywords;
using namespace rapidjson;

RegisterUI::RegisterUI()
:m_photo_agin(false)
, r(new Camera)
, bAlreadyTaked(false),
_license_id(0),
_identify_id(0),
_photo_id(0),
_unit_id(0),
_userinfo_id(0),
_pCameraUI(0)
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
	if (_tcscmp(pstrClass, DUI_CTR_CAMERAUI) == 0)
	{
		_pCameraUI = new CameraUI;
		return _pCameraUI;
	}
		 
	return NULL;
}

void RegisterUI::OnFinalMessage(HWND hWnd)
{

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
	_photo_for_user = dynamic_cast<CHorizontalLayoutUI*>(m_PaintManager.FindControl(_T("photo_for_user")));
	
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

	Close();

	try
	{
		SaveRegisterInformation();

		PlaySoundA(_T("ZC.wav"), NULL, SND_FILENAME | SND_ASYNC);
		m_closeApp = false;
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
		Session session("SQLite", "facerecog.db");
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
	//_value_photo_path = "photoshop/regiter.jpg";
	_value_sex = _sex->GetCurSel();
	_value_level = _license_level->GetCurSel() + 1;
}

void RegisterUI::Extract(IplImage *image, std::string datafile)
{
	if (!image) return; 

	THFI_FacePos ptfp[3];
	ptfp[0].dwReserved = (DWORD)new BYTE[512];
	int nFace = THFI_DetectFace(0, (BYTE*)image->imageData, /*image->depth*/24, image->width, image->height, ptfp, 3);
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
	if (EF_Extract(0, (BYTE*)image->imageData, image->width, image->height,3, (DWORD)&ptfp[nMaxIndex], pFeature.begin()))
	{
		File file(datafile);
		FileOutputStream fout(datafile);
		BinaryWriter writer(fout);
		writer.writeRaw((char*)pFeature.begin(), pFeature.size());
	}
}

void RegisterUI::TakePhoto(TNotifyUI& msg)
{
	if (!bAlreadyTaked)
	{
		LocalDateTime now;
		std::string jpg = Path::current()
			.append("photoshop/")
			.append(DateTimeFormatter::format(now, "%Y%n%d%H%M%S"))
			.append(".jpg");

		_pCameraUI->ScreenSnapshot(jpg);

		_value_photo_path = jpg;
		//判断是否有人脸，有则提取照片特征,保存到路径
		_value_feather_path = jpg + ".dat";
		IplImage *image = cvLoadImage(jpg.c_str());
		Extract(image, _value_feather_path);
		cvReleaseImage(&image);

		DisplayPhoto(_value_photo_path);
		bAlreadyTaked = true;
		/*std::string str = LangueConfig::GetShowText(4);
		_shutter->SetText(str.c_str());
		bAlreadyTaked = true;*/
	}
	//else
	//{
	//	//拍照
	//	DisplayCameraUI();
	//	std::string str = LangueConfig::GetShowText(3);
	//	_shutter->SetText(str.c_str());
	//	bAlreadyTaked = false;
	//}
}

void RegisterUI::DisplayPhoto(const std::string& photo_file_path)
{
	//显示拍照照片到界面上
	_photo_for_user->SetBkImage(photo_file_path.c_str());
}

void RegisterUI::DisplayCameraUI()
{
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

void RegisterUI::add_user_to_onduty(int server_id)
{
	//将注册用户增加到onduty排班表的userid字段并设置onduty=0
	struct duty
	{
		int id;
		int userid;
		int onduty;
	};

	duty status = { 0, server_id, 0 };

	Session session("SQLite", "facerecog.db");
	session << "SELECT max(id) FROM duty",
		into(status.id),
		now;

	status.id += 1;
	
	Statement insert(session);

	insert << "INSERT INTO duty VALUES(?, ?, ?)",
		use(status.id),
		use(status.userid),
		use(status.onduty),
		now;
}

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

	std::string server_dir("http://202.103.191.73/photos/");
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

				Session session("SQLite", "facerecog.db");
				Statement update(session);
				update << "UPDATE UserInfo SET id = ? WHERE id = ?",
					bind(std::stoi(server_id)),
					bind(_userinfo_id),
					now;

				add_user_to_onduty(std::stoi(server_id));
			}
		}
	}
	
	upload_result.wait();
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
	Session session("SQLite", "facerecog.db");
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
		use(li.level),
		now;
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
	Session session("SQLite", "facerecog.db");
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
		use(ident.code),
		now;
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
		std::string photo_file_path;
		std::string facefeatherpath;
	};


	int maxid = 0;
	Session session("SQLite", "facerecog.db");
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
		_value_photo_path,
		_value_feather_path,
	};

	Statement insert(session);

	insert << "INSERT INTO UserInfo VALUES(?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?)",
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
		use(user_info.authorized),
		use(user_info.photo_file_path),
		use(user_info.facefeatherpath),
		now;
}