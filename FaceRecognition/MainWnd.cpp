#include "stdafx.h"
#include "MainWnd.h"
#include "RegisterUI.h"
#include "MatchUI.h"
#include "MonitoringUI.h"
#include "SettingUI.h"

#include "RegisterInfo.h"

#include <windows.h>
#include <objbase.h>
#include <shellapi.h>

CMainWnd::CMainWnd()
{
	m_RegInfo = new CRegisterInfo;
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
	std::auto_ptr<MatchUI> pDlg(new MatchUI(m_RegInfo));
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