// DJRen.h : main header file for the DJREN application
//

#if !defined(AFX_DJREN_H__8EAD0521_0F66_432A_B454_C66C0EA22B43__INCLUDED_)
#define AFX_DJREN_H__8EAD0521_0F66_432A_B454_C66C0EA22B43__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#ifndef __AFXWIN_H__
	#error include 'stdafx.h' before including this file for PCH
#endif

#include "resource.h"		// main symbols

/////////////////////////////////////////////////////////////////////////////
// CDJRenApp:
// See DJRen.cpp for the implementation of this class
//

class CDJRenApp : public CWinApp
{
public:
	CDJRenApp();

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CDJRenApp)
	public:
	virtual BOOL InitInstance();
	//}}AFX_VIRTUAL

// Implementation

	//{{AFX_MSG(CDJRenApp)
		// NOTE - the ClassWizard will add and remove member functions here.
		//    DO NOT EDIT what you see in these blocks of generated code !
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};


/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_DJREN_H__8EAD0521_0F66_432A_B454_C66C0EA22B43__INCLUDED_)
