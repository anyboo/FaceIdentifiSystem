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

CMonitoringUI::CMonitoringUI()
	:r(new Camera)
{
<<<<<<< HEAD
	m_closeApp = true;
	m_bSendMsg = false;
	ValueSetting vSet;
	m_timeInterval = std::stoi(vSet.GetTime_interval()) * 1000 - 15000;
=======
>>>>>>> remotes/origin/TheFirstVersion
	//m_pCompare = new BitMapCompare(this);
	//Poco::ThreadPool::defaultPool().start(*m_pCompare);
}


CMonitoringUI::~CMonitoringUI()
{
	/*m_theEvent += Poco::delegate(m_pCompare, &BitMapCompare::onEvent);
	fireEvent(true);
	m_theEvent -= Poco::delegate(m_pCompare, &BitMapCompare::onEvent);*/
	//Poco::ThreadPool::defaultPool().joinAll();
	//delete m_pCompare;
}

DUI_BEGIN_MESSAGE_MAP(CMonitoringUI, WindowImplBase)
DUI_ON_CLICK_CTRNAME(BT_CLOSE_MonWnd, OnCloseWnd)
DUI_END_MESSAGE_MAP()

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
	return _T("xml\\MonitoringUI.xml");
}

void CMonitoringUI::OnFinalMessage(HWND hWnd)
{
	removeObserver(*this);
	r.stop();
	example.stop();

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
	SetTimer(GetHWND(), 1, m_timeInterval, nullptr);
	addObserver(*this);
	r.start();
	example.start();
	m_count = 0;
}


std::queue<CapBitmapData>& CMonitoringUI::getCapDataQueue()
{
	return m_capdata;
}


void CMonitoringUI::handle1(Poco::Notification* pNf)
{
	poco_check_ptr(pNf);

	Notification::Ptr pf(pNf);
	poco_check_ptr(pf.get());
	if (m_bSendMsg && m_count % 10 == 0)
	{	
		example.enqueueNotification(pf);	
		m_count++;
	}
	
	CaptureNotification::Ptr nf = pf.cast<CaptureNotification>();
	poco_check_ptr(nf.get());
	Picture::Ptr pic(nf->data());
	poco_check_ptr(pic.get());

<<<<<<< HEAD
	Util::DrawSomething(pic, m_photo_Ctrl, GetHWND());
=======

	CControlUI* Image = m_PaintManager.FindControl(_T("photo_video"));
	Util::DrawSomething(pic, Image, GetHWND());
>>>>>>> remotes/origin/TheFirstVersion
}

LRESULT CMonitoringUI::HandleCustomMessage(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
	LRESULT lRes = 0;
	switch (uMsg)
	{
	case WM_TIMER: lRes = OnTimer(uMsg, wParam, lParam, bHandled); break;
	case WM_DESTROY:
		if (m_closeApp)
		{
			::ShowWindow(::FindWindow("Shell_TrayWnd", NULL), SW_SHOW);
			::PostQuitMessage(0);
		}
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
	time(&m_nowTime);
	if (example.queryResult() && m_nowTime - m_lastTime <= 15)
	{
		m_bSendMsg = false;
		KillTimer(GetHWND(), 2);
		SetTimer(GetHWND(), 1, m_timeInterval, nullptr);
	}
<<<<<<< HEAD
	else if (!example.queryResult() && m_nowTime - m_lastTime > 15)
	{
		KillTimer(GetHWND(), 2);
		SetTimer(GetHWND(), 3, 500, nullptr);
		SetTimer(GetHWND(), 4, 1000, nullptr);
	}
}

void CMonitoringUI::OnTimer3()
{
	if (example.queryResult())
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
	}
}

void CMonitoringUI::OnTimer4()
{
	m_bSendMsg = (m_bSendMsg == false ? true : false);
}
=======
	return 0;
}
>>>>>>> remotes/origin/TheFirstVersion
