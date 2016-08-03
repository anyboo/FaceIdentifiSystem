#include "stdafx.h"
#include "RegisterUI.h"
#include "CaptureNotification.h"

#include <Poco/AutoPtr.h>
#include <Poco/Observer.h>
#include <Poco/NotificationCenter.h>
#include "Camera.h"

using Poco::AutoPtr;
using Poco::Observer;
using Poco::NotificationCenter;

RegisterUI::RegisterUI()
:m_nbmp(0), m_photo_agin(false), r(new Camera)
{
	m_RegID = 100001;
	m_RegisterInfo = new CRegisterInfo;

	addObserver();
	r.start();
}


RegisterUI::~RegisterUI()
{
	/*delete m_RegisterInfo;
	m_RegisterInfo = nullptr;*/
	removeObserver();
	r.stop();
}

DUI_BEGIN_MESSAGE_MAP(RegisterUI, WindowImplBase)
DUI_ON_CLICK_CTRNAME(BT_CLOSERWND, OnCloseRWnd)
DUI_ON_CLICK_CTRNAME(BT_GETPHOTO, OnGetPhoto)
DUI_ON_CLICK_CTRNAME(BT_OK_REGISTER, OnFilishi)
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

void RegisterUI::OnFinalMessage(HWND hWnd)
{
	WindowImplBase::OnFinalMessage(hWnd);
}

void RegisterUI::Notify(TNotifyUI& msg)
{
	WindowImplBase::Notify(msg);
}

void RegisterUI::OnCloseRWnd(TNotifyUI& msg)
{
	Close();
}

void RegisterUI::InitWindow()
{
	::SetTimer(GetHWND(), 1, 50, NULL);
}

void RegisterUI::OnFilishi(TNotifyUI& msg)
{
	CLabelUI* lab_Prompt = dynamic_cast<CLabelUI*>(m_PaintManager.FindControl(_T("lab_Prompt")));
	CButtonUI* bt_photo = dynamic_cast<CButtonUI*>(m_PaintManager.FindControl(_T("photo")));
	bool bRet = SaveRegisterInfo();
	if (!bRet)
	{
		lab_Prompt->SetText(_T("注册信息输入不完整！"));
		return;
	}
	if (bt_photo->GetText() == _T("拍照"))
	{
		lab_Prompt->SetText(_T("请先拍照!"));
		return;
	}
	Close();
}

void RegisterUI::OnGetPhoto(TNotifyUI& msg)
{
	CButtonUI* bt_photo = dynamic_cast<CButtonUI*>(m_PaintManager.FindControl(_T("photo")));
	if (m_photo_agin)
	{
		bt_photo->SetText(_T("拍照"));
		::SetTimer(GetHWND(), 1, 50, NULL);
		m_photo_agin = false;
	}
	else
	{
		bt_photo->SetText(_T("重新拍照"));
		::KillTimer(GetHWND(), 1);
		m_photo_agin = true;
	}
}

LRESULT RegisterUI::OnTimer(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
	m_nbmp = m_nbmp + 1;
	CVerticalLayoutUI* photo_Lyt = dynamic_cast<CVerticalLayoutUI*>(m_PaintManager.FindControl(_T("photo_wnd")));

	std::string strName = std::string(_T("file = 'bmp/test")) + std::to_string(m_nbmp) + std::string(".bmp'");
	photo_Lyt->SetBkImage(strName.c_str());
	if (m_nbmp == 150)
	{
		m_nbmp = 1;
	}
	return 0;
}

LRESULT RegisterUI::HandleCustomMessage(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
	LRESULT lRes = 0;
	switch (uMsg)
	{
	case WM_TIMER: lRes = OnTimer(uMsg, wParam, lParam, bHandled); break;
	}
	bHandled = FALSE;
	return 0;
}

bool RegisterUI::SaveRegisterInfo()
{
	CVerticalLayoutUI* photo_lyt = dynamic_cast<CVerticalLayoutUI*>(m_PaintManager.FindControl(_T("photo_wnd")));
	CEditUI* edit_name = dynamic_cast<CEditUI*>(m_PaintManager.FindControl(_T("Edit_Name")));
	CEditUI* edit_age = dynamic_cast<CEditUI*>(m_PaintManager.FindControl(_T("Edit_Age")));
	CEditUI* edit_sex = dynamic_cast<CEditUI*>(m_PaintManager.FindControl(_T("Edit_Sex")));
	CEditUI* edit_birth = dynamic_cast<CEditUI*>(m_PaintManager.FindControl(_T("Edit_Birth")));
	CEditUI* edit_address = dynamic_cast<CEditUI*>(m_PaintManager.FindControl(_T("Edit_Address")));
	CEditUI* edit_phone = dynamic_cast<CEditUI*>(m_PaintManager.FindControl(_T("Edit_Phone")));
	CEditUI* edit_CertID = dynamic_cast<CEditUI*>(m_PaintManager.FindControl(_T("Edit_IDnumber")));

	IdentityInfo* Item = new IdentityInfo;
	Item->strName = edit_name->GetText();
	Item->strAge = edit_age->GetText();
	Item->strSex = edit_sex->GetText();
	Item->strBirth = edit_birth->GetText();
	Item->strAdress = edit_address->GetText();
	Item->strPhone = edit_phone->GetText();
	Item->strCertID = edit_CertID->GetText();
	Item->strPhotoInfo = photo_lyt->GetBkImage();

	if (Item->strName == _T("") || Item->strAge == _T("") || Item->strSex == _T("") || Item->strBirth == _T("")
		|| Item->strAdress == _T("") || Item->strPhone == _T("") || Item->strCertID == _T(""))
	{
		return false;
	}

	m_RegisterInfo->Add(m_RegID, Item);
	return true;
}

CRegisterInfo* RegisterUI::GetRegisterInfo()
{
	return m_RegisterInfo;
}

void RegisterUI::handle1(Poco::Notification* pNf)
{
	poco_check_ptr(pNf);
	AutoPtr<Notification> nf = pNf;
}

void RegisterUI::addObserver()
{
	NotificationCenter& nc = NotificationCenter::defaultCenter();
	Observer<RegisterUI, Notification> o1(*this, &RegisterUI::handle1);
	nc.addObserver(o1);
}

void RegisterUI::removeObserver()
{
	NotificationCenter& nc = NotificationCenter::defaultCenter();
	nc.removeObserver(Observer<RegisterUI, Notification>(*this, &RegisterUI::handle1));
}