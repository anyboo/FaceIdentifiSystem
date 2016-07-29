// TestDlg.cpp : implementation file
//

#include "stdafx.h"
#include "Test.h"
#include "TestDlg.h"
#include <math.h>
/*
#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

  */

static void SaveBmp(BITMAPINFO& bi, BYTE* pDataBuf);
/////////////////////////////////////////////////////////////////////////////
// CAboutDlg dialog used for App About

class CAboutDlg : public CDialog
{
public:
	CAboutDlg();

// Dialog Data
	//{{AFX_DATA(CAboutDlg)
	enum { IDD = IDD_ABOUTBOX };
	//}}AFX_DATA

	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CAboutDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	//{{AFX_MSG(CAboutDlg)
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialog(CAboutDlg::IDD)
{
	//{{AFX_DATA_INIT(CAboutDlg)
	//}}AFX_DATA_INIT
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CAboutDlg)
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialog)
	//{{AFX_MSG_MAP(CAboutDlg)
		// No message handlers
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CTestDlg dialog

CTestDlg::CTestDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CTestDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CTestDlg)
	m_bEnroll = FALSE;
	m_bIdentify = FALSE;
	//}}AFX_DATA_INIT
	// Note that LoadIcon does not require a subsequent DestroyIcon in Win32
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
	m_pCap=NULL;

	m_nFeatureSize=0;
}

void CTestDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CTestDlg)
	DDX_Check(pDX, IDC_ENROLL, m_bEnroll);
	DDX_Check(pDX, IDC_IDENTIFY, m_bIdentify);
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CTestDlg, CDialog)
	//{{AFX_MSG_MAP(CTestDlg)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_WM_TIMER()
	ON_BN_CLICKED(IDC_TEST, OnTest)
	ON_BN_CLICKED(IDC_START, OnStart)
	ON_BN_CLICKED(IDC_MACAU_TEST, OnMacauTest)
	ON_BN_CLICKED(IDC_ENROLL, OnEnroll)
	ON_BN_CLICKED(IDC_IDENTIFY, OnIdentify)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CTestDlg message handlers

BOOL CTestDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	// Add "About..." menu item to system menu.

	// IDM_ABOUTBOX must be in the system command range.
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != NULL)
	{
		CString strAboutMenu;
		strAboutMenu.LoadString(IDS_ABOUTBOX);
		if (!strAboutMenu.IsEmpty())
		{
			pSysMenu->AppendMenu(MF_SEPARATOR);
			pSysMenu->AppendMenu(MF_STRING, IDM_ABOUTBOX, strAboutMenu);
		}
	}

	// Set the icon for this dialog.  The framework does this automatically
	//  when the application's main window is not a dialog
	SetIcon(m_hIcon, TRUE);			// Set big icon
	SetIcon(m_hIcon, FALSE);		// Set small icon

	THFI_Param param;

	param.nMinFaceSize=50;
	param.nRollAngle=45;
	param.bOnlyDetect=true;
	THFI_Create(1,&param);

	short ret=EF_Init(1);
	if(ret==1)
	{
		//AfxMessageBox("Feature init ok");
	}

	m_nFeatureSize=EF_Size();
	m_pEnrollFeature=new BYTE[m_nFeatureSize];

	CString str;
	str.Format("Feature size=%d",m_nFeatureSize);

	OnStart();



	return TRUE;  // return TRUE  unless you set the focus to a control
}

void CTestDlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	if ((nID & 0xFFF0) == IDM_ABOUTBOX)
	{
		CAboutDlg dlgAbout;
		dlgAbout.DoModal();
	}
	else
	{
		CDialog::OnSysCommand(nID, lParam);
	}
}

// If you add a minimize button to your dialog, you will need the code below
//  to draw the icon.  For MFC applications using the document/view model,
//  this is automatically done for you by the framework.

void CTestDlg::OnPaint() 
{
	if (IsIconic())
	{
		CPaintDC dc(this); // device context for painting

		SendMessage(WM_ICONERASEBKGND, (WPARAM) dc.GetSafeHdc(), 0);

		// Center icon in client rectangle
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// Draw the icon
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialog::OnPaint();
	}
}

// The system calls this to obtain the cursor to display while the user drags
//  the minimized window.
HCURSOR CTestDlg::OnQueryDragIcon()
{
	return (HCURSOR) m_hIcon;
}

BOOL CTestDlg::DestroyWindow() 
{
	//bool b = m_pCap->SetVideoFile("video.avi");
	KillTimer(100);
	Sleep(200);

	THFI_Release();
	EF_Release();
	

	try
	{
		if(m_pCap)
		{
			m_pCap->Stop();
			delete m_pCap;
			m_pCap=NULL;
		}
		::CoUninitialize();
	}
	catch(...)
	{
	//	AfxMessageBox("exit error");
	}


	

	delete []m_pEnrollFeature;
	exit(0);

	return CDialog::DestroyWindow();
}


void ContructBih(int nWidth,int nHeight,BITMAPINFOHEADER& bih)
{
	bih.biSize=40; 						// header size
	bih.biWidth=nWidth;
	bih.biHeight=nHeight;
	bih.biPlanes=1;
	bih.biBitCount=24;					// RGB encoded, 24 bit
	bih.biCompression=BI_RGB;			// no compression
	bih.biSizeImage=nWidth*nHeight*3;
	bih.biXPelsPerMeter=0;
	bih.biYPelsPerMeter=0;
	bih.biClrUsed=0;
	bih.biClrImportant=0;
}

#include <string>
void DrawBmpBuf(BITMAPINFOHEADER& bih,BYTE* pDataBuf,HWND hShowWnd,BOOL bFitWnd)
{
	RECT rc;
	::GetWindowRect( hShowWnd, &rc );
	int nWidth = rc.right - rc.left-2;
	int nHeight = rc.bottom - rc.top-2;

	BITMAPINFO bi;
	memset(&bi,0,sizeof(bi));
	memcpy( &(bi.bmiHeader), &bih, sizeof(BITMAPINFOHEADER) );
	int iWidth=bih.biWidth;
	int iHeight=bih.biHeight;

	SaveBmp(bi, pDataBuf);

	// display bitmap
	HDC hdcStill = ::GetDC( hShowWnd );
	PAINTSTRUCT ps;
	::BeginPaint( hShowWnd, &ps );


	::SetStretchBltMode( hdcStill, COLORONCOLOR );

	if(bFitWnd)
		::StretchDIBits( hdcStill, 0, 0, nWidth, nHeight,
			0, 0, iWidth, iHeight, pDataBuf, &bi,
			DIB_RGB_COLORS,	SRCCOPY );
	else
		::StretchDIBits( hdcStill, 0, 0, iWidth, iHeight,
			0, 0, iWidth, iHeight, pDataBuf, &bi,
			DIB_RGB_COLORS,	SRCCOPY );


	::EndPaint( hShowWnd, &ps );
	::ReleaseDC( hShowWnd, hdcStill );

}


void SaveBmp(BITMAPINFO& bi, BYTE* pDataBuf)
{
	
	DWORD word = 0;
	CString Path = "d:\\bmp\\";
	CString name = "test";
	CString ext = ".bmp";
	name += std::to_string(::GetTickCount()).c_str();
	Path.Append(name);
	Path.Append(ext);
	::OutputDebugStringA(Path + "\n");
	HANDLE hfile = CreateFile(Path, GENERIC_WRITE, FILE_SHARE_READ, 0, OPEN_ALWAYS, 0, 0);
	//fileheader
	BITMAPFILEHEADER bmfh;                         // Other BMP header
	int nBitsOffset = sizeof(BITMAPFILEHEADER) + bi.bmiHeader.biSize;
	LONG lImageSize = bi.bmiHeader.biSizeImage;
	LONG lFileSize = nBitsOffset + lImageSize;
	bmfh.bfType = 'B' + ('M' << 8);
	bmfh.bfOffBits = nBitsOffset;
	bmfh.bfSize = lFileSize;
	bmfh.bfReserved1 = bmfh.bfReserved2 = 0;
	WriteFile(hfile, &bmfh, sizeof(BITMAPFILEHEADER), &word, 0);
	//header
	WriteFile(hfile, &bi.bmiHeader, sizeof(BITMAPINFOHEADER), &word, 0);
	//info
	//WriteFile(hfile, &bi, sizeof(BITMAPINFO), &word, 0);
	//data
	int bit = COLORONCOLOR;
	WriteFile(hfile, pDataBuf, bi.bmiHeader.biWidth * bi.bmiHeader.biHeight * bit/*bih.biBitCount*/, &word, 0);
	CloseHandle(hfile);
}

void TiDrawFaceRects(BYTE* pRgbBuf,int nBufWidth,int nBufHeight,
				   RECT* pFaceRects,int nRectNum,
				   COLORREF color,int nPenWidth)
{
	if(pRgbBuf==NULL||pFaceRects==NULL||nRectNum==0||nPenWidth==0) return;
	RECT rc;
	int rcWidth,rcHeight;
	int widthBytes=nBufWidth*3;
	BYTE* image=pRgbBuf;

	int i,j,k,a;

	for(i=0;i<nRectNum;i++)
	{
		rc=pFaceRects[i];

		if(rc.left<=0) rc.left =1;
		if(rc.top<=0) rc.top=1;
		if(rc.right>=nBufWidth-1) rc.right=nBufWidth-2;
		if(rc.bottom>=nBufHeight-1) rc.bottom=nBufHeight-2;

		rcWidth=rc.right-rc.left;
		rcHeight=rc.bottom-rc.top;

		//top line
		for(j=rc.left;j<rc.right;j++)
		{
			for(a=0;a<nPenWidth;a++)
			{
				if(rc.top+a>nBufHeight-1) continue;
				image[j*3+(rc.top+a)*widthBytes+0]=GetBValue(color);
				image[j*3+(rc.top+a)*widthBytes+1]=GetGValue(color);
				image[j*3+(rc.top+a)*widthBytes+2]=GetRValue(color);
			}
		}
		//bottom line
		for(j=rc.left;j<rc.right;j++)
		{
			for(a=0;a<nPenWidth;a++)
			{
				if(rc.bottom-a<0) continue;
				image[j*3+(rc.bottom-a)*widthBytes+0]=GetBValue(color);
				image[j*3+(rc.bottom-a)*widthBytes+1]=GetGValue(color);
				image[j*3+(rc.bottom-a)*widthBytes+2]=GetRValue(color);
			}
		}
		//left line
		for(k=rc.top;k<rc.bottom;k++)
		{
			for(int a=0;a<nPenWidth;a++)
			{
				if(rc.left+a>nBufWidth-1) continue;
				image[(rc.left+a)*3+k*widthBytes+0]=GetBValue(color);
				image[(rc.left+a)*3+k*widthBytes+1]=GetGValue(color);
				image[(rc.left+a)*3+k*widthBytes+2]=GetRValue(color);
			}
		}
		//right line
		for(k=rc.top;k<rc.bottom;k++)
		{
			for(a=0;a<nPenWidth;a++)
			{
				if(rc.right-a<0) continue;

				image[(rc.right-a)*3+k*widthBytes+0]=GetBValue(color);
				image[(rc.right-a)*3+k*widthBytes+1]=GetGValue(color);
				image[(rc.right-a)*3+k*widthBytes+2]=GetRValue(color);
			}
		}
	}
}
#define WIDTHBYTES(bits)    (((bits) + 31) / 32 * 4)
BOOL MirrorDIB(LPSTR lpDIBBits, LONG lWidth, LONG lHeight, BOOL bDirection,int nImageBits)
{	 
	 // 指向源图像的指针
	 LPSTR	lpSrc; 
	 // 指向要复制区域的指针
	 LPSTR	lpDst;	 
	 // 指向复制图像的指针
	 LPSTR	lpBits;
	 HLOCAL	hBits;	 
	 // 循环变量
	 LONG	i;
	 LONG	j;
	 int nBits;//每像素占的位数
	 // 图像每行的字节数
	 LONG lLineBytes;
	 // 计算图像每行的字节数
	 lLineBytes = WIDTHBYTES(lWidth *nImageBits);
	 // 暂时分配内存，以保存一行图像
	 hBits = LocalAlloc(LHND, lLineBytes);
	 if (hBits == NULL)
	 {
		 // 分配内存失败
		 return FALSE;
	 }	 
	 // 锁定内存
	 lpBits = (char * )LocalLock(hBits);
	 int nStep=nImageBits/8;
	 long lCenter=lWidth/2*nStep;
	 // 判断镜像方式
	 if (bDirection)
	 {
		 // 水平镜像
		 // 针对图像每行进行操作
		 for(i = 0; i < lHeight; i++)
		 {
			 // 针对每行图像左半部分进行操作
			 for(j = 0; j < lCenter; j+=nStep)
			 {
				 for(nBits=0;nBits<nStep;nBits++)
				 {
					 lpSrc = (char *)lpDIBBits + lLineBytes * i +lCenter- j+nBits;
					 lpDst = (char *)lpDIBBits + lLineBytes * i +lCenter+ j+nBits;
					 *lpBits = *lpDst;
					 *lpDst = *lpSrc;
					 *lpSrc = *lpBits;
				 }
				 
			 }
			 
		 }
	 }
	 else
	 {
		 // 垂直镜像
		 // 针对上半图像进行操作
		 for(i = 0; i < lHeight / 2; i++)
		 {		 
			 // 指向倒数第i行象素起点的指针
			 lpSrc = (char *)lpDIBBits + lLineBytes * i;	 
			 // 指向第i行象素起点的指针
			 lpDst = (char *)lpDIBBits + lLineBytes * (lHeight - i - 1);		 
			 // 备份一行，宽度为lWidth
			 memcpy(lpBits, lpDst, lLineBytes);
			 // 将倒数第i行象素复制到第i行
			 memcpy(lpDst, lpSrc, lLineBytes);
			 // 将第i行象素复制到倒数第i行
			 memcpy(lpSrc, lpBits, lLineBytes);
			 
		 }
	 }	 
	 // 释放内存
	 LocalUnlock(hBits);
	 LocalFree(hBits);
	 // 返回
	 return TRUE;
}

void CTestDlg::OnTimer(UINT nIDEvent) 
{
	long nWidth=m_capParam.szCap.cx;
	long nHeight=m_capParam.szCap.cy;
	long lLength=nWidth*nHeight*3;
	
	HWND hFrameWnd;
	GetDlgItem(IDC_VIDEO,&hFrameWnd);
	
	
	BYTE* pDataBuf=NULL;
	
	BYTE* pCamBuf=new BYTE[lLength];
	BOOL b=m_pCap->GetFrame(pCamBuf, lLength);
	if(!b)
	{
		delete []pCamBuf;
		return;
	}
	MirrorDIB((LPSTR)pCamBuf, nWidth, nHeight, TRUE,24);
	MirrorDIB((LPSTR)pCamBuf, nWidth, nHeight, FALSE,24);

	BYTE* pFeature=new BYTE[m_nFeatureSize];

	THFI_FacePos ptfp[1];
	int k;
	for(k=0;k<1;k++)
	{
		ptfp[k].dwReserved=(DWORD)new BYTE[512];
	}
	int nFace=THFI_DetectFace(0,pCamBuf,24,nWidth,nHeight,ptfp,1);//only process one face

	CString str;


	if(nFace > 0)
	{
		RECT* pFace=new RECT[nFace];
		BOOL bMouth=TRUE;
		for(int i=0;i<nFace;++i)
		{
			pFace[i]=ptfp[i].rcFace;
		}

		if(m_bEnroll)//enroll
		{
			int ret=EF_Extract(0,pCamBuf,nWidth,nHeight,3,(DWORD)&ptfp[0],m_pEnrollFeature);

			m_bEnroll=FALSE;
			UpdateData(FALSE);
			AfxMessageBox("Enroll OK.");
		}
		else if(m_bIdentify)//compare
		{
			int ret=EF_Extract(0,pCamBuf,nWidth,nHeight,3,(DWORD)&ptfp[0],pFeature);

			float score=EF_Compare(pFeature,m_pEnrollFeature);

			CString str;
	    	str.Format("score=%d",(int)(score*100));
			SetDlgItemText(IDC_SCORE,str);
		}	


		//draw all face rectangle
		TiDrawFaceRects(pCamBuf,nWidth,nHeight,pFace,nFace,RGB(0,255,0),4);

		//only draw the first face(eye,mouth...) 
		RECT rcLeft,rcRight,rcMouth,rcNose;
		int offset=5;
		//left eye rectangle
		rcLeft.left=ptfp[0].ptLeftEye.x-offset;
		rcLeft.top=ptfp[0].ptLeftEye.y-offset;
		rcLeft.right=ptfp[0].ptLeftEye.x+offset;
		rcLeft.bottom=ptfp[0].ptLeftEye.y+offset;

		//right eye rectangle
		rcRight.left=ptfp[0].ptRightEye.x-offset;
		rcRight.top=ptfp[0].ptRightEye.y-offset;
		rcRight.right=ptfp[0].ptRightEye.x+offset;
		rcRight.bottom=ptfp[0].ptRightEye.y+offset;

		//mouth rectangle
		rcMouth.left=ptfp[0].ptMouth.x-offset;
		rcMouth.top=ptfp[0].ptMouth.y-offset;
		rcMouth.right=ptfp[0].ptMouth.x+offset;
		rcMouth.bottom=ptfp[0].ptMouth.y+offset;

		//nose rectangle
		rcNose.left=ptfp[0].ptNose.x-offset;
		rcNose.top=ptfp[0].ptNose.y-offset;
		rcNose.right=ptfp[0].ptNose.x+offset;
		rcNose.bottom=ptfp[0].ptNose.y+offset;

		//draw
		if(0)
		{
			TiDrawFaceRects(pCamBuf,nWidth,nHeight,&rcLeft,1,RGB(0,255,0),4);
			TiDrawFaceRects(pCamBuf,nWidth,nHeight,&rcRight,1,RGB(0,255,0),4);
			TiDrawFaceRects(pCamBuf,nWidth,nHeight,&rcMouth,1,RGB(0,255,0),4);
			TiDrawFaceRects(pCamBuf,nWidth,nHeight,&rcNose,1,RGB(0,255,0),4);
		}
		else
		{
			RECT temp[4];
			temp[0]=rcLeft;
			temp[1]=rcRight;
			temp[2]=rcMouth;
			temp[3]=rcNose;

			TiDrawFaceRects(pCamBuf,nWidth,nHeight,temp,4,RGB(0,255,0),4);
		}

		delete []pFace;
		
	}
	else
	{
	//	SetWindowText("No Face!");
	}

	for(k=0;k<1;k++)
	{
		delete [](BYTE*)ptfp[k].dwReserved;
	}

	MirrorDIB((LPSTR)pCamBuf, nWidth, nHeight, FALSE,24);
	//释放资源,显示视频帧
	BITMAPINFOHEADER bih;
	ContructBih(nWidth,nHeight,bih);
	
	DrawBmpBuf(bih,pCamBuf,hFrameWnd,TRUE);
	delete []pCamBuf;
	delete []pFeature;

	CDialog::OnTimer(nIDEvent);
}

void CTestDlg::OnTest() 
{
	m_pCap->DeviceConfig(0);
}


void CTestDlg::OnStart() 
{
	CWaitCursor cursor;
	m_capParam.szCap.cx=640;
	m_capParam.szCap.cy=480;
	m_capParam.eType=CAP_WDM;
	m_capParam.lIndex=0;	
	//memset(&m_capParam, sizeof(m_capParam),0);
	
	m_pCap=CCapture::Create(&m_capParam);
	
	if(NULL==m_pCap)
	{
		AfxMessageBox("Open camera device failed.");
		return;
	}
	/*{
		bool OK = m_pCap->SetVideoFile(_T("d:\\video.mp4"));
		if (!OK)
		{
		AfxMessageBox("SetVideoFile Failed");
		}
		}*/
	
	BOOL bOK=m_pCap->InitCapture();
	if(!bOK)
	{
		AfxMessageBox("Connect camera device Failed");
		delete m_pCap;
		m_pCap=NULL;
		return;
	}
	/*{
		bOK = m_pCap->SetVideoFile(_T("d:\\video.mp4"));
		if (!bOK)
		{
		AfxMessageBox("SetVideoFile Failed");
		}
		}*/
	m_pCap->Play();
	// TODO: Add extra initialization here
	
	SetTimer(100,50,NULL);	


	GetDlgItem(IDC_START)->EnableWindow(FALSE);
	
}

void CTestDlg::OnMacauTest() 
{

}

void CTestDlg::OnEnroll() 
{
	// TODO: Add your control notification handler code here
	UpdateData();

}

void CTestDlg::OnIdentify() 
{
	// TODO: Add your control notification handler code here
	UpdateData();
}
