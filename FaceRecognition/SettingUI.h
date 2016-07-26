#pragma once
#include <DuiLib/UIlib.h>


#define BT_CLOSESWnd		(_T("close_btn2"))

class CSettingUI :
	public WindowImplBase
{
public:
	CSettingUI();
	~CSettingUI();

	virtual void OnFinalMessage(HWND hWnd);
	virtual void Notify(TNotifyUI& msg);

	DUI_DECLARE_MESSAGE_MAP();

	void OnCloseSWnd(TNotifyUI& msg);

protected:
	virtual LPCTSTR GetWindowClassName() const;
	virtual CDuiString GetSkinFolder();
	virtual CDuiString GetSkinFile();
};