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
	, ptfp(CTestDlg::maxFace)
{
	//{{AFX_DATA_INIT(CTestDlg)
	m_bEnroll = FALSE;
	m_bIdentify = FALSE;
	//}}AFX_DATA_INIT
	// Note that LoadIcon does not require a subsequent DestroyIcon in Win32
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
	m_pCap=NULL;

	m_nFeatureSize=0;

	m_pGrayCached = new BYTE[MAXIMG_W_H];
	m_pGrayZoom = new BYTE[MAXIMG_W_H];
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
#ifdef NEWSDK
	THFaceInit(0);
	m_nFeatureSize = THFaceGetFeatureSize();
#else
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
	m_nFeatureSize = EF_Size();
#endif
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

	KillTimer(100);
	Sleep(200);
#ifdef NEWSDK
	THFaceUnInit();
#else
	THFI_Release();
	EF_Release();
#endif

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
	 // 指向复制图像的指�
	 LPSTR	lpBits;
	 HLOCAL	hBits;	 
	 // 循环变量
	 LONG	i;
	 LONG	j;
	 int nBits;//每像素占的位�
	 // 图像每行的字节数
	 LONG lLineBytes;
	 // 计算图像每行的字节数
	 lLineBytes = WIDTHBYTES(lWidth *nImageBits);
	 // 暂时分配内存，以保存一行图�
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
			 // 将倒数第i行象素复制到第i�
			 memcpy(lpDst, lpSrc, lLineBytes);
			 // 将第i行象素复制到倒数第i�
			 memcpy(lpSrc, lpBits, lLineBytes);
			 
		 }
	 }	 
	 // 释放内存
	 LocalUnlock(hBits);
	 LocalFree(hBits);
	 // 返回
	 return TRUE;
}

#include "FrImageFunc.h"
#ifdef NEWSDK
void CTestDlg::NewSDKMethod()
{
	long nWidth = m_capParam.szCap.cx;
	long nHeight = m_capParam.szCap.cy;
	long lLength = nWidth*nHeight * 3;
	int bpp = 24;
	const int maxFace = 1;

	HWND hFrameWnd;
	GetDlgItem(IDC_VIDEO, &hFrameWnd);


	BYTE* pDataBuf = NULL;

	BYTE* pCamBuf = new BYTE[lLength];
	BOOL b = m_pCap->GetFrame(pCamBuf, lLength);
	if (!b)
	{
		delete[]pCamBuf;
		return;
	}

	BYTE * ptRgb8 = NULL;
	ThFacePos ptfp[maxFace];
	BYTE* pFeature = new BYTE[m_nFeatureSize];
	int nFace = 0;

	{
		// 转换成灰度图
		Rgb24_To_Rgb8(pCamBuf, m_pGrayCached, nWidth, nHeight);
		ptRgb8 = m_pGrayCached;

		//缩放为原来图像的一�?		
		ResizeImage(m_pGrayCached, nWidth, nHeight, m_pGrayZoom, nWidth / 3, nHeight / 3, 1);
		ptRgb8 = m_pGrayZoom;
		
		nFace = THFaceDetectMT(ptRgb8, nWidth / 3, nHeight / 3, ptfp, pFeature);
		//pThis->vecRect.clear();
		for (int i = 0; i < nFace; ++i)
		{
			ptfp[i].rcFace.left *= 3;
			ptfp[i].rcFace.top *= 3;
			ptfp[i].rcFace.right *= 3;
			ptfp[i].rcFace.bottom *= 3;
			ptfp[i].ptRightEye.x *= 3;
			ptfp[i].ptLeftEye.x *= 3;

			RECT rcFace = ptfp[i].rcFace;
			//pThis->vecRect.push_back(rcFace);
	}
	
		FlipV_RGB(pCamBuf, nWidth, nHeight, m_pGrayCached, MAXIMG_W_H);
		ptRgb8 = m_pGrayCached;

		bpp = 8;

		delete[]pCamBuf;
		pCamBuf = ptRgb8;


		nWidth = nWidth / 3;
		nHeight = nHeight / 3;
	}

	//MirrorDIB((LPSTR)pCamBuf, nWidth, nHeight, TRUE, bpp);
	//MirrorDIB((LPSTR)pCamBuf, nWidth, nHeight, FALSE, bpp);

	if (nFace > 0)
	{
		RECT* pFace = new RECT[nFace];
		BOOL bMouth = TRUE;
		for (int i = 0; i<nFace; ++i)
		{
			pFace[i] = ptfp[i].rcFace;

			{
				RECT f = ptfp[i].rcFace;
				CString rc;
				rc.Format("Face Rect: left:%d, top:%d, right:%d, bottom:%d\n", f.left, f.top, f.right, f.bottom);
				//OutputDebugStringA(rc);

				POINT angle = ptfp[i].ptAngle;
				CString ale;
				ale.Format("FaceAngle: x:%d, y:%d", angle.x, angle.y);
				OutputDebugStringA(ale);

			}
		}

		if (m_bEnroll)//enroll
		{
			//int ret = EF_Extract(0, pCamBuf, nWidth, nHeight, 3, (DWORD)&ptfp[0], m_pEnrollFeature);

			m_bEnroll = FALSE;
			UpdateData(FALSE);
			AfxMessageBox("Enroll OK.");
		}
		else if (m_bIdentify)//compare
		{
			long t_start, t_end;

			{
				t_start = GetTickCount();

				float score = THFaceCompare(pFeature, m_pEnrollFeature);

				t_end = GetTickCount();
				CString time;
				time.Format("EF_Compare:time : %d\n", t_end - t_start);
				OutputDebugStringA(time);

			CString str;
				str.Format("score=%d", (int)(score * 100));
				SetDlgItemText(IDC_SCORE, str);
			}

		}


		//draw all face rectangle
		TiDrawFaceRects(pCamBuf, nWidth, nHeight, pFace, nFace, RGB(0, 255, 0), 4);

		//only draw the first face(eye,mouth...) 
		RECT rcLeft, rcRight, rcMouth, rcNose;
		int offset = 5;
		//left eye rectangle
		rcLeft.left = ptfp[0].ptLeftEye.x - offset;
		rcLeft.top = ptfp[0].ptLeftEye.y - offset;
		rcLeft.right = ptfp[0].ptLeftEye.x + offset;
		rcLeft.bottom = ptfp[0].ptLeftEye.y + offset;

		//right eye rectangle
		rcRight.left = ptfp[0].ptRightEye.x - offset;
		rcRight.top = ptfp[0].ptRightEye.y - offset;
		rcRight.right = ptfp[0].ptRightEye.x + offset;
		rcRight.bottom = ptfp[0].ptRightEye.y + offset;

		//mouth rectangle
		rcMouth.left = ptfp[0].ptMouth.x - offset;
		rcMouth.top = ptfp[0].ptMouth.y - offset;
		rcMouth.right = ptfp[0].ptMouth.x + offset;
		rcMouth.bottom = ptfp[0].ptMouth.y + offset;

		//nose rectangle
		rcNose.left = ptfp[0].ptAngle.x - offset;
		rcNose.top = ptfp[0].ptAngle.y - offset;
		rcNose.right = ptfp[0].ptAngle.x + offset;
		rcNose.bottom = ptfp[0].ptAngle.y + offset;

		//draw
		{
			RECT temp[4];
			temp[0] = rcLeft;
			temp[1] = rcRight;
			temp[2] = rcMouth;
			temp[3] = rcNose;

			TiDrawFaceRects(pCamBuf, nWidth, nHeight, temp, 4, RGB(0, 255, 0), 4);
		}

		delete[]pFace;

		}

	for (int k = 0; k<1; k++)
	{
		delete[](BYTE*)ptfp[k].dwReserved;
	}

	//MirrorDIB((LPSTR)pCamBuf, nWidth, nHeight, FALSE, bpp);
	//释放资源,显示视频�
	BITMAPINFOHEADER bih;
	ContructBih(nWidth, nHeight, bih);

	DrawBmpBuf(bih, pCamBuf, hFrameWnd, TRUE);
	//delete []pCamBuf;
	delete[]pFeature;
}
#else

void CTestDlg::HandleFaceImage()
{
	BYTE * ptRgb8 = NULL;
	long  W = nWidth / 3 , H = nHeight / 3;
	bpp = 8;
	//// 转换成灰度图
	
	Rgb24_To_Rgb8(pCamBuf, m_pGrayCached, nWidth, nHeight);
	ptRgb8 = m_pGrayCached;

	//缩放为原来图像的一�?		
	ResizeImage(m_pGrayCached, nWidth, nHeight, m_pGrayZoom, W, H, 1);
	ptRgb8 = m_pGrayZoom;

	long t_start, t_end;
	t_start = GetTickCount();

	MirrorDIB((LPSTR)ptRgb8, W, H, TRUE, bpp);
	MirrorDIB((LPSTR)ptRgb8, W, H, FALSE, bpp);

	t_end = GetTickCount();
	CString time;
	time.Format("MirrorDIB:time : %d\n", t_end - t_start);
	OutputDebugStringA(time);

	t_start = GetTickCount();
	ptfp.resize(maxFace);

	int nFace = THFI_DetectFace(0, ptRgb8, bpp, W, H, &ptfp[0], maxFace);//only process one face

	t_end = GetTickCount();
	time.Format("THFI_DetectFace:time : %d face number : %d  maxFace : %d\n", t_end - t_start, nFace, maxFace);
	OutputDebugStringA(time);


	if (nFace > 0)
	{
		pFace.resize(nFace);
		BOOL bMouth = TRUE;
		for (int i = 0; i < nFace; ++i)
		{
			pFace[i] = ptfp[i].rcFace;

			{
				RECT f = ptfp[i].rcFace;
				CString rc;
				rc.Format("Face Rect: left:%d, top:%d, right:%d, bottom:%d\n", f.left, f.top, f.right, f.bottom);
				OutputDebugStringA(rc);

				FaceAngle angle = ptfp[i].fAngle;
				CString ale;
				ale.Format("FaceAngle: pitch:%d, roll:%d, yaw:%d\n", angle.pitch, angle.roll, angle.yaw);
				OutputDebugStringA(ale);

				CString other;
				other.Format("nQuality:%d\n", ptfp[i].nQuality);
				OutputDebugStringA(rc + ale + other);
			}
		}

		if (m_bEnroll)//enroll
		{
			int ret = EF_Extract(0, ptRgb8, W, H, 3, (DWORD)&ptfp[0], m_pEnrollFeature);

			m_bEnroll = FALSE;
			UpdateData(FALSE);
			//AfxMessageBox("Enroll OK.");
		}
		else if (m_bIdentify)//compare
		{
			std::vector<BYTE> pFeature(m_nFeatureSize);

			long t_start, t_end;

		{
				t_start = GetTickCount();

				int ret = EF_Extract(0, ptRgb8, W, H, 3, (DWORD)&ptfp[0], &pFeature[0]);

				t_end = GetTickCount();
				CString time;
				time.Format("EF_Extract:time : %d, nWidth:%d, nHeight:%d\n", t_end - t_start, W, H);
				OutputDebugStringA(time);
		}

			{
				t_start = GetTickCount();

				float score = EF_Compare(&pFeature[0], m_pEnrollFeature);

				t_end = GetTickCount();
				CString time;
				time.Format("EF_Compare:time : %d, score : %f \n", t_end - t_start, score);
				OutputDebugStringA(time);

				CString str;
				str.Format("score=%d", (int)(score * 100));
				SetDlgItemText(IDC_SCORE, str);
			}
		}
	}
}

void CTestDlg::DrawFacialfeatures(BYTE* pRgbBuf, int nBufWidth, int nBufHeight)
{
	if (ptfp.empty()) return;

	for (size_t i = 0; i < 1; ++i)
	{
		//only draw the first face(eye,mouth...) 
		RECT rcLeft, rcRight, rcMouth, rcNose;
		int offset = 5;
		//left eye rectangle
		rcLeft.left = ptfp[i].ptLeftEye.x - offset;
		rcLeft.top = ptfp[i].ptLeftEye.y - offset;
		rcLeft.right = ptfp[i].ptLeftEye.x + offset;
		rcLeft.bottom = ptfp[i].ptLeftEye.y + offset;

		//right eye rectangle
		rcRight.left = ptfp[i].ptRightEye.x - offset;
		rcRight.top = ptfp[i].ptRightEye.y - offset;
		rcRight.right = ptfp[i].ptRightEye.x + offset;
		rcRight.bottom = ptfp[i].ptRightEye.y + offset;

		//mouth rectangle
		rcMouth.left = ptfp[i].ptMouth.x - offset;
		rcMouth.top = ptfp[i].ptMouth.y - offset;
		rcMouth.right = ptfp[i].ptMouth.x + offset;
		rcMouth.bottom = ptfp[i].ptMouth.y + offset;

		//nose rectangle
		rcNose.left = ptfp[i].ptNose.x - offset;
		rcNose.top = ptfp[i].ptNose.y - offset;
		rcNose.right = ptfp[i].ptNose.x + offset;
		rcNose.bottom = ptfp[i].ptNose.y + offset;

		//draw
	{
			RECT temp[4];
			temp[0] = rcLeft;
			temp[1] = rcRight;
			temp[2] = rcMouth;
			temp[3] = rcNose;

			TiDrawFaceRects(pRgbBuf, nBufWidth, nBufHeight, temp, 4, RGB(0, 255, 0), 4);
	}
	}
}

void CTestDlg::DramFaceRect()
{

	if (!m_pGrayCached)
		return;

	HWND hFrameWnd;
	GetDlgItem(IDC_VIDEO, &hFrameWnd);

	//draw all face rectangle
	if (!pFace.empty())
	{
		TiDrawFaceRects(m_pGrayCached, nWidth, nHeight, &pFace[0], pFace.size(), RGB(0, 255, 0), 4);
		DrawFacialfeatures(m_pGrayCached, nWidth, nHeight);
		pFace.clear();
	}

	//MirrorDIB((LPSTR)m_pGrayCached, nWidth, nHeight, FALSE, bpp);
	//释放资源,显示视频�
	BITMAPINFOHEADER bih;
	ContructBih(nWidth, nHeight, bih);
	FlipV_RGB(pCamBuf, nWidth, nHeight, m_pGrayCached, MAXIMG_W_H);

	DrawBmpBuf(bih, m_pGrayCached, hFrameWnd, TRUE);
	
}

void CTestDlg::OldSDKMethod()
{
	nWidth = m_capParam.szCap.cx;
	nHeight = m_capParam.szCap.cy;

	long lLength = nWidth*nHeight * 3;
	pCamBuf = new BYTE[lLength];

	BOOL b = m_pCap->GetFrame(pCamBuf, lLength);
	if (!b)
	{
		delete[]pCamBuf;
		return;
	}

	HandleFaceImage();
	DramFaceRect();

	delete[]pCamBuf;
}
#endif
void CTestDlg::OnTimer(UINT nIDEvent) 
{
#ifdef NEWSDK
	NewSDKMethod();
#else
	OldSDKMethod();
#endif
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
	
	m_pCap=CCapture::Create(&m_capParam);
	
	if(NULL==m_pCap)
	{
		AfxMessageBox("Open camera device failed.");
		return;
	}
	BOOL bOK=m_pCap->InitCapture();
	if(!bOK)
	{
		AfxMessageBox("Connect camera device Failed");
		delete m_pCap;
		m_pCap=NULL;
		return;
	}
	
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
