// D2Gateway.h : main header file for the D2GATEWAY application
//

#if !defined(AFX_D2GATEWAY_H__A4428F04_1D0C_4B33_9ED6_FBDB2FC3995C__INCLUDED_)
#define AFX_D2GATEWAY_H__A4428F04_1D0C_4B33_9ED6_FBDB2FC3995C__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#ifndef __AFXWIN_H__
	#error include 'stdafx.h' before including this file for PCH
#endif

#include "resource.h"		// main symbols
#include "Singleton.h"

/////////////////////////////////////////////////////////////////////////////
// CD2GatewayApp:
// See D2Gateway.cpp for the implementation of this class
//

class CD2GatewayApp : public CWinApp
{
public:
	CD2GatewayApp();
	CSingleton m_st;

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CD2GatewayApp)
	public:
	virtual BOOL InitInstance();
	virtual void WinHelp(DWORD dwData, UINT nCmd = HELP_CONTEXT);
	//}}AFX_VIRTUAL

// Implementation

	//{{AFX_MSG(CD2GatewayApp)
		// NOTE - the ClassWizard will add and remove member functions here.
		//    DO NOT EDIT what you see in these blocks of generated code !
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
protected:
	BOOL IsValidParam(CString& sBnetIP, CString& sPreferred) const;
};


/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_D2GATEWAY_H__A4428F04_1D0C_4B33_9ED6_FBDB2FC3995C__INCLUDED_)
