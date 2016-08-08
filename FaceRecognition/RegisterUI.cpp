#include "stdafx.h"
#include "RegisterUI.h"
#include "CaptureNotification.h"
#include "Camera.h"
#include "Util.h"
#include "LangueConfig.h"
#include "ClipUI.h"
#include "Employee.h"

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
	_name		 =	dynamic_cast<CEditUI*>(m_PaintManager.FindControl(_T("Edit_Name")));
	_age		 =	dynamic_cast<CEditUI*>(m_PaintManager.FindControl(_T("Edit_Age")));
	_sex		 =	dynamic_cast<CComboUI*>(m_PaintManager.FindControl(_T("combo_sex")));
	_birth		 =	dynamic_cast<CEditUI*>(m_PaintManager.FindControl(_T("Edit_Birth")));
	_address	 =	dynamic_cast<CEditUI*>(m_PaintManager.FindControl(_T("Edit_Address")));
	_phone		 =	dynamic_cast<CEditUI*>(m_PaintManager.FindControl(_T("Edit_Phone")));
	_certificate =	dynamic_cast<CEditUI*>(m_PaintManager.FindControl(_T("Edit_IDnumber")));
	
	_prompt		 =	dynamic_cast<CLabelUI*>(m_PaintManager.FindControl(_T("lab_Prompt")));
	
	_shutter	 =	dynamic_cast<CButtonUI*>(m_PaintManager.FindControl(_T("photo")));
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
		return _prompt->SetText(str.c_str());
	}

	if (!isValidInformation())
	{
		std::string str = LangueConfig::GetShowText(1);
		return _prompt->SetText(str.c_str());
	}

	SaveRegisterInformation();	

	Close();
}

void RegisterUI::TakePhoto(TNotifyUI& msg)
{
	if (bAlreadyTaked)
	{
		std::string str = LangueConfig::GetShowText(3);
		_shutter->SetText(str.c_str());
		bAlreadyTaked = false;
	}
	else
	{
		std::string str = LangueConfig::GetShowText(4);
		_shutter->SetText(str.c_str());
		bAlreadyTaked = true;
	}
}

bool RegisterUI::isValidInformation()
{
	if (_name->GetText().IsEmpty() ||
		_age->GetText().IsEmpty() ||
		_sex->GetText().IsEmpty() ||
		_birth->GetText().IsEmpty() ||
		_address->GetText().IsEmpty() ||
		_phone->GetText().IsEmpty() ||
		_certificate->GetText().IsEmpty()){

			return false;
	}

		
	return true;
}

void RegisterUI::SaveRegisterInformation()
{
	Employee e =
	{
		_name->GetText(),
		_age->GetText(),
		_sex->GetText(),
		_birth->GetText(),
		_address->GetText(),
		_phone->GetText(),
		_certificate->GetText()
	};
	
	Identity id(e,CurrentImage);
	IdentityDB::Instance().Add(id);
	//need photograph data
}

void RegisterUI::handle1(Poco::Notification* pNf)
{
	if (bAlreadyTaked) return;

	poco_check_ptr(pNf);
	Notification::Ptr pf(pNf);
	CaptureNotification::Ptr nf = pf.cast<CaptureNotification>();
	if (nf)
	{
		CurrentImage.assign(nf->data());
		CControlUI* Image = m_PaintManager.FindControl(_T("photo_wnd"));
		Util::DrawSomething(CurrentImage, Image, GetHWND());
	}
}
