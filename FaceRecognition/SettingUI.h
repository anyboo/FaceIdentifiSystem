#pragma once
#include <DuiLib/UIlib.h>
#include "DeviceManage.h"


#define BT_CLOSESWnd		(_T("close_btn2"))
#define BT_SAVELOG			(_T("saveLog"))
#define BT_SAVECONFIG		(_T("saveConfig"))
#define BT_RESETCONFIG		(_T("resetConfig"))

class CSettingUI :
	public WindowImplBase
{
public:
	CSettingUI();
	~CSettingUI();

	virtual void InitWindow();
	virtual void OnFinalMessage(HWND hWnd);
	virtual void Notify(TNotifyUI& msg);

	DUI_DECLARE_MESSAGE_MAP();

	void OnCloseSWnd(TNotifyUI& msg);
	void OnSaveLog(TNotifyUI& msg);
	void OnSaveConfig(TNotifyUI& msg);
	void OnResetConfig(TNotifyUI& msg);

	void OnSwitchWindow(TNotifyUI& msg);

//	void WriteConfig(const std::string timeD, const std::string siml, const std::string facesize);
//	void ReadConfig(std::string& timeD, std::string& siml, std::string& facesize);
	void ShowConfig();
	void SetCombText(const std::string value, CComboUI* pComb);

	virtual LRESULT HandleCustomMessage(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
private:
	bool		m_closeApp;
	CDeviceManage		_deviceManage;
protected:
	virtual LPCTSTR GetWindowClassName() const;
	virtual CDuiString GetSkinFolder();
	virtual CDuiString GetSkinFile();
};