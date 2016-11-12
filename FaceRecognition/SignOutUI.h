#pragma once
#include <DuiLib/UIlib.h>
#include <Poco/Timer.h>

class CameraUI;
class CRegisterInfo;

#define BT_CLOSE_MatchWnd		(_T("close_btn1"))
#define BT_SIGN_OUT			(_T("Sign_out"))

class CSignOutUI :
	public WindowImplBase
{
	struct Feather
	{
		int id;
		std::string path;
		std::vector<char> feather;
	};

	typedef std::vector<Feather> FeatherGroup;
public:

	CSignOutUI();
	~CSignOutUI();
	void InitWindow();
	virtual void OnFinalMessage(HWND hWnd);
	virtual void Notify(TNotifyUI& msg);
	virtual CControlUI* CreateControl(LPCTSTR pstrClass);
	DUI_DECLARE_MESSAGE_MAP();

	void OnCloseWnd(TNotifyUI& msg);
	void OnFilishMatch(TNotifyUI& msg);

	void ShowMatchInfo();

protected:
	virtual LPCTSTR GetWindowClassName() const;
	virtual CDuiString GetSkinFolder();
	virtual CDuiString GetSkinFile();
	void onTimer(Poco::Timer& timer);
	void popData2UI(int id);
	void update_db_onduty(int id);
	void commit_to_server(int id);
private:
	Poco::Timer _watch_dog;
	Poco::TimerCallback<CSignOutUI> _callback;

	std::string _value_unit;
	std::string _value_name;
	std::string _value_birthday;
	std::string _value_address;
	std::string _value_cellphone;
	std::string _value_license_code;
	std::string _value_issue_date;
	std::string _value_identify_code;
	std::string _value_photo_path;

	int _value_sex;
	int _value_level;
	int license_id;
	int identify_id;
	bool _StopWatchdog;
	bool		m_closeApp;

	CameraUI* _pCameraUI;
	FeatherGroup _fgroup;
	int _id_from_detected;
	int _simility_precent;
};

