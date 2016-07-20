// FaceAttendServer.cpp : Defines the class behaviors for the application.
//

#include "stdafx.h"
#include "FaceAttendServer.h"
#include "FaceAttendServerDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CNVL_TESTApp

BEGIN_MESSAGE_MAP(CNVL_TESTApp, CWinApp)
	//{{AFX_MSG_MAP(CNVL_TESTApp)
		// NOTE - the ClassWizard will add and remove mapping macros here.
		//    DO NOT EDIT what you see in these blocks of generated code!
	//}}AFX_MSG
	ON_COMMAND(ID_HELP, CWinApp::OnHelp)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CNVL_TESTApp construction
#include<vector>
using namespace std;

CNVL_TESTApp::CNVL_TESTApp()
{
	// TODO: add construction code here,
	// Place all significant initialization in InitInstance
}

/////////////////////////////////////////////////////////////////////////////
// The one and only CNVL_TESTApp object

CNVL_TESTApp theApp;

/////////////////////////////////////////////////////////////////////////////
// CNVL_TESTApp initialization

BOOL CNVL_TESTApp::InitInstance()
{

	
	HANDLE hMap=CreateFileMapping((HANDLE)0xffffffff,NULL,
		                          PAGE_READWRITE,0,128,"TH_FACER_DEMO");
	if(hMap==NULL)
	{
	}
	else if(GetLastError()==ERROR_ALREADY_EXISTS)//already run
	{
		CloseHandle(hMap);
		return FALSE;
	}
	else//first run
	{
	}

	CoInitializeEx(NULL,COINIT_MULTITHREADED);
	AfxEnableControlContainer();

	// Standard initialization
	// If you are not using these features and wish to reduce the size
	//  of your final executable, you should remove from the following
	//  the specific initialization routines you do not need.

	char pModuleName[256];
	::GetModuleFileName(NULL,pModuleName,256);
	CString strTemp;
	strTemp=pModuleName;
	int pos=strTemp.ReverseFind('\\');
	g_strAppPath=strTemp.Left(pos);
///////////////
	g_hModule=m_hInstance;
///////////////
	CNVL_TESTDlg dlg;
	m_pMainWnd = &dlg;
	int nResponse = dlg.DoModal();
	if (nResponse == IDOK)
	{
		// TODO: Place code here to handle when the dialog is
		//  dismissed with OK
	}
	else if (nResponse == IDCANCEL)
	{
		// TODO: Place code here to handle when the dialog is
		//  dismissed with Cancel
	}

	// Since the dialog has been closed, return FALSE so that we exit the
	//  application, rather than start the application's message pump.
	return FALSE;
}

int CNVL_TESTApp::ExitInstance() 
{
	CoUninitialize();
	return CWinApp::ExitInstance();
}
