#include "stdafx.h"
#include "RegisterUI.h"
#include "CaptureNotification.h"
#include "Camera.h"
#include "Util.h"
#include "SettingConfig.h"
#include "ClipUI.h"


#include "Mmsystem.h"

#include "Employee.h"


RegisterUI::RegisterUI()
:m_photo_agin(false)
, r(new Camera)
, bAlreadyTaked(false)
{
	m_closeApp = true;
}

RegisterUI::~RegisterUI()
{
	
}

DUI_BEGIN_MESSAGE_MAP(RegisterUI, WindowImplBase)

DUI_ON_CLICK_CTRNAME(BT_CLOSERWND, OnCloseRWnd)
//DUI_ON_CLICK_CTRNAME(BT_GETPHOTO, OnGetPhoto)
//DUI_ON_CLICK_CTRNAME(BT_OK_REGISTER, OnRegister)

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


void RegisterUI::OnCloseRWnd(TNotifyUI& msg)
{
	m_closeApp = false;
	Close();
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


	RegUserInfo::addUserInfo(m_userInfo);
	PlaySoundA(_T("ZC.wav"), NULL, SND_FILENAME | SND_ASYNC);
	m_closeApp = false;

	SaveRegisterInformation();	

	Close();
}

void RegisterUI::TakePhoto(TNotifyUI& msg)
{

	CButtonUI* btn_photo = dynamic_cast<CButtonUI*>(m_PaintManager.FindControl(_T("photo")));
	if (m_photo_agin)

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

	CEditUI* edit_name = dynamic_cast<CEditUI*>(m_PaintManager.FindControl(_T("Edit_Name")));
	CEditUI* edit_age = dynamic_cast<CEditUI*>(m_PaintManager.FindControl(_T("Edit_Age")));
	CComboUI* combo_sex = dynamic_cast<CComboUI*>(m_PaintManager.FindControl(_T("combo_sex")));
	CEditUI* edit_birth = dynamic_cast<CEditUI*>(m_PaintManager.FindControl(_T("Edit_Birth")));
	CEditUI* edit_address = dynamic_cast<CEditUI*>(m_PaintManager.FindControl(_T("Edit_Address")));
	CEditUI* edit_phone = dynamic_cast<CEditUI*>(m_PaintManager.FindControl(_T("Edit_Phone")));
	CEditUI* edit_CertID = dynamic_cast<CEditUI*>(m_PaintManager.FindControl(_T("Edit_IDnumber")));

	std::string strName = edit_name->GetText();
	std::string strAge = edit_age->GetText();
	std::string strSex = combo_sex->GetText();
	std::string strBirth = edit_birth->GetText();
	std::string strIDcard = edit_address->GetText();
	std::string strPhone = edit_phone->GetText();
	std::string strCertID = edit_CertID->GetText();


	if (strName == _T("") || strAge == _T("") || strSex == _T("") || strBirth == _T("")
		|| strIDcard == _T("") || strPhone == _T("") || strCertID == _T(""))
	{
		return false;
	}
	m_userInfo.set<0>(strName);
	m_userInfo.set<1>(stoi(strAge));
	m_userInfo.set<2>(strSex);
	m_userInfo.set<3>(strBirth);
	m_userInfo.set<4>(strIDcard);
	m_userInfo.set<5>(strPhone);
	m_userInfo.set<6>(strCertID);
	m_userInfo.set<7>(false);

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
	//Employee e =
	//{
	//	_name->GetText(),
	//	_age->GetText(),
	//	_sex->GetText(),
	//	_birth->GetText(),
	//	_address->GetText(),
	//	_phone->GetText(),
	//	_certificate->GetText()
	//};
	//
	//Identity id(e,CurrentImage);
	//IdentityDB::Instance().Add(id);
	//need photograph data
}

void RegisterUI::handle1(Poco::Notification* pNf)
{

	if (m_photo_agin)
	{
		Picture::Ptr userpic(new Picture(m_userInfo.get<8>().rawContent(), width * height * magic));
		userpic->SetWidth(width);
		userpic->SetHeight(height);

		std::string path = CPaintManagerUI::GetInstancePath();
		std::string imageName = userpic->out2bmp(path);

	if (bAlreadyTaked) return;


		CControlUI* hLyt = dynamic_cast<CControlUI*>(m_PaintManager.FindControl(_T("photo_wnd")));
		hLyt->SetBkImage(imageName.c_str());
		return;
	}
	poco_check_ptr(pNf);
	Notification::Ptr pf(pNf);
	CaptureNotification::Ptr nf = pf.cast<CaptureNotification>();

	poco_check_ptr(nf.get());
	Picture::Ptr pic(nf->data()); 
	poco_check_ptr(pic.get());

	Poco::Data::CLOB saveImage((const char*)pic->data(), pic->len());
	m_userInfo.set<8>(saveImage);

	CControlUI* Image = m_PaintManager.FindControl(_T("photo_wnd"));
	Util::DrawSomething(pic, Image, GetHWND());

	/*if (nf)
	{
		CurrentImage.assign(nf->data());
		CControlUI* Image = m_PaintManager.FindControl(_T("photo_wnd"));
		Util::DrawSomething(CurrentImage, Image, GetHWND());
	}*/

}

LRESULT RegisterUI::HandleCustomMessage(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
	if (uMsg == WM_DESTROY && m_closeApp)
	{
		::ShowWindow(::FindWindow("Shell_TrayWnd", NULL), SW_SHOW);
		::PostQuitMessage(0);
	}
	bHandled = FALSE;
	return 0;
}
