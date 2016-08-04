#include "stdafx.h"
#include "MainWnd.h"
#include "RegisterUI.h"
#include "MatchUI.h"
#include "MonitoringUI.h"
#include "SettingUI.h"
#include "SignOutUI.h"
#include "LogInUI.h"

#include "RegisterInfo.h"

#include <windows.h>
#include <objbase.h>
#include <shellapi.h>

CMainWnd::CMainWnd()
{
	m_Mod = 0;
	m_IsSet = false;
	strHotkey.clear();
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
	Show_HideTask(false);
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
	std::auto_ptr<MatchUI> pDlg(new MatchUI());
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

void CMainWnd::Show_HideTask(bool IsHide)
{
	int nCwdShow = -1;
	LPARAM lParam;
	HWND task = FindWindow(_T("Shell_TrayWnd"), NULL);
	if (IsHide)
	{
		lParam = ABS_AUTOHIDE | ABS_ALWAYSONTOP;
		nCwdShow = SW_HIDE;
	}
	else
	{
		lParam = ABS_ALWAYSONTOP;
		nCwdShow = SW_SHOW;
	}

	::ShowWindow(task, nCwdShow);

	APPBARDATA apBar;
	memset(&apBar, 0, sizeof(apBar));
	apBar.cbSize = sizeof(apBar);
	apBar.hWnd = task;
	if (apBar.hWnd != NULL)
	{
		apBar.lParam = lParam;
		SHAppBarMessage(ABM_SETSTATE, &apBar);
	}
}

void CMainWnd::OnSignOutWnd(TNotifyUI& msg)
{
	std::auto_ptr<CSignOutUI> pDlg(new CSignOutUI);
	assert(pDlg.get());
	pDlg->Create(this->GetHWND(), NULL, UI_WNDSTYLE_FRAME, 0L, 1024, 768, 0, 0);
	pDlg->CenterWindow();
	pDlg->ShowModal();
}

void CMainWnd::InitWindow()
{
	int nRet = RegisterHotKey(m_hWnd, 1, MOD_CONTROL, 'f');
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
		if (uMsg == WM_KEYDOWN)
		{
			if (wParam != VK_CONTROL && wParam != VK_SHIFT && !strHotkey.compare(_T("CTRL+SHIFT")))
			{
				if (wParam == 70){
					std::auto_ptr<CLogInUI> pDlg(new CLogInUI);
					assert(pDlg.get());
					pDlg->Create(this->GetHWND(), NULL, UI_WNDSTYLE_FRAME, 0L, 0, 0, 0, 0);
					pDlg->CenterWindow();
					pDlg->ShowModal();
					if (pDlg->m_LogResult == LogInSucceed)
					{
						CButtonUI* btn1 = dynamic_cast<CButtonUI*>(m_PaintManager.FindControl(_T("bt_register")));
						CButtonUI* btn2 = dynamic_cast<CButtonUI*>(m_PaintManager.FindControl(_T("bt_setting")));
						CButtonUI* btn3 = dynamic_cast<CButtonUI*>(m_PaintManager.FindControl(_T("bt_close")));
						btn1->SetVisible(true);
						btn2->SetVisible(true);
						btn3->SetVisible(true);
					}
				}
			}
			else
			{
				if (wParam == VK_CONTROL && !strHotkey.compare(_T("SHIFT")))
				{
					strHotkey = std::string(_T("+CTRL")) + strHotkey;
				}
				else if (wParam == VK_CONTROL && strHotkey == _T(""))
				{
					strHotkey = std::string(_T("CTRL"));
				}
				if(wParam == VK_SHIFT && !strHotkey.compare(_T("CTRL")))
				{
					strHotkey = strHotkey + std::string(_T("+SHIFT"));
				}
				else if (wParam == VK_SHIFT && strHotkey == _T(""))
				{
					strHotkey = std::string(_T("SHIFT"));
				}
			}
			return 0;
		}
		else if(uMsg == WM_KEYUP)
		{
			strHotkey.clear();
			return 0;
		}

	return __super::HandleMessage(uMsg, wParam, lParam);
}



