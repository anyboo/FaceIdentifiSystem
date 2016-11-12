#include "stdafx.h"
#include "SettingUI.h"
#include "CommDlg.h"

//#include "document.h"
//#include "prettywriter.h"
//#include "stringbuffer.h"
//
//#include "ostreamwrapper.h"
//#include "istreamwrapper.h"

#include <string>
#include <tchar.h>
#include <windows.h>
#include <fstream>
#include "DeviceUI.h"
#include "ScheduleUI.h"
#include "UserUI.h"
#include "SystemUI.h"
#include "SettingConfig.h"

#define BT_SYSTEM_SET		(_T("btn_SysSet"))
#define BT_DEVICE_MANAGE	(_T("btn_Device"))
#define BT_WORKFACE_MANAGE	(_T("btn_Workfoce"))
#define BT_USER_MANAGE		(_T("btn_User"))

#define BT_CLOSESWnd		(_T("close_btn2"))
#define BT_SAVELOG			(_T("saveLog"))
#define BT_SAVECONFIG		(_T("saveConfig"))
#define BT_RESETCONFIG		(_T("resetConfig"))
#define BT_REGISTER_DEVICE		(_T("register"))
#define BT_SAVE_CONFIGURATION	(_T("save"))

CSettingUI::CSettingUI():
_dbIndex(0)
{
}


CSettingUI::~CSettingUI()
{
}

DUI_BEGIN_MESSAGE_MAP(CSettingUI, WindowImplBase)
DUI_ON_CLICK_CTRNAME(BT_CLOSESWnd, OnCloseSWnd)
DUI_ON_CLICK_CTRNAME(BT_REGISTER_DEVICE, OnRegister)
DUI_ON_CLICK_CTRNAME(BT_SAVE_CONFIGURATION, OnSave)
DUI_END_MESSAGE_MAP()

LPCTSTR CSettingUI::GetWindowClassName() const
{
	return _T("CSettingUI");
}

CDuiString CSettingUI::GetSkinFolder()
{
	return _T("skin");
}

CDuiString CSettingUI::GetSkinFile()
{
	return _T("xml\\SettingUI.xml");
}

void CSettingUI::InitWindow()
{
	InitDB();
}

void CSettingUI::OnFinalMessage(HWND hWnd)
{
	WindowImplBase::OnFinalMessage(hWnd);
}

void CSettingUI::Notify(TNotifyUI& msg)
{
	DUITRACE("%s %s",msg.pSender->GetName().GetData() ,msg.sType);
	OnSwitch(msg);
	WindowImplBase::Notify(msg);
}

CControlUI* CSettingUI::CreateControl(LPCTSTR pstrClass)
{
	if (_tcscmp(pstrClass, _T("DeviceUI")) == 0) return new DeviceUI;
	if (_tcscmp(pstrClass, _T("UserUI")) == 0) return new UserUI;
	if (_tcscmp(pstrClass, _T("ScheduleUI")) == 0) return new ScheduleUI;
	if (_tcscmp(pstrClass, _T("SystemUI")) == 0) return new SystemUI;
	return NULL;
}

void CSettingUI::OnCloseSWnd(TNotifyUI& msg)
{
	Close();
}

void CSettingUI::OnSwitch(TNotifyUI& msg)
{
	CDuiString name = msg.pSender->GetName();
	CTabLayoutUI* pControl = static_cast<CTabLayoutUI*>(m_PaintManager.FindControl(_T("switch")));
	if (name == _T("device"))
		pControl->SelectItem(0);
	else if (name == _T("user"))
		pControl->SelectItem(1);
	else if (name == _T("schedule"))
		pControl->SelectItem(2);
	else if (name == _T("system"))
		pControl->SelectItem(3);
	else if (name == _T("other"))
		pControl->SelectItem(4);
}

void CSettingUI::OnSaveLog(TNotifyUI& msg)
{
	TCHAR szBuffer[MAX_PATH] = { 0 };

	OPENFILENAME  ofn = { 0 };
	ofn.lStructSize = sizeof(ofn);
	ofn.hwndOwner = m_hWnd;
	ofn.lpstrFilter = _T("TXT文件(*.txt)\0*.txt\0所有文件(*.*)\0*.*\0");
	ofn.lpstrInitialDir = _T("D:\\");   
	ofn.lpstrFile = szBuffer;			//save data path
	ofn.nMaxFile = sizeof(szBuffer) / sizeof(*szBuffer);
	ofn.nFilterIndex = 1;
	ofn.Flags = OFN_CREATEPROMPT | OFN_PATHMUSTEXIST | OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT;
	ofn.lpstrTitle = TEXT("save to file");
	BOOL bSel = GetSaveFileName(&ofn);
}

#include <Poco/Data/Session.h>
#include <Poco/Data/SQLite/Connector.h>
#include <Poco/Data/SQLite/SQLiteException.h>
#include <Poco/Timestamp.h>
#include "ActiveReporter.h"

using Poco::Data::Session;
using Poco::Data::Statement;

using namespace Poco::Data::Keywords;
using namespace rapidjson;
using Poco::DateTimeFormatter;
using Poco::LocalDateTime;

void CSettingUI::InitDB()
{
	try
	{
		Session session("SQLite", "facerecog.db");
		session << "SELECT max(id) FROM DeviceInfo",
			into(_dbIndex),
			now;
	}
	catch (Poco::DataException& e)
	{
		DUITRACE(e.displayText().c_str());
	}
	
}

void CSettingUI::OnRegister(TNotifyUI& msg)
{
	DUITRACE("OnRegister : %s", msg.pSender->GetName().GetData());
	/*
	devType
	devArea
	devUnit
	devName
	devDetailAddress
	devIP
	devRespon
	devResponPhone
	*/

	//get value from ui
	CEditUI* type = static_cast<CEditUI*>(m_PaintManager.FindControl(_T("devType")));
	CEditUI* area = static_cast<CEditUI*>(m_PaintManager.FindControl(_T("devArea")));
	CEditUI* unit = static_cast<CEditUI*>(m_PaintManager.FindControl(_T("devUnit")));
	CEditUI* name = static_cast<CEditUI*>(m_PaintManager.FindControl(_T("devName")));
	CEditUI* detail = static_cast<CEditUI*>(m_PaintManager.FindControl(_T("devDetailAddress")));
	CEditUI* ip = static_cast<CEditUI*>(m_PaintManager.FindControl(_T("devIP")));
	CEditUI* respon = static_cast<CEditUI*>(m_PaintManager.FindControl(_T("devRespon")));
	CEditUI* phone = static_cast<CEditUI*>(m_PaintManager.FindControl(_T("devResponPhone")));
	CEditUI* building = static_cast<CEditUI*>(m_PaintManager.FindControl(_T("building")));
	

	std::string _type = type->GetText();
	std::string _area = area->GetText();
	std::string _unit = unit->GetText();
	std::string _name = name->GetText();
	std::string _detail = detail->GetText();
	std::string _ip = ip->GetText();
	std::string _respon = respon->GetText();
	std::string _phone = phone->GetText();
	std::string _building = building->GetText();
	//pack to local db
	Session session("SQLite", "facerecog.db");
	
	if (_dbIndex != 0)
	{
		Statement del(session);
		del << "DELETE FROM DeviceInfo WHERE id = ?",
			use(_dbIndex),
			now;
	}
	
	_dbIndex = 1;

	int area_id = 1;
	int unit_id = 0;
	int network_status = 1;
	int authorized = 0;

	LocalDateTime now;
	std::string date_time = DateTimeFormatter::format(now, "%Y%n%d%H%M%S");

	Statement insert(session);
	insert << "INSERT INTO DeviceInfo VALUES(?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?,?)",
		use(_dbIndex),
		use(_name),
		use(_type),
		use(area_id),
		use(unit_id),
		use(_ip),
		use(date_time),
		use(_detail),
		use(_respon),
		use(_phone),
		use(network_status),
		use(authorized),
		use(_building),
		now;
	
	{
		/*
		body:{"api":"device_add", "type":"DVR", "area":"福田区", "unit":"深圳市龙脉安信软件技术有限公司", "name":"测试机1", "position":"深南大道2001嘉麟豪庭B2204","ip":"192.168.1.2","resp_man":"黄聪泉","resp_phone":"139xxxx0011", "register_time":"20120101", "building_name":"嘉麟豪庭B座"}
		*/
		//commit to server
		//body:{"api":"device_add", "type":2, "area":"福田区", "unit":"longmai", "name":"dvr1001", "position":"futianddwdewd",
		//"ip":"192.168.1.2",
		//"resp_man":"zhuangqx","resp_phone":"123455", "register_time":"20120101", "building_name":""}
		HTTPClientSession session("202.103.191.73");
		ActiveReporter rp(session);

		Document d;
		d.SetObject();

		d.AddMember("api", "device_add", d.GetAllocator());
		d.AddMember("type", StringRef(_type.c_str()), d.GetAllocator());
		d.AddMember("area", StringRef(_area.c_str()), d.GetAllocator());
		d.AddMember("unit", StringRef(_unit.c_str()), d.GetAllocator());
		d.AddMember("name", StringRef(_name.c_str()), d.GetAllocator());
		d.AddMember("position", StringRef(_detail.c_str()), d.GetAllocator());

		d.AddMember("ip", StringRef(_ip.c_str()), d.GetAllocator());
		d.AddMember("resp_man", StringRef(_respon.c_str()), d.GetAllocator());
		d.AddMember("resp_phone", StringRef(_phone.c_str()), d.GetAllocator());
		d.AddMember("register_time", StringRef(date_time.c_str()), d.GetAllocator());
		d.AddMember("building_name", StringRef(_building.c_str()), d.GetAllocator());

		StringBuffer sb;
		Writer<StringBuffer> writer(sb);
		d.Accept(writer);

		ActiveResult<std::string> result = rp.report(sb.GetString());
		result.wait();
		std::string received = result.data();
		DUITRACE("HTTP RECEIVED : %s", received);
	}
}

void CSettingUI::OnSave(TNotifyUI& msg)
{
	DUITRACE("OnSave : %s", msg.pSender->GetName().GetData());
	//get value from ui
	CEditUI* duty = static_cast<CEditUI*>(m_PaintManager.FindControl(_T("duty")));
	CEditUI* monitor = static_cast<CEditUI*>(m_PaintManager.FindControl(_T("monitor")));
	CEditUI* alarm = static_cast<CEditUI*>(m_PaintManager.FindControl(_T("alarm")));
	CEditUI* percent = static_cast<CEditUI*>(m_PaintManager.FindControl(_T("percent")));
	CEditUI* facesize = static_cast<CEditUI*>(m_PaintManager.FindControl(_T("facesize")));
	

	std::string _duty = duty->GetText();
	std::string _monitor = monitor->GetText();
	std::string _alarm = alarm->GetText();
	std::string _percent = percent->GetText();
	std::string _facesize = facesize->GetText();

	//pack to local db
	Session session("SQLite", "facerecog.db");

	{
		Statement update(session);
		update << "UPDATE SettingInfo SET value = ? WHERE key = certain_time_seonds; "
			"UPDATE SettingInfo SET value = ? WHERE key = sleep_seconds_after_success_verify; "
			"UPDATE SettingInfo SET value = ? WHERE key = simility_pass; ",
			use(_alarm),
			use(_percent),
			now;
	}

	{
		Statement update(session);
		update << "UPDATE ipcamerasetting SET ip = ? WHERE id = 1",
			use(_monitor),
			now;
	}
	
}