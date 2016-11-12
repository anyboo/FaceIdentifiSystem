#pragma once
#include <DuiLib/UIlib.h>

class CSettingUI :
	public WindowImplBase
{
public:
	CSettingUI();
	~CSettingUI();

	virtual void InitWindow();
	virtual void OnFinalMessage(HWND hWnd);
	virtual void Notify(TNotifyUI& msg);
	virtual CControlUI* CreateControl(LPCTSTR pstrClass);
	DUI_DECLARE_MESSAGE_MAP();

	void OnCloseSWnd(TNotifyUI& msg);
	void OnSaveLog(TNotifyUI& msg);
	void OnSwitch(TNotifyUI& msg);
	void OnRegister(TNotifyUI& msg);
	void OnSave(TNotifyUI& msg);

protected:
	virtual LPCTSTR GetWindowClassName() const;
	virtual CDuiString GetSkinFolder();
	virtual CDuiString GetSkinFile();
	void InitDB();
private:
	int _dbIndex;
};