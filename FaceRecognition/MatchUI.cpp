#include "stdafx.h"
#include "MatchUI.h"
#include "RegisterInfo.h"
#include "CaptureNotification.h"
#include "Camera.h"
#include "Util.h"

#include "SettingConfig.h"
#include "RegUserInfo.h"
#include <vector>

#include "Mmsystem.h"

MatchUI::MatchUI()
:m_nBmp(0), recoder(new Camera),
checktime(100, 1000), tc(*this, &MatchUI::onTimer)
, enableCompare(false), painting(true)
{
	m_closeApp = true;
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
	recoder.start();
	example.start();
	m_count = 0;
	checktime.start(tc);
}

void MatchUI::endTime()
{
	removeObserver(*this);
	recoder.stop();
	example.stop();
	checktime.restart(0);
}

void MatchUI::InitWindow()
{
	beginTime();
	BandingSubControl();
	_confirm->SetEnabled(false);
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
	m_closeApp = false;
	Close();
}

void MatchUI::SignIn(TNotifyUI& msg)
{
	PlaySoundA(_T("QD.wav"), NULL, SND_FILENAME | SND_ASYNC);
	m_closeApp = false;
	Close();
}

#include "Employee.h"

void MatchUI::ShowMatchInfo()
{

	//r.stop();
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
	CButtonUI* btn_SignIn = dynamic_cast<CButtonUI*>(m_PaintManager.FindControl(_T("Sign_In")));
	hLyt->SetBkImage(imageName.c_str());
	btn_SignIn->SetEnabled(true);

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
	//Image->SetBkImage(id.picutre_name().c_str());

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
	if ((m_count % 5) == 0 && enableCompare)
	{
		example.enqueueNotification(pf);
	}


	CaptureNotification::Ptr nf = pf.cast<CaptureNotification>();
	if (nf)
	{
		CurrentImage.assign(nf->data());
		CControlUI* Image = m_PaintManager.FindControl(_T("photo_video"));
		Util::DrawSomething(CurrentImage, Image, GetHWND());
	}
}


void MatchUI::onTimer(Poco::Timer& timer)
{
	enableCompare = !enableCompare;

	std::stringstream ss;
	ss << "enableCompare" << enableCompare << std::endl;

	OutputDebugStringA(ss.str().c_str());
	if (!enableCompare)
	{
		if (example.queryResult())
		{
			match_resulut();
		}
	}
	/* 
	= !example.queryResult();
	if (!enableCompare)
	{
		painting = false;
		match_resulut();
		endTime();
	}
	*/
}


void MatchUI::match_resulut()
{
	std::string str = LangueConfig::GetShowText(6);
	_matchMsg->SetText(str.c_str());
	ShowMatchInfo();
}


LRESULT MatchUI::HandleCustomMessage(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
	if (uMsg == WM_DESTROY && m_closeApp)
	{
		::ShowWindow(::FindWindow("Shell_TrayWnd", NULL), SW_SHOW);
		::PostQuitMessage(0);
	}
	bHandled = FALSE;
	return 0;
}