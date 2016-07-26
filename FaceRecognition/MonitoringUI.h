#pragma once
#include <DuiLib/UIlib.h>

#define BT_CLOSE_MonWnd		(_T("close_btn2"))

class CMonitoringUI :
	public WindowImplBase
{
public:
	CMonitoringUI();
	~CMonitoringUI();

	virtual void OnFinalMessage(HWND hWnd);
	virtual void Notify(TNotifyUI& msg);

	DUI_DECLARE_MESSAGE_MAP();

	void OnCloseWnd(TNotifyUI& msg);

protected:
	virtual LPCTSTR GetWindowClassName() const;
	virtual CDuiString GetSkinFolder();
	virtual CDuiString GetSkinFile();
};
