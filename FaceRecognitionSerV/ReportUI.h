#pragma once
#include <DuiLib/UIlib.h>

#define BT_CLOSE_REPORT			(_T("close_btn"))
#define BT_SELECT_ALL			(_T("All_Select"))

class CReportUI :
	public WindowImplBase
{
public:
	CReportUI();
	~CReportUI();

	virtual void OnFinalMessage(HWND hWnd);
	virtual void Notify(TNotifyUI& msg);

	DUI_DECLARE_MESSAGE_MAP();

	void OnCloseWnd(TNotifyUI& msg);
	void OnSelectAll(TNotifyUI& msg);

	void AddSubList();
	void SaveSelectOptionTag(std::string& strName);

private:
	int						m_optionSerial;
	std::vector<int>		m_SelectOptions;
protected:
	virtual LPCTSTR GetWindowClassName() const;
	virtual CDuiString GetSkinFolder();
	virtual CDuiString GetSkinFile();
};