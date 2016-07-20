// StuffManageDlg.cpp : implementation file
//

#include "stdafx.h"
#include "FaceAttendServer.h"
#include "StuffManageDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CStaffManageDlg dialog


CStaffManageDlg::CStaffManageDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CStaffManageDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CStaffManageDlg)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
}


void CStaffManageDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CStaffManageDlg)
	DDX_Control(pDX, IDC_STUFF_LIST_CTRL, m_StuffListCtrl);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CStaffManageDlg, CDialog)
	//{{AFX_MSG_MAP(CStaffManageDlg)
	ON_WM_CTLCOLOR()
	ON_NOTIFY(LVN_ITEMCHANGED, IDC_STUFF_LIST_CTRL, OnItemchangedStuffListCtrl)
	ON_NOTIFY(NM_DBLCLK, IDC_STUFF_LIST_CTRL, OnDblclkStuffListCtrl)
//	ON_BN_CLICKED(IDC_CHECK_ALL, OnCheckAll)
//	ON_BN_CLICKED(IDC_UNCHECK_ALL, OnUncheckAll)
	//}}AFX_MSG_MAP
	ON_BN_CLICKED(IDCANCEL, &CStaffManageDlg::OnBnClickedCancel)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CStaffManageDlg message handlers

BOOL CStaffManageDlg::DestroyWindow() 
{
	// TODO: Add your specialized code here and/or call the base class

	return CDialog::DestroyWindow();
}

HBRUSH CStaffManageDlg::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor) 
{
	HBRUSH hbr = CDialog::OnCtlColor(pDC, pWnd, nCtlColor);

	return hbr;
}

BOOL CStaffManageDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	// TODO: Add extra initialization here
//	SubClassCtrls();


	RECT rc; 
	m_StuffListCtrl.GetWindowRect(&rc);
	m_StuffListCtrl.InsertColumn(0, "Staff Name", LVCFMT_LEFT, (rc.right - rc.left)*95/100, -1);
//	m_StuffListCtrl.InsertColumn(1, "Staff ID", LVCFMT_LEFT, (rc.right - rc.left)*40/100, -1);
//	m_StuffListCtrl.InsertColumn(1, "Gender", LVCFMT_LEFT, (rc.right - rc.left)*38/100, -1);
//	m_StuffListCtrl.InsertColumn(3, "Department", LVCFMT_LEFT, (rc.right - rc.left)*20/100, -1);
//	m_StuffListCtrl.InsertColumn(4, "Telephone", LVCFMT_LEFT, (rc.right - rc.left)*20/100, -1);
//	m_StuffListCtrl.InsertColumn(5, "Email", LVCFMT_LEFT, (rc.right - rc.left)*20/100, -1);

    DWORD dwExStyle =  LVS_EX_FLATSB|LVS_EX_FULLROWSELECT | LVS_EX_HEADERDRAGDROP|LVS_EX_GRIDLINES;//|LVS_EX_CHECKBOXES;//

    m_StuffListCtrl.SetExtendedStyle(dwExStyle);


	LoadStuffInfoToListCtrl();


	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}







void CStaffManageDlg::LoadStuffInfoToListCtrl()
{
	m_StuffListCtrl.DeleteAllItems();
	int nCount=g_PersonInfoList.GetCount();
	POSITION pos=g_PersonInfoList.GetHeadPosition();

	for(int i=0;i<nCount;i++)
	{
		PersonInfo* ppi=(PersonInfo*)g_PersonInfoList.GetNext(pos);
		m_StuffListCtrl.InsertItem(i,ppi->sStuffName);

		DWORD dwData=(DWORD)ppi;//->nPersonBbID;
		m_StuffListCtrl.SetItemData(i,dwData);

	//	m_StuffListCtrl.SetCheck(i,g_PersonTemplateEnable[i]);
	}
}

void CStaffManageDlg::OnItemchangedStuffListCtrl(NMHDR* pNMHDR, LRESULT* pResult) 
{
	NM_LISTVIEW* pNMListView = (NM_LISTVIEW*)pNMHDR;

}
void CStaffManageDlg::UpdateStuffGuiInfo(PersonInfo* ppi)
{

}

void CStaffManageDlg::OnDblclkStuffListCtrl(NMHDR* pNMHDR, LRESULT* pResult) 
{
	*pResult = 0;
}



void CStaffManageDlg::OnBnClickedCancel()
{
	// TODO: 在此添加控件通知处理程序代码
	OnCancel();
}
