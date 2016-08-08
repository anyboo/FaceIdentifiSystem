#include "stdafx.h"
#include "SignOutUI.h"
#include "RegisterInfo.h"
#include "Util.h"
#include "CaptureNotification.h"
#include "LangueConfig.h"

#include "RegUserInfo.h"
#include <vector>

CSignOutUI::CSignOutUI()
:m_nBmp(0), r(new Camera)
, t(100, 1000), tc(*this, &CSignOutUI::onTimer)
, enableCompare(false), painting(true)
{
}


CSignOutUI::~CSignOutUI()
{
}


DUI_BEGIN_MESSAGE_MAP(CSignOutUI, WindowImplBase)
DUI_ON_CLICK_CTRNAME(BT_CLOSE_MatchWnd, OnCloseWnd)
DUI_ON_CLICK_CTRNAME(BT_SIGN_OUT, OnFilishMatch)
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

void CSignOutUI::beginTime()
{
	addObserver(*this);
	r.start();
	example.start();
	m_count = 0;
	t.start(tc);
}

void CSignOutUI::endTime()
{
	removeObserver(*this);
	r.stop();
	example.stop();
	t.stop();
}

void CSignOutUI::InitWindow()
{
	beginTime();
}

void CSignOutUI::OnFinalMessage(HWND hWnd)
{
	endTime();
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
	r.stop();
	int n = example.queryPerson();
	std::vector<readUserInfo> m_readInfo = RegUserInfo::getUserInfo();
	std::string strName = m_readInfo[n].get<1>();
	int age = m_readInfo[n].get<2>();
	std::string strAge = std::to_string(age);
	std::string strSex = m_readInfo[n].get<3>();
	std::string strBirth = m_readInfo[n].get<4>();
	std::string strIDcard = m_readInfo[n].get<5>();
	std::string strPhone = m_readInfo[n].get<6>();
	std::string strCertID = m_readInfo[n].get<7>();

	CLabelUI* edit_name = dynamic_cast<CLabelUI*>(m_PaintManager.FindControl(_T("Edit_Name")));
	CLabelUI* edit_age = dynamic_cast<CLabelUI*>(m_PaintManager.FindControl(_T("Edit_Age")));
	CLabelUI* edit_sex = dynamic_cast<CLabelUI*>(m_PaintManager.FindControl(_T("Edit_Sex")));
	CLabelUI* edit_birth = dynamic_cast<CLabelUI*>(m_PaintManager.FindControl(_T("Edit_Birth")));
	CLabelUI* edit_address = dynamic_cast<CLabelUI*>(m_PaintManager.FindControl(_T("Edit_Address")));
	CLabelUI* edit_phone = dynamic_cast<CLabelUI*>(m_PaintManager.FindControl(_T("Edit_Phone")));
	CLabelUI* edit_CertID = dynamic_cast<CLabelUI*>(m_PaintManager.FindControl(_T("Edit_IDnumber")));

	edit_name->SetText(strName.c_str());
	edit_age->SetText(strAge.c_str());
	edit_sex->SetText(strSex.c_str());
	edit_birth->SetText(strBirth.c_str());
	edit_address->SetText(strIDcard.c_str());
	edit_phone->SetText(strPhone.c_str());
	edit_CertID->SetText(strCertID.c_str());

	Picture::Ptr userpic(new Picture(m_readInfo[n].get<9>().rawContent(), 640 * 480 * 3));
	userpic->SetWidth(640);
	userpic->SetHeight(480);

	CControlUI* Image = m_PaintManager.FindControl(_T("photo_video"));
	Util::DrawSomething(userpic, Image, GetHWND());

	Sleep(2000);
	CButtonUI* btn = dynamic_cast<CButtonUI*>(m_PaintManager.FindControl(_T("Sign_out")));
	btn->SetEnabled(true);
}

void CSignOutUI::handle1(Poco::Notification* pNf)
{
	if (!painting) return;
	poco_check_ptr(pNf);
	Notification::Ptr pf(pNf);
	if ((m_count % 5) == 0)
	{
		if (enableCompare)
		example.enqueueNotification(pf);
	}
	m_count++;

	CaptureNotification::Ptr nf = pf.cast<CaptureNotification>();
	Picture::Ptr pic(nf->data());
	CControlUI* Image = m_PaintManager.FindControl(_T("photo_video"));
	Util::DrawSomething(pic, Image, GetHWND());
}

void CSignOutUI::match_resulut()
{
	CButtonUI* btn = dynamic_cast<CButtonUI*>(m_PaintManager.FindControl(_T("Sign_out")));
	CLabelUI* lab = dynamic_cast<CLabelUI*>(m_PaintManager.FindControl(_T("match_result")));
	std::string str = LangueConfig::GetShowText(6);
	lab->SetText(str.c_str());
	btn->SetEnabled(true);
	ShowMatchInfo();
}

void CSignOutUI::onTimer(Poco::Timer& timer)
{
	enableCompare = !example.queryResult();
	if (!enableCompare)
	{
		painting = false;
		match_resulut();
		//t.stop();
	}
}