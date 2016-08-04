#pragma once
#include <DuiLib/UIlib.h>
#include "RegisterInfo.h"
#include "ActivityDispatcher.h"

#define BT_CLOSERWND		(_T("close_btn"))
#define BT_GETPHOTO			(_T("photo"))
#define BT_OK_REGISTER		(_T("btn_ok"))

#include "Recorder.h"
#include "CaptureNotify.h"

#include "QMFileSqlite.h"

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


	virtual void handle1(Poco::Notification* pNf);
	DUI_DECLARE_MESSAGE_MAP();

	void OnCloseRWnd(TNotifyUI& msg);
	void OnGetPhoto(TNotifyUI& msg);
	void OnFilishi(TNotifyUI& msg);

	bool SaveRegisterInfo();

private:
	bool			m_photo_agin;
	QFileSqlite*		m_pDb;
	writeUserInfo	m_userInfo;
protected:
	virtual LPCTSTR GetWindowClassName() const;
	virtual CDuiString GetSkinFolder();
	virtual CDuiString GetSkinFile();

	Recorder r;
	ActivityDispatcher example;
	long         m_count;
};