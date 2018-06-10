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
	m_bAutoStart = FALSE;
	m_AutoCloseDate = COleDateTime::GetCurrentTime();
	m_bCloseWindows = FALSE;
	m_dwAutoCloseLen = 0;
	m_nAutoCloseType = CLOSE_NONE;
	m_AutoCloseTime = COleDateTime::GetCurrentTime();
	m_sParameters = _T("");
	m_nMaxD2Proc = 0;
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
	DDX_Control(pDX, IDC_MAXD2PROC, m_wndMaxD2Proc);
	DDX_Control(pDX, IDC_PARAMETERS, m_wndParameters);
	DDX_Control(pDX, IDC_D2EXEPATH, m_wndD2ExePath);
	DDX_Control(pDX, IDC_AUTOCLOSELEN, m_wndAutoCloseLen);
	DDX_Check(pDX, IDC_AUTOSTART, m_bAutoStart);
	DDX_DateTimeCtrl(pDX, IDC_DATETIMEPICKER1, m_AutoCloseDate);
	DDX_Check(pDX, IDC_SHUTDOWNWINDOWS, m_bCloseWindows);
	DDX_Text(pDX, IDC_AUTOCLOSELEN, m_dwAutoCloseLen);
	DDV_MinMaxDWord(pDX, m_dwAutoCloseLen, 1, 4294967295);
	DDX_Radio(pDX, IDC_AUTOCLOSE_NONE, m_nAutoCloseType);
	DDX_DateTimeCtrl(pDX, IDC_DATETIMEPICKER2, m_AutoCloseTime);
	DDX_Text(pDX, IDC_PARAMETERS, m_sParameters);
	DDX_Text(pDX, IDC_MAXD2PROC, m_nMaxD2Proc);
	DDV_MinMaxUInt(pDX, m_nMaxD2Proc, 1, 16);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CGeneralPage, CPropertyPage)
	//{{AFX_MSG_MAP(CGeneralPage)
	ON_BN_CLICKED(IDC_AUTOCLOSE_NONE, OnAutoclose)
	ON_BN_CLICKED(IDC_AUTOCLOSE_BYDATE, OnAutoclose)
	ON_BN_CLICKED(IDC_AUTOCLOSE_BYLEN, OnAutoclose)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CGeneralPage message handlers

void CGeneralPage::LoadSettings(const CD2ConfigData &data)
{
	m_sD2Executable = data.m_sD2Executable;
	m_sParameters = data.m_sParamLine;
	m_nMaxD2Proc = data.m_nMaxD2Proc;
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
		data.m_sParamLine = m_sParameters;
		data.m_nMaxD2Proc = m_nMaxD2Proc;
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
	m_wndD2ExePath.SetDialogTitle(_T("Diablo II可执行文件路径"));
	m_wndD2ExePath.SetTooltipText(_T("浏览"));
	m_wndD2ExePath.SetDefExt(_T("exe"));
	m_wndD2ExePath.SetFileFlags(OFN_PATHMUSTEXIST | OFN_FILEMUSTEXIST);
	m_wndD2ExePath.SetFilter(_T("Program Files(*.exe)|*.exe||)"));
	m_wndD2ExePath.SetPathName(m_sD2Executable);

	CheckStats();	

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CGeneralPage::CheckStats()
{
	GetDlgItem(IDC_DATETIMEPICKER1)->EnableWindow(m_nAutoCloseType == CLOSE_BY_DATE);
	GetDlgItem(IDC_DATETIMEPICKER2)->EnableWindow(m_nAutoCloseType == CLOSE_BY_DATE);
	GetDlgItem(IDC_AUTOCLOSELEN)->EnableWindow(m_nAutoCloseType == CLOSE_BY_LEN);
	GetDlgItem(IDC_SHUTDOWNWINDOWS)->EnableWindow(m_nAutoCloseType != CLOSE_NONE);
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

void CGeneralPage::OnOK() 
{
	// TODO: Add your specialized code here and/or call the base class
	if (!UpdateData(TRUE))
		return;	

	m_sParameters.TrimLeft();
	m_sParameters.TrimRight();
	if (m_sParameters.IsEmpty())
		m_sParameters = DEF_PARAM;
	m_sD2Executable = m_wndD2ExePath.GetPathName();
	CPropertyPage::OnOK();
}
