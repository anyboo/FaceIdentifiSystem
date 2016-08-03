#pragma once
#include <DuiLib/UIlib.h>
#include <queue>
#include "QMFileSqlite.h"
#include "Poco/BasicEvent.h"
#include "BitMapCompare.h"
#include <Poco/ThreadPool.h>
#include "Recorder.h"
#include "CaptureNotify.h"

#define BT_CLOSE_MonWnd		(_T("close_btn2"))


class CMonitoringUI :
	public WindowImplBase,
	public CaptureNotify
{
public:
	CMonitoringUI();
	~CMonitoringUI();
	virtual void InitWindow();
	virtual void OnFinalMessage(HWND hWnd);
	virtual void Notify(TNotifyUI& msg);
	virtual void handle1(Poco::Notification* pNf);
	DUI_DECLARE_MESSAGE_MAP();

	void OnCloseWnd(TNotifyUI& msg);

	void ShowMonitInfoList();

	virtual LRESULT OnTimer(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);

	virtual LRESULT HandleCustomMessage(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
	
public:
	std::queue<writeCompareInfo>& getCompareQueue();
	std::queue<CapBitmapData>& getCapDataQueue();
private:
	void fireEvent(bool n)
	{
		m_theEvent(this, n);
	}

private:
	int				m_nBmp;
	int				m_testID;
	std::queue<writeCompareInfo> m_compare;
	Poco::BasicEvent<bool> m_theEvent;
	BitMapCompare *m_pCompare;
	std::queue<CapBitmapData> m_capdata;
	Recorder r;
	
protected:
	virtual LPCTSTR GetWindowClassName() const;
	virtual CDuiString GetSkinFolder();
	virtual CDuiString GetSkinFile();
};