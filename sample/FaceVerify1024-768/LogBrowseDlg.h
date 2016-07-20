#pragma once
#include "afxcmn.h"
#include "afxdtctl.h"
#include "afxwin.h"


// CLogBrowseDlg 对话框

class CLogBrowseDlg : public CDialog
{
	DECLARE_DYNAMIC(CLogBrowseDlg)

public:
	CLogBrowseDlg(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CLogBrowseDlg();

// 对话框数据
	enum { IDD = IDD_LOG_DLG };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL DestroyWindow();
	afx_msg void OnDestroy();
	virtual BOOL OnInitDialog();
	CListCtrl m_LogListCtrl;
	afx_msg void OnBnClickedLogSearch();
	afx_msg void OnBnClickedCancel();
protected:
	virtual void OnOK();
public:
	CDateTimeCtrl m_DateTimeCtrl;
private:
	CClrButton	m_button1;
	CClrButton	m_button2;

	void SubButtons();
private:
	void ClearAllLogs();
	void AddLog(Log_Info* pli);
	void ShowLogInfo(Log_Info* pli);
private:
	CBitmap m_StaffPhotoBmp;
	CBitmap m_LogPhotoBmp;
public:
	afx_msg void OnLvnItemchangedLogListctrl(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnDtnDatetimechangeDatetimepicker(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnBnClickedDeleteLog();
	BOOL m_bInverseTime;
	afx_msg void OnBnClickedInverseTime();
	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
	CComboBox m_ComboBoxType;
	afx_msg void OnCbnSelchangeComboType();
	afx_msg void OnEnChangeLogTime();
};
