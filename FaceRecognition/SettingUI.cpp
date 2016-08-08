#include "stdafx.h"
#include "SettingUI.h"
#include "CommDlg.h"

CSettingUI::CSettingUI()
{
}


CSettingUI::~CSettingUI()
{
}

DUI_BEGIN_MESSAGE_MAP(CSettingUI, WindowImplBase)
DUI_ON_CLICK_CTRNAME(BT_CLOSESWnd, OnCloseSWnd)
DUI_ON_CLICK_CTRNAME(BT_SAVELOG, OnSaveLog)
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


void CSettingUI::OnSaveLog(TNotifyUI& msg)
{
	TCHAR szBuffer[MAX_PATH] = { 0 };

	OPENFILENAME  ofn = { 0 };
	ofn.lStructSize = sizeof(ofn);
	ofn.hwndOwner = m_hWnd;
	ofn.lpstrFilter = _T("TXT文件(*.txt)\0*.txt\0所有文件(*.*)\0*.*\0");
	ofn.lpstrInitialDir = _T("D:\\");   
	ofn.lpstrFile = szBuffer;   
	ofn.nMaxFile = sizeof(szBuffer) / sizeof(*szBuffer);
	ofn.nFilterIndex = 1;
	ofn.Flags = OFN_CREATEPROMPT | OFN_PATHMUSTEXIST | OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT;
	ofn.lpstrTitle = TEXT("保存到");
	BOOL bSel = GetSaveFileName(&ofn);
}