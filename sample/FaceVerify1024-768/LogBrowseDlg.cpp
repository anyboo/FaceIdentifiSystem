// LogBrowseDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "FaceAttendServer.h"
#include "LogBrowseDlg.h"


// CLogBrowseDlg 对话框

IMPLEMENT_DYNAMIC(CLogBrowseDlg, CDialog)

CLogBrowseDlg::CLogBrowseDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CLogBrowseDlg::IDD, pParent)
	, m_bInverseTime(TRUE)
{

}

CLogBrowseDlg::~CLogBrowseDlg()
{
}

void CLogBrowseDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_LOG_LISTCTRL, m_LogListCtrl);
	DDX_Control(pDX, IDC_DATETIMEPICKER, m_DateTimeCtrl);
	DDX_Check(pDX, IDC_INVERSE_TIME, m_bInverseTime);
	DDX_Control(pDX, IDC_COMBO_TYPE, m_ComboBoxType);
}


BEGIN_MESSAGE_MAP(CLogBrowseDlg, CDialog)
	ON_WM_DESTROY()
	ON_BN_CLICKED(IDC_LOG_SEARCH, &CLogBrowseDlg::OnBnClickedLogSearch)
	ON_BN_CLICKED(IDCANCEL, &CLogBrowseDlg::OnBnClickedCancel)
	ON_NOTIFY(LVN_ITEMCHANGED, IDC_LOG_LISTCTRL, &CLogBrowseDlg::OnLvnItemchangedLogListctrl)
	ON_NOTIFY(DTN_DATETIMECHANGE, IDC_DATETIMEPICKER, &CLogBrowseDlg::OnDtnDatetimechangeDatetimepicker)
	ON_BN_CLICKED(IDC_DELETE_LOG, &CLogBrowseDlg::OnBnClickedDeleteLog)
	ON_BN_CLICKED(IDC_INVERSE_TIME, &CLogBrowseDlg::OnBnClickedInverseTime)
	ON_WM_CTLCOLOR()
	ON_CBN_SELCHANGE(IDC_COMBO_TYPE, &CLogBrowseDlg::OnCbnSelchangeComboType)
	ON_EN_CHANGE(IDC_LOG_TIME, &CLogBrowseDlg::OnEnChangeLogTime)
END_MESSAGE_MAP()


// CLogBrowseDlg 消息处理程序

BOOL CLogBrowseDlg::DestroyWindow()
{
	// TODO: 在此添加专用代码和/或调用基类

	return CDialog::DestroyWindow();
}

void CLogBrowseDlg::OnDestroy()
{
	CDialog::OnDestroy();

	// TODO: 在此处添加消息处理程序代码
	
}

BOOL CLogBrowseDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	// TODO:  在此添加额外的初始化

	RECT rc; 
	m_LogListCtrl.GetWindowRect(&rc);
	m_LogListCtrl.InsertColumn(0, "姓名", LVCFMT_CENTER, (rc.right - rc.left)*30/100, -1);
	m_LogListCtrl.InsertColumn(1, "时间", LVCFMT_CENTER, (rc.right - rc.left)*30/100, -1);
	m_LogListCtrl.InsertColumn(2, "类型", LVCFMT_CENTER, (rc.right - rc.left)*20/100, -1);

    DWORD dwExStyle =  LVS_EX_FLATSB|LVS_EX_FULLROWSELECT | LVS_EX_HEADERDRAGDROP |LVS_EX_GRIDLINES;
    m_LogListCtrl.SetExtendedStyle(dwExStyle);

	m_ComboBoxType.SetCurSel(0);
	OnBnClickedLogSearch();
	SubButtons();

	return TRUE;  // return TRUE unless you set the focus to a control
	// 异常: OCX 属性页应返回 FALSE
}

void CLogBrowseDlg::OnBnClickedLogSearch()
{
	// TODO: 在此添加控件通知处理程序代码

	int nCurSel=m_ComboBoxType.GetCurSel();

	CWaitCursor wc;

	ClearAllLogs();

	COleDateTime selDateTime;
	m_DateTimeCtrl.GetTime(selDateTime);

	CString strLogFile;
	strLogFile.Format("%s\\id_logs\\%04d-%02d-%02d\\%04d-%02d-%02d.dat",
		g_strAppPath,
		selDateTime.GetYear(), selDateTime.GetMonth(), selDateTime.GetDay(),
		selDateTime.GetYear(), selDateTime.GetMonth(), selDateTime.GetDay());

	try
	{
		CFile file;
		if(file.Open(strLogFile,CFile::modeRead,NULL))
		{
			int nFileLen=file.GetLength();
			int nLogNum=nFileLen/sizeof(Log_Info);
			for(int i=0;i<nLogNum;i++)
			{
				Log_Info* pli=new Log_Info;
				file.Read(pli,sizeof(Log_Info));

				if(nCurSel==0)//all
				{
					AddLog(pli);
				}
				else if(nCurSel==1)//success
				{
					if(pli->nType==1)
					{
						AddLog(pli);
					}
				}
				else if(nCurSel==2)//failed
				{
					if(pli->nType==0)
					{
						AddLog(pli);
					}
				}
				else
				{

				}
				
			}
			file.Close();
		}
	}
	catch(...)
	{
		//AfxMessageBox("file error");
	}

}
void CLogBrowseDlg::AddLog(Log_Info* pli)
{
	int nListCount=m_LogListCtrl.GetItemCount();
	if(m_bInverseTime) nListCount=0;

	m_LogListCtrl.InsertItem(nListCount,pli->sName);

	CString strLogTime;
	strLogTime.Format("%04d-%02d-%02d %02d:%02d:%02d",
		pli->logTime.wYear,pli->logTime.wMonth,pli->logTime.wDay,
		pli->logTime.wHour,pli->logTime.wMinute,pli->logTime.wSecond);

	m_LogListCtrl.SetItemText(nListCount,1,strLogTime);
	if(pli->nType==1)
	{
		m_LogListCtrl.SetItemText(nListCount,2,"验证成功");
	}
	else
	{
		m_LogListCtrl.SetItemText(nListCount,2,"验证失败");
	}

	m_LogListCtrl.SetItemData(nListCount,(DWORD)pli);

}
void CLogBrowseDlg::ClearAllLogs()
{
	int nListCount=m_LogListCtrl.GetItemCount();
	for(int i=0;i<nListCount;i++)
	{
		Log_Info* pli=(Log_Info*)m_LogListCtrl.GetItemData(i);
		delete pli;
	}	
	m_LogListCtrl.DeleteAllItems();

	m_StaffPhotoBmp.LoadBitmap(IDB_FACE);
	((CStatic*)GetDlgItem(IDC_STAFF_PHOTO))->SetBitmap((HBITMAP)m_StaffPhotoBmp);

	m_LogPhotoBmp.LoadBitmap(IDB_FACE);
	((CStatic*)GetDlgItem(IDC_LOG_PHOTO))->SetBitmap((HBITMAP)m_LogPhotoBmp);

	SetDlgItemText(IDC_NAME,"");
	SetDlgItemText(IDC_SEX,"");
	SetDlgItemText(IDC_NATION,"");
	SetDlgItemText(IDC_BIRTHDAY,"");
	SetDlgItemText(IDC_ADDRESS,"");
	SetDlgItemText(IDC_ID2,"");
	SetDlgItemText(IDC_VALID_DAY2,"");
	SetDlgItemText(IDC_DEPARTMENT,"");
	
	SetDlgItemText(IDC_LOG_TIME,"");
	SetDlgItemText(IDC_IDENTIFY_SCORE,"");


}
void CLogBrowseDlg::OnBnClickedCancel()
{
	// TODO: 在此添加控件通知处理程序代码
	ClearAllLogs();
	OnCancel();
}

void CLogBrowseDlg::OnOK()
{
	// TODO: 在此添加专用代码和/或调用基类
//	CDialog::OnOK();
}
void CLogBrowseDlg::ShowLogInfo(Log_Info* pli)
{
	CString strLogTime;
	strLogTime.Format("%04d-%02d-%02d %02d:%02d:%02d",
		pli->logTime.wYear,pli->logTime.wMonth,pli->logTime.wDay,
		pli->logTime.wHour,pli->logTime.wMinute,pli->logTime.wSecond);
	CString strScore;
	strScore.Format("%lf",pli->fScore);

	SetDlgItemText(IDC_LOG_TIME,strLogTime);

	SetDlgItemText(IDC_IDENTIFY_SCORE,strScore);

	SetDlgItemText(IDC_NAME,pli->sName);
	SetDlgItemText(IDC_SEX,pli->sSex);
	SetDlgItemText(IDC_NATION,pli->sNation);
	SetDlgItemText(IDC_BIRTHDAY,pli->sBirthday);
	SetDlgItemText(IDC_ADDRESS,pli->sAddress);
	SetDlgItemText(IDC_ID2,pli->sId);
	CString str;
	str.Format("%s-%s",pli->sStartDate,pli->sEndDate);
	SetDlgItemText(IDC_VALID_DAY2,str);
	SetDlgItemText(IDC_DEPARTMENT,pli->sDepartment);

	//show staff photo
	m_StaffPhotoBmp.DeleteObject();
	
	CString strPathName;
	strPathName.Format("%s\\id_logs\\ID_Photos\\%s.jpg",g_strAppPath,pli->sId);

	int i,j;
	BITMAPINFOHEADER bih;
try
{
	Mat im=imread((LPCTSTR)strPathName);
	if(im.empty()) 
	{
		m_StaffPhotoBmp.LoadBitmap(IDB_FACE);
		((CStatic*)GetDlgItem(IDC_STAFF_PHOTO))->SetBitmap((HBITMAP)m_StaffPhotoBmp);
	}
	else
	{
		::ContructBih(im.cols,im.rows,bih);

		int nWidth=im.cols;
		int nHeight=im.rows;
	    LONG lLineBytes;
		lLineBytes = WIDTHBYTES(nWidth *24);
		BYTE* pTemp=new BYTE[lLineBytes*nHeight];
		for(i=0;i<nHeight;i++)
		{
			for(j=0;j<nWidth;j++)
			{
				pTemp[i*lLineBytes+j*3+0]=im.data[(nHeight-1-i)*nWidth*3+j*3+0];
				pTemp[i*lLineBytes+j*3+1]=im.data[(nHeight-1-i)*nWidth*3+j*3+1];
				pTemp[i*lLineBytes+j*3+2]=im.data[(nHeight-1-i)*nWidth*3+j*3+2];
			}
		}

		GetCBitmap(&bih,pTemp,m_StaffPhotoBmp);
		((CStatic*)(GetDlgItem(IDC_STAFF_PHOTO)))->SetBitmap((HBITMAP)m_StaffPhotoBmp);

		delete []pTemp;
	}
}
catch(...)
{
}
	//show identify photo
	m_LogPhotoBmp.DeleteObject();

	Log_Info& li=*pli;
	char sPhotoFile[256]="";
	sprintf(sPhotoFile,"%s\\id_logs\\%04d-%02d-%02d\\%04d%02d%02d%02d%02d%02d%04d.jpg",
		(LPCTSTR)g_strAppPath,
		li.logTime.wYear, li.logTime.wMonth, li.logTime.wDay,
		li.logTime.wYear, li.logTime.wMonth, li.logTime.wDay,
		li.logTime.wHour, li.logTime.wMinute, li.logTime.wSecond,li.logTime.wMilliseconds);
try
{
	Mat im=imread(sPhotoFile);
	if(im.empty()) 
	{
		m_LogPhotoBmp.LoadBitmap(IDB_FACE);
		((CStatic*)GetDlgItem(IDC_LOG_PHOTO))->SetBitmap((HBITMAP)m_LogPhotoBmp);
		return ;
	}
	else
	{
		::ContructBih(im.cols,im.rows,bih);

		int nWidth=im.cols;
		int nHeight=im.rows;
	    LONG lLineBytes;
		lLineBytes = WIDTHBYTES(nWidth *24);
		BYTE* pTemp=new BYTE[lLineBytes*nHeight];
		for(i=0;i<nHeight;i++)
		{
			for(j=0;j<nWidth;j++)
			{
				pTemp[i*lLineBytes+j*3+0]=im.data[(nHeight-1-i)*nWidth*3+j*3+0];
				pTemp[i*lLineBytes+j*3+1]=im.data[(nHeight-1-i)*nWidth*3+j*3+1];
				pTemp[i*lLineBytes+j*3+2]=im.data[(nHeight-1-i)*nWidth*3+j*3+2];
			}
		}

		GetCBitmap(&bih,pTemp,m_LogPhotoBmp);
		((CStatic*)(GetDlgItem(IDC_LOG_PHOTO)))->SetBitmap((HBITMAP)m_LogPhotoBmp);

		delete []pTemp;
	}
}
catch(...)
{
}


}

void CLogBrowseDlg::OnLvnItemchangedLogListctrl(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMLISTVIEW pNMLV = reinterpret_cast<LPNMLISTVIEW>(pNMHDR);
	// TODO: 在此添加控件通知处理程序代码
	*pResult = 0;

	BOOL bChange=FALSE;
	if(pNMLV->uChanged==LVIF_STATE)
	{
		if(pNMLV->uNewState & LVIS_SELECTED)
		{
			bChange=TRUE;
		}
	}

	if(!bChange) 
	{
		m_StaffPhotoBmp.LoadBitmap(IDB_FACE);
		((CStatic*)GetDlgItem(IDC_STAFF_PHOTO))->SetBitmap((HBITMAP)m_StaffPhotoBmp);

		m_LogPhotoBmp.LoadBitmap(IDB_FACE);
		((CStatic*)GetDlgItem(IDC_LOG_PHOTO))->SetBitmap((HBITMAP)m_LogPhotoBmp);

		SetDlgItemText(IDC_NAME,"");
		SetDlgItemText(IDC_SEX,"");
		SetDlgItemText(IDC_NATION,"");
		SetDlgItemText(IDC_BIRTHDAY,"");
		SetDlgItemText(IDC_ADDRESS,"");
		SetDlgItemText(IDC_ID2,"");
		SetDlgItemText(IDC_VALID_DAY2,"");
		SetDlgItemText(IDC_DEPARTMENT,"");

		SetDlgItemText(IDC_LOG_TIME,"");
		SetDlgItemText(IDC_IDENTIFY_SCORE,"");

		return;
	}

	int nIndex = m_LogListCtrl.GetNextItem(-1, LVNI_SELECTED);
	if(nIndex==-1)
	{
		m_StaffPhotoBmp.LoadBitmap(IDB_FACE);
		((CStatic*)GetDlgItem(IDC_STAFF_PHOTO))->SetBitmap((HBITMAP)m_StaffPhotoBmp);

		m_LogPhotoBmp.LoadBitmap(IDB_FACE);
		((CStatic*)GetDlgItem(IDC_LOG_PHOTO))->SetBitmap((HBITMAP)m_LogPhotoBmp);

		SetDlgItemText(IDC_NAME,"");
		SetDlgItemText(IDC_SEX,"");
		SetDlgItemText(IDC_NATION,"");
		SetDlgItemText(IDC_BIRTHDAY,"");
		SetDlgItemText(IDC_ADDRESS,"");
		SetDlgItemText(IDC_ID2,"");
		SetDlgItemText(IDC_VALID_DAY2,"");
		SetDlgItemText(IDC_DEPARTMENT,"");

		SetDlgItemText(IDC_LOG_TIME,"");
		SetDlgItemText(IDC_IDENTIFY_SCORE,"");

		return;
	}

	Log_Info* pli=(Log_Info*)m_LogListCtrl.GetItemData(nIndex);

	ShowLogInfo(pli);

	*pResult = 0;
}

void CLogBrowseDlg::OnDtnDatetimechangeDatetimepicker(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMDATETIMECHANGE pDTChange = reinterpret_cast<LPNMDATETIMECHANGE>(pNMHDR);
	// TODO: 在此添加控件通知处理程序代码
	*pResult = 0;

	OnBnClickedLogSearch();

	m_StaffPhotoBmp.LoadBitmap(IDB_FACE);
	((CStatic*)GetDlgItem(IDC_STAFF_PHOTO))->SetBitmap((HBITMAP)m_StaffPhotoBmp);

	m_LogPhotoBmp.LoadBitmap(IDB_FACE);
	((CStatic*)GetDlgItem(IDC_LOG_PHOTO))->SetBitmap((HBITMAP)m_LogPhotoBmp);

	SetDlgItemText(IDC_NAME,"");
	SetDlgItemText(IDC_SEX,"");
	SetDlgItemText(IDC_NATION,"");
	SetDlgItemText(IDC_BIRTHDAY,"");
	SetDlgItemText(IDC_ADDRESS,"");
	SetDlgItemText(IDC_ID2,"");
	SetDlgItemText(IDC_VALID_DAY2,"");
	SetDlgItemText(IDC_DEPARTMENT,"");

	SetDlgItemText(IDC_LOG_TIME,"");
	SetDlgItemText(IDC_IDENTIFY_SCORE,"");
}

void CLogBrowseDlg::OnBnClickedDeleteLog()
{
	COleDateTime selDateTime;
	m_DateTimeCtrl.GetTime(selDateTime);
	CString str;
	str.Format("确认删除验证记录[%04d-%02d-%02d]?",selDateTime.GetYear(), selDateTime.GetMonth(), selDateTime.GetDay());
	if(MessageBox(str,"确认删除",MB_YESNO|MB_ICONQUESTION|MB_DEFBUTTON2)==IDNO) 
	{
		return;
	}	

	CString strLogFile;
	strLogFile.Format("%s\\id_logs\\%04d-%02d-%02d",
		g_strAppPath,
		selDateTime.GetYear(), selDateTime.GetMonth(), selDateTime.GetDay());

	CWaitCursor wc;

try
{
	::DeleteDirFile(strLogFile);
}
catch(...)
{

}

	::CreateDirectory(strLogFile,NULL);

	ClearAllLogs();
}

void CLogBrowseDlg::OnBnClickedInverseTime()
{
	// TODO: 在此添加控件通知处理程序代码
	UpdateData();
	OnBnClickedLogSearch();
}

HBRUSH CLogBrowseDlg::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor)
{
	HBRUSH hbr = CDialog::OnCtlColor(pDC, pWnd, nCtlColor);
	int nID = pWnd->GetDlgCtrlID();
	switch(nCtlColor)
	{

	case CTLCOLOR_DLG:
//	case CTLCOLOR_EDIT:
	case CTLCOLOR_STATIC:
		pDC->SetBkMode(TRANSPARENT);
		static HBRUSH hbrgui=::CreateSolidBrush(RGB(0, 162, 232));
		return hbrgui;

//		return HBRUSH(GetStockObject(HOLLOW_BRUSH));//NULL_BRUSH; 
	}

	return hbr;
}

void CLogBrowseDlg::SubButtons()
{
	COLORREF fg = RGB(0, 0, 0);
	COLORREF bg = RGB(0, 162, 232);
	COLORREF dis = RGB(0, 132, 202);

	m_button1.SubclassDlgItem(IDC_DELETE_LOG, this);
	m_button2.SubclassDlgItem(IDCANCEL, this);

	m_button1.SetColour(fg, bg,dis);
	m_button2.SetColour(fg, bg,dis);
}
void CLogBrowseDlg::OnCbnSelchangeComboType()
{
	// TODO: 在此添加控件通知处理程序代码
	OnBnClickedLogSearch();
}

void CLogBrowseDlg::OnEnChangeLogTime()
{
	// TODO:  如果该控件是 RICHEDIT 控件，则它将不会
	// 发送该通知，除非重写 CDialog::OnInitDialog()
	// 函数并调用 CRichEditCtrl().SetEventMask()，
	// 同时将 ENM_CHANGE 标志“或”运算到掩码中。

	// TODO:  在此添加控件通知处理程序代码
}
