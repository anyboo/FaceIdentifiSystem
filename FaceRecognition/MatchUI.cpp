#include "stdafx.h"
#include "MatchUI.h"
#include "RegisterInfo.h"

MatchUI::MatchUI(CRegisterInfo* RegInfo)
:m_nBmp(0)
{
	m_RegInfo = RegInfo;
}


MatchUI::~MatchUI()
{
}


DUI_BEGIN_MESSAGE_MAP(MatchUI, WindowImplBase)
DUI_ON_CLICK_CTRNAME(BT_CLOSE_MatchWnd, OnCloseWnd)
DUI_ON_CLICK_CTRNAME(BT_OK_FILISH, OnFilishMatch)
DUI_END_MESSAGE_MAP()

LPCTSTR MatchUI::GetWindowClassName() const
{
	return _T("MatchUI");
}

CDuiString MatchUI::GetSkinFolder()
{
	return _T("skin");
}

CDuiString MatchUI::GetSkinFile()
{
	return _T("xml\\MatchUI.xml");
}

void MatchUI::InitWindow()
{
	::SetTimer(GetHWND(), 1, 50, nullptr);
}

void MatchUI::OnFinalMessage(HWND hWnd)
{
	WindowImplBase::OnFinalMessage(hWnd);
}

void MatchUI::Notify(TNotifyUI& msg)
{

	WindowImplBase::Notify(msg);
}

void MatchUI::OnCloseWnd(TNotifyUI& msg)
{
	Close();
}

void MatchUI::OnFilishMatch(TNotifyUI& msg)
{
	Close();
}

LRESULT MatchUI::OnTimer(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
	m_nBmp = m_nBmp + 1;
	CHorizontalLayoutUI* photo_Lyt = dynamic_cast<CHorizontalLayoutUI*>(m_PaintManager.FindControl(_T("photo_video")));

	std::string strName = std::string(_T("file = 'bmp/test")) + std::to_string(m_nBmp) + std::string(".bmp'");
	photo_Lyt->SetBkImage(strName.c_str());
	if (m_nBmp == 10)
	{
		::KillTimer(GetHWND(), 1);
		ShowMatchInfo();
	}
	return 0;
}

LRESULT MatchUI::HandleCustomMessage(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
	LRESULT lRes = 0;
	switch (uMsg)
	{
	case WM_TIMER: lRes = OnTimer(uMsg, wParam, lParam, bHandled); break;
	}
	bHandled = FALSE;
	return 0;
}

void MatchUI::ShowMatchInfo()
{
	if (m_RegInfo->GetSize() == 0)
		return;
	IdentityInfo* personInfo = m_RegInfo->Find(100001);
	CEditUI* edit_name = dynamic_cast<CEditUI*>(m_PaintManager.FindControl(_T("Edit_Name")));
	CEditUI* edit_age = dynamic_cast<CEditUI*>(m_PaintManager.FindControl(_T("Edit_Age")));
	CEditUI* edit_sex = dynamic_cast<CEditUI*>(m_PaintManager.FindControl(_T("Edit_Sex")));
	CEditUI* edit_birth = dynamic_cast<CEditUI*>(m_PaintManager.FindControl(_T("Edit_Birth")));
	CEditUI* edit_address = dynamic_cast<CEditUI*>(m_PaintManager.FindControl(_T("Edit_Address")));
	CEditUI* edit_phone = dynamic_cast<CEditUI*>(m_PaintManager.FindControl(_T("Edit_Phone")));
	CEditUI* edit_CertID = dynamic_cast<CEditUI*>(m_PaintManager.FindControl(_T("Edit_IDnumber")));
	CHorizontalLayoutUI* photo_Lyt = dynamic_cast<CHorizontalLayoutUI*>(m_PaintManager.FindControl(_T("photo_video")));


	edit_name->SetText(personInfo->strName.c_str());
	edit_age->SetText(personInfo->strAge.c_str());
	edit_sex->SetText(personInfo->strSex.c_str());
	edit_birth->SetText(personInfo->strBirth.c_str());
	edit_address->SetText(personInfo->strAdress.c_str());
	edit_phone->SetText(personInfo->strPhone.c_str());
	edit_CertID->SetText(personInfo->strCertID.c_str());
	photo_Lyt->SetBkImage(personInfo->strPhotoInfo.c_str());
}