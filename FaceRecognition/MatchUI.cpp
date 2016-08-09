#include "stdafx.h"
#include "MatchUI.h"
#include "RegisterInfo.h"
#include "CaptureNotification.h"
#include "Camera.h"
#include "Util.h"

#include "LangueConfig.h"
#include "RegUserInfo.h"
#include <vector>

#include "Mmsystem.h"

MatchUI::MatchUI()
:m_nBmp(0), r(new Camera), 
t(100, 1000), tc(*this, &MatchUI::onTimer)
, enableCompare(false), painting(true)
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

void MatchUI::OnCloseWnd(TNotifyUI& msg)
{
	//m_IsSignIn = SignIn_CANCEL;
	Close();
}

void MatchUI::OnFilishMatch(TNotifyUI& msg)
{
	//m_IsSignIn = SignIn_OK;
	PlaySoundA(_T("QD.wav"), NULL, SND_FILENAME | SND_ASYNC);
	Close();
}

void MatchUI::ShowMatchInfo()
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

	Picture::Ptr userpic(new Picture(m_readInfo[n].get<9>().rawContent(), width * height * magic));
	userpic->SetWidth(width);
	userpic->SetHeight(height);
	
	std::string path = CPaintManagerUI::GetInstancePath();
	std::string imageName = userpic->out2bmp(path);

	CHorizontalLayoutUI* hLyt = dynamic_cast<CHorizontalLayoutUI*>(m_PaintManager.FindControl(_T("photo_video")));
	hLyt->SetBkImage(imageName.c_str());


	Sleep(2000);
	CButtonUI* btn_SignIn = dynamic_cast<CButtonUI*>(m_PaintManager.FindControl(_T("Sign_In")));
	btn_SignIn->SetEnabled(true);
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
	m_count++;
	CaptureNotification::Ptr nf = pf.cast<CaptureNotification>();
	Picture::Ptr pic(nf->data());
	CControlUI* Image = m_PaintManager.FindControl(_T("photo_video"));
	Util::DrawSomething(pic, Image, GetHWND());
}

IsSignIn MatchUI::GetResult()
{
	return m_IsSignIn;
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
	CLabelUI* lab = dynamic_cast<CLabelUI*>(m_PaintManager.FindControl(_T("match_result")));
	std::string str = LangueConfig::GetShowText(6);
	lab->SetText(str.c_str());
	ShowMatchInfo();
}


LRESULT MatchUI::HandleCustomMessage(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
	if (uMsg == WM_DESTROY)
	{
		::PostQuitMessage(0);
	}
	bHandled = FALSE;
	return 0;
}