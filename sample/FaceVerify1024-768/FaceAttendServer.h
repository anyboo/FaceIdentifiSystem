// FaceAttendServer.h : main header file for the FACEATTENDSERVER application
//

#if !defined(AFX_FACEATTENDSERVER_H__988B5132_30FA_40F6_8E86_E75369FF99EA__INCLUDED_)
#define AFX_FACEATTENDSERVER_H__988B5132_30FA_40F6_8E86_E75369FF99EA__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#ifndef __AFXWIN_H__
	#error include 'stdafx.h' before including this file for PCH
#endif

#include "resource.h"		// main symbols

/////////////////////////////////////////////////////////////////////////////
// CNVL_TESTApp:
// See FaceAttendServer.cpp for the implementation of this class
//

class CNVL_TESTApp : public CWinApp
{
public:
	CNVL_TESTApp();

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CNVL_TESTApp)
	public:
	virtual BOOL InitInstance();
	virtual int ExitInstance();
	//}}AFX_VIRTUAL

// Implementation

	//{{AFX_MSG(CNVL_TESTApp)
		// NOTE - the ClassWizard will add and remove member functions here.
		//    DO NOT EDIT what you see in these blocks of generated code !
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};


/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_FACEATTENDSERVER_H__988B5132_30FA_40F6_8E86_E75369FF99EA__INCLUDED_)
