#include "stdafx.h"
#include "MonitoringUI.h"


CMonitoringUI::CMonitoringUI()
{
}


CMonitoringUI::~CMonitoringUI()
{
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
	WindowImplBase::OnFinalMessage(hWnd);
}

void CMonitoringUI::Notify(TNotifyUI& msg)
{

	WindowImplBase::Notify(msg);
}

void CMonitoringUI::OnCloseWnd(TNotifyUI& msg)
{
	Close();
}
