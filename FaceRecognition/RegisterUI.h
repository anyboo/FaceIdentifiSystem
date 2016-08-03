#pragma once
#include <DuiLib/UIlib.h>
#include "RegisterInfo.h"

#define BT_CLOSERWND		(_T("close_btn"))
#define BT_GETPHOTO			(_T("photo"))
#define BT_OK_REGISTER		(_T("btn_ok"))

#include "Recorder.h"
#include "CaptureNotify.h"

class RegisterUI :
	public WindowImplBase,
	public CaptureNotify
{
public:
	RegisterUI();
	~RegisterUI();
	CRegisterInfo* GetRegisterInfo();
	virtual void InitWindow();
	virtual void OnFinalMessage(HWND hWnd);
	virtual void Notify(TNotifyUI& msg);

	virtual LRESULT OnTimer(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);

	virtual LRESULT HandleCustomMessage(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
	virtual void handle1(Poco::Notification* pNf);
	DUI_DECLARE_MESSAGE_MAP();

	void OnCloseRWnd(TNotifyUI& msg);
	void OnGetPhoto(TNotifyUI& msg);
	void OnFilishi(TNotifyUI& msg);

	bool SaveRegisterInfo();


private:
	CRegisterInfo*	m_RegisterInfo;
private:
	int				m_nbmp;
	int				m_RegID;
	bool			m_photo_agin;

protected:
	virtual LPCTSTR GetWindowClassName() const;
	virtual CDuiString GetSkinFolder();
	virtual CDuiString GetSkinFile();

	Recorder r;//, p(dev);
};