#include "stdafx.h"
#include "RegisterUI.h"


RegisterUI::RegisterUI()
{
}


RegisterUI::~RegisterUI()
{
}


DUI_BEGIN_MESSAGE_MAP(RegisterUI, WindowImplBase)
DUI_ON_CLICK_CTRNAME(BT_CLOSERWND, OnCloseRWnd)
DUI_END_MESSAGE_MAP()

LPCTSTR RegisterUI::GetWindowClassName() const
{
	return _T("RegisterUI");
}

CDuiString RegisterUI::GetSkinFolder()
{
	return _T("skin");
}

CDuiString RegisterUI::GetSkinFile()
{
	return _T("xml\\RegisterUI.xml");
}

void RegisterUI::OnFinalMessage(HWND hWnd)
{
	WindowImplBase::OnFinalMessage(hWnd);
}

void RegisterUI::Notify(TNotifyUI& msg)
{

	WindowImplBase::Notify(msg);
}

void RegisterUI::OnCloseRWnd(TNotifyUI& msg)
{
	Close();
}
