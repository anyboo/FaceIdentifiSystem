#include "stdafx.h"
#include "SettingUI.h"
#include "CommDlg.h"

//#include "document.h"
//#include "prettywriter.h"
//#include "stringbuffer.h"
//
//#include "ostreamwrapper.h"
//#include "istreamwrapper.h"

#include <string>
#include <tchar.h>
#include <windows.h>
#include <fstream>
#include "DeviceUI.h"
#include "ScheduleUI.h"
#include "UserUI.h"
#include "SystemUI.h"
#include "SettingConfig.h"

#define BT_SYSTEM_SET		(_T("btn_SysSet"))
#define BT_DEVICE_MANAGE	(_T("btn_Device"))
#define BT_WORKFACE_MANAGE	(_T("btn_Workfoce"))
#define BT_USER_MANAGE		(_T("btn_User"))

#define BT_CLOSESWnd		(_T("close_btn2"))
#define BT_SAVELOG			(_T("saveLog"))
#define BT_SAVECONFIG		(_T("saveConfig"))
#define BT_RESETCONFIG		(_T("resetConfig"))

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
	return _T("CSettingUI");
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
	DUITRACE("%s %s",msg.pSender->GetName().GetData() ,msg.sType);
	OnSwitch(msg);
	WindowImplBase::Notify(msg);
}

CControlUI* CSettingUI::CreateControl(LPCTSTR pstrClass)
{
	if (_tcscmp(pstrClass, _T("DeviceUI")) == 0) return new DeviceUI;
	if (_tcscmp(pstrClass, _T("UserUI")) == 0) return new UserUI;
	if (_tcscmp(pstrClass, _T("ScheduleUI")) == 0) return new ScheduleUI;
	if (_tcscmp(pstrClass, _T("SystemUI")) == 0) return new SystemUI;
	return NULL;
}

void CSettingUI::OnCloseSWnd(TNotifyUI& msg)
{
	Close();
}

void CSettingUI::OnSwitch(TNotifyUI& msg)
{
	CDuiString name = msg.pSender->GetName();
	CTabLayoutUI* pControl = static_cast<CTabLayoutUI*>(m_PaintManager.FindControl(_T("switch")));
	if (name == _T("device"))
		pControl->SelectItem(0);
	else if (name == _T("user"))
		pControl->SelectItem(1);
	else if (name == _T("schedule"))
		pControl->SelectItem(2);
	else if (name == _T("system"))
		pControl->SelectItem(3);
	else if (name == _T("other"))
		pControl->SelectItem(4);
}

void CSettingUI::OnSaveLog(TNotifyUI& msg)
{
	TCHAR szBuffer[MAX_PATH] = { 0 };

	OPENFILENAME  ofn = { 0 };
	ofn.lStructSize = sizeof(ofn);
	ofn.hwndOwner = m_hWnd;
	ofn.lpstrFilter = _T("TXT文件(*.txt)\0*.txt\0所有文件(*.*)\0*.*\0");
	ofn.lpstrInitialDir = _T("D:\\");   
	ofn.lpstrFile = szBuffer;			//save data path
	ofn.nMaxFile = sizeof(szBuffer) / sizeof(*szBuffer);
	ofn.nFilterIndex = 1;
	ofn.Flags = OFN_CREATEPROMPT | OFN_PATHMUSTEXIST | OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT;
	ofn.lpstrTitle = TEXT("save to file");
	BOOL bSel = GetSaveFileName(&ofn);
}