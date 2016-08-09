#pragma once
#include <DuiLib/UIlib.h>


#define BT_CLOSESWnd		(_T("close_btn2"))
#define BT_SAVELOG			(_T(""))

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
	void OnSaveLog(TNotifyUI& msg);
	virtual LRESULT HandleCustomMessage(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
private:
	bool		m_closeApp;
protected:
	virtual LPCTSTR GetWindowClassName() const;
	virtual CDuiString GetSkinFolder();
	virtual CDuiString GetSkinFile();
};