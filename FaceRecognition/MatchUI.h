#pragma once
#include <DuiLib/UIlib.h>
#include "Recorder.h"
#include "CaptureNotify.h"
#include "ActivityDispatcher.h"
#include <Poco/Timer.h>

class CRegisterInfo;

#define BT_CLOSE_MatchWnd		(_T("close_btn1"))
#define BT_OK_FILISH			(_T("Sign_In"))

enum IsSignIn
{
	SignIn_OK = 0,
	SignIn_CANCEL
};


class MatchUI :
	public WindowImplBase,
	public CaptureNotify
{
public:
	MatchUI();
	~MatchUI();
	void InitWindow();
	IsSignIn GetResult();
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
	long  m_count;
	IsSignIn		m_IsSignIn;

	Poco::Timer t;
	Poco::TimerCallback<MatchUI> tc;
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
