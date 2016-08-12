#include "stdafx.h"
#include "RegisterUI.h"
#include "CaptureNotification.h"
#include "Camera.h"
#include "Util.h"
#include "SettingConfig.h"
#include "ClipUI.h"

#include "Mmsystem.h"
RegisterUI::RegisterUI()
:m_photo_agin(false), r(new Camera)
{
	m_closeApp = true;
}

RegisterUI::~RegisterUI()
{
	
}

DUI_BEGIN_MESSAGE_MAP(RegisterUI, WindowImplBase)
DUI_ON_CLICK_CTRNAME(BT_CLOSERWND, OnCloseRWnd)
DUI_ON_CLICK_CTRNAME(BT_GETPHOTO, OnGetPhoto)
DUI_ON_CLICK_CTRNAME(BT_OK_REGISTER, OnRegister)
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
}

void RegisterUI::OnRegister(TNotifyUI& msg)
{
	CLabelUI* lab_Prompt = dynamic_cast<CLabelUI*>(m_PaintManager.FindControl(_T("lab_Prompt")));
	bool bRet =	SaveRegisterInfo();
	if (!bRet)
	{
		std::string str = LangueConfig::GetShowText(1);
		lab_Prompt->SetText(str.c_str());
		return;
	}
	if (!m_photo_agin)
	{
		std::string str = LangueConfig::GetShowText(2);
		lab_Prompt->SetText(str.c_str());
		return;
	}

	RegUserInfo::addUserInfo(m_userInfo);
	PlaySoundA(_T("ZC.wav"), NULL, SND_FILENAME | SND_ASYNC);
	m_closeApp = false;
	Close();
}

void RegisterUI::OnGetPhoto(TNotifyUI& msg)
{
	CButtonUI* btn_photo = dynamic_cast<CButtonUI*>(m_PaintManager.FindControl(_T("photo")));
	if (m_photo_agin)
	{
		std::string str = LangueConfig::GetShowText(3);
		btn_photo->SetText(str.c_str());
		m_photo_agin = false;
	}
	else
	{
		std::string str = LangueConfig::GetShowText(4);
		btn_photo->SetText(str.c_str());
		m_photo_agin = true;
	}
}

bool RegisterUI::SaveRegisterInfo()
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

	return true;
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

		CControlUI* hLyt = dynamic_cast<CControlUI*>(m_PaintManager.FindControl(_T("photo_wnd")));
		hLyt->SetBkImage(imageName.c_str());
		return;
	}
	poco_check_ptr(pNf);
	Notification::Ptr pf(pNf);
	poco_check_ptr(pf.get());
	CaptureNotification::Ptr nf = pf.cast<CaptureNotification>();
	poco_check_ptr(nf.get());
	Picture::Ptr pic(nf->data()); 
	poco_check_ptr(pic.get());

	Poco::Data::CLOB saveImage((const char*)pic->data(), pic->len());
	m_userInfo.set<8>(saveImage);

	CControlUI* Image = m_PaintManager.FindControl(_T("photo_wnd"));
	Util::DrawSomething(pic, Image, GetHWND());
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
