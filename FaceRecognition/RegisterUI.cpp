#include "stdafx.h"
#include "RegisterUI.h"
#include "CaptureNotification.h"

#include "Camera.h"

RegisterUI::RegisterUI()
:m_photo_agin(false)/*, r(new Camera)*/
{
	/*addObserver(*this);
	r.start();*/
}


RegisterUI::~RegisterUI()
{
	/*delete m_RegisterInfo;
	m_RegisterInfo = nullptr;*/
	//removeObserver(*this);
	//r.stop();
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

}

void RegisterUI::OnFilishi(TNotifyUI& msg)
{
	CVerticalLayoutUI* lab_lyt = dynamic_cast<CVerticalLayoutUI*>(m_PaintManager.FindControl(_T("lab_lyt")));
	CLabelUI* lab_Prompt = dynamic_cast<CLabelUI*>(m_PaintManager.FindControl(_T("lab_Prompt")));
	bool bRet = SaveRegisterInfo();
	if (!bRet)
	{
		CDialogBuilder builder;
		CLabelUI* Lab = (CLabelUI*)(builder.Create(_T("xml//labUI1.xml"), (UINT)0, NULL, &m_PaintManager));
		lab_lyt->Remove(lab_Prompt);
		lab_lyt->Add(Lab);
		return;
	}
	if (!m_photo_agin)
	{
		CDialogBuilder builder;
		CLabelUI* Lab = (CLabelUI*)(builder.Create(_T("xml//labUI2.xml"), (UINT)0, NULL, &m_PaintManager));
		lab_lyt->Remove(lab_Prompt);
		lab_lyt->Add(Lab);
		return;
	}
	Close();
}

void RegisterUI::OnGetPhoto(TNotifyUI& msg)
{
	CVerticalLayoutUI* btn_lyt = dynamic_cast<CVerticalLayoutUI*>(m_PaintManager.FindControl(_T("vLyt")));
	CButtonUI* bt_photo = dynamic_cast<CButtonUI*>(m_PaintManager.FindControl(_T("photo")));
	if (m_photo_agin)
	{
		CDialogBuilder builder;
		CLabelUI* btn = (CLabelUI*)(builder.Create(_T("xml//labUI3.xml"), (UINT)0, NULL, &m_PaintManager));
		btn_lyt->Remove(bt_photo);
		btn_lyt->Add(btn);
		m_photo_agin = false;
	}
	else
	{
		CDialogBuilder builder;
		CLabelUI* btn = (CLabelUI*)(builder.Create(_T("xml//labUI4.xml"), (UINT)0, NULL, &m_PaintManager));
		btn_lyt->Remove(bt_photo);
		btn_lyt->Add(btn);
		m_photo_agin = true;
	}
}


bool RegisterUI::SaveRegisterInfo()
{
	CVerticalLayoutUI* photo_lyt = dynamic_cast<CVerticalLayoutUI*>(m_PaintManager.FindControl(_T("photo_wnd")));
	CEditUI* edit_name = dynamic_cast<CEditUI*>(m_PaintManager.FindControl(_T("Edit_Name")));
	CEditUI* edit_age = dynamic_cast<CEditUI*>(m_PaintManager.FindControl(_T("Edit_Age")));
	CComboUI* combo_sex = dynamic_cast<CComboUI*>(m_PaintManager.FindControl(_T("combo_sex")));
	CEditUI* edit_birth = dynamic_cast<CEditUI*>(m_PaintManager.FindControl(_T("Edit_Birth")));
	CEditUI* edit_address = dynamic_cast<CEditUI*>(m_PaintManager.FindControl(_T("Edit_Address")));
	CEditUI* edit_phone = dynamic_cast<CEditUI*>(m_PaintManager.FindControl(_T("Edit_Phone")));
	CEditUI* edit_CertID = dynamic_cast<CEditUI*>(m_PaintManager.FindControl(_T("Edit_IDnumber")));

	IdentityInfo* Item = new IdentityInfo;
	Item->strName = edit_name->GetText();
	Item->strAge = edit_age->GetText();
	Item->strSex = combo_sex->GetText();
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

	return true;
}



#include <Poco/AutoPtr.h>

using Poco::AutoPtr;

void RegisterUI::handle1(Poco::Notification* pNf)
{
	poco_check_ptr(pNf);
	CaptureNotify::handle1(pNf);
}