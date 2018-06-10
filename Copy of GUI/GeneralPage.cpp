// GeneralPage.cpp : implementation file
//

#include "stdafx.h"
#include "SIG.h"
#include "GeneralPage.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CGeneralPage property page

IMPLEMENT_DYNCREATE(CGeneralPage, CPropertyPage)

CGeneralPage::CGeneralPage() : CPropertyPage(CGeneralPage::IDD)
{
	//{{AFX_DATA_INIT(CGeneralPage)
	m_sD2Executable = _T("");
	m_bLQ = FALSE;
	m_bNS = FALSE;
	m_bRes800 = TRUE;
	m_bUseLocale = FALSE;
	m_nLocaleType = 0;
	m_bUseMpq = FALSE;
	m_sMpqFile = _T("");
	m_bAutoStart = FALSE;
	m_AutoCloseDate = COleDateTime::GetCurrentTime();
	m_bCloseWindows = FALSE;
	m_dwAutoCloseLen = 0;
	m_nAutoCloseType = CLOSE_NONE;
	m_AutoCloseTime = COleDateTime::GetCurrentTime();
	m_bNoPlugin = TRUE;
	m_bPdir = FALSE;
	m_sPdirPath = _T("plugin");
	//}}AFX_DATA_INIT

	m_bPageInitialized = FALSE;
}

CGeneralPage::~CGeneralPage()
{
}

void CGeneralPage::DoDataExchange(CDataExchange* pDX)
{
	CPropertyPage::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CGeneralPage)
	DDX_Control(pDX, IDC_NOPLUGIN, m_wndNoPlugin);
	DDX_Control(pDX, IDC_PDIRPATH, m_wndPdirPath);
	DDX_Control(pDX, IDC_AUTOCLOSELEN, m_wndAutoCloseLen);
	DDX_Control(pDX, IDC_MPGFILE, m_wndMpqFile);
	DDX_Control(pDX, IDC_D2EXE, m_wndD2Exe);
	DDX_Control(pDX, IDC_BROWSE, m_wndBrowse1);
	DDX_Text(pDX, IDC_D2EXE, m_sD2Executable);
	DDX_Check(pDX, IDC_LQ, m_bLQ);
	DDX_Check(pDX, IDC_NS, m_bNS);
	DDX_Check(pDX, IDC_RES800, m_bRes800);
	DDX_Check(pDX, IDC_LOCALE, m_bUseLocale);
	DDX_CBIndex(pDX, IDC_LOCALETYPE, m_nLocaleType);
	DDX_Check(pDX, IDC_MPQ, m_bUseMpq);
	DDX_Text(pDX, IDC_MPGFILE, m_sMpqFile);
	DDX_Check(pDX, IDC_AUTOSTART, m_bAutoStart);
	DDX_DateTimeCtrl(pDX, IDC_DATETIMEPICKER1, m_AutoCloseDate);
	DDX_Check(pDX, IDC_SHUTDOWNWINDOWS, m_bCloseWindows);
	DDX_Text(pDX, IDC_AUTOCLOSELEN, m_dwAutoCloseLen);
	DDV_MinMaxDWord(pDX, m_dwAutoCloseLen, 1, 4294967295);
	DDX_Radio(pDX, IDC_AUTOCLOSE_NONE, m_nAutoCloseType);
	DDX_DateTimeCtrl(pDX, IDC_DATETIMEPICKER2, m_AutoCloseTime);
	DDX_Check(pDX, IDC_NOPLUGIN, m_bNoPlugin);
	DDX_Check(pDX, IDC_PDIR, m_bPdir);
	DDX_Text(pDX, IDC_PDIRPATH, m_sPdirPath);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CGeneralPage, CPropertyPage)
	//{{AFX_MSG_MAP(CGeneralPage)
	ON_BN_CLICKED(IDC_BROWSE, OnBrowse1)
	ON_BN_CLICKED(IDC_LOCALE, OnLocale)
	ON_BN_CLICKED(IDC_MPQ, OnMpq)
	ON_BN_CLICKED(IDC_AUTOCLOSE_NONE, OnAutoclose)
	ON_BN_CLICKED(IDC_NOPLUGIN, OnNoplugin)
	ON_BN_CLICKED(IDC_AUTOCLOSE_BYDATE, OnAutoclose)
	ON_BN_CLICKED(IDC_AUTOCLOSE_BYLEN, OnAutoclose)
	ON_BN_CLICKED(IDC_PDIR, OnPdir)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CGeneralPage message handlers

void CGeneralPage::LoadSettings(const CD2ConfigData &data)
{
	m_sD2Executable = data.m_sD2Executable;
	m_bLQ = data.m_bLQ;
	m_bNS = data.m_bNS;
	m_bRes800 = data.m_bRes800;
	m_bUseLocale = data.m_bUseLocale;
	m_nLocaleType = data.m_nLocaleType;
	m_bUseMpq = data.m_bUseMpq;
	m_sMpqFile = data.m_sMpqFile;
	m_bNoPlugin = data.m_bNoPlugin;
	m_bPdir = data.m_bPdir;
	m_sPdirPath = data.m_sPdirPath;

	m_bAutoStart = data.m_bAutoStart;
	m_nAutoCloseType = data.m_nAutoCloseType;
	m_AutoCloseDate =  data.m_autoCloseDate;
	m_AutoCloseTime = data.m_autoCloseDate;
	m_dwAutoCloseLen = data.m_dwAutoCloseLen;
	m_bCloseWindows = data.m_bCloseWindows;

	const COleDateTime now = COleDateTime::GetCurrentTime();
	if (m_AutoCloseDate.GetStatus() == COleDateTime::invalid
		|| m_AutoCloseTime.GetStatus() == COleDateTime::invalid
		|| m_AutoCloseDate <= now
		|| m_AutoCloseTime <= now)
	{
		if (m_nAutoCloseType == CLOSE_BY_DATE)
			m_nAutoCloseType = CLOSE_NONE;

		m_AutoCloseDate = COleDateTime::GetCurrentTime();
		m_AutoCloseTime = COleDateTime::GetCurrentTime();
	}
}

void CGeneralPage::ApplySettings(CD2ConfigData &data) const
{
	if (m_bPageInitialized)
	{
		data.m_sD2Executable = m_sD2Executable;
		data.m_bLQ = m_bLQ;
		data.m_bNS = m_bNS;
		data.m_bRes800 = m_bRes800;
		data.m_bUseLocale = m_bUseLocale;
		data.m_nLocaleType = m_nLocaleType;
		data.m_bUseMpq = m_bUseMpq;
		data.m_sMpqFile = m_sMpqFile;
		data.m_sMpqFile.TrimLeft();
		data.m_sMpqFile.TrimRight();
		data.m_bNoPlugin = m_bNoPlugin;
		data.m_bPdir = m_bPdir;
		data.m_sPdirPath = m_sPdirPath;
		data.m_sPdirPath.TrimLeft();
		data.m_sPdirPath.TrimRight();

		data.m_bAutoStart = m_bAutoStart;
		data.m_nAutoCloseType = m_nAutoCloseType;
		data.m_autoCloseDate.SetDateTime(m_AutoCloseDate.GetYear(),
			m_AutoCloseDate.GetMonth(), 
			m_AutoCloseDate.GetDay(),
			m_AutoCloseTime.GetHour(),
			m_AutoCloseTime.GetMinute(),
			m_AutoCloseTime.GetSecond());
		data.m_dwAutoCloseLen = m_dwAutoCloseLen;
		data.m_bCloseWindows = m_bCloseWindows;
	}
}

BOOL CGeneralPage::OnInitDialog() 
{
	CPropertyPage::OnInitDialog();
	
	// TODO: Add extra initialization here
	
	m_bPageInitialized = TRUE;
	m_hBrowse = AfxGetApp()->LoadIcon(IDI_BROWSE);
	if (m_hBrowse != NULL)
	{
		m_wndBrowse1.SetIcon(m_hBrowse);
	}
	m_wndBrowse1.SetTooltipText(_T("浏览"));	

	CheckStats();	

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CGeneralPage::OnBrowse1() 
{
	// TODO: Add your control notification handler code here
	CFileDialog dlg(TRUE, _T("exe"), NULL, OFN_PATHMUSTEXIST | OFN_FILEMUSTEXIST, _T("Program Files(*.exe)|*.exe||)"));
	if (dlg.DoModal() == IDOK)
	{
		SetDlgItemText(IDC_D2EXE, dlg.GetPathName());
	}
}

void CGeneralPage::OnLocale() 
{
	// TODO: Add your control notification handler code here
	UpdateData(TRUE);
	GetDlgItem(IDC_LOCALETYPE)->EnableWindow(m_bUseLocale);
}

void CGeneralPage::OnMpq() 
{
	// TODO: Add your control notification handler code here
	UpdateData(TRUE);
	m_wndMpqFile.EnableWindow(m_bUseMpq);
}

void CGeneralPage::CheckStats()
{
	GetDlgItem(IDC_LOCALETYPE)->EnableWindow(m_bUseLocale);
	m_wndMpqFile.EnableWindow(m_bUseMpq);
	GetDlgItem(IDC_DATETIMEPICKER1)->EnableWindow(m_nAutoCloseType == CLOSE_BY_DATE);
	GetDlgItem(IDC_DATETIMEPICKER2)->EnableWindow(m_nAutoCloseType == CLOSE_BY_DATE);
	GetDlgItem(IDC_AUTOCLOSELEN)->EnableWindow(m_nAutoCloseType == CLOSE_BY_LEN);
	GetDlgItem(IDC_SHUTDOWNWINDOWS)->EnableWindow(m_nAutoCloseType != CLOSE_NONE);
	GetDlgItem(IDC_PDIR)->EnableWindow(!m_bNoPlugin);
	m_wndPdirPath.EnableWindow(!m_bNoPlugin && m_bPdir);
}

void CGeneralPage::OnAutoclose() 
{
	// TODO: Add your control notification handler code here
	UpdateData(TRUE);
	CheckStats();	
}

BOOL CGeneralPage::OnSetActive() 
{
	// TODO: Add your specialized code here and/or call the base class
	g_nActivePage = PAGE_GENERAL;
	return CPropertyPage::OnSetActive();
}

void CGeneralPage::OnNoplugin() 
{
	// TODO: Add your control notification handler code here
	if (!m_wndNoPlugin.GetCheck())
	{
		// give a warning
		if (MessageBox(_T("禁止\"-noplugin\"参数可能会引起SIG游戏内模块无法正常加载。并且指定的plugin目录内不允许存在\"D2Hackit.dll\"这一文件。\n\n确定要禁止\"-noplugin\"参数吗？"),
			_T("SIG警告"), MB_ICONQUESTION | MB_YESNO | MB_DEFBUTTON2) != IDYES)
		{
			m_wndNoPlugin.SetCheck(1);
			return;
		}
	}

	UpdateData(TRUE);
	GetDlgItem(IDC_PDIR)->EnableWindow(!m_bNoPlugin);
	m_wndPdirPath.EnableWindow(!m_bNoPlugin && m_bPdir);
}

void CGeneralPage::OnPdir() 
{
	// TODO: Add your control notification handler code here
	UpdateData(TRUE);
	m_wndPdirPath.EnableWindow(!m_bNoPlugin && m_bPdir);	
}
