
// RtspTestDlg.h : 头文件
//

#pragma once

#include "RtspClient.h"
#include "afxwin.h"

// CRtspTestDlg 对话框
class CRtspTestDlg : public CDialogEx
{
// 构造
public:
	CRtspTestDlg(CWnd* pParent = NULL);	// 标准构造函数

// 对话框数据
	enum { IDD = IDD_RTSPTEST_DIALOG };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV 支持


// 实现
protected:
	HICON m_hIcon;

	// 生成的消息映射函数
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedButton1();
private:
	RtspClient *m_rtspClient;
	CString m_IP;
	bool  m_bStart;
public:
	static void __stdcall RealFrameCBK(const char *  lpRealFrame, UINT nSize, int nWidth, int nHeight, UINT nUser);
	void DrawPicToHDC(cv::Mat srcMat, UINT ID);
	afx_msg void OnBnClickedButton2();
	afx_msg void OnDestroy();	
	
private:
	bool checkIp(CString IP);
public:
	afx_msg void OnBnClickedButton3();
	afx_msg void OnBnClickedButton4();
};
