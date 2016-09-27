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

#include "SettingConfig.h"
using namespace rapidjson;

#include "SettingConfig.h"

#define BT_SYSTEM_SET		(_T("btn_SysSet"))
#define BT_DEVICE_MANAGE	(_T("btn_Device"))
#define BT_WORKFACE_MANAGE	(_T("btn_Workfoce"))
#define BT_USER_MANAGE		(_T("btn_User"))

CSettingUI::CSettingUI()
{
	m_closeApp = true;
	_deviceManage.SetPaintMagager(&m_PaintManager);
	AddVirtualWnd(_T("DeviceManage"), &_deviceManage);
}


CSettingUI::~CSettingUI()
{
	RemoveVirtualWnd(_T("DeviceManage"));
}

DUI_BEGIN_MESSAGE_MAP(CSettingUI, WindowImplBase)
DUI_ON_CLICK_CTRNAME(BT_CLOSESWnd, OnCloseSWnd)
DUI_ON_CLICK_CTRNAME(BT_SAVELOG, OnSaveLog)
DUI_ON_CLICK_CTRNAME(BT_SAVECONFIG, OnSaveConfig)
DUI_ON_CLICK_CTRNAME(BT_RESETCONFIG, OnResetConfig)

DUI_ON_CLICK_CTRNAME(BT_SYSTEM_SET, OnSwitchWindow)
DUI_ON_CLICK_CTRNAME(BT_DEVICE_MANAGE, OnSwitchWindow)
DUI_ON_CLICK_CTRNAME(BT_WORKFACE_MANAGE, OnSwitchWindow)
DUI_ON_CLICK_CTRNAME(BT_USER_MANAGE, OnSwitchWindow)
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
	return _T("xml\\SystemManage.xml");
}

void CSettingUI::OnFinalMessage(HWND hWnd)
{
	WindowImplBase::OnFinalMessage(hWnd);
}

void CSettingUI::Notify(TNotifyUI& msg)
{

	WindowImplBase::Notify(msg);
}

void CSettingUI::InitWindow()
{
	CVerticalLayoutUI* Manage_Lyt = dynamic_cast<CVerticalLayoutUI*>(m_PaintManager.FindControl(_T("Manage_Lyt")));
	CDialogBuilder builder;
	CVerticalLayoutUI* SysSetWindow = (CVerticalLayoutUI*)(builder.Create(_T("xml//DeviceManage.xml"), (UINT)0, NULL, &m_PaintManager));
	Manage_Lyt->Add(SysSetWindow);
}

void CSettingUI::OnCloseSWnd(TNotifyUI& msg)
{
	m_closeApp = false;
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
	ofn.lpstrFile = szBuffer;			//save data path
	ofn.nMaxFile = sizeof(szBuffer) / sizeof(*szBuffer);
	ofn.nFilterIndex = 1;
	ofn.Flags = OFN_CREATEPROMPT | OFN_PATHMUSTEXIST | OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT;
	ofn.lpstrTitle = TEXT("保存到");
	BOOL bSel = GetSaveFileName(&ofn);
<<<<<<< HEAD
}


void CSettingUI::OnResetConfig(TNotifyUI& msg)
{
	std::string strTime(_T("30"));
	std::string strSiml(_T("0.6"));
	std::string strSize(_T("150"));

	ValueSetting setConfig;
	setConfig.SaveConfig(strTime, strSiml, strSize);

	ShowConfig();
	std::string str = LangueConfig::GetShowText(8);
	CLabelUI* lab = dynamic_cast<CLabelUI*>(m_PaintManager.FindControl(_T("result")));
	lab->SetText(str.c_str());
}


void CSettingUI::OnSaveConfig(TNotifyUI& msg)
{
	CComboUI* combo_time = dynamic_cast<CComboUI*>(m_PaintManager.FindControl(_T("combo_time")));
	CComboUI* combo_siml = dynamic_cast<CComboUI*>(m_PaintManager.FindControl(_T("combo_siml")));
	CComboUI* combo_FaceSize = dynamic_cast<CComboUI*>(m_PaintManager.FindControl(_T("combo_FaceSize")));
	std::string strTime = combo_time->GetText();
	std::string strSiml = combo_siml->GetText();
	std::string strSize = combo_FaceSize->GetText();

	ValueSetting setConfig;
	setConfig.SaveConfig(strTime, strSiml, strSize);
	std::string str = LangueConfig::GetShowText(7);
	CLabelUI* lab = dynamic_cast<CLabelUI*>(m_PaintManager.FindControl(_T("result")));
	lab->SetText(str.c_str());
}

void CSettingUI::ShowConfig()
{
	CComboUI* combo_time = dynamic_cast<CComboUI*>(m_PaintManager.FindControl(_T("combo_time")));
	CComboUI* combo_siml = dynamic_cast<CComboUI*>(m_PaintManager.FindControl(_T("combo_siml")));
	CComboUI* combo_FaceSize = dynamic_cast<CComboUI*>(m_PaintManager.FindControl(_T("combo_FaceSize")));
	std::string strTime, strSiml, strFacesize;
	ValueSetting setConfig;
	strTime = setConfig.GetTime_interval();
	strSiml = setConfig.GetSimilarity();
	strFacesize = setConfig.GetFaceSize();
	SetCombText(strTime, combo_time);
	SetCombText(strSiml, combo_siml);
	SetCombText(strFacesize, combo_FaceSize);
}

void CSettingUI::SetCombText(const std::string value, CComboUI* pComb)
{
	int numb = pComb->GetCount();
	for (int i = 0; i < numb; i++)
	{
		CListLabelElementUI* subList = dynamic_cast<CListLabelElementUI*>(m_PaintManager.FindSubControlByClass(pComb, DUI_CTR_LISTLABELELEMENT, i));
		std::string strTmp = subList->GetText();
		if (strTmp == value){
			subList->Select(true);
		}
	}
}

void CSettingUI::OnSwitchWindow(TNotifyUI& msg)
{
	CDuiString XmlFileName;
	if (msg.pSender->GetName() == _T("btn_Device")){
		XmlFileName.Append(_T("xml//DeviceManage.xml"));
	}
	else if (msg.pSender->GetName() == _T("btn_User")){
		XmlFileName.Append(_T("xml//UserManage.xml"));
	}
	else if(msg.pSender->GetName() == _T("btn_Workfoce")){
		XmlFileName.Append(_T("xml//WorkforceManage.xml"));
	}
	else if(msg.pSender->GetName() == _T("btn_SysSet")){
		XmlFileName.Append(_T("xml//SettingUI.xml"));
	}
	CVerticalLayoutUI* Manage_Lyt = dynamic_cast<CVerticalLayoutUI*>(m_PaintManager.FindControl(_T("Manage_Lyt")));
	CVerticalLayoutUI* subLyt = dynamic_cast<CVerticalLayoutUI*>(m_PaintManager.FindControl(_T("subwindow")));
	if (subLyt != nullptr)
		Manage_Lyt->Remove(subLyt);
	CDialogBuilder builder;
	CVerticalLayoutUI* NewSubWindow = (CVerticalLayoutUI*)(builder.Create(XmlFileName.GetData(), (UINT)0, NULL, &m_PaintManager));
	Manage_Lyt->Add(NewSubWindow);
	if (msg.pSender->GetName() == _T("btn_SysSet")){
		ShowConfig();
	}
	else if (msg.pSender->GetName() == _T("btn_User"))
	{
		CListUI* pList = dynamic_cast<CListUI*>(m_PaintManager.FindSubControlByName(NewSubWindow, _T("RepotList")));
		CDialogBuilder builder;
		CListContainerElementUI* ContList = (CListContainerElementUI*)(builder.Create(_T("xml//ReportListUI.xml"), (UINT)0, NULL, &m_PaintManager));
		pList->Add(ContList);
	}
}

LRESULT CSettingUI::HandleCustomMessage(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
	if (uMsg == WM_DESTROY && m_closeApp)
	{
		::ShowWindow(::FindWindow("Shell_TrayWnd", NULL), SW_SHOW);
		::PostQuitMessage(0);
	}
	bHandled = FALSE;
	return 0;
=======
>>>>>>> remotes/origin/TheFirstVersion
}