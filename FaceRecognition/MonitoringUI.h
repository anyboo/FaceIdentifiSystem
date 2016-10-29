#pragma once
#include <DuiLib/UIlib.h>
#include <Poco/Timer.h>

#define BT_CLOSE_MonWnd		(_T("close_btn2"))
#define BT_REMOVE_ALARM		(_T("btm_remove"))


struct alert_table;
struct alert_image;
class CMonitoringUI :
	public WindowImplBase
{
public:
	CMonitoringUI();
	~CMonitoringUI();
	virtual void InitWindow();
	virtual void OnFinalMessage(HWND hWnd);
	virtual void Notify(TNotifyUI& msg);
	DUI_DECLARE_MESSAGE_MAP();

	void OnCloseWnd(TNotifyUI& msg);
	void OnInitCtrl();

protected:
	virtual LPCTSTR GetWindowClassName() const;
	virtual CDuiString GetSkinFolder();
	virtual CDuiString GetSkinFile();
	void onTimer(Poco::Timer& timer);

	bool is_need_report();
	void report_to_user();
	void report_image_to_user();
	void pushImage(const alert_image& image);

private:
	Poco::Timer _monitor;
	Poco::TimerCallback<CMonitoringUI> _callback;

	int _notify_status;

	CVerticalLayoutUI*					m_Main_Lyt;
	CControlUI*							m_photo_Ctrl;
	CLabelUI*							m_Prompt_lab;
	bool								 m_closeApp;
	int left_image_pos;
};