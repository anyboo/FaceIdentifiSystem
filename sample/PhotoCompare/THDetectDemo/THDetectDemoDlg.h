// THDetectDemoDlg.h : header file
//

#pragma once


// CTHDetectDemoDlg dialog
class CTHDetectDemoDlg : public CDialog
{
// Construction
public:
	CTHDetectDemoDlg(CWnd* pParent = NULL);	// standard constructor

// Dialog Data
	enum { IDD = IDD_THDETECTDEMO_DIALOG };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support
public:
	bool m_bExit;

// Implementation
protected:
	HICON m_hIcon;

	// Generated message map functions
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedDetect();
	afx_msg void OnDestroy();
	CString m_strFileName;
	afx_msg void OnBnClickedBrowse();
	afx_msg void OnBnClickedPlay();
	afx_msg void OnBnClickedBrowse2();
	CString m_strPhoto1File;
	CString m_strPhoto2File;
protected:
	virtual void OnCancel();
public:
	afx_msg void OnBnClickedBrowse3();
};
