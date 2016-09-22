#pragma once
#include <DuiLib/UIlib.h>
#include "winuser.h"
#include "LogInUI.h"

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

	LRESULT MessageHandler(UINT uMsg, WPARAM wParam, LPARAM /*lParam*/, bool& bHandled);
	LRESULT HandleMessage(UINT uMsg, WPARAM wParam, LPARAM lParam);

	void Show_Off_loginWnd();
private:
	UINT			m_Mod;
	bool			m_IsSet;
	std::string		strHotkey;
protected:
	virtual LPCTSTR GetWindowClassName() const;
	virtual CDuiString GetSkinFolder();
	virtual CDuiString GetSkinFile();
};