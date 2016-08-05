#pragma once
#include <DuiLib/UIlib.h>

#define BT_CLOSE_WND		(_T("btn_cancel"))
#define BT_LOGIN			(_T("btn_ok"))

enum LogInResult
{
	LogInSucceed = 0,
	LogInFailure
};

class CLogInUI :
	public WindowImplBase
{
public:
	CLogInUI();
	~CLogInUI();

	LogInResult m_LogResult;
	virtual void OnFinalMessage(HWND hWnd);
	virtual void Notify(TNotifyUI& msg);

	DUI_DECLARE_MESSAGE_MAP();

	void OnCloseSWnd(TNotifyUI& msg);

	void OnLogIn(TNotifyUI& msg);




protected:
	virtual LPCTSTR GetWindowClassName() const;
	virtual CDuiString GetSkinFolder();
	virtual CDuiString GetSkinFile();
};
