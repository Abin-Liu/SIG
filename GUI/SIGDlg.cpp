// SIGDlg.cpp : implementation file
//

#include "stdafx.h"
#include "SIG.h"
#include "SIGDlg.h"
#include "..\\common.h"
#include "GeneralPage.h"
#include "LoginPage.h"
#include "CreatePage.h"
#include "DelayPage.h"
#include "HackPage.h"
#include "D2JspPage.h"
#include "MulePage.h"
#include "Ini.h"
#include "StrSplit.h"
#include "PopupMenu.h"
#include "exitDlg.h"
#include "realmInfo.h"
#include "d2spy.h"
#include "statisticsDlg.h"
#include "OSInfo.h"
#include "MD5Checksum.h"
#include "FileMD5Def.h"

#define BOOL_EQUAL(x, y) (((x) && (y)) || (!(x) && !(y)))

// a collection of possible error messages
LPCTSTR ERROR_MSG[] = {
					_T("D2Loader Error"),
					_T("D2loader.exe - Application Error"),
					_T("Hey guys"),
					_T("Diablo II Error"),
					_T("Diablo II Exception"),
					_T("Diablo II.exe - Application Error"),
					_T("DIABLO2SRV Error"),
					_T("DIABLO2SRV Exception"),
					_T("Ddhelp"),
					_T("Loader Error!"),
					NULL
					};

// next game stats
enum { NEXT_CREATE_AUTO = 0, NEXT_CREATE_SPEC, NEXT_JOIN_SPEC };

#define WM_ICON_NOTIFY  WM_APP + 10
#define AUTO_CONN_INC_BASE	5000
#define AUTO_CONN_RETRY_MAX	5
#define CLOSE_D2_RETRY	5
#define MAX_INACTIVITY	300000 // max inactivity length 

// define states
enum { ST_NONE = 0,			// Initial state
	   ST_LOAD_SIG,
	   ST_CHECK_SIG,
	   ST_CHECK_X_D2WND,	// are there x+ d2 windows in the system?
	   ST_LAUNCH_DIABLO,	// Launch Diablo II
	   ST_CHECK_DIABLO,		// Check wether Diablo II has been launched properly
	   ST_D2LOADED,			// d2 loaded
	   ST_CONNECTBNET,
	   ST_CANCEL_CONNECT,
	   ST_CHECK_CONNECT,
	   ST_LOGIN,			// Login
	   ST_CHECK_LOGIN,		// check login
	   ST_CANCEL_LOGIN,
	   ST_SHOW_CHARLIST,	// show char-selection screen
	   ST_CHECK_CHARLIST,	// check char list
	   ST_SELECT_CHAR,		// Select Character(1-8)
	   ST_CHECK_CHATROOM,	// check if in chat room
	   ST_CHANNEL_STUFF,	// In channel(Join channel, set away)
	   ST_CREATE_GAME,		// Create new game
	   ST_BEFORE_CHECKGAME, // After create action and before checking it
	   ST_CHECK_GAME,		// Check wether game has been created successfully
	   ST_CLEAN_ERRORMSG,	// Clean error message windows
	   ST_CLOSE_D2,			// Close Diablo II(send WM_CLOSE)
	   ST_KILL_D2_PROC,		// Kill d2 process
	   ST_CHECK_KILL_D2,	// Check if d2 killed
	   ST_WAIT_RESET,		// prepare to reset SIG
	   ST_WAIT_LAUNCH,		// Wait before launching Diablo II
	   ST_IN_GAME,			// Game created successfully
	   ST_EXIT_GAME,		// Exit game
	   ST_CHECK_EXIT_GAME,	// check if in chat room after exit game
	   ST_PREPARE_CREATE,   // prepare to create new game(random delay for anti-detect)
	   ST_EXIT_CHANNEL,		// Exit channel
	   ST_CHECK_EXIT_CHANNEL,// check if in char select screen after exit channel
	   ST_EXIT_CHARLIST,	// exit char-select screen
	   ST_CHECK_EXIT_CHARLIST, // check if in login screen after exit char select screen
	   ST_GET_NEXT_ACC,		// Get the info of next mule account
	   ST_RECONNECT,		// Reconnect to BNet
	   ST_CHECK_RECONNECT,	// check if 
	   ST_CHANNEL_RETRY 

 };

#define TICK	500

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

#include "HyperLink.h"

/////////////////////////////////////////////////////////////////////////////
// CAboutDlg dialog used for App About

class CAboutDlg : public CDialog
{
public:
	CAboutDlg();

// Dialog Data
	//{{AFX_DATA(CAboutDlg)
	enum { IDD = IDD_ABOUTBOX };
	CHyperLink	m_wndHomepage;
	CHyperLink	m_wndAbin;
	CString	m_sVer;
	//}}AFX_DATA

	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CAboutDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	//{{AFX_MSG(CAboutDlg)
	virtual BOOL OnInitDialog();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialog(CAboutDlg::IDD)
{
	//{{AFX_DATA_INIT(CAboutDlg)
	m_sVer = _T("");
	//}}AFX_DATA_INIT
	m_sVer.Format(_T("SIG v%s"), APP_VER);
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CAboutDlg)
	DDX_Control(pDX, IDC_HOMEPAGE, m_wndHomepage);
	DDX_Control(pDX, IDC_ABIN, m_wndAbin);
	DDX_Text(pDX, IDC_VER, m_sVer);
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialog)
	//{{AFX_MSG_MAP(CAboutDlg)
	//}}AFX_MSG_MAP

END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CSIGDlg dialog

CSIGDlg::CSIGDlg(CWnd* pParent /*=NULL*/)
	: m_stats(GetStatisticsIni()), CDialog(CSIGDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CSIGDlg)
	m_bMuleModeEnabled = FALSE;
	//}}AFX_DATA_INIT
	// Note that LoadIcon does not require a subsequent DestroyIcon in Win32
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);

	m_nCurState = ST_NONE;
	m_bSigLoaded = FALSE;
	m_bGameJoined = FALSE;

	m_dwCount = 0;
	m_dwJoinFail = 0;
	m_dwDHFail = 0;

	m_pActiveWnd = NULL;
	m_bBossModeEnabled = FALSE;
	m_bStarted = FALSE;
	m_sOrigInfo = _T("����");
	m_bInitialized = FALSE;
	m_bMovingOn = FALSE;
	m_dwPrevLocation = 0xffffffff;
	m_bStartOver = TRUE;

	m_nCurMuleChar = 0;
	m_rcOrigD2Wnd = CRect(0, 0, 0, 0);
	m_nRecentCreateResult = CREATE_NOT_YET;

	m_nAutoCloseRemain = 10000;

	memset(&m_wp, 0, sizeof(WINDOWPLACEMENT));
 	m_wp.length = sizeof(WINDOWPLACEMENT);
	m_bPlacementChanged = FALSE;

	m_dwConnRetry = 0;
	m_dwLoginFail = 0;
	m_dwCloseD2Fail = 0;
	m_dwAppTime = 0;

	m_bUseD2JSP = FALSE;

	GRADIENTDATA gd;
	gd.endColor = RGB(0, 0, 0);
	gd.startColor = RGB(96, 96, 96);
	gd.wCount = 150;
	m_gradient.SetData(&gd);
	m_wndHPManaBar.SetStatsPtr(&m_playerStats);
}

void CSIGDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CSIGDlg)
	DDX_Control(pDX, IDC_ST, m_wndST);
	DDX_Control(pDX, IDC_RV, m_wndRV);
	DDX_Control(pDX, IDC_LC, m_wndLC);
	DDX_Control(pDX, IDC_HB, m_wndHB);
	DDX_Control(pDX, IDC_AI, m_wndAI);
	DDX_Control(pDX, IDC_HPMANABAR, m_wndHPManaBar);
	DDX_Control(pDX, IDC_D2JSP, m_wndD2JSP);
	DDX_Control(pDX, IDC_SELFILE, m_wndSelFile);
	DDX_Control(pDX, IDC_VIEWFILE, m_wndViewFile);
	DDX_Control(pDX, IDC_KILLD2, m_wndKillD2);
	DDX_Control(pDX, IDC_STATISTICS, m_wndStatistics);
	DDX_Control(pDX, IDC_SHOWD2, m_wndShowD2);
	DDX_Control(pDX, IDC_MULEMODE, m_wndMuleMode);
	DDX_Control(pDX, IDC_CLASSNAME, m_wndCharName);
	DDX_Control(pDX, IDC_MENUHELP, m_wndHelp);
	DDX_Control(pDX, IDC_FILE, m_wndFile);
	DDX_Control(pDX, IDCANCEL, m_wndExit);
	DDX_Control(pDX, IDC_SETTINGS, m_wndSettings);
	DDX_Control(pDX, IDC_STOP, m_wndStop);
	DDX_Control(pDX, IDC_START, m_wndStart);
	DDX_Control(pDX, IDC_LABEL, m_wndLabel);
	DDX_Control(pDX, IDC_APPVER, m_wndAppVer);
	DDX_Control(pDX, IDC_APPNAME, m_wndAppName);
	DDX_Check(pDX, IDC_MULEMODE, m_bMuleModeEnabled);
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CSIGDlg, CDialog)
	//{{AFX_MSG_MAP(CSIGDlg)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_WM_DESTROY()
	ON_WM_TIMER()
	ON_BN_CLICKED(IDC_START, OnStart)
	ON_BN_CLICKED(IDC_SETTINGS, OnSettings)
	ON_BN_CLICKED(IDC_STOP, OnStop)
	ON_COMMAND(ID_HELP_HELP, OnHelpHelp)
	ON_COMMAND(ID_HELP_ABOUT, OnHelpAbout)
	ON_COMMAND(ID_POPUP_RESTORE, OnPopupRestore)
	ON_UPDATE_COMMAND_UI(ID_HELP_HELP, OnUpdateHelpHelp)
	ON_BN_CLICKED(IDC_MULEMODE, OnMulemode)
	ON_COMMAND(ID_STATISTICS_VIEWDATA, OnStatisticsViewdata)
	ON_COMMAND(ID_STATISTICS_VIEWLOG, OnStatisticsViewlog)
	ON_BN_CLICKED(IDC_KILLD2, OnKilld2)
	ON_WM_SIZE()
	ON_COMMAND(ID_POPUP_STARTPAUSERESUME, OnPopupStartpauseresume)
	ON_UPDATE_COMMAND_UI(ID_POPUP_STARTPAUSERESUME, OnUpdatePopupStartpauseresume)
	ON_COMMAND(ID_POPUP_STOP, OnPopupStop)
	ON_UPDATE_COMMAND_UI(ID_POPUP_STOP, OnUpdatePopupStop)
	ON_COMMAND(ID_POPUP_KILLD2, OnPopupKilld2)
	ON_UPDATE_COMMAND_UI(ID_POPUP_KILLD2, OnUpdatePopupKilld2)
	ON_BN_CLICKED(IDC_VIEWFILE, OnViewfile)
	ON_BN_CLICKED(IDC_SELFILE, OnSelfile)
	ON_COMMAND(ID_SHOWD2_ACTIVE, OnShowd2Active)
	ON_COMMAND(ID_SHOWD2_BKGND, OnShowd2Bkgnd)
	ON_UPDATE_COMMAND_UI(ID_SHOWD2_ACTIVE, OnUpdateShowd2Active)
	ON_UPDATE_COMMAND_UI(ID_SHOWD2_BKGND, OnUpdateShowd2Active)
	ON_WM_LBUTTONDBLCLK()
	//}}AFX_MSG_MAP

	// user defined message handlers
	ON_MESSAGE(WM_SIG_CREATE, OnSIGCreate)
	ON_MESSAGE(WM_SIG_LOADED, OnSIGLoaded)
	ON_MESSAGE(WM_SIG_UNLOADED, OnSIGUnloaded)
	ON_MESSAGE(WM_SIG_PAUSE, OnSIGPause)
	ON_MESSAGE(WM_SIG_RESUME, OnSIGResume)
	ON_MESSAGE(WM_SIG_HEARTBEAT, OnSIGHeartBeat)
	ON_MESSAGE(WM_SIG_MSGSTR, OnSIGMsgString)
	ON_MESSAGE(WM_SIG_STOP, OnSIGStop)
	ON_MESSAGE(WM_SIG_PKTRCV, OnSIGPktRcv)
	ON_MESSAGE(WM_SIG_PKTSND, OnSIGPktSnd)
	ON_MESSAGE(WM_SIG_ANTIIDLE, OnSIGAntiIdle)
	ON_MESSAGE(WM_SIG_QUITD2, OnSIGQuitD2)
	ON_MESSAGE(WM_SIG_NOTIFY, OnSIGNotify)
	ON_MESSAGE(WM_SIG_SHOWD2, OnSIGShowD2)
	ON_MESSAGE(WM_SIG_HIDED2, OnSIGHideD2)
	ON_MESSAGE(WM_SIG_KEY, OnSIGKey)
	ON_MESSAGE(WM_ICON_NOTIFY, OnTrayNotification)

END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CSIGDlg message handlers

BOOL CSIGDlg::OnInitDialog()
{
	CDialog::OnInitDialog();
	// Add "About..." menu item to system menu.

	// IDM_ABOUTBOX must be in the system command range.
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != NULL)
	{
		CString strAboutMenu;
		strAboutMenu.LoadString(IDS_ABOUTBOX);
		if (!strAboutMenu.IsEmpty())
		{
			pSysMenu->AppendMenu(MF_SEPARATOR);
			pSysMenu->AppendMenu(MF_STRING, IDM_ABOUTBOX, strAboutMenu);
		}
	}

	// Set the icon for this dialog.  The framework does this automatically
	//  when the application's main window is not a dialog
	SetIcon(m_hIcon, TRUE);			// Set big icon
	SetIcon(m_hIcon, FALSE);		// Set small icon

	// TODO: Add extra initialization here
	srand(time(NULL));
	CRealmInfo::DisableSkipToOpen();
	((CSIGApp*)AfxGetApp())->m_st.SetPrevWnd(GetSafeHwnd());

	m_wndHPManaBar.Stop();

	m_logger.SetFilePath(GetLogFilePath());
	m_logger.Clean();
	m_logger.Log(_T("SIG GUI Launched."));
	m_logger.AddLineBreak();
	
	if (!CreateSharedMem())
	{
		// error
		MessageBox(_T("�����ڴ�鴴��ʧ�ܣ�"), _T("��ʼ��ʧ��"), MB_ICONSTOP);
		m_logger.Log(_T("�����ڴ�鴴��ʧ�ܣ�������ֹ��"));
		OnCancel();
		return TRUE;
	}

	m_trayIcon.Create(this, WM_ICON_NOTIFY, APP_NAME, m_hIcon, IDR_MAINFRAME, FALSE);
	SetWindowText(APP_NAME);	

	m_wndFile.DrawTransparent();
	m_wndFile.SetColor(CButtonST::BTNST_COLOR_FG_OUT, RGB(192, 192, 192));
	m_wndFile.SetColor(CButtonST::BTNST_COLOR_FG_IN, RGB(192, 192, 0));
	m_wndFile.SetColor(CButtonST::BTNST_COLOR_FG_FOCUS, RGB(192, 192, 192));

	m_wndStatistics.DrawTransparent();
	m_wndStatistics.SetColor(CButtonST::BTNST_COLOR_FG_OUT, RGB(192, 192, 192));
	m_wndStatistics.SetColor(CButtonST::BTNST_COLOR_FG_IN, RGB(192, 192, 0));
	m_wndStatistics.SetColor(CButtonST::BTNST_COLOR_FG_FOCUS, RGB(192, 192, 192));

	CIni ini(GetSigIni());
	m_sViewFile = ini.GetString(_T("General"), _T("View File"), _T(""));

	m_wndHelp.DrawTransparent();
	m_wndHelp.SetColor(CButtonST::BTNST_COLOR_FG_OUT, RGB(192, 192, 192));
	m_wndHelp.SetColor(CButtonST::BTNST_COLOR_FG_IN, RGB(192, 192, 0));
	m_wndHelp.SetColor(CButtonST::BTNST_COLOR_FG_FOCUS, RGB(192, 192, 192));

	m_wndStart.DrawTransparent();
	m_wndStart.SetIcon(IDI_START_ACTIVATE, IDI_START_DEACTIVATE);
	m_wndStart.SetTooltipText(_T("��ʼ"));

	m_wndStop.DrawTransparent();
	m_wndStop.SetTooltipText(_T("ֹͣ"));
	m_wndStop.SetIcon(IDI_STOP_ACTIVATE, IDI_STOP_DEACTIVATE);

	m_wndSettings.DrawTransparent();
	m_wndSettings.SetTooltipText(_T("����"));
	m_wndSettings.SetIcon(IDI_SETTINGS_ACTIVATE, IDI_SETTINGS_DEACTIVATE);

	m_wndShowD2.DrawTransparent();
	m_wndShowD2.SetTooltipText(_T("����Diablo II��Ϸ����"));
	m_wndShowD2.SetIcon(IDI_SHOWD2_ACTIVATE, IDI_SHOWD2_DEACTIVATE);

	m_wndKillD2.DrawTransparent();
	m_wndKillD2.SetTooltipText(_T("�ر�Diablo II����������"));
	m_wndKillD2.SetIcon(IDI_KILLD2_ACTIVATE, IDI_KILLD2_DEACTIVATE);

	m_wndViewFile.DrawTransparent();
	m_wndViewFile.SetTooltipText(_T("�鿴bot��¼�ļ�"));
	m_wndViewFile.SetIcon(IDI_VIEWFILE_ACTIVATE, IDI_VIEWFILE_DEACTIVATE);

	m_wndSelFile.DrawTransparent();
	m_wndSelFile.SetTooltipText(_T("ѡ��bot��¼�ļ�"));
	m_wndSelFile.SetIcon(IDI_SELFILE_ACTIVATE, IDI_SELFILE_DEACTIVATE);

	m_wndExit.DrawTransparent();
	m_wndExit.SetTooltipText(_T("�ر�SIG"));
	m_wndExit.SetIcon(IDI_EXIT_ACTIVATE, IDI_EXIT_DEACTIVATE);

	m_wndAppName.SetTransparent(TRUE);
	m_wndAppVer.SetTransparent(TRUE);
	m_wndLabel.SetTransparent(TRUE);
	m_wndCharName.SetTransparent(TRUE);
	m_wndD2JSP.SetTransparent(TRUE);

	m_wndCharName.SetTextColor(RGB(192, 192, 192));
	m_wndCharName.SetFontSize(9);
	m_wndCharName.SetFontName(_T("system"));
	m_wndCharName.SetFontBold(TRUE);
	m_wndCharName.SetFontItalic(TRUE);
	m_wndCharName.SetText(_T(""));

	m_wndD2JSP.SetTextColor(RGB(192, 192, 192));
	m_wndD2JSP.SetFontSize(7);
	m_wndD2JSP.SetFontUnderline(TRUE);
	m_wndD2JSP.SetFontName(_T("arial black"));
	m_wndD2JSP.SetTextColor(RGB(0, 255, 0));
	//m_wndD2JSP.SetFontBold(TRUE);
	//m_wndD2JSP.SetFontItalic(TRUE);

	m_wndLabel.SetTextColor(RGB(0, 255, 0));

	m_wndAppName.SetFontSize(20);
	m_wndAppName.SetFontName(_T("system"));
	m_wndAppName.SetFontBold(TRUE);
	m_wndAppName.SetFontItalic(TRUE);
	m_wndAppName.SetTextColor(RGB(128, 128, 128));
	//m_wndAppName.SetSunken(1);
	m_wndAppName.SetFont3D(TRUE, CLabel::Sunken);
	m_wndAppName.SetText3DHiliteColor(RGB(255, 255, 255));
	m_wndAppName.SetText(APP_NAME);

	m_wndAppVer.SetFontSize(16);
	m_wndAppVer.SetFontName(_T("system"));
	m_wndAppVer.SetFontBold(TRUE);
	m_wndAppVer.SetFontItalic(TRUE);
	m_wndAppVer.SetTextColor(RGB(128, 128, 128));
	m_wndAppVer.SetFont3D(TRUE, CLabel::Sunken);
	m_wndAppVer.SetText3DHiliteColor(RGB(255, 255, 255));
	m_wndAppVer.SetText(CString(_T("v")) + g_sAppVer);	

	m_wndMuleMode.DrawTransparent();
	m_wndMuleMode.SetIcon(IDI_MULEMODE_ACTIVATE, IDI_MULEMODE_DEACTIVATE);
	m_wndMuleMode.SetTooltipText(_T("Muleģʽ(δ����)"));

	m_cfg.LoadData(GetSigIni(), GetD2HackitIni(), GetAccountsIni(), GetD2GatewayIni(), GetDictionary());
	m_mem.SetBool(m_cfg.m_bAntiIdle, MEM_SIG_ANTIIDLE);

	m_wndFile.SetMenu(IDR_FILE, GetSafeHwnd());
	m_wndStatistics.SetMenu(IDR_STATISTICS, GetSafeHwnd());
	m_wndHelp.SetMenu(IDR_HELP, GetSafeHwnd());
	m_wndShowD2.SetMenu(IDR_SHOWD2, GetSafeHwnd());
	m_bInitialized = TRUE;	
	m_stats.LoadFile();
	SetTimer(ID_TMR_APPTIME, 1000, NULL);
	CString sCmdLine(AfxGetApp()->m_lpCmdLine);
	sCmdLine.MakeLower();

	//m_wndMuleMode.EnableWindow(!m_cfg.m_bUseD2JSP);

	if (m_cfg.m_bAutoStart || sCmdLine.Find(_T("-autostart")) != -1)
		OnStart();

	m_wndLC.SetType(LIGHT_LC);	
	m_wndHB.SetType(LIGHT_HB);
	m_wndHB.SetColor(LIGHT_RED);
	m_wndRV.SetType(LIGHT_RV);
	m_wndRV.SetColor(LIGHT_YELLOW);
	m_wndST.SetType(LIGHT_ST);
	m_wndST.SetColor(LIGHT_YELLOW);
	m_wndAI.SetType(LIGHT_AI);
	m_wndAI.SetColor(LIGHT_RED);

	CPoint pt(240, 112);
	m_wndLC.AdjustWindow(pt);
	pt.x += 9;
	m_wndHB.AdjustWindow(pt);
	pt.x += 9;
	m_wndRV.AdjustWindow(pt);
	pt.x += 9;
	m_wndST.AdjustWindow(pt);
	pt.x += 9;
	m_wndAI.AdjustWindow(pt);
	return TRUE;  // return TRUE  unless you set the focus to a control
}

void CSIGDlg::OnDestroy() 
{
	CDialog::OnDestroy();
	
	// TODO: Add your message handler code here
	KillTimer(ID_TMR_APPTIME);
	KillTimer(ID_TMR_AUTOCLOSE);
	Terminate();	
	m_mem.EraseAll();
	m_mem.SetLong(FLAG_THISAPP, MEM_SIG_STOP);

	CIni ini(GetMemIDFile());
	ini.WriteString(_T("Default"), _T("Memory ID"), _T(""));
	m_logger.Log(_T("SIG terminated."));
	m_logger.Flush();

	RestoreD2Window(TRUE);
	CRealmInfo::SetDefaultRealm(m_cfg.m_sBnetIP, m_cfg.m_sSubRealm);
}

void CSIGDlg::OnSysCommand(UINT nID, LPARAM lParam)
{

	if ((nID & 0xFFF0) == IDM_ABOUTBOX)
	{
		CAboutDlg dlgAbout;
		dlgAbout.DoModal();
	}
	else if (nID == 61472) // the minimize button id
	{
		EnableBossMode(TRUE);
	}
	else
	{
		CDialog::OnSysCommand(nID, lParam);
	}
}

// If you add a minimize button to your dialog, you will need the code below
//  to draw the icon.  For MFC applications using the document/view model,
//  this is automatically done for you by the framework.

void CSIGDlg::OnPaint() 
{
	CPaintDC dc(this); // device context for painting
	if (IsIconic())
	{		
		SendMessage(WM_ICONERASEBKGND, (WPARAM) dc.GetSafeHdc(), 0);

		// Center icon in client rectangle
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// Draw the icon
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CRect rect;
		GetClientRect(&rect);
		m_gradient.FillGradientRect(&rect, &dc, CGradient::FILL_BEVEL_NWSE);
		//CDialog::OnPaint();
	}
}

// The system calls this to obtain the cursor to display while the user drags
//  the minimized window.
HCURSOR CSIGDlg::OnQueryDragIcon()
{
	return (HCURSOR) m_hIcon;
}

// user difined message handlers
LRESULT CSIGDlg::OnSIGCreate(WPARAM wParam, LPARAM lParam)
{
	return (LRESULT)0;
}

LRESULT CSIGDlg::OnSIGLoaded(WPARAM wParam, LPARAM lParam)
{
	if (!m_bUseD2JSP && m_bStarted)
	{
		m_bSigLoaded = TRUE;
		//ShowInfo(_T("SIG: ģ����سɹ�"));
	}
	return (LRESULT)0;
}

LRESULT CSIGDlg::OnSIGUnloaded(WPARAM wParam, LPARAM lParam)
{
	m_bSigLoaded = FALSE;
	if (m_playerStats.GetStats(0) && !m_bUseD2JSP)
	{
		// if sig.dll is unloaded while in game, something is wrong
		OnGameLeave();
		if (m_bStarted)
		{			
			ShowInfo(_T("SIG: ģ����ж��"));
			PrepareToCloseD2();
		}
	}
	
	return (LRESULT)0;
}

LRESULT CSIGDlg::OnSIGResume(WPARAM wParam, LPARAM lParam)
{
	if (!m_bUseD2JSP)
	{
		SIGPause(FALSE);
		m_wndLabel.RedrawWindow();
	}
	return (LRESULT)0;
}

LRESULT CSIGDlg::OnSIGPause(WPARAM wParam, LPARAM lParam)
{
	if (!!m_bUseD2JSP)
	{
		SIGPause(TRUE);
		m_wndLabel.RedrawWindow();
	}
	return (LRESULT)0;
}

LRESULT CSIGDlg::OnSIGHeartBeat(WPARAM wParam, LPARAM lParam)
{
	if (!m_bUseD2JSP && m_bStarted)
	{
		m_wndHB.Flash(2);
	}
	return (LRESULT)0;
}

LRESULT CSIGDlg::OnSIGPktRcv(WPARAM wParam, LPARAM lParam)
{
	if (!m_bUseD2JSP && m_bStarted)
	{
		m_wndRV.Flash(2);		
		if (m_bGameJoined)
			SetTimer(ID_TMR_ACTIVITY, MAX_INACTIVITY, NULL);
	}
	return (LRESULT)0;
}

LRESULT CSIGDlg::OnSIGPktSnd(WPARAM wParam, LPARAM lParam)
{
	if (!m_bUseD2JSP && m_bStarted)
	{
		m_wndST.Flash(2);
		if (m_bGameJoined)
			SetTimer(ID_TMR_ACTIVITY, MAX_INACTIVITY, NULL);
	}
	return (LRESULT)0;
}

LRESULT CSIGDlg::OnSIGAntiIdle(WPARAM wParam, LPARAM lParam)
{
	if (!m_bUseD2JSP && m_bStarted)
	{
		m_wndAI.Flash(2);
	}
	return (LRESULT)0;
}

LRESULT CSIGDlg::OnSIGMsgString(WPARAM wParam, LPARAM lParam)
{
	if (!m_bUseD2JSP)
	{
		TCHAR sz[MAX_MSGSTR / sizeof(TCHAR) + 1] = _T("");
		if (m_mem.GetString(sz, MAX_MSGSTR / sizeof(TCHAR), MEM_SIG_MSGSTR) > 0)
			ShowInfo(sz, FALSE);
	}
	return (LRESULT)0;
}

LRESULT CSIGDlg::OnSIGStop(WPARAM wParam, LPARAM lParam)
{
	if (!m_bUseD2JSP && m_bStarted)
	{
		Terminate();
		m_wndLabel.RedrawWindow();
	}
	return (LRESULT)0;
}

LRESULT CSIGDlg::OnSIGQuitD2(WPARAM wParam, LPARAM lParam)
{
	if (!m_bUseD2JSP && m_bStarted)
	{
		ShowInfo(_T("���յ��ر���Ϸָ�����ִ��"));
		m_ticker.ResetTicker(2000);
		m_nCurState = ST_CLOSE_D2;
	}

	return (LRESULT)0;
}

LRESULT CSIGDlg::OnSIGKey(WPARAM wParam, LPARAM lParam)
{
	if (!m_bUseD2JSP)
		m_auto.SendSysKey(UINT(wParam));
	return (LRESULT)0;
}

LRESULT CSIGDlg::OnSIGNotify(WPARAM wParam, LPARAM lParam)
{
	if (!m_bUseD2JSP)
	{
	}
	return (LRESULT)0;
}

LRESULT CSIGDlg::OnSIGShowD2(WPARAM wParam, LPARAM lParam)
{
	if (!m_bUseD2JSP)
		RestoreD2Window(TRUE);
	return (LRESULT)0;
}

LRESULT CSIGDlg::OnSIGHideD2(WPARAM wParam, LPARAM lParam)
{
	if (!m_bUseD2JSP)
		::ShowWindow(m_cfg.FindD2HWnd(), m_bBossModeEnabled ? SW_HIDE : SW_MINIMIZE);
	return (LRESULT)0;
}

void CSIGDlg::OnTimer(UINT nIDEvent) 
{
	// TODO: Add your message handler code here and/or call default
	if (!m_bStarted)
	{
		CDialog::OnTimer(nIDEvent);
		return;
	}

	static CString sMsg;
	static HWND hD2Wnd = NULL;

	switch (nIDEvent)
	{
	case ID_TMR_APPTIME:
		m_dwAppTime++;
		if ((m_cfg.m_nAutoCloseType == CLOSE_BY_DATE && m_cfg.m_autoCloseDate <= COleDateTime::GetCurrentTime())
			|| (m_cfg.m_nAutoCloseType == CLOSE_BY_LEN && m_cfg.m_dwAutoCloseLen * 3600L <= m_dwAppTime))
		{
			KillTimer(ID_TMR_APPTIME);
			Terminate();
			m_cfg.CloseDiablo2();
			m_wndLabel.SetTextColor(RGB(255, 255, 0));
			m_nAutoCloseRemain = 10000;
			SetTimer(ID_TMR_AUTOCLOSE, 1000, NULL);
		}

		break;

	case ID_TMR_STARTTIME:
		m_stats.m_dwTotalStartTime = DWORD(COleDateTimeSpan(COleDateTime::GetCurrentTime() - m_stats.m_startTime).GetTotalSeconds());
		break;
	
	case ID_TMR_INGAMETIME:
		m_stats.UpdateInGameTime(1);
		if (m_cfg.m_bInGameLimit && m_cfg.m_dwInGameLimitLen < m_stats.GetCurrentGameTime())
		{
			if (!m_bPaused)
			{
				KillTimer(ID_TMR_FORCED2ONTOP);
				OnGameLeave(FALSE);					
				ShowInfo(_T("���ͣ��ʱ���ѵ���ǿ���˳���Ϸ��"));
				if (!m_cfg.LeaveGame())
					PrepareToCloseD2(2000);	
			}
		}
		break;	

	case ID_TMR_FORCED2ONTOP:
		hD2Wnd = m_cfg.FindD2HWnd();
		if (::IsIconic(hD2Wnd) || !::IsWindowVisible(hD2Wnd))
		{
			::ShowWindow(hD2Wnd, SW_SHOWNOACTIVATE);				
			::SetWindowPos(hD2Wnd, HWND_BOTTOM, 0, 0, 0, 0, SWP_NOMOVE | SWP_NOSIZE | SWP_NOACTIVATE);
		}
		break;

	case ID_TMR_ACTIVITY:
		KillTimer(ID_TMR_ACTIVITY);
		OnGameLeave();
		break;

	case ID_TMR_CHECKSTATS:
		TmrProc_CheckStats();
		break;

	case ID_TMR_MAIN:
		TmrProc_Main();
		break;

	case ID_TMR_PLAYER:
		TmrProc_Player();
		break;

	case ID_TMR_AUTOCLOSE:		
		m_nAutoCloseRemain -= 1000;
		sMsg.Format(_T("SIG�Զ��ر�ʱ���ѵ�������%d���ر�..."), m_nAutoCloseRemain / 1000);
		ShowInfo(sMsg, FALSE);

		if (m_nAutoCloseRemain <= 0)
		{
			KillTimer(ID_TMR_AUTOCLOSE);

			if (m_cfg.m_bCloseWindows)
			{
				COSInfo osi;
				osi.WinShutDown();
			}			
			EndDialog(IDCANCEL);
		}
		break;

	default:
		break;
	}
	
	CDialog::OnTimer(nIDEvent);
}

void CSIGDlg::ShowInfo(LPCTSTR lpInfo, BOOL bLog)
{
	if (m_sOrigInfo == lpInfo)
		return;

	m_sOrigInfo = lpInfo;

	if (!m_bPaused)
		m_wndLabel.SetText(lpInfo);

	m_trayIcon.SetTooltipText(lpInfo);

	if (bLog)
		m_logger.Log(lpInfo);
}

void CSIGDlg::TmrProc_Main()
{
	static CString sMsg;
	static CString sName, sPwd;
	static int nNum = 0;
	static HWND hD2Wnd = NULL;

	if (!m_bStarted || m_bPaused)
		return;

	m_ticker.TickIt(TICK);	

	switch (m_nCurState)
	{
	case ST_CLOSE_D2:		
		
		if (!m_ticker.IsTickEnded())
			break;
		
		m_wndLabel.SetTextColor(RGB(0, 255, 0));
		ShowInfo(_T("���ڹر�Diablo II"));	
		m_cfg.CloseDiablo2();
		m_ticker.ResetTicker(6000);		
		m_nCurState = ST_CHECK_KILL_D2;			
		break;

	case ST_CHECK_KILL_D2:
		if (m_cfg.FindD2HWnd())
		{
			// close failed, retry
			m_dwCloseD2Fail++;

			if (m_dwCloseD2Fail > CLOSE_D2_RETRY)
			{
				// failed to kill d2, terminate
				Terminate(FALSE);
				m_wndLabel.SetTextColor(RGB(255, 255, 0));
				ShowInfo(_T("�޷�ɱ��Diablo II���̣�������ֹ"));
			}
			else
			{
				// retry
				ShowInfo(_T("��ֹDiablo II����ʧ�ܣ�׼������..."));
				m_ticker.ResetTicker(3000);
				m_nCurState = ST_CLOSE_D2;	
			}
		}
		else
		{
			// close succeeded
			m_dwCloseD2Fail = 0;
			m_ticker.ResetTicker(m_cfg.m_dwErrorDelay);
			m_nCurState = ST_CHECK_X_D2WND;
		}
			
		break;

	case ST_CHECK_X_D2WND:
		if (!m_ticker.IsTickEnded())
			break;
		
		nNum = CheckAndKillxD2();
		if ((UINT)nNum + 1 > m_cfg.m_nMaxD2Proc)
		{
			sMsg.Format(_T("ϵͳD2���̽�Ҫ����%d�������ڳ������..."), nNum);
			ShowInfo(sMsg);
			m_ticker.ResetTicker(10000);
		}
		else
		{
			MoveOn(ST_WAIT_LAUNCH);
		}
		break;

	case ST_WAIT_LAUNCH:

		sMsg.Format(_T("%d�������Diablo II"), m_ticker.GetRemainer() / 1000);
		ShowInfo(sMsg, FALSE);

		if (!m_ticker.IsTickEnded())
			break;

		ShowInfo(_T("׼������Diablo II"), FALSE);	

		MoveOn(ST_LAUNCH_DIABLO);
		break;

	case ST_LAUNCH_DIABLO:

		m_bSigLoaded = FALSE;
		m_dwLoginFail = 0;
		m_dwCloseD2Fail = 0;

		if (!m_ticker.IsTickEnded())
			break;

		m_spy.SetD2Wnd(NULL);
		m_wndLabel.SetTextColor(RGB(0, 255, 0));
		ShowInfo(_T("��������Diablo II..."));
		sMsg = m_cfg.m_sParamLine;
		sMsg.MakeLower();

		if (sMsg.Find(_T("-title")) != -1)
		{
			m_wndLabel.SetTextColor(RGB(255, 255, 0));			
			Terminate();
			ShowInfo(_T("���ַǷ�����\"-title\"���޷�����Diablo II��"));
			MessageBox("�����в����в��ú���\"-title\"������", _T("�Ƿ�����"), MB_ICONEXCLAMATION);
			break;
		}
	
		m_logger.Log(m_cfg.GetD2FullCmdLine());

		//hFg = ::GetForegroundWindow();
		if (m_cfg.LaunchDiablo2())
		{
			// diablo launched(file-wise)
			m_ticker.ResetTicker(m_cfg.m_dwLaunchDelay);
			m_nCurState = ST_CHECK_DIABLO;
		}
		else
		{
			// launching diablo failed
			Terminate();
			m_wndLabel.SetTextColor(RGB(255, 255, 0));
			ShowInfo(_T("�޷�����Diablo II����ȷ��·�����ļ�������ȷ�ԡ�"));
		}

		break;
		
	case ST_CHECK_DIABLO:
		
		ShowInfo(_T("���ڼ����Ϸ״̬"), FALSE);
		
		hD2Wnd = m_cfg.FindD2HWnd();
		if (hD2Wnd)
		{
			m_cfg.SetD2WndFound(hD2Wnd);
			ShowInfo(_T("��Ϸ�������������ڼ������..."));
			
			// save window placement
			memset(&m_wp, 0, sizeof(WINDOWPLACEMENT));
 			m_wp.length = sizeof(WINDOWPLACEMENT); 
 			::GetWindowPlacement(hD2Wnd, &m_wp);	

			m_spy.SetD2Wnd(hD2Wnd);
			m_auto.SetTargetHWnd(hD2Wnd);

			//HWND hNext = ::GetNextWindow(hD2Wnd, GW_HWNDNEXT);
			//::SetWindowPos(hD2Wnd, HWND_BOTTOM, 0, 0, 0, 0, SWP_NOMOVE | SWP_NOSIZE | SWP_NOACTIVATE);
			//SetForegroundWindowEx(hFg);			
			
			m_nCurState = ST_D2LOADED;
			m_ticker.ResetTicker(8000);
			break;
		}		

		if (m_ticker.IsTickEnded())	
		{
			// failed
			ShowInfo(_T("����Diablo II.exe��ʱ��׼������..."));
			PrepareToCloseD2();
			break;
		}
			
		break;

	case ST_D2LOADED:

		if (m_ticker.IsTickEnded() || m_spy.IsBNetConnectSuccessful())
		{
			m_nCurState = ST_CANCEL_CONNECT;
			m_ticker.ResetTicker(m_cfg.m_dwMinConnDelay);			
		}
		break;

	case ST_CONNECTBNET:

		ShowInfo(_T("�������ӵ�Battle.net..."));
		if (m_ticker.IsTickEnded())
		{
			m_auto.ConnectToBNet();					
			m_ticker.ResetTicker(m_cfg.m_dwMinConnDelay + m_dwConnRetry * AUTO_CONN_INC_BASE);
			m_nCurState = ST_CANCEL_CONNECT;			
		}
		break;

	case ST_CANCEL_CONNECT:		
		
		sMsg.Format(_T("���ڼ��Battle.net���ӣ�ʣ��%d��"), m_ticker.GetRemainer() / 1000);
		ShowInfo(sMsg, FALSE);
		if (m_ticker.IsTickEnded())
		{	
			m_logger.Log(_T("���ڼ��Battle.net����"));
			m_auto.CancelConnect();
			m_nCurState = ST_CHECK_CONNECT;
			m_ticker.ResetTicker(3000);
		}
		break;

	case ST_CHECK_CONNECT:

		if (m_spy.IsBNetConnectSuccessful())
		{
			//m_stats.m_dwSuccessConnect++;
			ShowInfo(_T("Battle.net���ӳɹ�"));			
			MoveOn(ST_LOGIN);
		}
		else
		{			
			ShowInfo(_T("Battle.net���ӳ�ʱ��׼������"));			

			if (m_dwConnRetry < AUTO_CONN_RETRY_MAX)
			{
				m_dwConnRetry++;
				MoveOn(ST_CONNECTBNET);
			}
			else
			{
				m_dwConnRetry = 0;
				MoveOn(ST_CLOSE_D2);
			}
		}

		break;

	case ST_LOGIN:

		if (!m_ticker.IsTickEnded())
			break;		

		sMsg.Format(_T("���ڵ�¼�˺�\"%s\""), m_bMuleModeEnabled ? m_sCurMuleAccountName : m_cfg.m_sAccount);
		ShowInfo(sMsg);		

		if (m_bMuleModeEnabled)
		{
			m_auto.Login(m_sCurMuleAccountName, m_sCurMulePassword);
		}
		else
		{
			m_auto.Login(m_cfg.m_sAccount, m_cfg.m_sPassword);
		}
			
		m_stats.m_dwTotalLogin++;
		m_nCurState = ST_CHECK_LOGIN;
		m_ticker.ResetTicker(4000 + m_cfg.m_dwLoginDelay);

		//if (::GetForegroundWindow() != m_cfg.FindD2HWnd())
		MoveD2WndOut(); // activate d2 window
		break;

	case ST_CHECK_LOGIN:
		ShowInfo(_T("���ں�ʵ��¼���"), FALSE);

		//if (m_ticker.GetElapsed() > 5000)
		//{
			// Limit the max time that d2 wnd is out
			//MoveD2WndIn();
		//}

		if (m_spy.IsBNetLoginSuccessful())
		{
			//MoveD2WndIn();
			ShowInfo(_T("��¼�ɹ�"));
			m_dwConnRetry = 0;			
			MoveOn(ST_SHOW_CHARLIST);
			break;
		}

		if (m_ticker.IsTickEnded())
		{
			MoveD2WndIn();
			m_dwLoginFail++;
			m_auto.CancelConnect();
			ShowInfo(_T("��¼Battle.net��ʱ,׼������..."));
			m_ticker.ResetTicker(3000);
			m_nCurState = ST_CANCEL_LOGIN;
			break;
		}		

		break;

	case ST_CANCEL_LOGIN:
		if (m_ticker.IsTickEnded())
		{
			if (m_dwLoginFail < 3)
			{
				if (m_spy.IsBNetConnectSuccessful())
				{
					MoveOn(ST_LOGIN);
				}
				else
				{
					MoveOn(ST_CONNECTBNET);
				}
			}
			else
			{
				if (m_bMuleModeEnabled)
				{					
					m_cfg.DisableCurrentMule();
					if (MULE_NO_MORE == m_cfg.GetNextMule(m_sCurMuleAccountName, m_sCurMulePassword, m_nCurMuleChar))
					{
						MuleEnd();
					}
					else
					{
						ShowInfo(_T("��ǰ�˺ŷ���ʧ��,׼����¼��һ���˺�..."));
						MoveOn(ST_CLOSE_D2);
					}
				}
				else
				{
					MoveOn(ST_CLOSE_D2);
				}
			}
		}
		break;

	case ST_SHOW_CHARLIST:		

		ShowInfo(_T("���ڵȴ���ɫ�б�..."), FALSE);		
		MoveD2WndOut();
		m_ticker.ResetTicker(m_cfg.m_dwLoginDelay);
		m_nCurState = ST_CHECK_CHARLIST;
		break;

	case ST_CHECK_CHARLIST:

		//if (m_ticker.GetElapsed() > 5000)
		//{
			// Limit the max time that d2 wnd is out
		//	MoveD2WndIn();
		//}

		nNum = m_spy.GetCharCount();
		if (nNum > 0)
		{
			sMsg.Format(_T("��ɫ�б���ճɹ����ܹ�%d����ɫ"), nNum);
			ShowInfo(sMsg);
			MoveD2WndIn();

			if (!m_bMuleModeEnabled && m_cfg.m_nCharPosition > nNum)
			{
				// invalid char position
				//Terminate();
				//sMsg.Format(_T("�˺�\"%s\"�й�����%d����ɫ���޷�ѡ���%d�����뵽\"���� | ��¼\"����ҳ����ѡ���ɫλ�á�"), m_cfg.m_sAccount, nNum, m_cfg.m_nCharPosition);
				sMsg.Format(_T("��Ч��ɫλ��(��%d�� ��%d��)"), m_cfg.m_nCharPosition, nNum);
				m_wndLabel.SetTextColor(RGB(255, 255, 0));
				ShowInfo(sMsg);
				MoveOn(ST_CLOSE_D2, 5000);
				//m_cfg.CloseDiablo2();
				//MessageBox(sMsg, _T("��Ч�û�����"), MB_ICONEXCLAMATION);
				break;
			}

			if (m_bMuleModeEnabled && m_nCurMuleChar > nNum)
			{
				ShowInfo(_T("��Ч��ɫλ��"));
				m_cfg.UpdateMuleCharCount(nNum);
				MoveOn(ST_EXIT_CHARLIST);
				break;
			}

			MoveOn(ST_SELECT_CHAR);
			break;
		}

		if (m_ticker.IsTickEnded())
		{
			MoveD2WndIn();
			ShowInfo(_T("��ɫ�б�ʱ��׼������..."));
			MoveOn(ST_CLOSE_D2);	
			break;
		}	

		break;

	case ST_SELECT_CHAR:

		sMsg.Format(_T("����ѡȡ��ɫ(λ��%d)"), m_bMuleModeEnabled ? m_nCurMuleChar : m_cfg.m_nCharPosition);
		ShowInfo(sMsg);

		if (m_bMuleModeEnabled)
		{
			m_auto.SelectChar(m_nCurMuleChar);
		}
		else
		{
			 m_auto.SelectChar(m_cfg.m_nCharPosition);
		}
		
		m_ticker.ResetTicker(m_cfg.m_dwCharSelDelay);		
		m_nCurState = ST_CHECK_CHATROOM;
		break;

	case ST_CHECK_CHATROOM:

		ShowInfo(_T("���ں˲��ɫѡ��"), FALSE);

		if (m_spy.IsInChatRoom())
		{
			MoveOn(ST_CHANNEL_STUFF);
			break;
		}

		if (m_ticker.IsTickEnded())
		{
			m_auto.CleanDlg();
			ShowInfo(_T("��ɫѡ��ʱ��׼������..."));
			if (m_bMuleModeEnabled)
			{
				// cannot select this char, move on
				//if (m_cfg.GetNextMule(m_sCurMuleAccountName, m_sCurMulePassword, m_nCurMuleChar) == MULE_NO_MORE)
				//	MuleEnd();
				//else
					m_ticker.ResetTicker(3000);
					m_nCurState = ST_EXIT_CHARLIST;
			}
			else
			{
				MoveOn(ST_CLOSE_D2);
			}
			
			break;
		}

		break;

	case ST_CHANNEL_STUFF:	
		
		// load d2jsp?
		if (m_bUseD2JSP && !m_cfg.IsD2JspLoaded())
		{
			ShowInfo(_T("���ڼ���D2JSP"));
			if (!m_cfg.LoadD2JSP())
				ShowInfo(_T("D2JSP����ʧ��"));
		}

		ShowInfo(_T("���ڽ��벢���Battle.netƵ����Ϣ..."));
		m_dwJoinFail = 0;
		
		if (m_bBossModeEnabled)
			::ShowWindow(m_cfg.FindD2HWnd(), SW_HIDE);

		if (!m_cfg.m_sChannelName.IsEmpty())
		{
			sMsg.Empty();
			DWORD dwDelay = 0;
			if (!m_cfg.m_sChannelName.IsEmpty())
			{
				sMsg.Format(_T("����Ƶ��\"%s\""), m_cfg.m_sChannelName);
				dwDelay += 2500;
			}

			/*
			if (!m_cfg.m_sMessage.IsEmpty())
			{
				if (!sMsg.IsEmpty())
					sMsg += _T("��");
				sMsg += _T("ע���Զ��ظ����");	
				dwDelay += 2500;
			}
			*/

			ShowInfo(sMsg);

			m_auto.JoinChannel(m_cfg.m_sChannelName, NULL);
			m_ticker.ResetTicker(dwDelay);			
		}
		else
		{
			m_ticker.ResetTicker(0);
		}
		
		m_nCurState = m_bUseD2JSP ?  ST_PREPARE_CREATE : ST_LOAD_SIG;
		break;

	case ST_LOAD_SIG:
		if (!m_bUseD2JSP && !m_bSigLoaded)
		{
			ShowInfo(_T("���ڼ���SIG���..."), TRUE);
			if (!m_cfg.LaunchD2Hackit())
			{
				// load failed
				m_wndLabel.SetTextColor(RGB(255, 255, 0));			
				ShowInfo(_T("SIG�������ʧ�ܣ�׼������..."));
				m_ticker.ResetTicker(10000);
				m_nCurState = ST_WAIT_RESET;
				break;
			}

			m_ticker.ResetTicker(60000); // on damn minute enough for loading the fucking stuff? 
		}
		m_nCurState = ST_CHECK_SIG;
		break;

	case ST_CHECK_SIG:
		
		if (m_bUseD2JSP || m_bSigLoaded)
		{
			MoveOn(ST_PREPARE_CREATE);
		}
		else if (m_ticker.IsTickEnded())
		{
			// still not loaded
			m_wndLabel.SetTextColor(RGB(255, 255, 0));			
			ShowInfo(_T("SIG�������ʧ�ܣ�׼������..."));
			m_ticker.ResetTicker(10000);
			m_nCurState = ST_WAIT_RESET;
		}
		else
		{
			ShowInfo(_T("���ڼ���SIG���..."), FALSE);
		}
		break;

	case ST_PREPARE_CREATE:
		nNum = m_cfg.GetCreateDelay();
		m_ticker.ResetTicker(nNum);
		sMsg.Format(_T("׼����������Ϸ(����ӳ�%d)..."), nNum);
		ShowInfo(sMsg, FALSE);
		m_nCurState = ST_CREATE_GAME;
		break;

	case ST_CREATE_GAME:		

		if (m_bBossModeEnabled)
			::ShowWindow(m_cfg.FindD2HWnd(), SW_HIDE);

		if (!m_spy.IsInChatRoom())
		{
			MoveOn(ST_CHANNEL_RETRY, 15000);
			break;
		}

		if (!m_ticker.IsTickEnded())
			break;

		if (m_bBossModeEnabled)
			::ShowWindow(m_cfg.FindD2HWnd(), SW_HIDE);

		if (!m_bUseD2JSP && !m_bSigLoaded)
		{
			m_wndLabel.SetTextColor(RGB(255, 255, 0));
			ShowInfo(_T("SIG�������ʧ�ܣ�׼������..."));
			m_ticker.ResetTicker(10000);
			m_nCurState = ST_WAIT_RESET;
			break;
		}

		m_dwCount = 0;
		m_bGameJoined = FALSE;		
		m_stats.m_dwTotalCreate++;
		
		nNum = GetNextGameStats(sName, sPwd);
		if (nNum == NEXT_JOIN_SPEC)
		{
			m_sCurGamename = sName;
			sMsg.Format(_T("���ڼ�����Ϸ\"%s"), m_sCurGamename);
			m_auto.JoinGame(m_sCurGamename, sPwd, m_cfg.m_dwCreateBtnDelay, 200);
		}
		else if (nNum == NEXT_CREATE_SPEC)
		{
			m_sCurGamename = sName;
			sMsg.Format(_T("���ڴ�����Ϸ\"%s"), m_sCurGamename);
			m_auto.CreateGame(m_sCurGamename, sPwd, m_cfg.m_nDifficulty, m_cfg.m_dwCreateBtnDelay, 200);
		}
		else
		{
			m_sCurGamename = m_cfg.GenerateGameName();
			sPwd = m_cfg.GenerateGamePwd();
			sMsg.Format(_T("���ڴ�����Ϸ\"%s"), m_sCurGamename);
			m_auto.CreateGame(m_sCurGamename, sPwd, m_cfg.m_nDifficulty, m_cfg.m_dwCreateBtnDelay, 200);
		}	
		
		ShowInfo(sMsg + _T("/") + sPwd + _T("\"..."));	
		m_nCurState = ST_BEFORE_CHECKGAME;
		m_ticker.ResetTicker(3500 + m_cfg.m_dwCreateBtnDelay);
		break;

	case ST_CHANNEL_RETRY:
		ShowInfo(_T("����̽���ɫ״��"));
		if (m_spy.IsInChatRoom())
		{
			MoveOn(ST_PREPARE_CREATE);
		}
		else if (m_ticker.IsTickEnded())
		{
			if (m_spy.IsBNetLoginSuccessful())
			{
				if (m_spy.GetCharCount() > 0)
				{
					// we are in char sel screen now
					MoveOn(ST_SELECT_CHAR);
				}
				else
				{
					// we are in login screen now
					MoveOn(ST_LOGIN);
				}
			}
			else
			{
				// we are completely dropped from bnet
				MoveOn(ST_RECONNECT);
			}
		}
	
		break;

	case ST_WAIT_RESET:
		ResetSig();
		break;

	case ST_BEFORE_CHECKGAME:
		if (m_ticker.IsTickEnded())
		{
			m_nCurState = ST_CHECK_GAME;
			m_ticker.ResetTicker(m_cfg.m_dwJoinDelay);
		}
		break;
		
	case ST_CHECK_GAME:

		nNum = (int)m_spy.GetCreationQueue();

		if (nNum)
			sMsg.Format(_T("������Ϸ���У�%d"), nNum);
		else
			sMsg = _T("���ں˲���Ϸ״̬...");

		ShowInfo(sMsg, FALSE);
		if (m_bGameJoined) // joined game successfully
		{				
			m_dwJoinFail = 0;
			MoveOn(ST_IN_GAME); // ready			
			break;
		}
		else
		{
			// not yet joined, check if we got some problem

			if (m_ticker.GetElapsed() > 4000)
			{
				m_nRecentCreateResult = m_spy.GetCreationResult();
				if (m_nRecentCreateResult == CREATE_NAME_INVALID
					|| m_nRecentCreateResult == CREATE_NAME_EXIST)
				{
					m_ticker.ResetTicker(0);
				}
				else if (m_nRecentCreateResult == CREATE_FAILED)
				{
					ShowInfo(_T("������Ϸʧ��..."));
				}
				else if (m_nRecentCreateResult == CREATE_SERVER_DOWN)
				{
					ShowInfo(_T("Server Down..."));
				}	
			}
		}

		//if (IsInChatRoom(m_cfg.FindD2HWnd()))
		//{
			// we are in chat room means joining failed, for whatever reason
			// but how to handle that without causing any other probelms?...
		//}

		if (m_ticker.IsTickEnded())
		{
			// join timeout, retry

			if (m_dwJoinFail < (DWORD)(m_cfg.m_nJoinRetry))
			{
				// not yet exceed maximum retry counter, so try again
				m_auto.CancelQueue();
				m_dwJoinFail++;

				if (m_nRecentCreateResult == CREATE_NAME_INVALID)
				{
					sMsg.Format(_T("��Ϸ��\"%s\"�Ƿ���׼���ؽ�..."), m_sCurGamename);
					ShowInfo(sMsg);
					m_ticker.ResetTicker(5000);

				}
				else if (m_nRecentCreateResult == CREATE_NAME_EXIST)
				{
					sMsg.Format(_T("��Ϸ��\"%s\"�Ѵ��ڣ�׼���ؽ�..."), m_sCurGamename);
					ShowInfo(sMsg);
					m_ticker.ResetTicker(5000);
				}
				else
				{
					ShowInfo(_T("���볬ʱ��׼������..."));
					m_ticker.ResetTicker(10000);
				}

				m_nCurState = ST_CREATE_GAME;
			}
			else
			{
				// completely failed

				// the user specified "next game" stats seems invalid, clean them
				CleanNextGameStats();

				if (!m_bMuleModeEnabled)
				{
					// not running mule mode, just shut down D2					
					ShowInfo(_T("������Ϸʧ�ܣ�׼���ر�Diablo II..."));
					PrepareToCloseD2();
				}
				else
				{
					m_nCurState = ST_EXIT_CHANNEL;
					m_ticker.ResetTicker(10000);
					ShowInfo(_T("�ý�ɫ������Ϸʧ�ܣ�׼��������һ����ɫ"));
				}
			}
		}
		
		break;

	case ST_IN_GAME:			
		
		break;

	case ST_CHECK_EXIT_GAME:

		//ShowInfo(_T("���ں˲���Ϸ�˳�״��"), FALSE);

		if (m_bMuleModeEnabled)
		{
			m_ticker.ResetTicker((rand() % 6000) + 4000);
			m_nCurState = ST_EXIT_CHANNEL;
			ShowInfo(_T("��Ϸ�˳���׼��������һ����ɫ"));	
		}
		else
		{
			MoveOn(ST_PREPARE_CREATE);
			ShowInfo(_T("��Ϸ�˳���׼���ؽ�..."));
		}

		break;

	case ST_EXIT_CHANNEL:
		
		if (m_ticker.IsTickEnded())
		{
			ShowInfo(_T("�����˳�Battle.netƵ��"));
			m_auto.SendEscape();
			m_nCurState = ST_EXIT_CHARLIST;
			m_ticker.ResetTicker(8000);
		}		
		
		break;

	case ST_EXIT_CHARLIST:

		if (/*GetCharCount(m_cfg.FindD2HWnd()) > 0 || */m_ticker.IsTickEnded())
		{
			int nType = m_cfg.GetNextMule(m_sCurMuleAccountName, m_sCurMulePassword, m_nCurMuleChar);
			if (nType == MULE_SAME_ACCOUNT)
			{
				// log on next char
				MoveOn(ST_SHOW_CHARLIST);
			}
			else if (nType == MULE_SWITCH_ACCOUNT)
			{
				// no more char in this mule account, log on next account
				ShowInfo(_T("�����˳���ɫѡ�����"));
				m_auto.SendEscape();
				m_ticker.ResetTicker(8000);
				m_nCurState = ST_RECONNECT;
			}
			else
			{
				MuleEnd();
			}
		}		
		
		break;

	case ST_RECONNECT:
		if (!m_ticker.IsTickEnded())
			break;
		m_dwConnRetry = 0;
		MoveOn(m_spy.IsInChatRoom() ? ST_PREPARE_CREATE : ST_CONNECTBNET);
		break;
		
	default:
		break;
	} // switch (m_nCurState)
}

void CSIGDlg::TmrProc_CheckStats()
{
	if (!m_bStarted)
		return;	

	CheckGameInOut();

	if (m_bPaused)
		return;
	
	// exclude special states
	if (m_nCurState == ST_LAUNCH_DIABLO
		|| m_nCurState == ST_CHECK_DIABLO		
		|| m_nCurState == ST_CLEAN_ERRORMSG
		|| m_nCurState == ST_CLOSE_D2
		|| m_nCurState == ST_KILL_D2_PROC
		|| m_nCurState == ST_CHECK_KILL_D2
		|| m_nCurState == ST_CHECK_X_D2WND
		|| m_nCurState == ST_WAIT_LAUNCH
		|| m_nCurState == ST_NONE)
		return;	

	if (CheckAndCloseErrors() || m_cfg.FindD2HWnd() == NULL) // error occurred
	{
		KillTimer(ID_TMR_FORCED2ONTOP);
		OnGameLeave(FALSE);
		ShowInfo(_T("���ڼ�Ⲣ�ų�������Ϣ..."));
		m_stats.m_dwTotalCrash++;
		PrepareToCloseD2(6000);
	}
}

void CSIGDlg::Terminate(BOOL bUnloadModules)
{	
	KillTimer(ID_TMR_ACTIVITY);
	KillTimer(ID_TMR_PLAYER);
	KillTimer(ID_TMR_INGAMETIME);
	KillTimer(ID_TMR_STARTTIME);
	KillTimer(ID_TMR_MAIN);
	KillTimer(ID_TMR_CHECKSTATS);
	KillTimer(ID_TMR_FORCED2ONTOP);	

	CRealmInfo::BNetIPCleanup(m_cfg.m_sBnetIP);
	m_dwConnRetry = 0;
	if (m_bStarted)
	{
		OnGameLeave();

		m_nCurState = ST_NONE;	
		
		m_wndLabel.FlashText(FALSE);
		m_bStarted = FALSE;
		m_bPaused = FALSE;
		m_mem.SetLong(FLAG_THISAPP, MEM_SIG_STOP);
		m_mem.SetLong(0, MEM_SIG_PAUSE);
		m_wndStart.SetIcon(IDI_START_ACTIVATE, IDI_START_DEACTIVATE);
		ShowInfo(_T("ֹͣ"), FALSE);
		m_wndStart.SetTooltipText(_T("��ʼ"));

		if (!m_wndShowD2.IsWindowEnabled())
			MoveD2WndIn();

		m_logger.Log(_T("SIG Stopped."));
		m_logger.AddLineBreak();
		m_logger.Flush();

		m_stats.UpdateStartTime(TRUE);
		m_stats.SaveFile();
	}
}

void CSIGDlg::OnStart() 
{
	// TODO: Add your control notification handler code here
	if (!m_bStarted)
	{			
		m_bUseD2JSP = m_cfg.m_bUseD2JSP;
		
		m_aCriticalFiles.RemoveAll();
		m_aCriticalFiles.Add(g_sAppDir + _T("\\SigRst.exe"));
		
		if (m_bUseD2JSP)
		{
			// Files needed for D2JSP
			m_aCriticalFiles.Add(GetD2JspDir() + _T("\\D2Jsp.dll"));
			m_aCriticalFiles.Add(GetD2JspDir() + _T("\\D2Jsp.exe"));
			m_aCriticalFiles.Add(GetD2JspDir() + _T("\\D2Jsp.key"));
		}
		else
		{
			// Files needed for D2Hackit
			CheckSIGinINI();
			m_aCriticalFiles.Add(GetHackDir() + _T("\\D2Hackit.dll"));
			m_aCriticalFiles.Add(GetHackDir() + _T("\\D2Hackit.ini"));
			m_aCriticalFiles.Add(g_sAppDir + _T("\\LoaderEx.dll"));
			m_aCriticalFiles.Add(GetHackDir() + _T("\\Sig.dll"));
			m_aCriticalFiles.Add(g_sAppDir + _T("\\SigPack.dll"));
		}

		int nMissingFile = IsFileMissing();
		if (nMissingFile != -1)
		{
			// some files are missing
			ShowInfo(_T("SIG��ʼ��ʧ��"));
			CString sErr;
			sErr.Format(_T("�ļ�ȱʧ��\n�޷��ҵ�������ļ�\"%s\"��\n\n����ʧ�ܡ�"), m_aCriticalFiles[nMissingFile]);
			MessageBox(sErr, _T("SIG��ʼ��ʧ��"), MB_ICONEXCLAMATION);
			return;
		}
		
		m_dwConnRetry = 0;
		if (m_cfg.m_sD2Executable.IsEmpty())
		{
			MessageBox(_T("��ָ��Diablo II��Ϸ�����ִ���ļ�����·����"), _T("��Ч�û�����"), MB_ICONEXCLAMATION);
			DoConfig(0);
			return;
		}

		if (m_cfg.m_sAccount.IsEmpty() || m_cfg.m_sPassword.IsEmpty())
		{
			MessageBox(_T("����ȷ�����˺���Ϣ��"), _T("��Ч�û�����"), MB_ICONEXCLAMATION);
			DoConfig(1);
			return;
		}	
		
		// checl D2hackit.dll and D2loader MD5
		if (!CheckD2LoaderMd5())
			return;

		// are we running under mule-mode?		
		if (m_bMuleModeEnabled)
		{
			// some ppl say this message is annoying so... :(
			//if (MessageBox(_T("���Ѿ�ѡ��������Muleģʽ���ڸ�ģʽ�³��򽫽�����������ָ�����˺��еĽ�ɫ��¼һ�飬Ȼ���Զ�ֹͣ���С�\n\nȷ��Ҫ������Muleģʽ����"), _T("Muleģʽ"), MB_ICONQUESTION | MB_YESNO) == IDNO)
				//return;

			m_cfg.StartMuleMode();

			if (m_cfg.GetNextMule(m_sCurMuleAccountName, m_sCurMulePassword, m_nCurMuleChar) == MULE_NO_MORE)
			{
				// there're no mule accounts configured
				MessageBox(_T("����δ����Mule�˺��б���Mule�˺��б���û�а����κν�ɫ��"), _T("Muleģʽ"), MB_ICONEXCLAMATION);
				MuleEnd();
				return;
			}	

			m_logger.Log(_T("Muleģʽ���п�ʼ"));
		}
		else
		{
			m_cfg.StopMuleMode();
			m_logger.Log(_T("����ģʽ���п�ʼ"));
		}		

		// start it
		ShowInfo(_T("���ڳ�ʼ��SIG���л���..."));	
		CString sCmdLine(AfxGetApp()->m_lpCmdLine);
		sCmdLine.MakeLower();

		if (sCmdLine.Find(_T("-noclean")) == -1)
			m_stats.Initialize(TRUE);
		else
			m_stats.ContinueStats();

		SetTimer(ID_TMR_STARTTIME, 1000, NULL);

		// clean log
		CleanLogs();
		m_wndLabel.SetTextColor(RGB(0, 255, 0));
		//m_logger.Clean(TRUE); // empty log file
		m_bStartOver = TRUE;
		m_cfg.CloseDiablo2();
		//m_ticker.ResetTicker(4000);
		m_nCurState = ST_CHECK_X_D2WND;
		SetTimer(ID_TMR_CHECKSTATS, TICK * 2, NULL);
		SetTimer(ID_TMR_MAIN, TICK, NULL);
		//SetTimer(ID_TMR_PLAYER, 500, NULL);

		m_bStarted = TRUE;
		m_mem.SetLong(0, MEM_SIG_STOP);
		m_bPaused = FALSE;
		m_mem.SetLong(0, MEM_SIG_PAUSE);
		m_wndStart.SetIcon(IDI_PAUSE_ACTIVATE, IDI_PAUSE_DEACTIVATE);
		m_wndStart.SetTooltipText(_T("��ͣ"));
		HideLightsForD2JSP();
	}
	else
	{
		if (!m_bPaused)
		{
			// pause it
			SIGPause(TRUE);
		}
		else
		{
			// resume it
			SIGPause(FALSE);
		}
	}	
}


void CSIGDlg::OnSettings() 
{
	// TODO: Add your control notification handler code here
	DoConfig(0);
}

void CSIGDlg::OnStop() 
{
	// TODO: Add your control notification handler code here
	Terminate();
}

BOOL CSIGDlg::PreTranslateMessage(MSG* pMsg)
{
	if (pMsg->message == WM_LBUTTONDOWN
		|| pMsg->message == WM_RBUTTONDOWN
		|| pMsg->message == WM_MBUTTONDOWN
		|| pMsg->message == WM_KEYDOWN
		|| pMsg->message == WM_NCLBUTTONDOWN
		|| pMsg->message == WM_NCRBUTTONDOWN)
	{
		m_wndHPManaBar.HideStatsWnd();
	}

	if (pMsg->message == WM_KEYDOWN)
	{
		if (pMsg->wParam == VK_RETURN || pMsg->wParam == VK_SPACE)
			return TRUE;

		if (pMsg->wParam == VK_ESCAPE)
		{
			this->EnableBossMode(TRUE);
			return TRUE;
		}
	}	

	return CDialog::PreTranslateMessage(pMsg);	// CG: This was added by the ToolTips component.
}

int CSIGDlg::IsFileMissing() const
{
	CFileFind finder;

	for (int i = 0; i < m_aCriticalFiles.GetSize(); i++)
	{
		if (!finder.FindFile(m_aCriticalFiles[i]))
		{
			finder.Close();
			return i; // the missing file
		}
	}

	finder.Close();
	return -1;
}

void CSIGDlg::CheckSIGinINI()
{
	CIni ini(GetD2HackitIni());
	CString sLine = ini.GetString(_T("Misc"), _T("Autoload"), _T(""));
	CStringArray a;
	StrSplit(sLine, a, _T(','));

	BOOL bFound = FALSE;
	for (int i = 0; i < a.GetSize(); i++)
	{
		if (a[i].CompareNoCase(_T("sig")) == 0)
			return; // found sig module
	}

	// reaching here means sig is not in autoload list of D2hackit.ini, add it
	ini.WriteString(_T("Misc"), _T("Autoload"), sLine + _T(",sig"));
}

void CSIGDlg::OnHelpHelp() 
{
	// TODO: Add your command handler code here
	AfxGetApp()->WinHelp(0, 0);
}

void CSIGDlg::OnHelpAbout() 
{
	// TODO: Add your command handler code here
	CAboutDlg dlg;
	dlg.DoModal();
}

void CSIGDlg::EnableBossMode(BOOL bEnable)
{
	if (m_pActiveWnd != NULL)
		m_pActiveWnd->EndDialog(IDCANCEL);

	if (bEnable)
	{
		//m_cfg.ShowD2Wnd(SW_MINIMIZE);
		::ShowWindow(m_cfg.FindD2HWnd(), SW_HIDE);
		m_trayIcon.MinimiseToTray(this);
		//m_trayIcon.AddIcon();
	}
	else
	{
		RestoreD2Window();
		//m_cfg.ShowD2Wnd(SW_SHOWNOACTIVATE);
		//m_cfg.ShowD2Wnd(SW_MINIMIZE);
		m_trayIcon.MaximiseFromTray(this);
		this->SetForegroundWindow();
		//m_trayIcon.RemoveIcon();
	}

	m_bBossModeEnabled = bEnable;
}

LRESULT CSIGDlg::OnTrayNotification(WPARAM wParam, LPARAM lParam)
{
	UINT nMsg = (UINT)lParam;
	if (nMsg == WM_LBUTTONDOWN)
	{
		EnableBossMode(!m_bBossModeEnabled);
	}	
	else if (nMsg == WM_RBUTTONUP)
	{
		// pop up menu
		CMenu menu;
		CMenu* popUp;
		CPoint point;

		::GetCursorPos(&point);
		if (!menu.LoadMenu(IDR_POPUP))
			return (LRESULT)1;
	
		popUp = menu.GetSubMenu(0); // Chinese menu

		if (popUp)
		{
			HWND hWnd = this->GetSafeHwnd();
			::SetForegroundWindow(hWnd);
			PopupMenu(popUp, this, TPM_LEFTALIGN, point.x, point.y);
			::PostMessage(hWnd, WM_NULL, 0, 0);
		}
	}
	else
	{
		// do nothing
	}

	return (LRESULT)0;
}

void CSIGDlg::OnPopupRestore() 
{
	// TODO: Add your command handler code here
	this->ShowWindow(SW_MINIMIZE);
	EnableBossMode(FALSE);	
}

void CSIGDlg::OnUpdateHelpHelp(CCmdUI* pCmdUI) 
{
	// TODO: Add your command update UI handler code here
	pCmdUI->SetCheck(1);
}

void CSIGDlg::SIGPause(BOOL bPaused)
{
	if (!m_bStarted)
		return;

	m_bPaused = bPaused;
	m_mem.SetLong(bPaused ? FLAG_THISAPP : 0, MEM_SIG_PAUSE);
	
	if (bPaused)
	{
		m_wndStart.SetIcon(IDI_START_ACTIVATE, IDI_START_DEACTIVATE);		
		m_wndLabel.FlashText(TRUE);
		m_wndStart.SetTooltipText(_T("����"));
		m_wndLabel.SetText(_T("��ͣ"));
	}
	else
	{
		m_wndStart.SetIcon(IDI_PAUSE_ACTIVATE, IDI_PAUSE_DEACTIVATE);
		m_wndLabel.FlashText(FALSE);		
		m_wndStart.SetTooltipText(_T("��ͣ"));
		m_wndLabel.SetText(m_sOrigInfo);
	}
}

void CSIGDlg::OnCancel() 
{
	// TODO: Add extra cleanup here
	if (m_bStarted)
	{
		if (MessageBox(_T("�������������У�ȷ��Ҫ�˳���"), _T("�رճ���"), MB_YESNO | MB_ICONQUESTION) != IDYES)
			return;
	}
	CDialog::OnCancel();
/*
	CExitDlg dlg;
	dlg.Initialize(m_cfg.m_bCloseD2, m_cfg.m_bCloseGUI);

	if (dlg.DoModal() == IDOK)
	{
		m_cfg.m_bCloseD2 = dlg.IsCloseDiabloII();
		m_cfg.m_bCloseGUI = dlg.IsCloseGUI();
		m_cfg.SaveExitInfo(GetSigIni());

		if (dlg.IsCloseDiabloII())
			m_cfg.CloseDiablo2();

		if (dlg.IsCloseGUI())
		{
			CDialog::OnCancel();
		}
	}
	*/
}

CString CSIGDlg::GetCharClassName(DWORD dwClass, BOOL bLongName)
{
	LPCTSTR CHARCLASSES[] = { _T("Amazon"), 
						  _T("Sorceress"),
						  _T("Necromancer"),
						  _T("Paladin"),
						  _T("Barbarian"),
						  _T("Druid"),
						  _T("Assassin"),
						  NULL};
	CString str;

	if (dwClass < 7)
	{
		str = CHARCLASSES[dwClass];
		if (!bLongName)
			str = str.Left(3);

		if (str == _T("Ass"))
			str = _T("Asn");
	}

	return str;
}

void CSIGDlg::OnGameLeave(BOOL bRecord)
{
	m_wndLC.TurnOn(FALSE);
	m_wndHPManaBar.Stop();
	KillTimer(ID_TMR_PLAYER);
	KillTimer(ID_TMR_INGAMETIME);

	if (bRecord)
		m_stats.RecordGameLeave();

	m_bGameJoined = FALSE;

	KillTimer(ID_TMR_ACTIVITY);	
	m_wndCharName.SetText(_T(""));	

	if (m_bStarted)
	{
		MoveOn(ST_CHECK_EXIT_GAME);
		//m_nCurState = ST_CHECK_EXIT_GAME;
		//m_ticker.ResetTicker(m_cfg.m_dwExitDelay);
	}	
}

void CSIGDlg::PlayerMoving()
{
	//m_wndLocation.SetIcon(m_bMovingOn ? IDI_LIGHT_LC_YELLOW : IDI_LIGHT_LC_GRAY);
	m_bMovingOn = !m_bMovingOn;
}

void CSIGDlg::PrepareToCloseD2(DWORD dwWait)
{
	m_spy.SetD2Wnd(NULL);
	OnGameLeave();
	m_bStartOver = FALSE;
	m_nCurState = ST_CLOSE_D2;
	m_dwCloseD2Fail = 0;
	m_ticker.ResetTicker(dwWait);
}

void CSIGDlg::OnMulemode() 
{
	// TODO: Add your control notification handler code here
	if (m_bStarted)
	{
		m_wndMuleMode.SetCheck(!m_wndMuleMode.GetCheck());
		MessageBox(_T("SIG��Ϸ��ģ�����������У������޷����û��ֹMuleģʽ��\n\n���ȵ��\"ֹͣ\"��ť��Ȼ��������û��ֹMuleģʽ��"), _T("Mule Mode"), MB_ICONEXCLAMATION);
	}
	else
	{
		UpdateData(TRUE);
		if (m_bMuleModeEnabled)
		{
			m_wndMuleMode.SetIcon(IDI_MULEMODE_CHECKED);
			m_wndMuleMode.SetTooltipText(_T("Muleģʽ(������)"));
		}
		else
		{
			m_wndMuleMode.SetIcon(IDI_MULEMODE_ACTIVATE, IDI_MULEMODE_DEACTIVATE);
			m_wndMuleMode.SetTooltipText(_T("Muleģʽ(δ����)"));
		}		
	}	
}

BOOL CSIGDlg::CreateSharedMem()
{
	m_sMemoryID.Format(_T("%s[%s]"), CreateFakeGUIDStr(), g_sAppDir);
	m_sMemoryID.Replace(_T("\\"), _T("/"));
	CIni ini(GetMemIDFile());
	const BOOL bRes = m_mem.Create(m_sMemoryID, MEM_ALLOC_SIZE);

	if (bRes)
	{
		m_mem.EraseAll();
		m_mem.SetLong((long)GetSafeHwnd(), MEM_SIG_HWND);
		m_mem.SetLong(FLAG_THISAPP, MEM_SIG_STOP);
	}
	else
	{
		m_sMemoryID = _T("");
	}
	
	ini.WriteString(_T("Default"), _T("Memory ID"), m_sMemoryID);
	return bRes;
}

void CSIGDlg::MoveOn(int nNewState, DWORD dwDelay)
{
	m_nCurState = nNewState;
	m_ticker.ResetTicker(dwDelay);
}

void CSIGDlg::MoveD2WndIn()
{
	KillTimer(ID_TMR_FORCED2ONTOP);

	HWND hD2Wnd = m_cfg.FindD2HWnd();

	if (hD2Wnd == NULL)
		return;	

	if (::IsWindowVisible(hD2Wnd) && !::IsIconic(hD2Wnd))
	{
		CRect rect;
		::GetWindowRect(hD2Wnd, &rect);
		if (rect.Width() > 500 && rect.Height() > 400)
			return;
	}
	
	m_bPlacementChanged = FALSE;
	::ShowWindow(hD2Wnd, SW_MINIMIZE);
	::SetWindowPlacement(hD2Wnd, &m_wp);
	::ShowWindow(hD2Wnd, m_bBossModeEnabled ? SW_HIDE : SW_MINIMIZE);
	m_wndShowD2.EnableWindow(TRUE);
}

void CSIGDlg::MoveD2WndOut()
{
	HWND hD2Wnd = m_cfg.FindD2HWnd();

	if (hD2Wnd == NULL || (::IsWindowVisible(hD2Wnd) && !::IsIconic(hD2Wnd)))
		return;
	
	m_bPlacementChanged = TRUE;
	WINDOWPLACEMENT wp;
	memcpy(&wp, &m_wp, sizeof(WINDOWPLACEMENT));
	wp.rcNormalPosition.left = 800;
	wp.rcNormalPosition.top = 3000;
	wp.rcNormalPosition.right = 801;
	wp.rcNormalPosition.bottom = 3001;
	::SetWindowPlacement(hD2Wnd, &wp);

	if (::IsIconic(hD2Wnd) || !::IsWindowVisible(hD2Wnd))
	{
		::ShowWindow(hD2Wnd, SW_SHOWNOACTIVATE);				
		::SetWindowPos(hD2Wnd, HWND_BOTTOM, 0, 0, 0, 0, SWP_NOMOVE | SWP_NOSIZE | SWP_NOACTIVATE);
	}

	SetTimer(ID_TMR_FORCED2ONTOP, 100, NULL);
}

void CSIGDlg::MuleEnd()
{
	Terminate();
	m_cfg.CloseDiablo2();
	ShowInfo(_T("Muleģʽ���н�����"));
}

void CSIGDlg::OnStatisticsViewdata() 
{
	// TODO: Add your command handler code here
	CStatisticsDlg dlg(&m_stats); 
	dlg.DoModal();
}

void CSIGDlg::OnStatisticsViewlog() 
{
	// TODO: Add your command handler code here
	m_logger.Flush();
	::ShellExecute(NULL, _T("open"), GetLogFilePath(), NULL, g_sAppDir, SW_SHOW);
}

void CSIGDlg::OnKilld2() 
{
	// TODO: Add your control notification handler code here
	if (m_bStarted)
	{
		if (MessageBox(_T("�������������У�ȷ�Ž������в�ɱ��Diablo II�����𣿣�"), _T("�ر�Diablo II"), MB_YESNO | MB_ICONQUESTION) != IDYES)
			return;
	}

	Terminate();
	m_cfg.CloseDiablo2();
}

BOOL CAboutDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	// TODO: Add extra initialization here
	m_wndAbin.SetURL(_T("mailto:abinn32@yahoo.com"));
	m_wndHomepage.SetURL(_T("http://www.wxjindu.com/abin/"));
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CSIGDlg::DoConfig(int nActivePage)
{
	g_nActivePage = -1;
	CPropertySheet sheet(_T("S.I.G.��������"));
	sheet.m_psh.dwFlags |= PSH_NOAPPLYNOW;

	CGeneralPage pageGeneral;
	CLoginPage pageLogin;
	CCreatePage pageCreate;
	CDelayPage pageDelay;
	CMulePage pageMule;
	CHackPage pageHack;
	CD2JspPage pageD2Jsp;

	pageGeneral.LoadSettings(m_cfg);
	pageLogin.LoadSettings(m_cfg);
	pageCreate.LoadSettings(m_cfg);
	pageDelay.LoadSettings(m_cfg);
	pageMule.LoadSettings(m_cfg);
	pageD2Jsp.LoadSettings(m_cfg);
	pageHack.LoadSettings(m_cfg);

	sheet.AddPage(&pageGeneral);
	sheet.AddPage(&pageLogin);
	sheet.AddPage(&pageCreate);
	sheet.AddPage(&pageDelay);
	sheet.AddPage(&pageMule);
	sheet.AddPage(&pageHack);
	sheet.AddPage(&pageD2Jsp);

	sheet.SetActivePage(nActivePage);

	if (sheet.DoModal() == IDOK)
	{
		pageGeneral.ApplySettings(m_cfg);
		pageLogin.ApplySettings(m_cfg);
		pageCreate.ApplySettings(m_cfg);
		pageDelay.ApplySettings(m_cfg);
		pageMule.ApplySettings(m_cfg);
		pageD2Jsp.ApplySettings(m_cfg);
		pageHack.ApplySettings(m_cfg);

		m_mem.SetBool(pageCreate.IsAntiIdleEnabled() , MEM_SIG_ANTIIDLE);
		m_cfg.SaveData(GetSigIni(), GetD2HackitIni(), GetAccountsIni(), GetD2GatewayIni(), GetDictionary());

		//m_wndMuleMode.EnableWindow(!m_cfg.m_bUseD2JSP);

		if (m_bStarted)
		{
			MessageBox(_T("����SIG��Ϸ�ڿ���ģ���Ѿ��������У�ĳЩ���õĸ��Ŀ���Ҫ����һ������ʱ�Ż���Ч��"), _T("Settings Changed"), MB_ICONINFORMATION);
		}
	}

	g_nActivePage = -1;
}

void CSIGDlg::OnSize(UINT nType, int cx, int cy) 
{
	if (nType == SIZE_MINIMIZED)
	{
		EnableBossMode(TRUE);
	}
	else
	{
		CDialog::OnSize(nType, cx, cy);
	}
	
	// TODO: Add your message handler code here
	
}

void CSIGDlg::OnPopupStartpauseresume() 
{
	// TODO: Add your command handler code here
	OnStart();
}

void CSIGDlg::OnUpdatePopupStartpauseresume(CCmdUI* pCmdUI) 
{
	// TODO: Add your command update UI handler code here
	if (!m_bStarted)
	{
		pCmdUI->SetText(_T("��ʼ(&S)"));
	}
	else
	{
		if (m_bPaused)
			pCmdUI->SetText(_T("����(&U)"));
		else
			pCmdUI->SetText(_T("��ͣ(&P)"));
	}
}

void CSIGDlg::OnPopupStop() 
{
	// TODO: Add your command handler code here
	OnStop();
}

void CSIGDlg::OnUpdatePopupStop(CCmdUI* pCmdUI) 
{
	// TODO: Add your command update UI handler code here
	pCmdUI->Enable(m_bStarted);
}

void CSIGDlg::OnPopupKilld2() 
{
	// TODO: Add your command handler code here
	OnKilld2();
}

void CSIGDlg::OnUpdatePopupKilld2(CCmdUI* pCmdUI) 
{
	// TODO: Add your command update UI handler code here
	pCmdUI->Enable(m_cfg.FindD2HWnd() != NULL);
}

BOOL CSIGDlg::CheckAndCloseErrors()
{
	int nErr = 0;

	HWND hWnd = NULL;
		
	for (int i = 0; ERROR_MSG[i] != NULL; i++)
	{
		hWnd = ::FindWindow(NULL, ERROR_MSG[i]);
		if (hWnd != NULL)
		{
			DWORD dwID = 0;
			GetWindowThreadProcessId(hWnd, &dwID);
			if (dwID > 0 && dwID == m_cfg.GetD2ProcID())
			{
				nErr++;
				COSInfo::CloseWnd(hWnd, 5000);
				COSInfo::KillProcess(dwID);
			}			
		}
	}

	return nErr > 0;
}

void CSIGDlg::CleanLogs()
{
	CIni ini(GetSigIni());
	const BOOL bClean = ini.GetBool(_T("General"), _T("Clean Log"), TRUE);
	if (bClean)
	{
		ShowInfo(_T("�������log�ļ�"));
		CFileFind ff;
		BOOL b = ff.FindFile(GetLogFileDir() + _T("\\*.txt"));

		while (b)
		{
			b = ff.FindNextFile();
			::DeleteFile(ff.GetFilePath());
		}
	}

	ini.WriteBool(_T("General"), _T("Clean Log"), bClean);
}

void CSIGDlg::ResetSig()
{
	Terminate();
	m_cfg.CloseDiablo2();
	EndDialog(IDCANCEL);
	::ShellExecute(NULL, _T("open"), _T("SigRst.exe"), _T("60"), g_sAppDir, SW_SHOW);
}

void CSIGDlg::OnViewfile() 
{
	// TODO: Add your control notification handler code here
	if (m_sViewFile.IsEmpty())
	{
		MessageBox(_T("����ѡ��Ҫ�鿴��bot��¼�ļ���"));
		OnSelfile();
	}
	else
	{
		// get the work directory
		CString sDir;
		const int n = m_sViewFile.ReverseFind(_T('\\'));
		if (n >= 1)
		{
			sDir = m_sViewFile.Left(n);
		}
	
		if (m_sViewFile.Right(3).CompareNoCase(_T("log")) == 0
			|| m_sViewFile.Right(3).CompareNoCase(_T("dat")) == 0
			|| m_sViewFile.Right(3).CompareNoCase(_T("pwn")) == 0
			|| m_sViewFile.Right(3).CompareNoCase(_T("d2j")) == 0
			|| m_sViewFile.Right(3).CompareNoCase(_T("d2l")) == 0
			|| m_sViewFile.Right(3).CompareNoCase(_T("cfg")) == 0)
		{
			::ShellExecute(NULL, _T("open"), _T("notepad.exe"), m_sViewFile, NULL, SW_SHOW);
		}
		else
		{
			::ShellExecute(NULL, _T("open"), m_sViewFile, NULL, (sDir.IsEmpty() ? NULL : (LPCTSTR)sDir) , SW_SHOW);
		}
	}
}

void CSIGDlg::OnSelfile() 
{
	// TODO: Add your control notification handler code here

	CFileDialog dlg(TRUE,
					NULL,
					m_sViewFile,
					0,
					_T("Bot Logs (*.txt; *.log; *.ini; *.htm; *.html)|*.txt;*.log;*.ini;*.htm;*.html|All Files(*.*)|*.*||"),
					NULL);
	if (dlg.DoModal() == IDOK)
	{
		m_sViewFile = dlg.GetPathName();
		CIni ini(GetSigIni());
		ini.WriteString(_T("General"), _T("View File"), m_sViewFile);
	}
}

int CSIGDlg::GetNextGameStats(CString &sName, CString &sPwd)
{
	LPTSTR lpName = sName.GetBuffer(17);
	LPTSTR lpPwd = sPwd.GetBuffer(17);
	m_mem.GetString(lpName, 16, MEM_SIG_NEXTNAME);
	m_mem.GetString(lpPwd, 16, MEM_SIG_NEXTPWD);
	sName.ReleaseBuffer();
	sPwd.ReleaseBuffer();
	sName.TrimLeft();
	sName.TrimRight();
	sPwd.TrimLeft();
	sPwd.TrimRight();

	if (m_mem.GetLong(MEM_SIG_NEXTGAME) == 1) // join
	{
		return sName.GetLength() > 0 ? NEXT_JOIN_SPEC : NEXT_CREATE_AUTO;
	}
	else // create
	{
		return sName.GetLength() > 0 ? NEXT_CREATE_SPEC : NEXT_CREATE_AUTO;
	}
}

void CSIGDlg::CleanNextGameStats()
{
	// clean the "next game" stats that were specified by user in previous game
	m_mem.SetLong(0, MEM_SIG_NEXTGAME);
	m_mem.SetString(_T(""), MEM_SIG_NEXTNAME);
	m_mem.SetString(_T(""), MEM_SIG_NEXTPWD);
}

void CSIGDlg::OnGameJoin(BYTE iCharClass)
{
	m_dwJoinFail = 0;
	if (m_bStarted)
	{
		SetTimer(ID_TMR_PLAYER, 500, NULL);
		m_playerStats.SetD2Wnd(m_cfg.FindD2HWnd());		
		CleanNextGameStats();
		m_stats.RecordNewGame();
		//SetTimer(ID_TMR_PLAYER, 100, NULL);
		SetTimer(ID_TMR_INGAMETIME, 1000, NULL);
		DWORD dwStay = m_cfg.m_bInGameLimit ? m_cfg.m_dwInGameLimitLen : 0;
		
		if (m_bMuleModeEnabled)
			dwStay = m_cfg.m_nMuleCharStayLength;

		m_dwPrevLocation = 0xffffffff;
		m_bGameJoined = TRUE;
		m_bMovingOn = TRUE;
		MoveOn(ST_IN_GAME);
		m_wndLC.SetColor(LIGHT_GREEN);
		m_wndLC.TurnOn(TRUE);

		CString str, sPlayerName, sClass;
		m_spy.UpdatePlayerData();
		sPlayerName = m_spy.GetPlayer()->szPlayeName;
		sClass = GetCharClassName(m_spy.GetPlayer()->iCharClass, TRUE);
		str.Format(_T("%s %s"), GetCharClassName(m_spy.GetPlayer()->iCharClass, FALSE), m_spy.GetPlayer()->szPlayeName);
		m_wndCharName.SetText(str);

		CString sMsg;

		if (!m_cfg.m_bInGameLimit)
			sMsg = _T("������Ϸ�ɹ�");
		else
			sMsg.Format(_T("������Ϸ�ɹ�(ͣ��ʱ��%d��)"), m_cfg.m_dwInGameLimitLen);
		ShowInfo(sMsg);
		
		m_wndHPManaBar.SetPlayerNameClass(sPlayerName, sClass);
		m_wndHPManaBar.Start(500);

		if (!m_bUseD2JSP)
			SetTimer(ID_TMR_ACTIVITY, MAX_INACTIVITY, NULL);
	}
}

void CSIGDlg::CheckGameInOut()
{
	BOOL INGAME = m_spy.IsInGame();
	if (m_bGameJoined && !INGAME)
		OnGameLeave();
	else if (!m_bGameJoined && INGAME)
		OnGameJoin(0);
}

void CSIGDlg::TmrProc_Player()
{
	if (!m_bGameJoined)
		return;
	
	if (!m_spy.UpdatePlayerData())
		return;

	switch (m_spy.GetPlayer()->dwPlayerLocation)
	{
	case 0x05:
		m_wndLC.SetColor(LIGHT_GREEN);
		m_wndLC.Flash(0);
		break;

	case 0x01:
		m_wndLC.SetColor(LIGHT_RED);
		m_wndLC.Flash(0);
		break;

	default:
		m_wndLC.SetColor(LIGHT_YELLOW);
		m_wndLC.Flash(-1);
		break;
	}
}

void CSIGDlg::HideLightsForD2JSP()
{
	m_wndD2JSP.ShowWindow(m_bUseD2JSP);
	m_wndHB.ShowWindow(!m_bUseD2JSP);
	m_wndRV.ShowWindow(!m_bUseD2JSP);
	m_wndST.ShowWindow(!m_bUseD2JSP);
	m_wndAI.ShowWindow(!m_bUseD2JSP);
}

BOOL CSIGDlg::CheckD2LoaderMd5()
{
	CString sErr;

	// check for sigpack.dll
	if (!CheckFileMd5(g_sAppDir + _T("\\SigPack.dll"), MD5_SIGPACK_DLL))
	{		
		sErr.Format(_T("�ļ�MD5У��ʧ�ܣ�\n\"%s\"!\n���������غͰ�װSIG��"), g_sAppDir + _T("\\SigPack.dll"));
		MessageBox(sErr, _T("�ļ���"), MB_ICONEXCLAMATION);
		return FALSE;
	}

	CResPack rp;
	rp.OpenPackage(g_sAppDir + _T("\\SigPack.dll"), FALSE); // this will never fail since we passed md5 check

	// check for loaderex.exe
	if (!m_bUseD2JSP && !CheckFileMd5(g_sAppDir + _T("\\LoaderEx.dll"), MD5_LOADEREX_DLL))
	{
		if (!UpdateFile(rp, LOADEREX_DLL, g_sAppDir + _T("\\LoaderEx.dll")))
			return FALSE;
	}

	// check for d2hackit.dll
	//if (!m_bUseD2JSP && !CheckFileMd5(GetHackDir() + _T("\\D2HackIt.dll"), MD5_D2HACKIT_DLL))
	//{
	//	if (!UpdateFile(rp, D2HACKIT_DLL, GetHackDir() + _T("\\D2HackIt.dll")))
	//		return FALSE;
	//}

	// check for d2 executable
	if (CheckFileMd5(m_cfg.GetD2Executable(), MD5_D2_EXE))
		return TRUE;

	sErr.Format(_T("�ļ�MD5У��ʧ�ܣ�\n\n\"%s\"\n\nSIG��Ҫ������D2Loader�ļ�����ΪD2Loader 1.10b14�������С��Ƿ�Ҫ�������£�"), m_cfg.GetD2Executable());
	if (MessageBox(sErr, _T("�ļ�����"), MB_ICONQUESTION | MB_YESNO) != IDYES)
	{
		return MessageBox(_T("��ѡ���˲������ļ����£�SIG�����޷��������С�\n�Ƿ�������У�"), _T("SIG����"), MB_YESNO | MB_ICONQUESTION | MB_DEFBUTTON2) == IDYES;
	}

	// back up first
	CString sBackup = m_cfg.GetD2Path() + _T("\\") + m_cfg.GetD2ExeFileTitle() + _T(".bak");
	::DeleteFile(sBackup);
	::CopyFile(m_cfg.GetD2Executable(), sBackup, FALSE);

	if (!UpdateFile(rp, D2_EXE, m_cfg.GetD2Executable()))
	{
		MessageBox(_T("�ļ�����ʧ�ܣ�ԭ�ļ���������ʹ�á�"), _T("�ļ�����"), MB_ICONEXCLAMATION);
		return FALSE;
	}
		
	sErr.Format(_T("�ļ����³ɹ���ԭ�����ļ��ѱ�����Ϊ\"%s\"��"), m_cfg.GetD2ExeFileTitle() + _T(".bak"));
	MessageBox(sErr, _T("�ļ�����"), MB_ICONINFORMATION);
	return TRUE;
}

BOOL CSIGDlg::CheckFileMd5(LPCTSTR lpszFilePath, LPCTSTR lpszMd5)
{
	if (lpszFilePath == NULL || lpszFilePath[0] == _T('\0') || lpszMd5 == NULL)
		return FALSE;

	return CMD5Checksum::GetFileMD5(lpszFilePath).CompareNoCase(lpszMd5) == 0;
}

BOOL CSIGDlg::UpdateFile(CResPack &rp, UINT nResID, LPCTSTR lpszFilePath)
{
	ASSERT(lpszFilePath);
	if (rp.SaveResToFile(nResID, lpszFilePath))
		return TRUE;

	CString sErr;
	sErr.Format(_T("�ļ�MD5У��ʧ�ܣ�\n\"%s\"!\n��ر����е�Ӧ�ó�������ԣ����������Ȼ���ڣ��������غͰ�װSIG��"), lpszFilePath);
	MessageBox(sErr, _T("�ļ�ϵͳ����"), MB_ICONEXCLAMATION);
	return FALSE;
}

void CSIGDlg::RestoreD2Window(BOOL bBringToTop)
{
	KillTimer(ID_TMR_FORCED2ONTOP);

	HWND hD2Wnd = m_cfg.FindD2HWnd();
	if (hD2Wnd == NULL)
		return;
	
	if (m_bPlacementChanged)
	{
		::SetWindowPlacement(hD2Wnd, &m_wp);
		m_bPlacementChanged = FALSE;
	}

	if (bBringToTop)
	{
		::ShowWindow(hD2Wnd, SW_MINIMIZE);
		::ShowWindow(hD2Wnd, SW_RESTORE);
		::SetWindowPos(hD2Wnd, HWND_TOP, 0, 0, 0, 0, SWP_NOMOVE | SWP_NOSIZE);
	}
	else
	{
		::ShowWindow(hD2Wnd, SW_SHOWNOACTIVATE);
		::SetWindowPos(hD2Wnd, HWND_BOTTOM, 0, 0, 0, 0, SWP_NOMOVE | SWP_NOSIZE | SWP_NOACTIVATE);
	}
	
}

void CSIGDlg::OnShowd2Active() 
{
	// TODO: Add your command handler code here
	RestoreD2Window(TRUE);
}

void CSIGDlg::OnShowd2Bkgnd() 
{
	// TODO: Add your command handler code here
	RestoreD2Window(FALSE);
}

void CSIGDlg::OnUpdateShowd2Active(CCmdUI* pCmdUI) 
{
	// TODO: Add your command update UI handler code here
	pCmdUI->Enable(m_cfg.FindD2HWnd() != NULL);
}

BOOL CSIGDlg::SetForegroundWindowEx(HWND hwnd) const
{
	if (hwnd == NULL || !::IsWindow(hwnd))
		return FALSE;
	
	HWND hfg = ::GetForegroundWindow();
	if (hfg == hwnd)
		return TRUE;

	DWORD dwTargetProc = 0;
	
	DWORD dwTargetThread = ::GetWindowThreadProcessId(hwnd, &dwTargetProc);
	if (dwTargetProc == ::GetCurrentProcessId())
		return ::SetForegroundWindow(hwnd);

	DWORD dwThisThread = ::GetWindowThreadProcessId(m_hWnd, NULL);
	if (!::AttachThreadInput(dwThisThread, dwTargetThread, TRUE))
		return FALSE;

	::BringWindowToTop(hwnd); 
	::SetForegroundWindow(hwnd);	
	//::SetFocus(hwnd);
	::AttachThreadInput(dwThisThread, dwTargetThread, FALSE);
	
	return TRUE;

	/*
	GetWindowThreadProcessID(GetForegroundWindow,nil);
	ThisThreadID := GetWindowThreadPRocessId(hwnd,nil);
	if AttachThreadInput(ThisThreadID, ForegroundThreadID, true) then
	begin
	BringWindowToTop(hwnd); 
	SetForegroundWindow(hwnd);
	AttachThreadInput(ThisThreadID, ForegroundThreadID, false);
	*/
}

UINT CSIGDlg::CheckAndKillxD2() const
{
	CArray<HWND, HWND> aList;
	::EnumWindows(EnumWindowsProc, (LPARAM)&aList);
	if ((UINT)aList.GetSize() + 1 > m_cfg.m_nMaxD2Proc)
	{
		for (int i = 0; i < aList.GetSize(); i++)
		{
			DWORD dwProcID = COSInfo::GetProcessID(aList[i]);
			COSInfo::CloseWnd(aList[i], 5000);
			COSInfo::KillProcess(dwProcID);
		}
	}

	return (UINT)aList.GetSize();
}

BOOL CSIGDlg::EnumWindowsProc(HWND hwnd, LPARAM lParam)
{
	CArray<HWND, HWND>* pArray = (CArray<HWND, HWND>*)lParam;
	if (pArray == NULL)
		return FALSE;

	TCHAR szClass[256] = _T("");
	::GetClassName(hwnd, szClass, 255);
	if (_tcsicmp(szClass, _T("Diablo II")) == 0)
		pArray->Add(hwnd);
	return TRUE;
}

void CSIGDlg::OnLButtonDblClk(UINT nFlags, CPoint point) 
{
	// TODO: Add your message handler code here and/or call default
	CDialog::OnLButtonDblClk(nFlags, point);
}
