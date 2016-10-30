#pragma once
#include <DuiLib/UIlib.h>
//#include "RegisterInfo.h"
//#include "Recorder.h"
//#include "CaptureNotify.h"
//#include "QMFileSqlite.h"
//#include "RegUserInfo.h"
#include <cv.h>  
#include <cxcore.h>  
#include <highgui.h>


#define BT_CLOSERWND		(_T("close_btn"))
#define BT_GETPHOTO			(_T("photo"))
#define BT_OK_REGISTER		(_T("btn_ok"))


class CameraUI;
class CClipUI;
class RegisterUI :
	public WindowImplBase
{
public:
	RegisterUI();
	~RegisterUI();

	virtual void InitWindow();
	virtual void OnFinalMessage(HWND hWnd);
	virtual void Notify(TNotifyUI& msg);

	virtual CControlUI* CreateControl(LPCTSTR pstrClass);
	DUI_DECLARE_MESSAGE_MAP();

	void OnCloseRWnd(TNotifyUI& msg);
	/*void OnGetPhoto(TNotifyUI& msg);
	void OnRegister(TNotifyUI& msg);*/

	bool SaveRegisterInfo();

private:
	bool				m_closeApp;
	//writeUserInfo	m_userInfo;

	const int width = 640;
	const int height = 480;
	const int magic = 3;

private:
	CLabelUI*		m_Prompt_lab;

	//void OnCloseRWnd(TNotifyUI& msg);

protected:
	virtual LPCTSTR GetWindowClassName() const;
	virtual CDuiString GetSkinFolder();
	virtual CDuiString GetSkinFile();

	void BandingSubControl();

	void Backward(TNotifyUI& msg);
	void TakePhoto(TNotifyUI& msg);
	void SignUp(TNotifyUI& msg);
	
	bool isValidInformation();
	void SaveRegisterInformation(); 
	void commit_to_server();
	void GetDataFromUI();
	bool IsDuplicate();
	void add_user_info();
	void add_user_license();
	void add_user_identify();
	void Extract(IplImage *image, std::string datafile);
	void DisplayPhoto(const std::string& photo_file_path);
	void DisplayCameraUI();
	void add_user_to_onduty(int server_id);
private:
	bool   m_photo_agin;
	//Recorder r;
	//CClipUI* customizedImage;

	CEditUI*	_name;
	CEditUI*	_age;
	CEditUI*	_birth;
	CEditUI*	_address;
	CEditUI*	_phone;
	CEditUI*	_identify;
	CEditUI*	_license_code;
	CComboUI*	_license_level;
	CEditUI*	_issue_date;
	CEditUI*	_license_image;
	CHorizontalLayoutUI*	_photo_for_user;

	CComboUI*	_sex;
	CLabelUI*	_prompt;
	CButtonUI* _shutter;

	bool bAlreadyTaked;

	int _userinfo_id;
	int _license_id;
	int _identify_id;
	int _photo_id;
	int _unit_id;

	std::string _value_unit;
	std::string _value_name;
	std::string _value_birthday;
	std::string _value_address;
	std::string _value_cellphone;
	std::string _value_license_code;
	std::string _value_issue_date;
	std::string _value_identify_code;
	std::string _value_photo_path;
	std::string _value_feather_path;
	int _value_sex;
	int _value_level;

	CameraUI* _pCameraUI;
};