#pragma once
#include <DuiLib/UIlib.h>
#include <queue>
#include "QMFileSqlite.h"
#include "Poco/BasicEvent.h"
#include "BitMapCompare.h"
#include <Poco/ThreadPool.h>
#include "Recorder.h"
#include "CaptureNotify.h"
#include "ActivityDispatcher.h"

#include "Poco/Data/Session.h"
#include <Poco/Net/FTPClientSession.h>
#include "ActivityCommit.h"

#define BT_CLOSE_MonWnd		(_T("close_btn2"))
#define BT_REMOVE_ALARM		(_T("btm_remove"))


struct alert_table;

class CMonitoringUI :
	public WindowImplBase
{
public:
	CMonitoringUI();
	~CMonitoringUI();
	virtual void InitWindow();
	virtual void OnFinalMessage(HWND hWnd);
	virtual void Notify(TNotifyUI& msg);
	//virtual void handle1(Poco::Notification* pNf);
	DUI_DECLARE_MESSAGE_MAP();

	void OnCloseWnd(TNotifyUI& msg);

	void OnRemoveAlarm();
	void OnInitCtrl();

	void OnRemoveAlarm(TNotifyUI& msg);


	virtual LRESULT HandleCustomMessage(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
	virtual LRESULT OnTimer(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);

	void OnTimer1();
	void OnTimer2();
	void OnTimer3();
	void OnTimer4();
	void onTimer(Poco::Timer& timer);
public:
	std::queue<writeCompareInfo>& getCompareQueue();
	std::queue<CapBitmapData>& getCapDataQueue();

private:
	void fireEvent(bool n)
	{
		m_theEvent(this, n);
	}

private:

	std::queue<writeCompareInfo>		 m_compare;
	Poco::BasicEvent<bool>				 m_theEvent;
	BitMapCompare*						 m_pCompare;
	std::queue<CapBitmapData>			 m_capdata;

	long								 m_count;
	time_t								 m_lastTime;
	time_t								 m_nowTime;
	SYSTEMTIME							 m_systime;
	bool								 m_closeApp;
	bool								 m_bSendMsg;
	int									 m_timeInterval;

private:
	CVerticalLayoutUI*					m_Main_Lyt;
	CControlUI*							m_photo_Ctrl;
	CLabelUI*							m_Prompt_lab;

protected:
	virtual LPCTSTR GetWindowClassName() const;
	virtual CDuiString GetSkinFolder();
	virtual CDuiString GetSkinFile();

	Poco::Timer t;
	Poco::TimerCallback<CMonitoringUI> tc;

	bool is_need_report();
	void report_to_user();

	ActivityCommit _commit;

	int _notify_status;
};