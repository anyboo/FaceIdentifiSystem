// FaceAttendServerDlg.h : header file
//

#if !defined(AFX_FACEATTENDSERVERDLG_H__00E91E23_04B0_4D31_B6DB_4BAF8A455D2E__INCLUDED_)
#define AFX_FACEATTENDSERVERDLG_H__00E91E23_04B0_4D31_B6DB_4BAF8A455D2E__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

/////////////////////////////////////////////////////////////////////////////
// CNVL_TESTDlg dialog

#include "StuffManageDlg.h"
#include "CardReader.h"


#define WM_SAY_SORRY WM_USER+101
#define WM_IDENTIFY_VERIFY WM_USER+102

class CNVL_TESTDlg : public CDialog
{
// Construction
public:
	CNVL_TESTDlg(CWnd* pParent = NULL);	// standard constructor

// Dialog Data
	//{{AFX_DATA(CNVL_TESTDlg)
	enum { IDD = IDD_FACE_DIALOG };
	int		m_nIdentifyThreshold;
	int		m_nDebug;
	int		m_nTimeOut;
	int		m_nCardReaderPort;
	//}}AFX_DATA

	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CNVL_TESTDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support
	//}}AFX_VIRTUAL
public:
	void LoadConfig();
public:
	CBitmap m_GuiBmp;
private:
	CClrButton	m_button1;
	CClrButton	m_button2;
	CClrButton	m_button3;
	CClrButton	m_button4;

	void SubButtons();
private:
	CBitmap m_LogPersonBmp;
	CBitmap m_LogPersonLiveBmp;
	BYTE* m_pLogPersonLiveBuf;
	int m_nVideoWidth;
	int m_nVideoHeight;
protected:
	void ExitSystem();
public:
	//video thread
	CWinThread* m_pCapThread;
	BOOL m_bCapThreadExit;
	BOOL m_bCapThreadWorking;
	static UINT VideoThreadProc(LPVOID lpVoid);
	void ProcessCurrFrame();

public:
	//compare thread
	FaceFrameInfo* m_pffi;
	static UINT CompareThreadProc(LPVOID lpVoid);
	CWinThread* m_pCompareThread;
	BOOL m_bCompareThreadExit;
	BOOL m_bCompareThreadWorking;
	void CompareProcRun();
	void CompareProcess(FaceFrameInfo* pffi);
public:
	HWND m_hVideoWnd;
	long m_nIdentifyStatus;
	CTime m_StartIdentifyTime;
	CString m_strTopTip;
	COLORREF m_clTopTip;
public:
	//card reader thread
	CCardReader m_CardReader;
	BYTE* m_pIDFeature;
	BYTE* m_pIDPhotoBuf;
	int m_nIDPhotoWidth;
	int m_nIDPhotoHeight;
	IDInfo m_idi;

	static UINT CardReaderThreadProc(LPVOID lpVoid);
	CWinThread* m_pCardReaderThread;
	BOOL m_bCardReaderThreadExit;
	void CardReaderProcRun();
public:
	Log_Info m_FailedLog;
	Mat m_FailedFace;
// Implementation
protected:
	HICON m_hIcon;
	// Generated message map functions
	//{{AFX_MSG(CNVL_TESTDlg)
	virtual BOOL OnInitDialog();
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	virtual void OnCancel();
	afx_msg void OnClose();
	afx_msg void OnStaffManage();
	afx_msg void OnCameraConfig();

	//}}AFX_MSG
	LRESULT OnSaySorry(WPARAM wParam,LPARAM lParam);
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedHorizontal();
	BOOL m_bHorizontalImage;
	afx_msg void OnBnClickedThresholdApply();
	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
protected:
	virtual void OnOK();
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_FACEATTENDSERVERDLG_H__00E91E23_04B0_4D31_B6DB_4BAF8A455D2E__INCLUDED_)
