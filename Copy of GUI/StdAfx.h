// stdafx.h : include file for standard system include files,
//  or project specific include files that are used frequently, but
//      are changed infrequently
//

#if !defined(AFX_STDAFX_H__DC4144CF_39CD_471F_AD17_AA39FFC0DEFA__INCLUDED_)
#define AFX_STDAFX_H__DC4144CF_39CD_471F_AD17_AA39FFC0DEFA__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#define VC_EXTRALEAN		// Exclude rarely-used stuff from Windows headers

#include <afxwin.h>         // MFC core and standard components
#include <afxext.h>         // MFC extensions
#include <afxdtctl.h>		// MFC support for Internet Explorer 4 Common Controls
#ifndef _AFX_NO_AFXCMN_SUPPORT
#include <afxcmn.h>			// MFC support for Windows Common Controls
#endif // _AFX_NO_AFXCMN_SUPPORT

//#include "Language.h"
//extern CLanguage g_lang;
//#define LANG(x)	(g_lang.(x))
extern CString g_sAppDir;
extern CString g_sAppVer;
extern int g_nActivePage;
enum SIG_PAGE { PAGE_GENERAL = 0, PAGE_LOGIN, PAGE_CREATE, PAGE_DELAY, PAGE_MULEMODE, PAGE_HACKS, PAGE_D2JSP };
#define APP_NAME	_T("Stay in Game")
#define APP_VER		_T("3.09")


//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_STDAFX_H__DC4144CF_39CD_471F_AD17_AA39FFC0DEFA__INCLUDED_)
