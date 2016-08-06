#include "stdafx.h"
#include "RegisterUI.h"
#include "CaptureNotification.h"
#include "Camera.h"
#include "Util.h"
#include "LangueConfig.h"
#include "ClipUI.h"


RegisterUI::RegisterUI()
:m_photo_agin(false)
, r(new Camera)
, bAlreadyTaked(false)
{

}

RegisterUI::~RegisterUI()
{
	
}

DUI_BEGIN_MESSAGE_MAP(RegisterUI, WindowImplBase)
DUI_ON_CLICK_CTRNAME(BT_CLOSERWND, Backward)
DUI_ON_CLICK_CTRNAME(BT_GETPHOTO, TakePhoto)
DUI_ON_CLICK_CTRNAME(BT_OK_REGISTER, SignUp)
DUI_END_MESSAGE_MAP()

LPCTSTR RegisterUI::GetWindowClassName() const
{
	return _T("RegisterUI");
}

CDuiString RegisterUI::GetSkinFolder()
{
	return _T("skin");
}

CDuiString RegisterUI::GetSkinFile()
{
	return _T("xml\\RegisterUI.xml");
}

CControlUI* RegisterUI::CreateControl(LPCTSTR pstrClass)
{
	if (_tcscmp(pstrClass, DUI_CTR_CLIP) == 0) 
		return new CClipUI;
	return NULL;
}

void RegisterUI::OnFinalMessage(HWND hWnd)
{
	removeObserver(*this);
	r.stop();

	WindowImplBase::OnFinalMessage(hWnd);
}

void RegisterUI::Notify(TNotifyUI& msg)
{
	WindowImplBase::Notify(msg);
}

void RegisterUI::InitWindow()
{	
	addObserver(*this);
	r.start();
	BandingSubControl();
}

void RegisterUI::BandingSubControl()
{
	name		=	dynamic_cast<CEditUI*>(m_PaintManager.FindControl(_T("Edit_Name")));
	age			=	dynamic_cast<CEditUI*>(m_PaintManager.FindControl(_T("Edit_Age")));
	sex			=	dynamic_cast<CComboUI*>(m_PaintManager.FindControl(_T("combo_sex")));
	birth		=	dynamic_cast<CEditUI*>(m_PaintManager.FindControl(_T("Edit_Birth")));
	address		=	dynamic_cast<CEditUI*>(m_PaintManager.FindControl(_T("Edit_Address")));
	phone		=	dynamic_cast<CEditUI*>(m_PaintManager.FindControl(_T("Edit_Phone")));
	certificate =	dynamic_cast<CEditUI*>(m_PaintManager.FindControl(_T("Edit_IDnumber")));

	prompt		=	dynamic_cast<CLabelUI*>(m_PaintManager.FindControl(_T("lab_Prompt")));

	shutter		=	dynamic_cast<CButtonUI*>(m_PaintManager.FindControl(_T("photo")));
}

void RegisterUI::Backward(TNotifyUI& msg)
{
	Close();
}

void RegisterUI::SignUp(TNotifyUI& msg)
{
	if (!bAlreadyTaked)
	{
		std::string str = LangueConfig::GetShowText(2);
		return prompt->SetText(str.c_str());
	}

	if (!isValidInformation())
	{
		std::string str = LangueConfig::GetShowText(1);
		return prompt->SetText(str.c_str());
	}

	SaveRegisterInformation();	

	Close();
}

void RegisterUI::TakePhoto(TNotifyUI& msg)
{
	if (bAlreadyTaked)
	{
		std::string str = LangueConfig::GetShowText(3);
		shutter->SetText(str.c_str());
		bAlreadyTaked = false;
	}
	else
	{
		std::string str = LangueConfig::GetShowText(4);
		shutter->SetText(str.c_str());
		bAlreadyTaked = true;
	}
}

bool RegisterUI::isValidInformation()
{
	if (name->GetText().IsEmpty() ||
		age->GetText().IsEmpty() ||
		sex->GetText().IsEmpty() ||
		birth->GetText().IsEmpty() ||
		address->GetText().IsEmpty() ||
		phone->GetText().IsEmpty() ||
		certificate->GetText().IsEmpty()){

			return false;
	}

		
	return true;
}

void RegisterUI::SaveRegisterInformation()
{
	IdentityInfo* Item = new IdentityInfo;
	Item->strName = name->GetText();
	Item->strAge = age->GetText();
	Item->strSex = sex->GetText();
	Item->strBirth = birth->GetText();
	Item->strIDcard = address->GetText();
	Item->strPhone = phone->GetText();
	Item->strCertID = certificate->GetText();

	/*Poco::Data::CLOB saveImage((const char*)pic->data(), pic->len());
	m_userInfo.set<8>(saveImage);*/
	//m_userInfo.set<0>(Item->strName);
	//m_userInfo.set<1>(stoi(Item->strAge));
	//m_userInfo.set<2>(Item->strSex);
	//m_userInfo.set<3>(Item->strBirth);
	//m_userInfo.set<4>(Item->strIDcard);
	//m_userInfo.set<5>(Item->strPhone);
	//m_userInfo.set<6>(Item->strCertID);
	//m_userInfo.set<7>(false);
	//RegUserInfo::addUserInfo(m_userInfo);
}

void RegisterUI::handle1(Poco::Notification* pNf)
{
	if (bAlreadyTaked) return;

	poco_check_ptr(pNf);
	Notification::Ptr pf(pNf);
	poco_check_ptr(pf.get());
	CaptureNotification::Ptr nf = pf.cast<CaptureNotification>();
	poco_check_ptr(nf.get());
	Picture::Ptr pic(nf->data());
	poco_check_ptr(pic.get());

	CControlUI* Image = m_PaintManager.FindControl(_T("photo_wnd"));
	Util::DrawSomething(pic, Image, GetHWND());
}
