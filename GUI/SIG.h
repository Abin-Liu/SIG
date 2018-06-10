// SIG.h : main header file for the SIG application
//

#if !defined(AFX_SIG_H__31543391_9049_481F_A9AB_286C1D80A8BB__INCLUDED_)
#define AFX_SIG_H__31543391_9049_481F_A9AB_286C1D80A8BB__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#ifndef __AFXWIN_H__
	#error include 'stdafx.h' before including this file for PCH
#endif

#include "resource.h"		// main symbols
#include "Singleton.h"

/////////////////////////////////////////////////////////////////////////////
// CSIGApp:
// See SIG.cpp for the implementation of this class
//

CString GetHackDir();
CString GetD2JspBackupDir();
CString GetD2JspDir();
CString GetD2JspScriptsDir();
CString GetLogFileDir();
CString GetD2JspBotsDir();
CString GetSigIni();
CString GetD2HackitIni();
CString GetAccountsIni();
CString GetD2GatewayIni();
CString GetMemIDFile();
CString GetDictionary();
CString GetLogFilePath();
CString GetStatisticsIni();
CString GetTimeString(DWORD dwSeconds);

class CSIGApp : public CWinApp
{
public:
	BOOL IsMultiRunning() const;
	void ShowHelp(int nActivePage = -1) const;
	CSIGApp();
	CSingleton m_st;
	CString m_sUID;

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CSIGApp)
	public:
	virtual BOOL InitInstance();
	virtual void WinHelp(DWORD dwData, UINT nCmd = HELP_CONTEXT);
	//}}AFX_VIRTUAL

// Implementation

	//{{AFX_MSG(CSIGApp)
		// NOTE - the ClassWizard will add and remove member functions here.
		//    DO NOT EDIT what you see in these blocks of generated code !
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
	
private:
	BOOL m_bIsMultiRunning; 
protected:
};


/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_SIG_H__31543391_9049_481F_A9AB_286C1D80A8BB__INCLUDED_)
