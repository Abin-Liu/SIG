// SigRst.h : main header file for the SIGRST application
//

#if !defined(AFX_SIGRST_H__9555B3AF_0F90_45DA_802C_4C2E92712822__INCLUDED_)
#define AFX_SIGRST_H__9555B3AF_0F90_45DA_802C_4C2E92712822__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#ifndef __AFXWIN_H__
	#error include 'stdafx.h' before including this file for PCH
#endif

#include "resource.h"		// main symbols

/////////////////////////////////////////////////////////////////////////////
// CSigRstApp:
// See SigRst.cpp for the implementation of this class
//

class CSigRstApp : public CWinApp
{
public:
	CSigRstApp();

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CSigRstApp)
	public:
	virtual BOOL InitInstance();
	//}}AFX_VIRTUAL

// Implementation

	//{{AFX_MSG(CSigRstApp)
		// NOTE - the ClassWizard will add and remove member functions here.
		//    DO NOT EDIT what you see in these blocks of generated code !
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};


/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_SIGRST_H__9555B3AF_0F90_45DA_802C_4C2E92712822__INCLUDED_)
