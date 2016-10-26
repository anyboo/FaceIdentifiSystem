#include "stdafx.h"
#include "MainWnd.h"
#include "RegisterUI.h"
#include "MatchUI.h"
#include "LogInUI.h"
#include "MonitoringUI.h"
#include "SettingUI.h"
#include "SignOutUI.h"
#include "SignInUI.h"


//#include "RegisterInfo.h"

#define BT_CLOSEMWND		(_T("bt_close"))
#define BT_RegisterWnd		(_T("bt_register"))
#define BT_MATCHWND			(_T("bt_match"))
#define BT_Monitoring		(_T("bt_monitoring"))
#define BT_SETTINGWND		(_T("bt_setting"))
#define BT_SIGNOUTWND		(_T("bt_SignOut"))

CMainWnd::CMainWnd()
{
}


CMainWnd::~CMainWnd()
{

}

DUI_BEGIN_MESSAGE_MAP(CMainWnd, WindowImplBase)
DUI_ON_CLICK_CTRNAME(BT_CLOSEMWND, OnCloseWnd)
DUI_ON_CLICK_CTRNAME(BT_RegisterWnd, OnRegisterWnd)
DUI_ON_CLICK_CTRNAME(BT_MATCHWND, OnMatchWnd)
DUI_ON_CLICK_CTRNAME(BT_Monitoring, OnMonitoringWnd)
DUI_ON_CLICK_CTRNAME(BT_SETTINGWND, OnSettingWnd)
DUI_ON_CLICK_CTRNAME(BT_SIGNOUTWND, OnSignOutWnd)
DUI_END_MESSAGE_MAP()

LPCTSTR CMainWnd::GetWindowClassName() const
{
	return _T("CMainWnd");
}

CDuiString CMainWnd::GetSkinFolder()
{
	return _T("skin");
}

CDuiString CMainWnd::GetSkinFile()
{
	return _T("xml\\MainWnd.xml");
}

void CMainWnd::OnFinalMessage(HWND hWnd)
{
	WindowImplBase::OnFinalMessage(hWnd);
}

void CMainWnd::Notify(TNotifyUI& msg)
{
	WindowImplBase::Notify(msg);
}

void CMainWnd::OnCloseWnd(TNotifyUI& msg)
{
	::PostQuitMessage(0L);
}

void CMainWnd::OnRegisterWnd(TNotifyUI& msg)
{
	std::auto_ptr<RegisterUI> pDlg(new RegisterUI);
	assert(pDlg.get());
	pDlg->Create(this->GetHWND(), NULL, UI_WNDSTYLE_FRAME, 0L, 1024, 768, 0, 0);
	pDlg->CenterWindow();
	pDlg->ShowModal();

}

void CMainWnd::OnMatchWnd(TNotifyUI& msg)
{
	std::auto_ptr<CSignInUI> pDlg(new CSignInUI());
	assert(pDlg.get());
	pDlg->Create(this->GetHWND(), NULL, UI_WNDSTYLE_FRAME, 0L, 1024, 768, 0, 0);
	pDlg->CenterWindow();
	pDlg->ShowModal();
}


void CMainWnd::OnMonitoringWnd(TNotifyUI& msg)
{
	std::auto_ptr<CMonitoringUI> pDlg(new CMonitoringUI);
	assert(pDlg.get());
	pDlg->Create(this->GetHWND(), NULL, UI_WNDSTYLE_FRAME, 0L, 1024, 768, 0, 0);
	pDlg->CenterWindow();
	pDlg->ShowModal();
}


void CMainWnd::OnSettingWnd(TNotifyUI& msg)
{
	std::auto_ptr<CSettingUI> pDlg(new CSettingUI);
	assert(pDlg.get());
	pDlg->Create(this->GetHWND(), NULL, UI_WNDSTYLE_FRAME, 0L, 1024, 768, 0, 0);
	pDlg->CenterWindow();
	pDlg->ShowModal();
}

void CMainWnd::OnSignOutWnd(TNotifyUI& msg)
{
	std::auto_ptr<CSignOutUI> pDlg(new CSignOutUI);
	assert(pDlg.get());
	pDlg->Create(this->GetHWND(), NULL, UI_WNDSTYLE_FRAME, 0L, 1024, 768, 0, 0);
	pDlg->CenterWindow();
	pDlg->ShowModal();
}

void CMainWnd::Show_Off_loginWnd()
{
	CButtonUI* btn1 = dynamic_cast<CButtonUI*>(m_PaintManager.FindControl(_T("bt_register")));
	CButtonUI* btn2 = dynamic_cast<CButtonUI*>(m_PaintManager.FindControl(_T("bt_setting")));
	CButtonUI* btn3 = dynamic_cast<CButtonUI*>(m_PaintManager.FindControl(_T("bt_close")));
	if (btn1->IsVisible())
	{
		btn1->SetVisible(false);
		btn2->SetVisible(false);
		btn3->SetVisible(false);
	}
	else
	{
		std::auto_ptr<CLogInUI> pDlg(new CLogInUI);
		assert(pDlg.get());
		pDlg->Create(this->GetHWND(), NULL, UI_WNDSTYLE_FRAME, 0L, 0, 0, 0, 0);
		pDlg->CenterWindow();
		pDlg->ShowModal();
		if (pDlg->m_LogResult == LogInSucceed)
		{
			btn1->SetVisible(true);
			btn2->SetVisible(true);
			btn3->SetVisible(true);
		}
	}
}


LRESULT CMainWnd::MessageHandler(UINT uMsg, WPARAM wParam, LPARAM /*lParam*/, bool& bHandled)
{
	if (uMsg == WM_KEYDOWN && wParam == VK_ESCAPE)
	{
		bHandled = true;
	}
	return FALSE;
}


LRESULT CMainWnd::HandleMessage(UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	/*else if (uMsg == WM_KEYDOWN)
	{
		if (wParam == 70 && !strHotkey.compare(_T("CTRL+SHIFT")))
		{
			Show_Off_loginWnd();
		}
		else if (wParam == VK_CONTROL && -1 == strHotkey.find(_T("CTRL")))
		{
			strHotkey = std::string(_T("CTRL")) + strHotkey;
		}
		else if (wParam == VK_SHIFT && -1 == strHotkey.find(_T("+SHIFT")))
		{
			strHotkey = strHotkey + std::string(_T("+SHIFT"));
		}
		return 0;
	}
	else if(uMsg == WM_KEYUP)
	{
		strHotkey.clear();
		return 0;
	}*/

	return __super::HandleMessage(uMsg, wParam, lParam);
}



