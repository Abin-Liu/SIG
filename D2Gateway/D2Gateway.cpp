// D2Gateway.cpp : Defines the class behaviors for the application.
//

#include "stdafx.h"
#include "D2Gateway.h"
#include "D2GatewayDlg.h"
#include "strsplit.h"
#include "realmInfo.h"

CString g_sAppDir;

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CD2GatewayApp

BEGIN_MESSAGE_MAP(CD2GatewayApp, CWinApp)
	//{{AFX_MSG_MAP(CD2GatewayApp)
		// NOTE - the ClassWizard will add and remove mapping macros here.
		//    DO NOT EDIT what you see in these blocks of generated code!
	//}}AFX_MSG
	ON_COMMAND(ID_HELP, CWinApp::OnHelp)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CD2GatewayApp construction

CD2GatewayApp::CD2GatewayApp()
{
	// TODO: add construction code here,
	// Place all significant initialization in InitInstance
}

/////////////////////////////////////////////////////////////////////////////
// The one and only CD2GatewayApp object

CD2GatewayApp theApp;

/////////////////////////////////////////////////////////////////////////////
// CD2GatewayApp initialization

BOOL CD2GatewayApp::InitInstance()
{
	// Standard initialization
	// If you are not using these features and wish to reduce the size
	//  of your final executable, you should remove from the following
	//  the specific initialization routines you do not need.

#ifdef _AFXDLL
	Enable3dControls();			// Call this when using MFC in a shared DLL
#else
	Enable3dControlsStatic();	// Call this when linking to MFC statically
#endif

	CString sAppDir;
	::GetModuleFileName(NULL, sAppDir.GetBuffer(MAX_PATH), MAX_PATH);
	sAppDir.ReleaseBuffer();

	if (sAppDir.Find('\\') != -1)
	{
		for (int i = sAppDir.GetLength() - 1; i >= 0; i--)
		{
			TCHAR ch = sAppDir[i];
			sAppDir.Delete(i);
			if (ch == '\\')
				break;	
		}
	}

	g_sAppDir = sAppDir;

	CString sBnetIP, sPreferred;
	if (IsValidParam(sBnetIP, sPreferred))
	{
		CRealmInfo::SetDefaultRealm(sBnetIP, sPreferred);
		CRealmInfo::DisableSkipToOpen(TRUE);
		return FALSE;
	}	

	if (m_st.Create(_T("{BFBDF7DD-3387-4905-B95C-23BB70B8697A}")))
	{
		if (!m_st.IsInstanceUnique())
		{
			m_st.RestorePrevWnd();
			return FALSE;
		}
	}

	m_st.SetPrevInstance(m_hInstance);

	CD2GatewayDlg dlg;
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

void CD2GatewayApp::WinHelp(DWORD dwData, UINT nCmd) 
{
	// TODO: Add your specialized code here and/or call the base class
	::ShellExecute(NULL, _T("open"), g_sAppDir + _T("\\Readme.txt"), NULL, g_sAppDir, SW_SHOW);
	//CWinApp::WinHelp(dwData, nCmd);
}

BOOL CD2GatewayApp::IsValidParam(CString &sBnetIP, CString& sPreferred) const
{
	GetParameter(m_lpCmdLine, _T("-subrealm"), sPreferred);
	return GetParameter(m_lpCmdLine, _T("-bnetip"), sBnetIP);
}
