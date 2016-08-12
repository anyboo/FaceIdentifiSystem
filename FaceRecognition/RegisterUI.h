#pragma once
#include <DuiLib/UIlib.h>
#include "RegisterInfo.h"
#include "Recorder.h"
#include "CaptureNotify.h"
#include "QMFileSqlite.h"
#include "RegUserInfo.h"



#define BT_CLOSERWND		(_T("close_btn"))
#define BT_GETPHOTO			(_T("photo"))
#define BT_OK_REGISTER		(_T("btn_ok"))




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

	void OnCloseRWnd(TNotifyUI& msg);
	void OnGetPhoto(TNotifyUI& msg);
	void OnRegister(TNotifyUI& msg);

	bool SaveRegisterInfo();
	virtual LRESULT HandleCustomMessage(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);

private:
	bool			m_photo_agin;
	bool				m_closeApp;
	writeUserInfo	m_userInfo;

	const int width = 640;
	const int height = 480;
	const int magic = 3;

private:
	CLabelUI*		m_Prompt_lab;

protected:
	virtual LPCTSTR GetWindowClassName() const;
	virtual CDuiString GetSkinFolder();
	virtual CDuiString GetSkinFile();

	Recorder r;
};