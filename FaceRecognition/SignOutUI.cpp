#include "stdafx.h"
#include "SignOutUI.h"
#include "RegisterInfo.h"
#include "Util.h"
#include "CaptureNotification.h"
#include "SettingConfig.h"

#include "RegUserInfo.h"
#include <vector>

#include "Mmsystem.h"
#include "Picture.h"



CSignOutUI::CSignOutUI()
:m_nBmp(0), r(new Camera)
, t(100, 1000), tc(*this, &CSignOutUI::onTimer)
, enableCompare(false), painting(true)
{
	m_closeApp = true;
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
	t.restart(0); 
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
	m_closeApp = false;
	Close();
}

void CSignOutUI::OnFilishMatch(TNotifyUI& msg)
{
	PlaySoundA(_T("QT.wav"), NULL, SND_FILENAME | SND_ASYNC);
	m_closeApp = false;
	Close();
}

void CSignOutUI::ShowMatchInfo()
{
	//r.stop();
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

	CButtonUI* btn_SignIn = dynamic_cast<CButtonUI*>(m_PaintManager.FindControl(_T("Sign_out")));
	btn_SignIn->SetEnabled(true);
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
	CLabelUI* lab = dynamic_cast<CLabelUI*>(m_PaintManager.FindControl(_T("match_result")));
	std::string str = LangueConfig::GetShowText(6);
	lab->SetText(str.c_str());
	ShowMatchInfo();
}

void CSignOutUI::onTimer(Poco::Timer& timer)
{
	enableCompare = !example.queryResult();
	if (!enableCompare)
	{
		painting = false;
		match_resulut();
		endTime();
	}
}

LRESULT CSignOutUI::HandleCustomMessage(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
	if (uMsg == WM_DESTROY && m_closeApp)
	{
		::ShowWindow(::FindWindow("Shell_TrayWnd", NULL), SW_SHOW);
		::PostQuitMessage(0);
	}
	bHandled = FALSE;
	return 0;
}

