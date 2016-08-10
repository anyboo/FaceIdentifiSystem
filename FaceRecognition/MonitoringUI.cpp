#include "stdafx.h"
#include "MonitoringUI.h"

#include "Poco/Delegate.h"
#include "Picture.h"
#include "BitMapCompare.h"

#include "Util.h"

#include "Mmsystem.h"

#include "WaittingUI.h"

CMonitoringUI::CMonitoringUI()
	:r(new Camera)
{
	m_closeApp = true;
	m_bSendMsg = false;
	time(&m_lastTime);
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
//DUI_ON_CLICK_CTRNAME(BT_REMOVE_ALARM, OnRemoveAlarm)
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

void CMonitoringUI::OnRemoveAlarm()
{
	//std::auto_ptr<CWaittingUI> pDlg(new CWaittingUI);
	//assert(pDlg.get());
	//pDlg->Create(this->GetHWND(), NULL, UI_WNDSTYLE_FRAME, 0L, 1024, 768, 0, 0);
	//pDlg->CenterWindow();
	//pDlg->ShowModal();

	CVerticalLayoutUI* vLyt = dynamic_cast<CVerticalLayoutUI*>(m_PaintManager.FindControl(_T("vLyt_")));
//	CLabelUI* lab = dynamic_cast<CLabelUI*>(m_PaintManager.FindControl(_T("result")));
//	lab->SetVisible(false);

	vLyt->SetBkColor(0x00000000);
}

void CMonitoringUI::InitWindow()
{
	SetTimer(GetHWND(), 3, 15000, nullptr);

	addObserver(*this);
	r.start();
	example.start();
	m_count = 0;
}


std::queue<CapBitmapData>& CMonitoringUI::getCapDataQueue()
{
	return m_capdata;
}

#include "CaptureNotification.h"
void CMonitoringUI::handle1(Poco::Notification* pNf)
{
	poco_check_ptr(pNf);

	Notification::Ptr pf(pNf);
	poco_check_ptr(pf.get());
	if (m_bSendMsg)
	{
		if ((m_count % 10) == 0)
		{
			example.enqueueNotification(pf);
		}
		m_count++;
	}
	
	CaptureNotification::Ptr nf = pf.cast<CaptureNotification>();
	poco_check_ptr(nf.get());
	Picture::Ptr pic(nf->data());
	poco_check_ptr(pic.get());


	CControlUI* Image = m_PaintManager.FindControl(_T("photo_video"));
	Util::DrawSomething(pic, Image, GetHWND());
}

LRESULT CMonitoringUI::HandleCustomMessage(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
	LRESULT lRes = 0;
	switch (uMsg)
	{
	case WM_TIMER: lRes = OnTimer(uMsg, wParam, lParam, bHandled); break;
	case WM_DESTROY:
		if (m_closeApp){
			::PostQuitMessage(0);
		}
		break;
	}
	bHandled = FALSE;
	return 0;
}

LRESULT CMonitoringUI::OnTimer(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{  
	if (wParam == 3)
	{ 
		m_bSendMsg = true;
		time(&m_lastTime);
		SetTimer(GetHWND(), 1, 500, nullptr);	
		KillTimer(GetHWND(), 3);
	}
	else if (wParam == 1)
	{
		time(&m_nowTime);
		if (example.queryResult() && m_nowTime - m_lastTime <= 15)
		{
			m_bSendMsg = false;
			KillTimer(GetHWND(), 1);
			SetTimer(GetHWND(), 3, 15000, nullptr);
		}
		else if (!example.queryResult() && m_nowTime - m_lastTime > 15)
		{
			KillTimer(GetHWND(), 1);
			SetTimer(GetHWND(), 4, 1000, nullptr);
			SetTimer(GetHWND(), 2, 500, nullptr);			
		}
	}
	else if (wParam == 2)
	{		
		if (example.queryResult())
		{		
			KillTimer(GetHWND(), 2);
			KillTimer(GetHWND(), 4);
			SetTimer(GetHWND(), 3, 15000, nullptr);
			m_bSendMsg = false;
			CLabelUI* lab = dynamic_cast<CLabelUI*>(m_PaintManager.FindControl(_T("result")));
			lab->SetVisible(true);

			OnRemoveAlarm();
			return 0;
		}
		PlaySoundA(_T("BJ.wav"), NULL, SND_FILENAME | SND_ASYNC);
		CVerticalLayoutUI* vLyt = dynamic_cast<CVerticalLayoutUI*>(m_PaintManager.FindControl(_T("vLyt_")));
		CLabelUI* lab = dynamic_cast<CLabelUI*>(m_PaintManager.FindControl(_T("result")));
		lab->SetVisible(false);
		DWORD bkcolor = vLyt->GetBkColor();
		if (bkcolor == 0xFFFF9999)
		{
			vLyt->SetBkColor(0x00000000);
		}
		else
		{
			vLyt->SetBkColor(0xFFFF9999);
		}
	}
	else if (wParam == 4)
	{
		m_bSendMsg = (m_bSendMsg == false ? true : false);
	}
	return 0;
}
