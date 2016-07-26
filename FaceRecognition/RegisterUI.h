#pragma once
#include <DuiLib/UIlib.h>


#define BT_CLOSERWND		(_T("close_btn"))

class RegisterUI :
	public WindowImplBase
{
public:
	RegisterUI();
	~RegisterUI();

	virtual void OnFinalMessage(HWND hWnd);
	virtual void Notify(TNotifyUI& msg);

	DUI_DECLARE_MESSAGE_MAP();

	void OnCloseRWnd(TNotifyUI& msg);

protected:
	virtual LPCTSTR GetWindowClassName() const;
	virtual CDuiString GetSkinFolder();
	virtual CDuiString GetSkinFile();
};