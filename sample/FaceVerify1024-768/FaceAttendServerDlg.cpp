// FaceAttendServerDlg.cpp : implementation file
//

#include "stdafx.h"
#include "FaceAttendServer.h"
#include "FaceAttendServerDlg.h"
#include "LogBrowseDlg.h"
/*
#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif
*/
/////////////////////////////////////////////////////////////////////////////
// CNVL_TESTDlg dialog

CRITICAL_SECTION g_CriticalSection;
class MyLock
{
public:
	MyLock(){EnterCriticalSection(&g_CriticalSection);};
	~MyLock(){LeaveCriticalSection(&g_CriticalSection);};
};

CNVL_TESTDlg::CNVL_TESTDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CNVL_TESTDlg::IDD, pParent)
	, m_bHorizontalImage(FALSE)
{
	//{{AFX_DATA_INIT(CNVL_TESTDlg)
	m_nIdentifyThreshold = 75;
	//}}AFX_DATA_INIT
	// Note that LoadIcon does not require a subsequent DestroyIcon in Win32
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
	m_bCapThreadExit=TRUE;
	m_pCapThread=NULL;
	m_nIdentifyStatus=0;

	m_pLogPersonLiveBuf=NULL;
	m_bHorizontalImage=TRUE;

	m_bCapThreadWorking=TRUE;

	m_pIDFeature=NULL;
	m_pIDPhotoBuf=NULL;
	m_nIDPhotoWidth=102;
	m_nIDPhotoHeight=126;
	m_pCardReaderThread=NULL;
	m_bCardReaderThreadExit=FALSE;

	m_nCardReaderPort=1001;

	m_pffi=NULL;

}

void CNVL_TESTDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CNVL_TESTDlg)
	DDX_Text(pDX, IDC_THRESHOLD, m_nIdentifyThreshold);
	DDV_MinMaxInt(pDX, m_nIdentifyThreshold, 0, 100);
	DDX_Check(pDX, IDC_HORIZONTAL, m_bHorizontalImage);
	//}}AFX_DATA_MAP
	
}

BEGIN_MESSAGE_MAP(CNVL_TESTDlg, CDialog)
	ON_MESSAGE(WM_SAY_SORRY,OnSaySorry)
	//{{AFX_MSG_MAP(CNVL_TESTDlg)
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_WM_CLOSE()
	ON_BN_CLICKED(IDC_STUFF_MANAGE, OnStaffManage)
	ON_BN_CLICKED(IDC_CAMERA_CONFIG, OnCameraConfig)
	ON_BN_CLICKED(IDC_HORIZONTAL, OnBnClickedHorizontal)
	ON_BN_CLICKED(IDC_THRESHOLD_APPLY, OnBnClickedThresholdApply)
	//}}AFX_MSG_MAP
	ON_WM_CTLCOLOR()
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CNVL_TESTDlg message handlers

BOOL CNVL_TESTDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	// Set the icon for this dialog.  The framework does this automatically
	//  when the application's main window is not a dialog
	SetIcon(m_hIcon, TRUE);			// Set big icon
	SetIcon(m_hIcon, FALSE);		// Set small icon
	
	// TODO: Add extra initialization here
	
	InitializeCriticalSection(&g_CriticalSection);

	m_GuiBmp.LoadBitmap(IDB_GUI1); 
	((CStatic*)GetDlgItem(IDC_GUI))->SetBitmap((HBITMAP)m_GuiBmp);	
	SubButtons();

	//初始化人脸检测与人脸特征比对SDK
	THFI_Param param;

	param.nMinFaceSize=50;
	param.nRollAngle=45;
	param.bOnlyDetect=true;

	THFI_Create(1,&param);

	EF_Init(1);

	//人脸特征大小
	g_nFeatureSize=EF_Size();
	CString str;
	str.Format("size=%d",g_nFeatureSize);
//	AfxMessageBox(str);

	//读配置信息
	LoadConfig();

	::CreateDirectory(g_strAppPath+"\\mvp",NULL);

	g_hMainWnd=m_hWnd;

	///////////////
	//打开摄像头设备
	m_pCap[0]=MCapture::Create(&m_capParam[0]);
	if(NULL==m_pCap[0])
	{
		AfxMessageBox("Open WDM camera device failed.");
		ExitSystem();
		return FALSE;
	}
	BOOL bOK=m_pCap[0]->InitCapture();
	if(!bOK)
	{
		AfxMessageBox("Connect WDM camera device Failed");
		delete m_pCap[0];
		m_pCap[0]=NULL;
		ExitSystem();
		return FALSE;
	}

	m_strTopTip="请刷身份证";
	m_clTopTip=RGB(0,0,255);

	m_pCap[0]->Play();

//	m_nIdentifyThreshold=75;
//	m_nTimeOut=5;

	//是否显示调试信息控件
	if(m_nDebug==0)
		GetDlgItem(IDC_SCORE)->ShowWindow(SW_HIDE);
	else
		GetDlgItem(IDC_SCORE)->ShowWindow(SW_SHOW);


	UpdateData(FALSE);
	
	//启动视频捕获线程
	m_pCapThread=AfxBeginThread(VideoThreadProc,this);	
	//启动人脸比对线程
	m_pCompareThread=AfxBeginThread(CompareThreadProc,this);	

	//初始化身份证读卡器模块
	m_CardReader.InitModal();
	m_pIDFeature=new BYTE[g_nFeatureSize];
	
	//启动身份证读卡器线程
	m_pCardReaderThread=AfxBeginThread(CardReaderThreadProc,this);	

	return TRUE;  // return TRUE  unless you set the focus to a control
}

// If you add a minimize button to your dialog, you will need the code below
//  to draw the icon.  For MFC applications using the document/view model,
//  this is automatically done for you by the framework.

void CNVL_TESTDlg::OnPaint() 
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
HCURSOR CNVL_TESTDlg::OnQueryDragIcon()
{
	return (HCURSOR) m_hIcon;
}

void CNVL_TESTDlg::LoadConfig()
{
	char szSettingFile[256];
	strcpy(szSettingFile,g_strAppPath);
	strcat(szSettingFile,"\\config.ini");

//default value
	m_capParam[0].szCap.cx=640;
	m_capParam[0].szCap.cy=480;
	m_capParam[0].eType=CAP_WDM;
	m_capParam[0].lIndex=0;

	m_capParam[0].lIndex=GetPrivateProfileInt("camera","id",0,szSettingFile);//WDM设备ID
	m_capParam[0].szCap.cx=GetPrivateProfileInt("camera","cx",640,szSettingFile);//图像分辨率cx
	m_capParam[0].szCap.cy=GetPrivateProfileInt("camera","cy",480,szSettingFile);//图像分辨率cy

	m_nIdentifyThreshold=GetPrivateProfileInt("identify","threshold",75,szSettingFile);//人脸验证门限
	m_nDebug=GetPrivateProfileInt("identify","debug",0,szSettingFile);//是否显示调试信息
	m_nTimeOut=GetPrivateProfileInt("identify","timeout",5,szSettingFile);//人脸验证超时时间

	m_nCardReaderPort=GetPrivateProfileInt("cardreader","port",1001,szSettingFile);//身份证读卡器端口号
}


void CNVL_TESTDlg::OnCancel() 
{
	
	if(MessageBox("Really Exit System?          ","Confirm Exit",MB_YESNO|MB_ICONQUESTION|MB_DEFBUTTON2)==IDYES) 
	{
		ExitSystem();
		CDialog::OnCancel();
	}	
    
}
//退出系统，释放相关资源
void CNVL_TESTDlg::ExitSystem()
{
	try
	{
		m_bCardReaderThreadExit=TRUE;
		Sleep(300);
		m_bCapThreadExit=TRUE;
		Sleep(500);
		m_bCompareThreadExit=TRUE;
		Sleep(500);

		if(m_pCap[0])
		{
			m_pCap[0]->Stop();
			delete m_pCap[0];
			m_pCap[0]=NULL;
		}

		m_LogPersonBmp.DeleteObject();	
		m_LogPersonLiveBmp.DeleteObject();

		delete []m_pLogPersonLiveBuf;m_pLogPersonLiveBuf=NULL;
		delete []m_pIDFeature;m_pIDFeature=NULL;
		delete []m_pIDPhotoBuf;m_pIDPhotoBuf=NULL;	
	}
	catch(...)
	{
		m_LogPersonBmp.DeleteObject();	
		m_LogPersonLiveBmp.DeleteObject();	
		delete []m_pLogPersonLiveBuf;m_pLogPersonLiveBuf=NULL;
		delete []m_pIDFeature;m_pIDFeature=NULL;
		delete []m_pIDPhotoBuf;m_pIDPhotoBuf=NULL;		
	}

	DeleteCriticalSection(&g_CriticalSection);

	THFI_Release();
	EF_Release();
	
	m_CardReader.UnInitModal();

	CDialog::OnCancel();

	exit(0);
	
}
void CNVL_TESTDlg::OnClose() 
{
	OnCancel();
}

void CNVL_TESTDlg::OnStaffManage() 
{	
	CLogBrowseDlg dlg;
	dlg.DoModal();
}

//视频捕获线程，获取摄像头设备的帧图像，进行人脸检测和图像显示
UINT CNVL_TESTDlg::VideoThreadProc(LPVOID lpVoid)
{
	::CoInitialize(NULL);
	CNVL_TESTDlg* pDlg=(CNVL_TESTDlg*)lpVoid;

	pDlg->m_bCapThreadExit=FALSE;

	while(!pDlg->m_bCapThreadExit)
	{
		if(!pDlg->m_bCapThreadWorking)
		{
			Sleep(100);
			continue;
		}

		try
		{
			pDlg->ProcessCurrFrame();

		}
		catch(...)
		{
		
		}
		Sleep(30);
	}

	pDlg->m_pCapThread=NULL;
	::CoUninitialize();

	return 1;
}

void CNVL_TESTDlg::ProcessCurrFrame()
{

	GetDlgItem(IDC_VIDEO,&m_hVideoWnd);

	//判别人脸验证是否超时失败
	if(m_nIdentifyStatus==1)//当前为正在人脸验证状态
	{
		CTimeSpan timeSpan=CTime::GetCurrentTime()-m_StartIdentifyTime;
		if(timeSpan.GetSeconds()>=m_nTimeOut)//timeout , face verify failed
		{
			//显示人脸验证失败的相关信息并复位当前状态
			::SendMessage(g_hMainWnd,WM_SAY_SORRY,(WPARAM)0,(LPARAM)0);

			Sleep(200);
			m_nIdentifyStatus=0;
			m_bCompareThreadWorking=FALSE;
			SetDlgItemText(IDC_VERIFY_INFO,"人脸验证失败");
			m_strTopTip="人脸验证失败，请重刷身份证!";
			m_clTopTip=RGB(255,0,0);

			//写识别失败记录
			if(!m_FailedFace.empty())
			{
				Mat imFaceStd;
				resize(m_FailedFace,imFaceStd,Size(m_nIDPhotoWidth,m_nIDPhotoHeight),0,0,INTER_LINEAR);
				WriteLog(m_FailedLog,&imFaceStd);
				m_FailedFace.release();
			}
		}
	}

	long nWidth=m_capParam[0].szCap.cx;
	long nHeight=m_capParam[0].szCap.cy;
	long lLength=nWidth*nHeight*3;

	int i,j;

	BYTE* pCamBuf=new BYTE[lLength];
	
	//获取摄像头图像帧
	long lLegthCap=lLength;
	BOOL bGet=m_pCap[0]->GetFrame(pCamBuf, lLegthCap);
	MirrorDIB((LPSTR)pCamBuf, nWidth, nHeight, FALSE,24);
	if(m_bHorizontalImage)
		MirrorDIB((LPSTR)pCamBuf, nWidth, nHeight, TRUE,24);

	//当前为人脸验证状态，将进行人脸检测等，否则直接显示画面.
	if(m_nIdentifyStatus==1)
	{	
		DWORD t1,t2;
		t1=GetTickCount();

		//对帧图像进行人脸检测(仅检测一个人脸并且是最大人脸)
		THFI_FacePos ptfp[1];
		ptfp[0].dwReserved=(DWORD)new BYTE[512];//必须分配一段内存以返回人脸内部数据,用于适配新的特征提取算法
		int nFace=0;

		nFace=THFI_DetectFace(0,pCamBuf,24,nWidth,nHeight,ptfp,1);

		t2=GetTickCount();
		CString str;
		str.Format("time=%d",t2-t1);
	/////////////////////
		//检测到人脸
		if(nFace > 0)
		{
			if(m_nIdentifyStatus==1)
			{
				//通过全局内存与人脸验证线程交互人脸帧数据，
				MyLock lock;//必须锁定全局线程交互内存
				delete m_pffi;
				m_pffi=new FaceFrameInfo(nWidth,nHeight,&ptfp[0]);
				memcpy(m_pffi->pRgb24Buf,pCamBuf,nWidth*nHeight*3);
			}
			//在帧内存上绘制最大人脸框
			DrawFaceRects(pCamBuf,nWidth,nHeight,&ptfp[0].rcFace,1,RGB(0,255,0),4);
		}
	}

	//显示视频帧
	//show video frame
	BITMAPINFOHEADER bih;
	ContructBih(nWidth,nHeight,bih);
	MirrorDIB((LPSTR)pCamBuf, nWidth, nHeight, FALSE,24);
//	DrawBmpBuf(bih,pCamBuf,m_hVideoWnd,TRUE);

	DrawBmpBuf1(bih,pCamBuf,m_hVideoWnd,TRUE,(LPCTSTR)m_strTopTip,m_clTopTip);

	delete []pCamBuf;

}
//人脸验证线程(不断获取视频捕获线程中提交的全局人脸帧数据，进行人脸特征提取，并与身份证图片进行比对)
UINT CNVL_TESTDlg::CompareThreadProc(LPVOID lpVoid)
{
	CNVL_TESTDlg* pDlg=(CNVL_TESTDlg*)lpVoid;
	
	pDlg->CompareProcRun();

	pDlg->m_pCompareThread=NULL;

	return 0;
}

void CNVL_TESTDlg::CompareProcRun()
{
	m_bCompareThreadExit=FALSE;
	m_bCompareThreadWorking=FALSE;

	while(!m_bCompareThreadExit)
	{
		if(!m_bCompareThreadWorking)
		{
			Sleep(100);
			continue;
		}

		FaceFrameInfo* pffi=NULL;
		if(1)//获取全局人脸帧数据
		{
			MyLock lock;//必须锁定线程交互内存
			pffi=m_pffi;
			m_pffi=NULL;
		}

		if(pffi==NULL) 
		{
			Sleep(20);
			continue;
		}
		//人脸验证(当前摄像头帧人脸特征与身份证人脸特征比对)
		CompareProcess(pffi);
		delete pffi;
		Sleep(10);		
	}

}
//人脸验证逻辑(用当前摄像头的人脸特征与身份证人脸特征进行人脸验证)
void CNVL_TESTDlg::CompareProcess(FaceFrameInfo* pffi)
{

	BYTE* pCamBuf=pffi->pRgb24Buf;
	int nWidth=pffi->m_nWidth;
	int nHeight=pffi->m_nHeight;
	
	BYTE* pFeature=new BYTE[g_nFeatureSize];

	DWORD t1,t2;
	t1=GetTickCount();
	//EF_Extract的参数pFacialData必须是一个完整的THFI_FacePos,并且THFI_FacePos的成员dwReserved是合法内存(由THFI_DetectFace返回的人脸内部数据)
	int ret=EF_Extract(0,pCamBuf,nWidth,nHeight,3,(DWORD)pffi->pFaceData,pFeature);
	t2=GetTickCount();

	CString str1;
	str1.Format("time=%d",t2-t1);
//	SetWindowText(str1);

	if(!ret)
	{
		delete []pFeature;
		return;
	}
	
	double nBestScore=0;

	nBestScore=EF_Compare(m_pIDFeature,pFeature)*100+20.0f;//特征比对
	if(nBestScore>=99.99) nBestScore=99.99;

	if(nBestScore>=m_nIdentifyThreshold)//人脸验证成功
	{
		//复位当前工作状态
		m_nIdentifyStatus=0;
		m_bCompareThreadWorking=FALSE;
		delete m_pffi;m_pffi=NULL;

		//相关人脸验证成功UI信息
		CString str;
		str.Format("人脸验证成功[%s , 相似度: %d%%]",m_idi.strName,(int)nBestScore);//nScore
		SetDlgItemText(IDC_VERIFY_INFO,str);
		m_strTopTip.Format("人脸验证成功[%s],请刷身份证",m_idi.strName);
		m_clTopTip=RGB(0,255,0);


		//提取当前视频帧中的人脸画面(根据人脸框大小)
		int nFaceWidth=pffi->pFaceData->rcFace.right-pffi->pFaceData->rcFace.left;
		int nFaceHeight=pffi->pFaceData->rcFace.bottom-pffi->pFaceData->rcFace.top;

		BYTE* pFaceBuf=new BYTE[nFaceWidth*nFaceHeight*3];
		::GetPartImage(pCamBuf,nWidth,nHeight,pFaceBuf,pffi->pFaceData->rcFace.left,pffi->pFaceData->rcFace.top,pffi->pFaceData->rcFace.right,pffi->pFaceData->rcFace.bottom);

		BITMAPINFOHEADER bih;
		::ContructBih(nFaceWidth,nFaceHeight,bih);
	    LONG lLineBytes;
		lLineBytes = WIDTHBYTES(nFaceWidth *24);

		//4字节对齐图像缓冲
		BYTE* pTemp=new BYTE[lLineBytes*nFaceHeight];
		for(int i=0;i<nFaceHeight;i++)
		{
			for(int j=0;j<nFaceWidth;j++)
			{
				pTemp[i*lLineBytes+j*3+0]=pFaceBuf[(nFaceHeight-1-i)*nFaceWidth*3+j*3+0];
				pTemp[i*lLineBytes+j*3+1]=pFaceBuf[(nFaceHeight-1-i)*nFaceWidth*3+j*3+1];
				pTemp[i*lLineBytes+j*3+2]=pFaceBuf[(nFaceHeight-1-i)*nFaceWidth*3+j*3+2];
			}
		}
		//把人脸画面采样到固定大小(102*126)
		BYTE* pTemp1=new BYTE[WIDTHBYTES(m_nIDPhotoWidth *24)*m_nIDPhotoHeight];
		::ResampleBmp(bih,pTemp,this->m_nIDPhotoWidth,this->m_nIDPhotoHeight,pTemp1);

		//显示人脸画面
		m_LogPersonLiveBmp.DeleteObject();
		GetCBitmap(&bih, pTemp1,m_LogPersonLiveBmp);
		((CStatic*)GetDlgItem(IDC_RESULT_PHOTO))->SetBitmap((HBITMAP)m_LogPersonLiveBmp);

		//write log
		Mat imFace(nFaceHeight,nFaceWidth,CV_8UC3);
		imFace.data=pFaceBuf;
		Mat imFaceStd;
		resize(imFace,imFaceStd,Size(m_nIDPhotoWidth,m_nIDPhotoHeight),0,0,INTER_LINEAR);

		SYSTEMTIME	stTime = { NULL };
		GetLocalTime(&stTime);

		Log_Info li;
		li.logTime=stTime;

		strcpy(li.sName,m_idi.strName);
		strcpy(li.sSex,m_idi.strSex);
		strcpy(li.sNation,m_idi.strNation);
		strcpy(li.sBirthday,m_idi.strBirthday);
		strcpy(li.sAddress,m_idi.strAddress);
		strcpy(li.sId,m_idi.strID);
		strcpy(li.sStartDate,m_idi.strStartDate);
		strcpy(li.sEndDate,m_idi.strEndDate);
		strcpy(li.sDepartment,m_idi.strDepartment);

		li.fScore=nBestScore;
		li.nType=1;

		WriteLog(li,&imFaceStd);

		delete []pFaceBuf;
		delete []pTemp;
		delete []pTemp1;
	}
	else//记录验证识别信息
	{
		//提取当前视频帧中的人脸画面(根据人脸框大小)
		int nFaceWidth=pffi->pFaceData->rcFace.right-pffi->pFaceData->rcFace.left;
		int nFaceHeight=pffi->pFaceData->rcFace.bottom-pffi->pFaceData->rcFace.top;

		BYTE* pFaceBuf=new BYTE[nFaceWidth*nFaceHeight*3];
		::GetPartImage(pCamBuf,nWidth,nHeight,pFaceBuf,pffi->pFaceData->rcFace.left,pffi->pFaceData->rcFace.top,pffi->pFaceData->rcFace.right,pffi->pFaceData->rcFace.bottom);

		m_FailedFace.release();
		m_FailedFace.create(nFaceHeight,nFaceWidth,CV_8UC3);
		memcpy(m_FailedFace.data,pFaceBuf,nFaceWidth*nFaceHeight*3);

		SYSTEMTIME	stTime = { NULL };
		GetLocalTime(&stTime);

		m_FailedLog.logTime=stTime;

		strcpy(m_FailedLog.sName,m_idi.strName);
		strcpy(m_FailedLog.sSex,m_idi.strSex);
		strcpy(m_FailedLog.sNation,m_idi.strNation);
		strcpy(m_FailedLog.sBirthday,m_idi.strBirthday);
		strcpy(m_FailedLog.sAddress,m_idi.strAddress);
		strcpy(m_FailedLog.sId,m_idi.strID);
		strcpy(m_FailedLog.sStartDate,m_idi.strStartDate);
		strcpy(m_FailedLog.sEndDate,m_idi.strEndDate);
		strcpy(m_FailedLog.sDepartment,m_idi.strDepartment);

		m_FailedLog.fScore=nBestScore;
		m_FailedLog.nType=0;

		delete []pFaceBuf;

	}
	CString str;
	str.Format("score=%f",nBestScore);//nScore
	///SetWindowText(str);
	SetDlgItemText(IDC_SCORE,str);


	delete []pFeature;

}
//身份证读卡器线程，不断的帧听身份证设备，看是否放置身份证
UINT CNVL_TESTDlg::CardReaderThreadProc(LPVOID lpVoid)
{
	CNVL_TESTDlg* pDlg=(CNVL_TESTDlg*)lpVoid;
	
	pDlg->CardReaderProcRun();

	pDlg->m_pCompareThread=NULL;

	return 0;
}
//身份证读卡器侦听
void CNVL_TESTDlg::CardReaderProcRun()
{
	m_bCardReaderThreadExit=FALSE;

	BOOL bOpen=m_CardReader.OpenDevice(m_nCardReaderPort);//open card reader
	if(!bOpen)
	{
		AfxMessageBox("打开身份证读卡器失败.");
	}

	while(!m_bCardReaderThreadExit)
	{
		//如果当前正在进行人脸识别比对，就不检测当前身份证读卡器状态
		if(m_bCompareThreadWorking==TRUE) 
		{
			Sleep(300);
			continue;
		}
		

		if(m_CardReader.ValidateCard())
		{
			BYTE* pBuffer=new BYTE[64*1024];
			int len=64*1024;
			//读取身份证信息
			int ret=m_CardReader.ReadCard(m_idi,pBuffer,len);

			if(ret==0)
			{
	
				int i;

				//解码身份证图片信息
				vector<BYTE> pBuffs;
				pBuffs.reserve(len);
				for(i=0;i<len;i++)
					pBuffs.push_back(pBuffer[i]);

				Mat im;
				im = imdecode(Mat(pBuffs),CV_LOAD_IMAGE_COLOR);

				m_nIDPhotoWidth=im.cols;
				m_nIDPhotoHeight=im.rows;

				if(m_pIDPhotoBuf==NULL)
				{
					m_pIDPhotoBuf=new BYTE[m_nIDPhotoWidth*m_nIDPhotoHeight*3];
				}
				memcpy(m_pIDPhotoBuf,im.data,m_nIDPhotoWidth*m_nIDPhotoHeight*3);
				int nWidth=m_nIDPhotoWidth;
				int nHeight=m_nIDPhotoHeight;

				//人脸检测,仅检测一个人脸并且是最大人脸
				THFI_FacePos ptfp[1];
				ptfp[0].dwReserved=(DWORD)new BYTE[512];//必须分配一段内存以返回人脸内部数据,用于适配新的特征提取算法
				int nFace=THFI_DetectFace(0,m_pIDPhotoBuf,24,nWidth,nHeight,ptfp,1);
					
				if(nFace)
				{				
					//提取人脸特征
					memset(m_pIDFeature,0,g_nFeatureSize);
				
					int ret=EF_Extract(0,m_pIDPhotoBuf,nWidth,nHeight,3,(DWORD)&ptfp[0],m_pIDFeature);
				
					if(ret)
					{
						m_FailedFace.release();
						WriteIDPhoto(&im,(LPCTSTR)m_idi.strID);
						//复位当前工作状态信息，便于其他线程进行人脸验证
						m_bCompareThreadWorking=TRUE;
						m_StartIdentifyTime=CTime::GetCurrentTime();
						m_nIdentifyStatus=1;
						CString str;

						SetDlgItemText(IDC_NAME,m_idi.strName);
						SetDlgItemText(IDC_SEX,m_idi.strSex);
						SetDlgItemText(IDC_NATION,m_idi.strNation);
						SetDlgItemText(IDC_BIRTHDAY,m_idi.strBirthday);
						SetDlgItemText(IDC_ADDRESS,m_idi.strAddress);
						SetDlgItemText(IDC_ID,m_idi.strID);
						str.Format("%s-%s",m_idi.strStartDate,m_idi.strEndDate);
						SetDlgItemText(IDC_VALID_DAY,str);
						SetDlgItemText(IDC_DEPARTMENT,m_idi.strDepartment);

						SetDlgItemText(IDC_VERIFY_INFO,"人脸验证中...");
						m_strTopTip="人脸验证中...";
						m_clTopTip=RGB(0,0,255);


						//显示身份证图片信息到界面
						m_LogPersonBmp.DeleteObject();
						
						BITMAPINFOHEADER bih;
						::ContructBih(m_nIDPhotoWidth,m_nIDPhotoHeight,bih);
					    LONG lLineBytes;
						lLineBytes = WIDTHBYTES(m_nIDPhotoWidth *24);
						BYTE* pTemp=new BYTE[lLineBytes*m_nIDPhotoHeight];
						for(i=0;i<m_nIDPhotoHeight;i++)
						{
							for(int j=0;j<m_nIDPhotoWidth;j++)
							{
								pTemp[i*lLineBytes+j*3+0]=m_pIDPhotoBuf[(m_nIDPhotoHeight-1-i)*nWidth*3+j*3+0];
								pTemp[i*lLineBytes+j*3+1]=m_pIDPhotoBuf[(m_nIDPhotoHeight-1-i)*nWidth*3+j*3+1];
								pTemp[i*lLineBytes+j*3+2]=m_pIDPhotoBuf[(m_nIDPhotoHeight-1-i)*nWidth*3+j*3+2];
							}
						}
						
						GetCBitmap(&bih, pTemp,m_LogPersonBmp);
						((CStatic*)GetDlgItem(IDC_ID_PHOTO))->SetBitmap((HBITMAP)m_LogPersonBmp);
						delete []pTemp;


						m_LogPersonLiveBmp.DeleteObject();
						m_LogPersonLiveBmp.LoadBitmap(IDB_FACE); 
						((CStatic*)GetDlgItem(IDC_RESULT_PHOTO))->SetBitmap((HBITMAP)m_LogPersonLiveBmp);
					}
				}
				delete [](BYTE*)ptfp[0].dwReserved;
			}
			delete []pBuffer;
		}
		Sleep(300);
	}

	m_CardReader.CloseDevice();//close card reader

}
//人脸验证超时识别提示信息
LRESULT CNVL_TESTDlg::OnSaySorry(WPARAM wParam,LPARAM lParam)
{
	m_LogPersonLiveBmp.DeleteObject();
	m_LogPersonLiveBmp.LoadBitmap(IDB_SORRY); 
	((CStatic*)GetDlgItem(IDC_RESULT_PHOTO))->SetBitmap((HBITMAP)m_LogPersonLiveBmp);

	return 0;
}
//配置摄像头参数
void CNVL_TESTDlg::OnCameraConfig() 
{
	if(m_pCap[0])
		m_pCap[0]->DeviceConfig(0);
}

//水平镜像摄像头画面
void CNVL_TESTDlg::OnBnClickedHorizontal()
{
	// TODO: 在此添加控件通知处理程序代码
	UpdateData();
	g_bHorizontalImage=m_bHorizontalImage;
}
//设置人脸验证门限
void CNVL_TESTDlg::OnBnClickedThresholdApply()
{
	// TODO: 在此添加控件通知处理程序代码

	int nThreshold=GetDlgItemInt(IDC_THRESHOLD);

	char szSettingFile[256];
	strcpy(szSettingFile,g_strAppPath);
	strcat(szSettingFile,"\\config.ini");
	char t[16]="";

	if(m_nIdentifyThreshold!=nThreshold)
	{
		m_nIdentifyThreshold=nThreshold;

		sprintf(t,"%d",m_nIdentifyThreshold);
		WritePrivateProfileString("identify","threshold",t,szSettingFile);
	}


}


HBRUSH CNVL_TESTDlg::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor)
{
	HBRUSH hbr = CDialog::OnCtlColor(pDC, pWnd, nCtlColor);
	int nID = pWnd->GetDlgCtrlID();

	switch(nCtlColor)
	{

//	case CTLCOLOR_DLG:
//	case CTLCOLOR_EDIT:
	case CTLCOLOR_STATIC:

		pDC->SetBkMode(TRANSPARENT);
		static HBRUSH hbrgui=::CreateSolidBrush(RGB(0, 162, 232));
		return hbrgui;

//		return HBRUSH(GetStockObject(HOLLOW_BRUSH));//NULL_BRUSH; 
	}

	return hbr;
}

void CNVL_TESTDlg::SubButtons()
{
	COLORREF fg = RGB(0, 0, 0);
	COLORREF bg = RGB(0, 162, 232);

	m_button1.SubclassDlgItem(IDC_CAMERA_CONFIG, this);
	m_button2.SubclassDlgItem(IDC_STUFF_MANAGE, this);
	m_button3.SubclassDlgItem(IDC_THRESHOLD_APPLY, this);
	m_button4.SubclassDlgItem(IDCANCEL, this);

	m_button1.SetColour(fg, bg);
	m_button2.SetColour(fg, bg);
	m_button3.SetColour(fg, bg);
	m_button4.SetColour(fg, bg);

}
void CNVL_TESTDlg::OnOK()
{
	// TODO: 在此添加专用代码和/或调用基类


}
