// THDetectDemoDlg.cpp : implementation file
//

#include "stdafx.h"
#include "THDetectDemo.h"
#include "THDetectDemoDlg.h"

/*
#ifdef _DEBUG
#define new DEBUG_NEW
#endif
*/

// CAboutDlg dialog used for App About

class CAboutDlg : public CDialog
{
public:
	CAboutDlg();

// Dialog Data
	enum { IDD = IDD_ABOUTBOX };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

// Implementation
protected:
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialog(CAboutDlg::IDD)
{
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialog)
END_MESSAGE_MAP()


// CTHDetectDemoDlg dialog




CTHDetectDemoDlg::CTHDetectDemoDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CTHDetectDemoDlg::IDD, pParent)
	, m_strFileName(_T(""))
	, m_strPhoto1File(_T(""))
	, m_strPhoto2File(_T(""))
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CTHDetectDemoDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_FILENAME, m_strFileName);
	DDX_Text(pDX, IDC_FILENAME2, m_strPhoto1File);
	DDX_Text(pDX, IDC_FILENAME3, m_strPhoto2File);
}

BEGIN_MESSAGE_MAP(CTHDetectDemoDlg, CDialog)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	//}}AFX_MSG_MAP
	ON_BN_CLICKED(IDC_DETECT, &CTHDetectDemoDlg::OnBnClickedDetect)
	ON_WM_DESTROY()
	ON_BN_CLICKED(IDC_BROWSE, &CTHDetectDemoDlg::OnBnClickedBrowse)
	ON_BN_CLICKED(IDC_PLAY, &CTHDetectDemoDlg::OnBnClickedPlay)
	ON_BN_CLICKED(IDC_BROWSE2, &CTHDetectDemoDlg::OnBnClickedBrowse2)
	ON_BN_CLICKED(IDC_BROWSE3, &CTHDetectDemoDlg::OnBnClickedBrowse3)
END_MESSAGE_MAP()


// CTHDetectDemoDlg message handlers

BOOL CTHDetectDemoDlg::OnInitDialog()
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

	// TODO: Add extra initialization here
	THFI_Param param;

	param.nMinFaceSize=50;
	param.nRollAngle=45;
	param.bOnlyDetect=true;
	THFI_Create(1,&param);

	EF_Init(1);
	
	return TRUE;  // return TRUE  unless you set the focus to a control
}

void CTHDetectDemoDlg::OnSysCommand(UINT nID, LPARAM lParam)
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

void CTHDetectDemoDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // device context for painting

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

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

// The system calls this function to obtain the cursor to display while the user drags
//  the minimized window.
HCURSOR CTHDetectDemoDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}
 
bool Drawing=false;
Rect box;
void DrawRect(Mat* mat,Rect rc)
{
	rectangle(*mat, rc.tl(), rc.br(),cvScalar(255,0,0),2,CV_AA,0);
}


void CTHDetectDemoDlg::OnBnClickedDetect()
{

	UpdateData();

	Mat im=imread((LPCTSTR)m_strFileName);

	if(im.data==NULL)
	{
		AfxMessageBox("Load image file failed.");
		return;
	}
	CString str;

	DWORD t1,t2;

//	BYTE* pImgData = (BYTE *)(im.ptr());

	RECT* pRect=NULL;
	int nWidth=im.cols;
	int nHeight=im.rows;
	BYTE* pGray=new BYTE[nWidth*nHeight];
	for(int i=0;i<nHeight;i++)
	{
		for(int j=0;j<nWidth;j++)
		{
			pGray[i*nWidth+j]=(im.data[i*nWidth*3+j*3+0]+im.data[i*nWidth*3+j*3+1]+im.data[i*nWidth*3+j*3+2])/3;
		//	pGray[i*nWidth+j]=(pImgData[i*nWidth*3+j*3+0]+pImgData[i*nWidth*3+j*3+1]+pImgData[i*nWidth*3+j*3+2])/3;
		}
	}

	t1=GetTickCount();
	THFI_FacePos ptfp[100];
	int nNum=0;

	nNum=THFI_DetectFace(0,pGray,8,nWidth,nHeight,ptfp,100);

	t2=GetTickCount();

	for (int i=0;i<nNum;i++)
	{
		int offset=5;

		//face
		rectangle(im, Point(ptfp[i].rcFace.left,ptfp[i].rcFace.top), Point(ptfp[i].rcFace.right,ptfp[i].rcFace.bottom),cvScalar(0,255,0),2,CV_AA,0);
	//	continue;
		offset=2;
		//left eye
		line(im,Point(ptfp[i].ptLeftEye.x-offset,ptfp[i].ptLeftEye.y),Point(ptfp[i].ptLeftEye.x+offset,ptfp[i].ptLeftEye.y),cvScalar(0,255,0),2,CV_AA,0);
		line(im,Point(ptfp[i].ptLeftEye.x,ptfp[i].ptLeftEye.y-offset),Point(ptfp[i].ptLeftEye.x,ptfp[i].ptLeftEye.y+offset),cvScalar(0,255,0),2,CV_AA,0);
		//right eye
		line(im,Point(ptfp[i].ptRightEye.x-offset,ptfp[i].ptRightEye.y),Point(ptfp[i].ptRightEye.x+offset,ptfp[i].ptRightEye.y),cvScalar(0,255,0),2,CV_AA,0);
		line(im,Point(ptfp[i].ptRightEye.x,ptfp[i].ptRightEye.y-offset),Point(ptfp[i].ptRightEye.x,ptfp[i].ptRightEye.y+offset),cvScalar(0,255,0),2,CV_AA,0);
		//mouth
		line(im,Point(ptfp[i].ptMouth.x-offset,ptfp[i].ptMouth.y),Point(ptfp[i].ptMouth.x+offset,ptfp[i].ptMouth.y),cvScalar(0,255,0),2,CV_AA,0);
		line(im,Point(ptfp[i].ptMouth.x,ptfp[i].ptMouth.y-offset),Point(ptfp[i].ptMouth.x,ptfp[i].ptMouth.y+offset),cvScalar(0,255,0),2,CV_AA,0);
	}


	namedWindow("detected");
	imshow("detected",im);

	waitKey(30);

	delete []pGray;

}

void CTHDetectDemoDlg::OnDestroy()
{
	CDialog::OnDestroy();

	m_bExit=true;
	Sleep(200);
	// TODO: 在此处添加消息处理程序代码
	THFI_Release();

	EF_Release();
}

void CTHDetectDemoDlg::OnBnClickedBrowse()
{
	CFileDialog filedlg( TRUE, NULL, NULL, NULL, "Image files(*.bmp;*.jpg;*.png)|*.jpg;*.bmp;*.png||", this );
	if (filedlg.DoModal() == IDOK ) 
	{
		m_strFileName=filedlg.GetPathName();
	}

	UpdateData(0);
}
void DrawFaceRects(BYTE* pRgbBuf,int nBufWidth,int nBufHeight,
				   RECT* pFaceRects,int nRectNum,
				   COLORREF color,int nPenWidth)
{
	if(pRgbBuf==NULL||pFaceRects==NULL||nRectNum==0||nPenWidth==0) return;
	RECT rc;
	int rcWidth,rcHeight;
	int widthBytes=nBufWidth*3;
	BYTE* image=pRgbBuf;

	int i,j,k;
	for(i=0;i<nRectNum;i++)
	{
		rc=pFaceRects[i];
		rcWidth=rc.right-rc.left;
		rcHeight=rc.bottom-rc.top;

		for(j=rc.left;j<rc.right;j++)
		{
			for(int a=0;a<nPenWidth;a++)
			{
				image[j*3+(rc.top+a)*widthBytes+0]=GetBValue(color);
				image[j*3+(rc.top+a)*widthBytes+1]=GetGValue(color);
				image[j*3+(rc.top+a)*widthBytes+2]=GetRValue(color);
			}
		}
		for(j=rc.left;j<rc.right;j++)
		{
			for(int a=0;a<nPenWidth;a++)
			{
				image[j*3+(rc.bottom-a)*widthBytes+0]=GetBValue(color);
				image[j*3+(rc.bottom-a)*widthBytes+1]=GetGValue(color);
				image[j*3+(rc.bottom-a)*widthBytes+2]=GetRValue(color);
			}
		}
		for(k=rc.top;k<rc.bottom;k++)
		{
			for(int a=0;a<nPenWidth;a++)
			{
				image[(rc.left+a)*3+k*widthBytes+0]=GetBValue(color);
				image[(rc.left+a)*3+k*widthBytes+1]=GetGValue(color);
				image[(rc.left+a)*3+k*widthBytes+2]=GetRValue(color);
			}
		}
		for(k=rc.top;k<rc.bottom;k++)
		{
			for(int a=0;a<nPenWidth;a++)
			{
				image[(rc.right-a)*3+k*widthBytes+0]=GetBValue(color);
				image[(rc.right-a)*3+k*widthBytes+1]=GetGValue(color);
				image[(rc.right-a)*3+k*widthBytes+2]=GetRValue(color);
			}
		}
	}
}
void CTHDetectDemoDlg::OnBnClickedPlay()
{

	UpdateData();

	int i,j;
	long t_start, t_end;
	CString str;
	t_start = GetTickCount();
	//获取第一副照片
	Mat im1=imread((LPCTSTR)m_strPhoto1File);
	
	if(im1.empty()) 
	{
		AfxMessageBox("Load Photo1 File failed.");
		return ;
	}	

	int nWidth1=im1.cols;
	int nHeight1=im1.rows;

	//face detect
	THFI_FacePos ptfp1[1];
	int k;
	for(k=0;k<1;k++)
	{
		ptfp1[k].dwReserved=(DWORD)new BYTE[512];
	}
	//获取面部
	int nNum1=THFI_DetectFace(0,im1.data,24,nWidth1,nHeight1,ptfp1,1);//only process one face

	RECT rcFace=ptfp1[0].rcFace;
	//第一副照片需要比较的内容
	BYTE* pFeature1=new BYTE[EF_Size()];

	//only extract the first face(max size face)获取面部信息
	int ret=EF_Extract(0,im1.data,nWidth1,nHeight1,3,(DWORD)&ptfp1[0],pFeature1);
	if (ret)
	{
		
	}
	else
	{
		delete []pFeature1;
		pFeature1=NULL;
	}
	for(k=0;k<1;k++)
	{
		delete [](BYTE*)ptfp1[k].dwReserved;
	}
		
	//绘制图像
	/*for (i=0;i<nNum1;i++)
	{
		rectangle(im1, Point(ptfp1[i].rcFace.left,ptfp1[i].rcFace.top), Point(ptfp1[i].rcFace.right,ptfp1[i].rcFace.bottom),cvScalar(0,255,0),2,CV_AA,0);
	}

	imshow("Detect Photo1",im1);
*/
	//waitKey(30);
	//读取第二副照片信息
	Mat im2=imread((LPCTSTR)m_strPhoto2File);
	if(im2.empty()) 
	{
		delete []pFeature1;
		AfxMessageBox("Load Photo2 File failed.");
		return ;
	}

	int nWidth2=im2.cols;
	int nHeight2=im2.rows;

	//face detect获取面部信息
	THFI_FacePos ptfp2[1];
	for(k=0;k<1;k++)
	{
		ptfp2[k].dwReserved=(DWORD)new BYTE[512];
	}
	int nNum2=THFI_DetectFace(0,im2.data,24,nWidth2,nHeight2,ptfp2,1);


	rcFace=ptfp2[0].rcFace;

	BYTE* pFeature2=new BYTE[EF_Size()];

	//only extract the first face(max size face)获取比较内容
	ret=EF_Extract(0,im2.data,nWidth2,nHeight2,3,(DWORD)&ptfp2[0],pFeature2);
	if (ret)
	{

	}
	else
	{
		delete []pFeature2;
		pFeature2=NULL;
	}
	//绘制第二副图像
	/*for (i=0;i<nNum2;i++)
	{
		rectangle(im2, Point(ptfp2[i].rcFace.left,ptfp2[i].rcFace.top), Point(ptfp2[i].rcFace.right,ptfp2[i].rcFace.bottom),cvScalar(0,255,0),2,CV_AA,0);
	}*/

	for(k=0;k<1;k++)
	{
		delete [](BYTE*)ptfp2[k].dwReserved;
	}

	//imshow("Detect Photo2",im2);

	//waitKey(30);
	//开始比较
	if ((nNum1 > 0) && (nNum2 > 0))
	{
		float score = 0.0f;
		score = EF_Compare(pFeature1, pFeature2);
		t_end = GetTickCount();
		str.Format("s=%f, time=%d", score, t_end - t_start);
		AfxMessageBox(str);
	}
	else
	{
		str.Format("nNum1=%d, nNum2=%d", nNum1, nNum2);
		AfxMessageBox(str);
	}

	delete []pFeature1;
	delete []pFeature2;
	
}

void CTHDetectDemoDlg::OnBnClickedBrowse2()
{
	CFileDialog filedlg( TRUE, NULL, NULL, NULL, "Image files(*.bmp;*.jpg;*.png)|*.jpg;*.bmp;*.png||", this );
	if (filedlg.DoModal() == IDOK ) 
	{
		m_strPhoto1File=filedlg.GetPathName();
	}

	UpdateData(0);


}

void CTHDetectDemoDlg::OnCancel()
{
	// TODO: 在此添加专用代码和/或调用基类
	m_bExit=true;
	CDialog::OnCancel();
}


void CTHDetectDemoDlg::OnBnClickedBrowse3()
{
	CFileDialog filedlg( TRUE, NULL, NULL, NULL, "Image files(*.bmp;*.jpg;*.png)|*.jpg;*.bmp;*.png||", this );
	if (filedlg.DoModal() == IDOK ) 
	{
		m_strPhoto2File=filedlg.GetPathName();
	}

	UpdateData(0);
}

