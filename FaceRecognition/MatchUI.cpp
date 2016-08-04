#include "stdafx.h"
#include "MatchUI.h"
#include "RegisterInfo.h"

MatchUI::MatchUI()
:m_nBmp(0)
{

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
	edit_address->SetText(personInfo->strIDcard.c_str());
	edit_phone->SetText(personInfo->strPhone.c_str());
	edit_CertID->SetText(personInfo->strCertID.c_str());
	photo_Lyt->SetBkImage(personInfo->strPhotoInfo.c_str());
}