#if !defined(AFX_STUFFMANAGEDLG_H__2A2E3DE0_5BF0_4F68_9163_3A8A6FCC1F93__INCLUDED_)
#define AFX_STUFFMANAGEDLG_H__2A2E3DE0_5BF0_4F68_9163_3A8A6FCC1F93__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// StuffManageDlg.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CStaffManageDlg dialog

class CStaffManageDlg : public CDialog
{
// Construction
public:
	CStaffManageDlg(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CStaffManageDlg)
	enum { IDD = IDD_STUFF_MANAGE_DLG };
	CListCtrl	m_StuffListCtrl;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CStaffManageDlg)
	public:
	virtual BOOL DestroyWindow();
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL
private:
	void LoadStuffInfoToListCtrl();
	void UpdateStuffGuiInfo(PersonInfo* ppi);

	CBitmap m_StuffPhotoBmp;
// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CStaffManageDlg)
	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
	virtual BOOL OnInitDialog();
	afx_msg void OnItemchangedStuffListCtrl(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnDblclkStuffListCtrl(NMHDR* pNMHDR, LRESULT* pResult);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedCancel();
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_STUFFMANAGEDLG_H__2A2E3DE0_5BF0_4F68_9163_3A8A6FCC1F93__INCLUDED_)
