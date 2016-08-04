#include "stdafx.h"
#include "WaittingUI.h"
#include "MonitoringUI.h"

CWaittingUI::CWaittingUI()
{
}


CWaittingUI::~CWaittingUI()
{
	KillTimer(GetHWND(), 1);
}

DUI_BEGIN_MESSAGE_MAP(CWaittingUI, WindowImplBase)
DUI_END_MESSAGE_MAP()

LPCTSTR CWaittingUI::GetWindowClassName() const
{
	return _T("CWaittingUI");
}

CDuiString CWaittingUI::GetSkinFolder()
{
	return _T("skin");
}

CDuiString CWaittingUI::GetSkinFile()
{
	return _T("xml\\WaittingUI.xml");
}

void CWaittingUI::OnFinalMessage(HWND hWnd)
{
	WindowImplBase::OnFinalMessage(hWnd);
}

void CWaittingUI::Notify(TNotifyUI& msg)
{
	
	WindowImplBase::Notify(msg);
}

void CWaittingUI::InitWindow()
{
	SetTimer(GetHWND(), 1, 3000, nullptr);
}

LRESULT CWaittingUI::HandleCustomMessage(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
	LRESULT lRes = 0;
	switch (uMsg)
	{
	case WM_TIMER: lRes = OnTimer(uMsg, wParam, lParam, bHandled); break;
	}
	bHandled = FALSE;
	return 0;
}

LRESULT CWaittingUI::OnTimer(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
	Close();
	return 0;
}
