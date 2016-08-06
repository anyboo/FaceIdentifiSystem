#pragma once
#include <DuiLib/UIlib.h>
#include "Recorder.h"
#include "CaptureNotify.h"
#include "ActivityDispatcher.h"
#include <Poco/Timer.h>

class CRegisterInfo;

#define BT_CLOSE_MatchWnd		(_T("close_btn1"))
#define BT_SIGN_OUT			(_T("Sign_out"))

class CSignOutUI :
	public WindowImplBase,
	public CaptureNotify
{
public:
	CSignOutUI();
	~CSignOutUI();
	void InitWindow();
	virtual void OnFinalMessage(HWND hWnd);
	virtual void Notify(TNotifyUI& msg);
	virtual void handle1(Poco::Notification* pNf);

	DUI_DECLARE_MESSAGE_MAP();

	void OnCloseWnd(TNotifyUI& msg);
	void OnFilishMatch(TNotifyUI& msg);

	void ShowMatchInfo();

private:
	int				m_nBmp;
	CRegisterInfo*	m_RegInfo;
	Recorder r;
	ActivityDispatcher example;
	long m_count;

	Poco::Timer t;
	Poco::TimerCallback<CSignOutUI> tc;
	bool enableCompare;
	bool painting;
protected:
	virtual LPCTSTR GetWindowClassName() const;
	virtual CDuiString GetSkinFolder();
	virtual CDuiString GetSkinFile();
	void onTimer(Poco::Timer& timer);
	void match_resulut();
	void beginTime();
	void endTime();
};

