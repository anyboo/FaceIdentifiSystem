// TestDlg.h : header file
//

#if !defined(AFX_TESTDLG_H__85E8D483_7487_48F6_A87B_C37F8760B0A8__INCLUDED_)
#define AFX_TESTDLG_H__85E8D483_7487_48F6_A87B_C37F8760B0A8__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

/////////////////////////////////////////////////////////////////////////////
// CTestDlg dialog
#include "TiCapture2.h"

class CTestDlg : public CDialog
{
// Construction
public:
	CTestDlg(CWnd* pParent = NULL);	// standard constructor

// Dialog Data
	//{{AFX_DATA(CTestDlg)
	enum { IDD = IDD_TEST_DIALOG };
	BOOL	m_bEnroll;
	BOOL	m_bIdentify;
	//}}AFX_DATA

	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CTestDlg)
	public:
	virtual BOOL DestroyWindow();
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support
	//}}AFX_VIRTUAL
public:
	CCapture* m_pCap;
	SCapParam m_capParam;
	int m_iFrame;
	long m_nFeatureSize;
	
	BYTE* m_pEnrollFeature;
// Implementation
protected:
	HICON m_hIcon;

	// Generated message map functions
	//{{AFX_MSG(CTestDlg)
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	afx_msg void OnTimer(UINT nIDEvent);
	afx_msg void OnTest();
	afx_msg void OnStart();
	afx_msg void OnMacauTest();
	afx_msg void OnEnroll();
	afx_msg void OnIdentify();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_TESTDLG_H__85E8D483_7487_48F6_A87B_C37F8760B0A8__INCLUDED_)
