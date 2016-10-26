#include "stdafx.h"
#include "MonitoringUI.h"

#include "Poco/Delegate.h"
#include "Picture.h"
#include "BitMapCompare.h"

#include "Util.h"

#include "Mmsystem.h"
#include "CaptureNotification.h"
#include "WaittingUI.h"
#include "SettingConfig.h"

#include "Poco/Data/Session.h"
#include "Poco/Data/SQLite/Connector.h"


//#include "document.h" 
//
//using namespace rapidjson;

using Poco::Data::Session;
using Poco::Data::Statement;
using namespace Poco::Data::Keywords;

CMonitoringUI::CMonitoringUI()
:t(100, 2000), tc(*this, &CMonitoringUI::onTimer),
_notify_status(0)
{
	m_closeApp = true;
	m_bSendMsg = false;
	ValueSetting vSet;
	m_timeInterval = std::stoi(vSet.GetTime_interval()) * 1000 - 15000;

	t.start(tc);
	_commit.start();
}


CMonitoringUI::~CMonitoringUI()
{
	t.stop();
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
				return false;
			if (d.HasMember("open"))
			{
				return (d["open"].GetInt() == 1);
			}
		}
		
		return false;
	}
	catch (Poco::Exception& e)
	{
		DUITRACE(e.displayText().c_str());
	}
}

void CMonitoringUI::report_to_user()
{
	PlaySoundA(_T("ALARM7.wav"), NULL, SND_FILENAME | SND_ASYNC);
	//m_Prompt_lab->SetVisible(false);
	/*DWORD bkcolor = m_Main_Lyt->GetBkColor();
	DWORD itembkcolor = bkcolor == 0x00000000 ? 0xFFFF9999 : 0x00000000;
	m_Main_Lyt->SetBkColor(itembkcolor);*/
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

void CMonitoringUI::OnRemoveAlarm()
{
	//std::auto_ptr<CWaittingUI> pDlg(new CWaittingUI);
	//assert(pDlg.get());
	//pDlg->Create(this->GetHWND(), NULL, UI_WNDSTYLE_FRAME, 0L, 1024, 768, 0, 0);
	//pDlg->CenterWindow();
	//pDlg->ShowModal();
}

void CMonitoringUI::InitWindow()
{
	m_count = 0;
}


std::queue<CapBitmapData>& CMonitoringUI::getCapDataQueue()
{
	return m_capdata;
}

/*
void CMonitoringUI::handle1(Poco::Notification* pNf)
{
	poco_check_ptr(pNf);
	CaptureNotification* nf = dynamic_cast<CaptureNotification*>(pNf);
	if (nf)
	{

		CControlUI* Image = m_PaintManager.FindControl(_T("photo_video"));
		Util::DrawSomething(nf->data(), Image, GetHWND());
	}
	pNf->release();
}
*/

LRESULT CMonitoringUI::HandleCustomMessage(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
	LRESULT lRes = 0;
	switch (uMsg)
	{
	case WM_TIMER: lRes = OnTimer(uMsg, wParam, lParam, bHandled);
		break;
	}
	bHandled = FALSE;
	return 0;
}

LRESULT CMonitoringUI::OnTimer(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
	switch (wParam)
	{
	case 1: OnTimer1();	break;
	case 2: OnTimer2(); break;
	case 3: OnTimer3(); break;
	case 4: OnTimer4(); break;
	default:
		break;
	}
	return 0;
}

void CMonitoringUI::OnTimer1()
{
	m_bSendMsg = true;
	time(&m_lastTime);
	SetTimer(GetHWND(), 2, 250, nullptr);
	KillTimer(GetHWND(), 1);
}

void CMonitoringUI::OnTimer2()
{
	/*time(&m_nowTime);
	if (example.queryResult() && m_nowTime - m_lastTime <= 15)
	{
		m_bSendMsg = false;
		KillTimer(GetHWND(), 2);
		SetTimer(GetHWND(), 1, m_timeInterval, nullptr);
	}
	else if (!example.queryResult() && m_nowTime - m_lastTime > 15)
	{
		KillTimer(GetHWND(), 2);
		SetTimer(GetHWND(), 3, 500, nullptr);
		SetTimer(GetHWND(), 4, 1000, nullptr);
	}*/
}

void CMonitoringUI::OnTimer3()
{
	/*if (example.queryResult())
	{
		KillTimer(GetHWND(), 3);
		KillTimer(GetHWND(), 4);
		SetTimer(GetHWND(), 1, m_timeInterval, nullptr);
		m_bSendMsg = false;
		m_Prompt_lab->SetVisible(true);
		m_Main_Lyt->SetBkColor(0x00000000);
	}
	else
	{
		PlaySoundA(_T("BJ.wav"), NULL, SND_FILENAME | SND_ASYNC);
		m_Prompt_lab->SetVisible(false);
		DWORD bkcolor = m_Main_Lyt->GetBkColor();
		DWORD itembkcolor = bkcolor == 0x00000000 ? 0xFFFF9999 : 0x00000000;
		m_Main_Lyt->SetBkColor(itembkcolor);
	}*/
}

void CMonitoringUI::OnTimer4()
{
	m_bSendMsg = (m_bSendMsg == false ? true : false);
}


