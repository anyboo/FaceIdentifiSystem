#pragma once
#include <DuiLib/UIlib.h>
#include "Recorder.h"
#include "CaptureNotify.h"
#include "ActivityDispatcher.h"
#include <Poco/Timer.h>

class CRegisterInfo;

#define BT_CLOSE_MatchWnd		(_T("close_btn1"))
#define BT_OK_FILISH			(_T("Sign_In"))

class MatchUI :
	public WindowImplBase,
	public CaptureNotify
{
public:
	MatchUI();
	~MatchUI();
	void InitWindow();

	virtual void OnFinalMessage(HWND hWnd);
	virtual void Notify(TNotifyUI& msg);
	virtual void handle1(Poco::Notification* pNf);

	DUI_DECLARE_MESSAGE_MAP();

	void Backward(TNotifyUI& msg);
	void SignIn(TNotifyUI& msg);
	void ShowMatchInfo();
	virtual LRESULT HandleCustomMessage(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);

private:
	int				m_nBmp;
	CRegisterInfo*	m_RegInfo;
	Recorder recoder;
	ActivityDispatcher example;
	long  m_count;

	Poco::Timer checktime;
	Poco::TimerCallback<MatchUI> tc;
	bool enableCompare;
	bool painting;

	bool   m_closeApp;
	const int width = 640;
	const int height = 480;
	const int magic = 3;


	CLabelUI*	_name;
	CLabelUI*	_age;
	CLabelUI*	_birth;
	CLabelUI*	_address;
	CLabelUI*	_phone;
	CLabelUI*	_certificate;
	CLabelUI*	_sex;
	CLabelUI*	_matchMsg;
	CButtonUI*	_confirm;

	Picture::Ptr CurrentImage;

protected:
	virtual LPCTSTR GetWindowClassName() const;
	virtual CDuiString GetSkinFolder();
	virtual CDuiString GetSkinFile();
	void onTimer(Poco::Timer& timer);
	void match_resulut();
	void beginTime();
	void endTime();

	void BandingSubControl();
};
