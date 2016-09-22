#include "stdafx.h"
#include "LogInUI.h"
#include "SettingConfig.h"

CLogInUI::CLogInUI()
{

}


CLogInUI::~CLogInUI()
{
}

DUI_BEGIN_MESSAGE_MAP(CLogInUI, WindowImplBase)
DUI_ON_CLICK_CTRNAME(BT_CLOSE_WND, OnCloseSWnd)
DUI_ON_CLICK_CTRNAME(BT_LOGIN, OnLogIn)
DUI_END_MESSAGE_MAP()

LPCTSTR CLogInUI::GetWindowClassName() const
{
	return _T("CLogInUI");
}

CDuiString CLogInUI::GetSkinFolder()
{
	return _T("skin");
}

CDuiString CLogInUI::GetSkinFile()
{
	return _T("xml\\LogInUI.xml");
}

void CLogInUI::OnFinalMessage(HWND hWnd)
{
	WindowImplBase::OnFinalMessage(hWnd);
}

void CLogInUI::Notify(TNotifyUI& msg)
{

	WindowImplBase::Notify(msg);
}

void CLogInUI::OnCloseSWnd(TNotifyUI& msg)
{
	m_LogResult = LogInFailure;
	Close();
}

void CLogInUI::OnLogIn(TNotifyUI& msg)
{
	CEditUI* edit_pswd = dynamic_cast<CEditUI*>(m_PaintManager.FindControl(_T("edit_pswd")));
	CLabelUI* lab_log = dynamic_cast<CLabelUI*>(m_PaintManager.FindControl(_T("warning")));
	std::string str = edit_pswd->GetText();
	if (str.compare(_T("123456")) == 0)
	{
		m_LogResult = LogInSucceed;
		Close();
	}
	else
	{
		std::string result = LangueConfig::GetShowText(5);
		lab_log->SetText(result.c_str());
	}
}


