#include "stdafx.h"
#include "MonitoringUI.h"

//#include "Poco/Delegate.h"
//#include "Picture.h"
//#include "BitMapCompare.h"

//#include "Util.h"

#include "Mmsystem.h"
//#include "CaptureNotification.h"
//#include "WaittingUI.h"
//#include "SettingConfig.h"
#include "document.h"
#include "prettywriter.h"
#include "stringbuffer.h"
#include <Poco/Data/Session.h>
#include <Poco/Data/SQLite/Connector.h>

using Poco::Data::Session;
using Poco::Data::Statement;

using namespace Poco::Data::Keywords;
using namespace rapidjson;

CMonitoringUI::CMonitoringUI():
_notify_status(0), _monitor(100,2000),
_callback(*this, &CMonitoringUI::onTimer)
{
	m_closeApp = true;
	_monitor.start(_callback);
}


CMonitoringUI::~CMonitoringUI()
{
	_monitor.stop();
}

DUI_BEGIN_MESSAGE_MAP(CMonitoringUI, WindowImplBase)
DUI_ON_CLICK_CTRNAME(BT_CLOSE_MonWnd, OnCloseWnd)
DUI_END_MESSAGE_MAP()

void CMonitoringUI::onTimer(Poco::Timer& timer)
{
	DUITRACE("CMonitoringUI::onTimer");
	if (is_need_report())
	{
		report_to_user();
	}
}

bool CMonitoringUI::is_need_report()
{
	struct notify_main
	{
		int id;
		int opcode;
		std::string comment;
		int status;
		std::string json;
	};

	try
	{
		Session session("SQLite", "facerecog.db");
		notify_main notify;
		session << "SELECT id, opcode, comment, status, json_param FROM notify_main",
			into(notify.id),
			into(notify.opcode),
			into(notify.comment),
			into(notify.status),
			into(notify.json),
			now;

		//DUITRACE("%d,%d,%s,%d\n", notify.id, notify.opcode, notify.comment, notify.status);

		if (notify.status)
		{
			//解析json
			Document d;
			if (d.Parse(notify.json.c_str()).HasParseError())
				throw Poco::Exception("Json Has Parse Error");

			if (d.HasMember("open"))
			{
				int result = d["open"].GetInt();
				DUITRACE("check notify_main open : %d", result);
				return (result == 1);
			}
			return false;
		}
		
		return false;
	}
	catch (Poco::Exception& e)
	{
		DUITRACE(e.displayText().c_str());
		return false;
	}
}

void CMonitoringUI::report_to_user()
{
	PlaySoundA(_T("ALARM7.wav"), NULL, SND_FILENAME | SND_ASYNC);
	//更新报警图片 将alert中的registerfilepath和filepath分别显示在界面上
	//alert db - filepath vs registerfilepath
	//改变报警窗口背景颜色
}

LPCTSTR CMonitoringUI::GetWindowClassName() const
{
	return _T("CMonitoringUI");
}

CDuiString CMonitoringUI::GetSkinFolder()
{
	return _T("skin");
}

CDuiString CMonitoringUI::GetSkinFile()
{
	return _T("xml\\\\MonitoringUI.xml");
}

void CMonitoringUI::OnFinalMessage(HWND hWnd)
{
	WindowImplBase::OnFinalMessage(hWnd);
}

void CMonitoringUI::Notify(TNotifyUI& msg)
{
	WindowImplBase::Notify(msg);
}

void CMonitoringUI::OnCloseWnd(TNotifyUI& msg)
{
	m_closeApp = false;
	Close();
}

void CMonitoringUI::OnInitCtrl()
{
	m_Main_Lyt = dynamic_cast<CVerticalLayoutUI*>(m_PaintManager.FindControl(_T("vLyt_")));
	m_Prompt_lab = dynamic_cast<CLabelUI*>(m_PaintManager.FindControl(_T("result")));
	m_photo_Ctrl = dynamic_cast<CControlUI*>(m_PaintManager.FindControl(_T("photo_video")));
}

void CMonitoringUI::InitWindow()
{
}