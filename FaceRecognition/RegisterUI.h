#pragma once
#include <DuiLib/UIlib.h>
#include "RegisterInfo.h"
#include "Recorder.h"
#include "CaptureNotify.h"
#include "RegUserInfo.h"

#define BT_CLOSERWND		(_T("close_btn"))
#define BT_GETPHOTO			(_T("photo"))
#define BT_OK_REGISTER		(_T("btn_ok"))



class CClipUI;
class RegisterUI :
	public WindowImplBase,
	public CaptureNotify
{
public:
	RegisterUI();
	~RegisterUI();

	virtual void InitWindow();
	virtual void OnFinalMessage(HWND hWnd);
	virtual void Notify(TNotifyUI& msg);

	virtual CControlUI* CreateControl(LPCTSTR pstrClass);
	virtual void handle1(Poco::Notification* pNf);
	DUI_DECLARE_MESSAGE_MAP();

	//void OnCloseRWnd(TNotifyUI& msg);
	

protected:
	virtual LPCTSTR GetWindowClassName() const;
	virtual CDuiString GetSkinFolder();
	virtual CDuiString GetSkinFile();

	void BandingSubControl();

	void Backward(TNotifyUI& msg);
	void TakePhoto(TNotifyUI& msg);
	void SignUp(TNotifyUI& msg);
	
	bool isValidInformation();
	void SaveRegisterInformation();
	
private:
	bool   m_photo_agin;
	Recorder r;
	CClipUI* customizedImage;
	writeUserInfo	m_userInfo;

	CEditUI*	name;
	CEditUI*	age;
	CEditUI*	birth;
	CEditUI*	address;
	CEditUI*	phone;
	CEditUI*	certificate;

	CComboUI*	sex;
	CLabelUI*	prompt;
	CButtonUI*  shutter;

	bool bAlreadyTaked;
};