#pragma once
#include <DuiLib/UIlib.h>

#define BT_CLOSE_MatchWnd		(_T("close_btn1"))

class MatchUI :
	public WindowImplBase
{
public:
	MatchUI();
	~MatchUI();

	virtual void OnFinalMessage(HWND hWnd);
	virtual void Notify(TNotifyUI& msg);

	DUI_DECLARE_MESSAGE_MAP();

	void OnCloseWnd(TNotifyUI& msg);

protected:
	virtual LPCTSTR GetWindowClassName() const;
	virtual CDuiString GetSkinFolder();
	virtual CDuiString GetSkinFile();
};
