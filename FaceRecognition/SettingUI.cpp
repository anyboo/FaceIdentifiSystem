#include "stdafx.h"
#include "SettingUI.h"


CSettingUI::CSettingUI()
{
}


CSettingUI::~CSettingUI()
{
}

DUI_BEGIN_MESSAGE_MAP(CSettingUI, WindowImplBase)
DUI_ON_CLICK_CTRNAME(BT_CLOSESWnd, OnCloseSWnd)
DUI_END_MESSAGE_MAP()

LPCTSTR CSettingUI::GetWindowClassName() const
{
	return _T("SettingUI");
}

CDuiString CSettingUI::GetSkinFolder()
{
	return _T("skin");
}

CDuiString CSettingUI::GetSkinFile()
{
	return _T("xml\\SettingUI.xml");
}

void CSettingUI::OnFinalMessage(HWND hWnd)
{
	WindowImplBase::OnFinalMessage(hWnd);
}

void CSettingUI::Notify(TNotifyUI& msg)
{

	WindowImplBase::Notify(msg);
}

void CSettingUI::OnCloseSWnd(TNotifyUI& msg)
{
	Close();
}