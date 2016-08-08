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
:m_nBmp(0), r(new Camera), 
t(100, 1000), tc(*this, &MatchUI::onTimer)
, enableCompare(false), painting(true)
{
	_confirm->SetEnabled(false);
}


MatchUI::~MatchUI()
{
}


DUI_BEGIN_MESSAGE_MAP(MatchUI, WindowImplBase)
DUI_ON_CLICK_CTRNAME(BT_CLOSE_MatchWnd, Backward)
DUI_ON_CLICK_CTRNAME(BT_OK_FILISH, SignIn)
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

void MatchUI::beginTime()
{
	addObserver(*this);
	r.start();
	example.start();
	m_count = 0;
	t.start(tc);
}

void MatchUI::endTime()
{
	removeObserver(*this);
	r.stop();
	example.stop();
	t.stop();
}

void MatchUI::InitWindow()
{
	beginTime();
	BandingSubControl();
}

void MatchUI::BandingSubControl()
{
	_name			=	dynamic_cast<CLabelUI*>(m_PaintManager.FindControl(_T("Edit_Name")));
	_age			=	dynamic_cast<CLabelUI*>(m_PaintManager.FindControl(_T("Edit_Age")));
	_sex			=	dynamic_cast<CLabelUI*>(m_PaintManager.FindControl(_T("Edit_Sex")));
	_birth			=	dynamic_cast<CLabelUI*>(m_PaintManager.FindControl(_T("Edit_Birth")));
	_address		=	dynamic_cast<CLabelUI*>(m_PaintManager.FindControl(_T("Edit_Address")));
	_phone			=	dynamic_cast<CLabelUI*>(m_PaintManager.FindControl(_T("Edit_Phone")));
	_certificate	=	dynamic_cast<CLabelUI*>(m_PaintManager.FindControl(_T("Edit_IDnumber")));

	_matchMsg		=	dynamic_cast<CLabelUI*>(m_PaintManager.FindControl(_T("match_result")));
	_confirm			=	dynamic_cast<CButtonUI*>(m_PaintManager.FindControl(_T("Sign_In")));
}

void MatchUI::OnFinalMessage(HWND hWnd)
{
	endTime();
	WindowImplBase::OnFinalMessage(hWnd);
}

void MatchUI::Notify(TNotifyUI& msg)
{

	WindowImplBase::Notify(msg);
}

void MatchUI::Backward(TNotifyUI& msg)
{
	//m_IsSignIn = SignIn_CANCEL;
	Close();
}

void MatchUI::SignIn(TNotifyUI& msg)
{
	//m_IsSignIn = SignIn_OK;
	Close();
}

#include "Employee.h"

void MatchUI::ShowMatchInfo()
{
	Identity id;
	IdentityDB::Instance().Get(1, id);

	_name->SetText(id.name().c_str());
	_age->SetText(id.age().c_str());
	_sex->SetText(id.sex().c_str());
	_birth->SetText(id.birth().c_str());
	_address->SetText(id.address().c_str());
	_phone->SetText(id.phone().c_str());
	_certificate->SetText(id.certificate().c_str());

	CControlUI* Image = m_PaintManager.FindControl(_T("photo_video"));
	Image->SetBkImage(id.picutre_name().c_str());

	//Picture::Ptr userpic(new Picture(m_readInfo[0].get<9>().rawContent(), 640 * 480 * 3));
	//userpic->SetWidth(640);
	//userpic->SetHeight(480);
	//CControlUI* Image = m_PaintManager.FindControl(_T("photo_video"));
	//Util::DrawSomething(userpic, Image, GetHWND());

	_confirm->SetEnabled(true);
}

void MatchUI::handle1(Poco::Notification* pNf)
{
	if (!painting) return;

	poco_check_ptr(pNf);
	Notification::Ptr pf(pNf);
	if ((m_count % 5) == 0)
	{
		if (enableCompare)
		example.enqueueNotification(pf);
	}

	CaptureNotification::Ptr nf = pf.cast<CaptureNotification>();
	if (nf)
	{
		CurrentImage.assign(nf->data());
		CControlUI* Image = m_PaintManager.FindControl(_T("photo_wnd"));
		Util::DrawSomething(CurrentImage, Image, GetHWND());
	}
}

void MatchUI::onTimer(Poco::Timer& timer)
{
	enableCompare = !example.queryResult();
	if (!enableCompare)
	{
		painting = false;
		match_resulut();
	}
}


void MatchUI::match_resulut()
{
	std::string str = LangueConfig::GetShowText(6);
	_matchMsg->SetText(str.c_str());
	ShowMatchInfo();
}