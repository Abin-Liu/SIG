// SIG.cpp : Defines the class behaviors for the application.
//

#include "stdafx.h"
#include "SIG.h"
#include "SIGDlg.h"
#include "Ini.h"
#include "MD5Checksum.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

CString g_sAppDir;
CString g_sAppVer;
//CLanguage g_lang;

CString GetHackDir()
{
	return g_sAppDir + _T("\\Hacks");
}

CString GetD2JspDir()
{
	return g_sAppDir + _T("\\D2JSP");
}

CString GetD2JspScriptsDir()
{
	return g_sAppDir + _T("\\D2JSP\\Scripts");
}

CString GetD2JspBotsDir()
{
	return g_sAppDir + _T("\\D2JSP\\Scripts\\Bots");
}

CString GetD2JspBackupDir()
{
	return g_sAppDir + _T("\\D2JSP\\Backup");
}

CString GetSigIni()
{
	return g_sAppDir + _T("\\Sig.ini");
}

CString GetD2HackitIni()
{
	return g_sAppDir + _T("\\Hacks\\D2Hackit.ini");
}

CString GetAccountsIni()
{
	return g_sAppDir + _T("\\Accounts.dat");
}

CString GetD2GatewayIni()
{
	return g_sAppDir + _T("\\D2Gateway.dat");
}

CString GetMemIDFile()
{
	return g_sAppDir + _T("\\Hacks\\SigMemory.ini");
}

CString GetDictionary()
{
	return g_sAppDir + _T("\\GameNames.txt");
}

CString GetLogFilePath()
{
	return g_sAppDir + _T("\\Log.txt");
}

CString GetStatisticsIni()
{
	return g_sAppDir + _T("\\Statistics.ini");
}

CString GetLogFileDir()
{
	return g_sAppDir + _T("\\Hacks\\Scripts\\Output");
}

CString GetTimeString(DWORD dwSeconds)
{
	DWORD dwHour = dwSeconds / 3600;
	dwSeconds %= 3600;
	DWORD dwMinute = dwSeconds / 60;
	dwSeconds %= 60;

	CString sHour, sMinute, sSecond;
	if (dwHour > 0)
		sHour.Format(_T("%dÐ¡Ê±"),dwHour);
	if (dwMinute > 0)
		sMinute.Format(_T("%d·Ö"), dwMinute);
	sSecond.Format(_T("%dÃë"), dwSeconds);
	return sHour + sMinute + sSecond;
}

/////////////////////////////////////////////////////////////////////////////
// CSIGApp

BEGIN_MESSAGE_MAP(CSIGApp, CWinApp)
	//{{AFX_MSG_MAP(CSIGApp)
		// NOTE - the ClassWizard will add and remove mapping macros here.
		//    DO NOT EDIT what you see in these blocks of generated code!
	//}}AFX_MSG
	ON_COMMAND(ID_HELP, CWinApp::OnHelp)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CSIGApp construction

CSIGApp::CSIGApp()
{
	// TODO: add construction code here,
	// Place all significant initialization in InitInstance
	m_bIsMultiRunning = FALSE;
}

/////////////////////////////////////////////////////////////////////////////
// The one and only CSIGApp object

CSIGApp theApp;
int g_nActivePage;

/////////////////////////////////////////////////////////////////////////////
// CSIGApp initialization

BOOL CSIGApp::InitInstance()
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

	srand(time(NULL));

	g_nActivePage = -1; 

	TCHAR szPath[MAX_PATH] = _T("");
	::GetModuleFileName(NULL, szPath, MAX_PATH);

	int nLen = _tcslen(szPath);
	for (int i = nLen - 1; i >= 0; i--)
	{
		if (szPath[i] == '\\')
		{
			szPath[i] = '\0';
			break;
		}
	}
	
	g_sAppDir = szPath;

	CString sMutex(_T("{DB82BA6D-5BE4-4F48-8639-3B38A8930703}"));
	sMutex += g_sAppDir;
	sMutex.Replace('\\', '/'); // mutex name cannot contain back-slash
	sMutex.MakeUpper();

	if (m_st.Create(sMutex))
	{
		if (!m_st.IsInstanceUnique())
		{
			m_st.RestorePrevWnd();
			return FALSE;
		}
	}

	m_st.SetPrevInstance(m_hInstance);

	free((void*)m_pszAppName);
	m_pszAppName=_tcsdup(_T("SIG"));
	SetRegistryKey(_T("Abin")); // company name

	g_sAppVer = APP_VER;
	WriteProfileString(_T(""), _T("Version"), g_sAppVer); // version
	WriteProfileString(_T(""), _T("Path"), g_sAppDir); // app path

	m_sUID = GetProfileString(_T(""), _T("UID"), _T(""));
	if (m_sUID.IsEmpty())
	{
		DWORD dwTick = ::GetTickCount();
		m_sUID = CMD5Checksum::GetBlockMD5((const BYTE*)&dwTick, sizeof(DWORD));
		WriteProfileString(_T(""), _T("UID"), m_sUID);
		CIni ini(GetSigIni());
		ini.WriteString(_T("Login"), _T("Password"), _T(""));
	}

	CSIGDlg dlg;
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

void CSIGApp::WinHelp(DWORD dwData, UINT nCmd) 
{
	// TODO: Add your specialized code here and/or call the base class
	//CWinApp::WinHelp(dwData, nCmd);
	ShowHelp(g_nActivePage);
}

void CSIGApp::ShowHelp(int nActivePage) const
{
	CString sHelpParam = g_sAppDir + (_T("\\SIG.chm"));
	switch (nActivePage)
	{
	case PAGE_GENERAL:
		sHelpParam += _T("::/settings.htm#general");
		break;

	case PAGE_LOGIN:
		sHelpParam += _T("::/settings.htm#login");
		break;

	case PAGE_CREATE:
		sHelpParam += _T("::/settings.htm#create");
		break;

	case PAGE_DELAY:
		sHelpParam += _T("::/settings.htm#delay");
		break;

	case PAGE_MULEMODE:
		sHelpParam += _T("::/settings.htm#mulemode");
		break;

	case PAGE_HACKS:
		sHelpParam += _T("::/settings.htm#hacks");
		break;

	case PAGE_D2JSP:
		sHelpParam += _T("::/settings.htm#d2jsp");
		break;

	default:
		break;
	}

	ShellExecute(NULL, _T("open"), _T("hh.exe"), sHelpParam, g_sAppDir, SW_SHOW);
}

BOOL CSIGApp::IsMultiRunning() const
{
	return m_bIsMultiRunning;
}