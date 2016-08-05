#include "stdafx.h"
#include "MatchUI.h"
#include "RegisterInfo.h"
#include "CaptureNotification.h"
#include "Camera.h"
#include "Util.h"

#include "LangueConfig.h"
#include "RegUserInfo.h"
#include <vector>

MatchUI::MatchUI()
:m_nBmp(0), r(new Camera)
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
	SetTimer(GetHWND(), 1, 1000, nullptr);
	addObserver(*this);
	r.start();
	example.start();
	m_count = 0;
}

void MatchUI::OnFinalMessage(HWND hWnd)
{
	removeObserver(*this);
	r.stop();
	example.stop();

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
	r.stop();
	std::vector<readUserInfo> m_readInfo = RegUserInfo::getUserInfo();
	
	CButtonUI* btn_SignIn = dynamic_cast<CButtonUI*>(m_PaintManager.FindControl(_T("Sign_In")));
	CEditUI* edit_name = dynamic_cast<CEditUI*>(m_PaintManager.FindControl(_T("Edit_Name")));
	CEditUI* edit_age = dynamic_cast<CEditUI*>(m_PaintManager.FindControl(_T("Edit_Age")));
	CEditUI* edit_sex = dynamic_cast<CEditUI*>(m_PaintManager.FindControl(_T("Edit_Sex")));
	CEditUI* edit_birth = dynamic_cast<CEditUI*>(m_PaintManager.FindControl(_T("Edit_Birth")));
	CEditUI* edit_address = dynamic_cast<CEditUI*>(m_PaintManager.FindControl(_T("Edit_Address")));
	CEditUI* edit_phone = dynamic_cast<CEditUI*>(m_PaintManager.FindControl(_T("Edit_Phone")));
	CEditUI* edit_CertID = dynamic_cast<CEditUI*>(m_PaintManager.FindControl(_T("Edit_IDnumber")));

	std::string strName = m_readInfo[0].get<1>();
	int age = m_readInfo[0].get<2>();
	std::string strAge = std::to_string(age);
	std::string strSex = m_readInfo[0].get<3>();
	std::string strBirth = m_readInfo[0].get<4>();
	std::string strIDcard = m_readInfo[0].get<5>();
	std::string strPhone = m_readInfo[0].get<6>();
	std::string strCertID = m_readInfo[0].get<7>();

	Picture::Ptr userpic(new Picture(m_readInfo[0].get<9>().rawContent(), 640 * 480 * 3));
	userpic->SetWidth(640);
	userpic->SetHeight(480);

	CControlUI* Image = m_PaintManager.FindControl(_T("photo_video"));
	
	Util::DrawSomething(userpic, Image, GetHWND());

	btn_SignIn->SetEnabled(true);
	edit_name->SetText(strName.c_str());
	edit_age->SetText(strAge.c_str());
	edit_sex->SetText(strSex.c_str());
	edit_birth->SetText(strBirth.c_str());
	edit_address->SetText(strIDcard.c_str());
	edit_phone->SetText(strPhone.c_str());
	edit_CertID->SetText(strCertID.c_str());
}

void MatchUI::handle1(Poco::Notification* pNf)
{
	poco_check_ptr(pNf);
	Notification::Ptr pf(pNf);
	poco_check_ptr(pf.get());
	if ((m_count % 5) == 0)
	{
		example.enqueueNotification(pf);
	}
	m_count++;

	CaptureNotification::Ptr nf = pf.cast<CaptureNotification>();
	poco_check_ptr(nf.get());
	Picture::Ptr pic(nf->data());
	poco_check_ptr(pic.get());

	CControlUI* Image = m_PaintManager.FindControl(_T("photo_video"));
	Util::DrawSomething(pic, Image, GetHWND());
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

LRESULT MatchUI::OnTimer(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
	if (example.queryResult())
	{
		CLabelUI* lab = dynamic_cast<CLabelUI*>(m_PaintManager.FindControl(_T("match_result")));
		std::string str = LangueConfig::GetShowText(6);
		lab->SetText(str.c_str());
		ShowMatchInfo();
		KillTimer(GetHWND(), 1);
	}
	return 0;
}