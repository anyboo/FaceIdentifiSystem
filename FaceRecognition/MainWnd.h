#pragma once
#include <DuiLib/UIlib.h>

class CRegisterInfo;


#define BT_CLOSEMWND		(_T("bt_close"))
#define BT_RegisterWnd		(_T("bt_register"))
#define BT_MATCHWND			(_T("bt_match"))
#define BT_Monitoring		(_T("bt_monitoring"))
#define BT_SETTINGWND		(_T("bt_setting"))
#define BT_SIGNOUTWND		(_T("bt_SignOut"))

class CMainWnd :
	public WindowImplBase
{
public:
	CMainWnd();
	~CMainWnd();

	virtual void OnFinalMessage(HWND hWnd);
	virtual void Notify(TNotifyUI& msg);

	DUI_DECLARE_MESSAGE_MAP();

	void OnCloseWnd(TNotifyUI& msg);
	void OnRegisterWnd(TNotifyUI& msg);
	void OnMatchWnd(TNotifyUI& msg);
	void OnMonitoringWnd(TNotifyUI& msg);
	void OnSettingWnd(TNotifyUI& msg);
	void OnSignOutWnd(TNotifyUI& msg);

	void Show_HideTask(bool IsHide);

	virtual LRESULT OnKeyDown(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);


	CRegisterInfo* m_RegInfo;
protected:
	virtual LPCTSTR GetWindowClassName() const;
	virtual CDuiString GetSkinFolder();
	virtual CDuiString GetSkinFile();
};