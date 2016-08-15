
// RtspTestDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "RtspTest.h"
#include "RtspTestDlg.h"
#include "afxdialogex.h"
#include <string>
#include "CvvImage.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// 用于应用程序“关于”菜单项的 CAboutDlg 对话框

class CAboutDlg : public CDialogEx
{
public:
	CAboutDlg();

// 对话框数据
	enum { IDD = IDD_ABOUTBOX };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

// 实现
protected:
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialogEx(CAboutDlg::IDD)
{
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialogEx)
END_MESSAGE_MAP()


// CRtspTestDlg 对话框



CRtspTestDlg::CRtspTestDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(CRtspTestDlg::IDD, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CRtspTestDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CRtspTestDlg, CDialogEx)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDC_BUTTON1, &CRtspTestDlg::OnBnClickedButton1)
END_MESSAGE_MAP()


// CRtspTestDlg 消息处理程序

BOOL CRtspTestDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// 将“关于...”菜单项添加到系统菜单中。

	// IDM_ABOUTBOX 必须在系统命令范围内。
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != NULL)
	{
		BOOL bNameValid;
		CString strAboutMenu;
		bNameValid = strAboutMenu.LoadString(IDS_ABOUTBOX);
		ASSERT(bNameValid);
		if (!strAboutMenu.IsEmpty())
		{
			pSysMenu->AppendMenu(MF_SEPARATOR);
			pSysMenu->AppendMenu(MF_STRING, IDM_ABOUTBOX, strAboutMenu);
		}
	}

	// 设置此对话框的图标。  当应用程序主窗口不是对话框时，框架将自动
	//  执行此操作
	SetIcon(m_hIcon, TRUE);			// 设置大图标
	SetIcon(m_hIcon, FALSE);		// 设置小图标

	// TODO:  在此添加额外的初始化代码

	return TRUE;  // 除非将焦点设置到控件，否则返回 TRUE
}

void CRtspTestDlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	if ((nID & 0xFFF0) == IDM_ABOUTBOX)
	{
		CAboutDlg dlgAbout;
		dlgAbout.DoModal();
	}
	else
	{
		CDialogEx::OnSysCommand(nID, lParam);
	}
}

// 如果向对话框添加最小化按钮，则需要下面的代码
//  来绘制该图标。  对于使用文档/视图模型的 MFC 应用程序，
//  这将由框架自动完成。

void CRtspTestDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // 用于绘制的设备上下文

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// 使图标在工作区矩形中居中
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// 绘制图标
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialogEx::OnPaint();
	}
}

//当用户拖动最小化窗口时系统调用此函数取得光标
//显示。
HCURSOR CRtspTestDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}



void CRtspTestDlg::OnBnClickedButton1()
{
	RtspClient::init();
	//std::string szRtspURL("rtsp://184.72.239.149/vod/mp4://BigBuckBunny_175k.mov");
	std::string szRtspURL("rtsp://192.168.0.15:554/user=admin&password=&channel=1&stream=0.sdp");
	//std::string szRtspURL("rtsp://172.16.0.180:554/user=admin&password=&channel=1&stream=0.sdp");
	m_rtspClient->start(szRtspURL.c_str(), (DownloadCallback)CRtspTestDlg::RealFrameCBK, this);
	RtspClient::uninit();
}

void CRtspTestDlg::DrawPicToHDC(cv::Mat srcMat, UINT ID)
{
	HWND hFrameWnd;
	GetDlgItem(ID, &hFrameWnd);
	RtspClient::DrawBmpBuf(srcMat, hFrameWnd);
	/*IplImage * Image;
	CvSize Size = cvSize(srcMat.rows, srcMat.cols);
	Image = cvCreateImage(Size, IPL_DEPTH_8U, 3);
	memcpy(Image->imageData, srcMat.data, srcMat.rows*srcMat.cols * 3);
	Image->widthStep = srcMat.rows * 3;
	Image->origin = 0;

	CDC *pDC = GetDlgItem(ID)->GetDC();
	HDC hDC = pDC->GetSafeHdc();
	CRect rect;
	GetDlgItem(ID)->GetClientRect(&rect);
	CvvImage cimg;
	cimg.CopyOf(Image, 1);

	cimg.DrawToHDC(hDC, &rect);
	ReleaseDC(pDC);
	cvReleaseImage(&Image);*/
}

void CRtspTestDlg::RealFrameCBK(LPBYTE lpRealFrame, UINT nSize, int nWidth, int nHeight, UINT nUser)
{
	CRtspTestDlg * ptThis = (CRtspTestDlg *)nUser;

	cv::Mat srcMat1(nWidth, nHeight, CV_8UC3, (char *)lpRealFrame);
	//RtspClient::MirrorDIB((LPSTR)srcMat1.data, nWidth, nHeight, TRUE, 24);
	RtspClient::MirrorDIB((LPSTR)srcMat1.data, nWidth, nHeight, FALSE, 24);
	ptThis->DrawPicToHDC(srcMat1, IDC_VIDEO);
}