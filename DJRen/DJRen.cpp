// DJRen.cpp : Defines the class behaviors for the application.
//

#include "stdafx.h"
#include "DJRen.h"
#include "FileManip.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CDJRenApp

BEGIN_MESSAGE_MAP(CDJRenApp, CWinApp)
	//{{AFX_MSG_MAP(CDJRenApp)
		// NOTE - the ClassWizard will add and remove mapping macros here.
		//    DO NOT EDIT what you see in these blocks of generated code!
	//}}AFX_MSG
	ON_COMMAND(ID_HELP, CWinApp::OnHelp)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDJRenApp construction

CDJRenApp::CDJRenApp()
{
	// TODO: add construction code here,
	// Place all significant initialization in InitInstance
}

/////////////////////////////////////////////////////////////////////////////
// The one and only CDJRenApp object

CDJRenApp theApp;

/////////////////////////////////////////////////////////////////////////////
// CDJRenApp initialization

BOOL CDJRenApp::InitInstance()
{
	// Standard initialization
	// If you are not using these features and wish to reduce the size
	//  of your final executable, you should remove from the following
	//  the specific initialization routines you do not need.

	if (m_lpCmdLine == NULL)
		return FALSE;

	CString sTarget = m_lpCmdLine;

	if (!sTarget.IsEmpty() && CFileManip::Existence(sTarget) == CFileManip::FM_DIRECTORY)
	{
		const int n = sTarget.ReverseFind(_T('\\'));
		sTarget = n == -1 ? _T("") : sTarget.Left(n + 1);

		CFileManip::Move(m_lpCmdLine, sTarget + _T("D2J_Backup"));
	}

	return FALSE;
}
