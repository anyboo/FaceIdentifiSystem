#include "stdafx.h"
#include "SettingUI.h"
#include "CommDlg.h"

#include "document.h"
#include "prettywriter.h"
#include "stringbuffer.h"

#include "ostreamwrapper.h"
#include "istreamwrapper.h"

#include <string>
#include <tchar.h>
#include <windows.h>
#include <fstream>
using namespace rapidjson;

#include "SettingConfig.h"

CSettingUI::CSettingUI()
{
	m_closeApp = true;
}


CSettingUI::~CSettingUI()
{
}

DUI_BEGIN_MESSAGE_MAP(CSettingUI, WindowImplBase)
DUI_ON_CLICK_CTRNAME(BT_CLOSESWnd, OnCloseSWnd)
DUI_ON_CLICK_CTRNAME(BT_SAVELOG, OnSaveLog)
DUI_ON_CLICK_CTRNAME(BT_SAVECONFIG, OnSaveConfig)
DUI_ON_CLICK_CTRNAME(BT_RESETCONFIG, OnResetConfig)
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
	ofn.lpstrFile = szBuffer;   
	ofn.nMaxFile = sizeof(szBuffer) / sizeof(*szBuffer);
	ofn.nFilterIndex = 1;
	ofn.Flags = OFN_CREATEPROMPT | OFN_PATHMUSTEXIST | OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT;
	ofn.lpstrTitle = TEXT("保存到");
	BOOL bSel = GetSaveFileName(&ofn);
}


void CSettingUI::OnResetConfig(TNotifyUI& msg)
{
	std::string strTime(_T("30"));
	std::string strSiml(_T("0.6"));
	std::string strSize(_T("150"));
	WriteConfig(strTime, strSiml, strSize);

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

	WriteConfig(strTime, strSiml, strSize);
	std::string str = LangueConfig::GetShowText(7);
	CLabelUI* lab = dynamic_cast<CLabelUI*>(m_PaintManager.FindControl(_T("result")));
	lab->SetText(str.c_str());

}

void CSettingUI::WriteConfig(std::string& timeD, std::string& siml, std::string& facesize)
{
	std::string configFile;
	TCHAR PATH[MAX_PATH] = { 0 };
	std::string AppPath = std::string(PATH, ::GetModuleFileNameA(NULL, PATH, MAX_PATH));
	configFile = AppPath.substr(0, AppPath.find_last_of("\\") + 1) + std::string(_T("Setting.json"));

	ofstream ofs(configFile);
	locale utf8;
	ofs.imbue(utf8);
	OStreamWrapper osw(ofs);
	Document d;

	Document::AllocatorType& alloc = d.GetAllocator();
	Value root(kObjectType);
	Value t(kObjectType), s(kObjectType), f(kObjectType);
	t.SetString(timeD.c_str(), timeD.length(), alloc);
	s.SetString(siml.c_str(), siml.length(), alloc);
	f.SetString(facesize.c_str(), facesize.length(), alloc);

	root.AddMember(_T("time_interval"), t, alloc);
	root.AddMember(_T("similarity"), s, alloc);
	root.AddMember(_T("MinFaceSize"), f, alloc);

	Writer<OStreamWrapper> writer(osw);
	root.Accept(writer);
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
}