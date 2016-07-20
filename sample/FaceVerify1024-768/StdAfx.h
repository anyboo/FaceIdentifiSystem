// stdafx.h : include file for standard system include files,
//  or project specific include files that are used frequently, but
//      are changed infrequently
//

#if !defined(AFX_STDAFX_H__747AEC21_F857_450A_8ACB_B4C8AC5DD725__INCLUDED_)
#define AFX_STDAFX_H__747AEC21_F857_450A_8ACB_B4C8AC5DD725__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
#define WINVER  0x0501
#define _WIN32_WINNT		_WIN32_WINNT_WINXP
#define _CRT_SECURE_NO_WARNINGS
#define VC_EXTRALEAN		// Exclude rarely-used stuff from Windows headers

#include <afxwin.h>         // MFC core and standard components
#include <afxext.h>         // MFC extensions
//#include <afxdisp.h>        // MFC Automation classes
#include <afxdtctl.h>		// MFC support for Internet Explorer 4 Common Controls
#ifndef _AFX_NO_AFXCMN_SUPPORT
#include <afxcmn.h>			// MFC support for Windows Common Controls
#endif // _AFX_NO_AFXCMN_SUPPORT

#include "public.h"


#include <algorithm>
#include <string>
#include <fstream>
#include <iterator>
using namespace std;



#include "cv.h"
#include "cxcore.h"
#include "highgui.h"


using namespace cv;

//#include "SkinMagicLib.h"
//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_STDAFX_H__747AEC21_F857_450A_8ACB_B4C8AC5DD725__INCLUDED_)
