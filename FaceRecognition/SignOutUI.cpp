#include "stdafx.h"
#include "SignOutUI.h"
#include "RegisterInfo.h"
#include "Util.h"
#include "CaptureNotification.h"

CSignOutUI::CSignOutUI()
:m_nBmp(0), r(new Camera)
{
}


CSignOutUI::~CSignOutUI()
{
}


DUI_BEGIN_MESSAGE_MAP(CSignOutUI, WindowImplBase)
DUI_ON_CLICK_CTRNAME(BT_CLOSE_MatchWnd, OnCloseWnd)
DUI_ON_CLICK_CTRNAME(BT_OK_FILISH, OnFilishMatch)
DUI_END_MESSAGE_MAP()

LPCTSTR CSignOutUI::GetWindowClassName() const
{
	return _T("CSignOutUI");
}

CDuiString CSignOutUI::GetSkinFolder()
{
	return _T("skin");
}

CDuiString CSignOutUI::GetSkinFile()
{
	return _T("xml\\SignOutUI.xml");
}

void CSignOutUI::InitWindow()
{
	addObserver(*this);
	r.start();
	example.start();
}

void CSignOutUI::OnFinalMessage(HWND hWnd)
{
	removeObserver(*this);
	r.stop();
	example.stop();

	WindowImplBase::OnFinalMessage(hWnd);
}

void CSignOutUI::Notify(TNotifyUI& msg)
{

	WindowImplBase::Notify(msg);
}

void CSignOutUI::OnCloseWnd(TNotifyUI& msg)
{
	Close();
}

void CSignOutUI::OnFilishMatch(TNotifyUI& msg)
{
	Close();
}

void CSignOutUI::ShowMatchInfo()
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

void CSignOutUI::handle1(Poco::Notification* pNf)
{
	poco_check_ptr(pNf);
	Notification::Ptr pf(pNf);
	poco_check_ptr(pf.get());
	example.enqueueNotification(pf);

	CaptureNotification::Ptr nf = pf.cast<CaptureNotification>();
	poco_check_ptr(nf.get());
	Picture::Ptr pic(nf->data());
	poco_check_ptr(pic.get());

	CControlUI* Image = m_PaintManager.FindControl(_T("photo_video"));
	Util::DrawSomething(pic, Image, GetHWND());
}