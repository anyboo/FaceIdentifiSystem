#pragma once
#include <DuiLib/UIlib.h>
#include "RegisterInfo.h"

#define BT_CLOSERWND		(_T("close_btn"))
#define BT_GETPHOTO			(_T("photo"))
#define BT_OK_REGISTER		(_T("btn_ok"))

#include <Poco/Notification.h>
#include "Recorder.h"

class RegisterUI :
	public WindowImplBase
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

	void handle1(Poco::Notification* pNf);
	void addObserver();
	void removeObserver();
	Recorder r;//, p(dev);
};