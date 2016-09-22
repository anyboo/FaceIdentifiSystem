// stdafx.h : include file for standard system include files,
//  or project specific include files that are used frequently, but
//      are changed infrequently
//

#if !defined(AFX_STDAFX_H__2C77F06F_6C8D_41F0_8187_AAB76C10C165__INCLUDED_)
#define AFX_STDAFX_H__2C77F06F_6C8D_41F0_8187_AAB76C10C165__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#define WINVER  0x0501
#define _WIN32_WINNT		_WIN32_WINNT_WINXP
#define _CRT_SECURE_NO_WARNINGS
#define VC_EXTRALEAN		// Exclude rarely-used stuff from Windows headers

#include <afxwin.h>         // MFC core and standard components
#include <afxext.h>         // MFC extensions
#include <afxdisp.h>        // MFC Automation classes
#include <afxdtctl.h>		// MFC support for Internet Explorer 4 Common Controls
#ifndef _AFX_NO_AFXCMN_SUPPORT
#include <afxcmn.h>			// MFC support for Windows Common Controls
#endif // _AFX_NO_AFXCMN_SUPPORT


#include "../../include-mt/THFaceImage_i.h"
#pragma comment (lib, "../../lib-mt/THFaceImage.lib")

#include "../../include-mt/THFeature_i.h"
#pragma comment(lib,"../../lib-mt/THFeature.lib")

//#include "../../include-mt/THFace_i.h"
//#pragma comment (lib, "../../lib-mt/THFace.lib")

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_STDAFX_H__2C77F06F_6C8D_41F0_8187_AAB76C10C165__INCLUDED_)
