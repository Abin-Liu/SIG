// DelayPage.cpp : implementation file
//

#include "stdafx.h"
#include "SIG.h"
#include "DelayPage.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CDelayPage property page

IMPLEMENT_DYNCREATE(CDelayPage, CPropertyPage)

CDelayPage::CDelayPage() : CPropertyPage(CDelayPage::IDD)
{
	//{{AFX_DATA_INIT(CDelayPage)
	m_dwCharSelDelay = 0;
	m_dwErrorDelay = 0;
	m_dwJoinDelay = 0;
	m_dwLaunchDelay = 0;
	m_dwLoginDelay = 0;
	m_dwCreateBtnDelay = 0;
	m_dwMinConndelay = 0;
	m_dwCreateDelayMin = 0;
	m_dwCreateDelayMax = 0;
	//}}AFX_DATA_INIT

	m_bPageInitialized = FALSE;
	m_pDescDlg = new CDescDlg;
}

CDelayPage::~CDelayPage()
{
}

void CDelayPage::DoDataExchange(CDataExchange* pDX)
{
	CPropertyPage::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDelayPage)
	DDX_Control(pDX, IDC_CREATEDELAYMAX, m_wndCreateDelayMax);
	DDX_Control(pDX, IDC_CREATEDELAYMIN, m_wndCreateDelayMin);
	DDX_Control(pDX, IDC_MINCONN, m_wndMinConnDelay);
	DDX_Control(pDX, IDC_HELPMAXWAIT, m_wndHelpAbs);
	DDX_Control(pDX, IDC_HELPABSWAIT, m_wndHelMax);
	DDX_Control(pDX, IDC_LOGINDELAY, m_wndLogindelay);
	DDX_Control(pDX, IDC_LAUNCHDELAY, m_wndLaunchDelay);
	DDX_Control(pDX, IDC_JOINDELAY, m_wndJoinDelay);
	DDX_Control(pDX, IDC_ERRORDELAY, m_wndErrordelay);
	DDX_Control(pDX, IDC_CREATEBTNDELAY, m_wndCreateBtnDelay);
	DDX_Control(pDX, IDC_CHARSELDELAY, m_wndCharSelDelay);
	DDX_Text(pDX, IDC_CHARSELDELAY, m_dwCharSelDelay);
	DDV_MinMaxDWord(pDX, m_dwCharSelDelay, 5000, 9999999);
	DDX_Text(pDX, IDC_ERRORDELAY, m_dwErrorDelay);
	DDV_MinMaxDWord(pDX, m_dwErrorDelay, 10000, 9999999);
	DDX_Text(pDX, IDC_JOINDELAY, m_dwJoinDelay);
	DDV_MinMaxDWord(pDX, m_dwJoinDelay, 5000, 9999999);
	DDX_Text(pDX, IDC_LAUNCHDELAY, m_dwLaunchDelay);
	DDV_MinMaxDWord(pDX, m_dwLaunchDelay, 5000, 9999999);
	DDX_Text(pDX, IDC_LOGINDELAY, m_dwLoginDelay);
	DDV_MinMaxDWord(pDX, m_dwLoginDelay, 5000, 9999999);
	DDX_Text(pDX, IDC_CREATEBTNDELAY, m_dwCreateBtnDelay);
	DDV_MinMaxDWord(pDX, m_dwCreateBtnDelay, 1000, 9999999);
	DDX_Text(pDX, IDC_MINCONN, m_dwMinConndelay);
	DDV_MinMaxDWord(pDX, m_dwMinConndelay, 5000, 9999999);
	DDX_Text(pDX, IDC_CREATEDELAYMIN, m_dwCreateDelayMin);
	DDV_MinMaxDWord(pDX, m_dwCreateDelayMin, 3000, 9999999);
	DDX_Text(pDX, IDC_CREATEDELAYMAX, m_dwCreateDelayMax);
	DDV_MinMaxDWord(pDX, m_dwCreateDelayMax, 3000, 9999999);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CDelayPage, CPropertyPage)
	//{{AFX_MSG_MAP(CDelayPage)
	ON_BN_CLICKED(IDC_DEFAULT, OnDefault)
	ON_WM_DESTROY()
	ON_BN_CLICKED(IDC_HELPMAXWAIT, OnHelpmaxwait)
	ON_BN_CLICKED(IDC_HELPABSWAIT, OnHelpabswait)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDelayPage message handlers

void CDelayPage::LoadSettings(const CD2ConfigData &data)
{
	m_dwCharSelDelay = data.m_dwCharSelDelay;
	m_dwErrorDelay = data.m_dwErrorDelay;
	m_dwCreateDelayMin = data.m_dwCreateDelayMin;
	m_dwCreateDelayMax = data.m_dwCreateDelayMax;
	m_dwJoinDelay = data.m_dwJoinDelay;
	m_dwLaunchDelay = data.m_dwLaunchDelay;
	m_dwLoginDelay = data.m_dwLoginDelay;
	m_dwCreateBtnDelay = data.m_dwCreateBtnDelay;
	m_dwMinConndelay = data.m_dwMinConnDelay;
}

void CDelayPage::ApplySettings(CD2ConfigData &data) const
{
	if (m_bPageInitialized)
	{
		data.m_dwCharSelDelay = m_dwCharSelDelay;
		data.m_dwErrorDelay = m_dwErrorDelay;
		data.m_dwCreateDelayMin = m_dwCreateDelayMin;
		data.m_dwCreateDelayMax = m_dwCreateDelayMax;
		data.CheckCreateDelays();
		data.m_dwJoinDelay = m_dwJoinDelay;
		data.m_dwLaunchDelay = m_dwLaunchDelay;
		data.m_dwLoginDelay = m_dwLoginDelay;
		data.m_dwCreateBtnDelay = m_dwCreateBtnDelay;
		data.m_dwMinConnDelay = m_dwMinConndelay;
	}
}

BOOL CDelayPage::OnInitDialog() 
{
	CPropertyPage::OnInitDialog();
	
	// TODO: Add extra initialization here
	m_wndHelMax.SetIcon(IDI_QUESTIONMARK);
	m_wndHelpAbs.SetIcon(IDI_QUESTIONMARK);
	
	m_bPageInitialized = TRUE;
	
	// CG: The following block was added by the ToolTips component.
	{
		// Create the ToolTip control.
		m_tooltip.Create(this);
		m_tooltip.Activate(TRUE);

		// TODO: Use one of the following forms to add controls:
		// m_tooltip.AddTool(GetDlgItem(IDC_<name>), <string-table-id>);
		// m_tooltip.AddTool(GetDlgItem(IDC_<name>), "<text>");
		m_tooltip.AddTool(GetDlgItem(IDC_HELPMAXWAIT), _T("点击鼠标查看提示"));
		m_tooltip.AddTool(GetDlgItem(IDC_HELPABSWAIT), _T("点击鼠标查看提示"));
		m_tooltip.AddTool(GetDlgItem(IDC_DEFAULT), _T("恢复适合于绝大多数用户的默认设置"));
	}
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CDelayPage::OnDefault() 
{
	// TODO: Add your control notification handler code here
	m_dwCharSelDelay = DEF_DELAY_CHARSEL;
	m_dwErrorDelay = DEF_DELAY_ERROR;
	m_dwCreateDelayMin = DEF_DELAY_CREATEMIN;
	m_dwCreateDelayMax = DEF_DELAY_CREATEMAX;
	m_dwJoinDelay = DEF_DELAY_JOIN;
	m_dwLaunchDelay = DEF_DELAY_LAUNCH;
	m_dwLoginDelay = DEF_DELAY_LOGIN;
	m_dwCreateBtnDelay = DEF_DELAY_CREATEBTN;
	m_dwMinConndelay = DEF_DELAY_MINCONN;
	UpdateData(FALSE);
}

void CDelayPage::OnDestroy() 
{
	CPropertyPage::OnDestroy();
	
	// TODO: Add your message handler code here
	if (m_pDescDlg != NULL)
	{
		m_pDescDlg->DestroyWindow();
		delete m_pDescDlg;
	}
}

void CDelayPage::OnHelpmaxwait() 
{
	// TODO: Add your control notification handler code here
	m_pDescDlg->DestroyWindow();
	m_pDescDlg->Create(this);
	CRect rc;
	m_pDescDlg->GetWindowRect(&rc);
	POINT pt;
	::GetCursorPos(&pt);
	
	CRect rc2(pt, CSize(1, 1));
	rc2.InflateRect(rc.Width() / 2, rc.Height() / 2);

	m_pDescDlg->MoveWindow(rc2);
	
	m_pDescDlg->SetHeader(_T("最大等待时限"));
	m_pDescDlg->SetText(_T("    \"最大等待时限\"只是一个上限，程序并不一定非要等待这么久才进行下一项操作，而是将根据实际运行结果来动态决定何时开始下一项操作。只有在最大时限内未能完成相应操作才视为失败。"));
}

void CDelayPage::OnHelpabswait() 
{
	// TODO: Add your control notification handler code here
	m_pDescDlg->DestroyWindow();
	m_pDescDlg->Create(this);
	CRect rc;
	m_pDescDlg->GetWindowRect(&rc);
	POINT pt;
	::GetCursorPos(&pt);
	
	CRect rc2(pt, CSize(1, 1));
	rc2.InflateRect(rc.Width() / 2, rc.Height() / 2);

	m_pDescDlg->MoveWindow(rc2);
	m_pDescDlg->SetHeader(_T("绝对等待时限"));
	m_pDescDlg->SetText(_T("    \"绝对等待时限\"是指程序在运行过程中无法更改的等待时限。无论实际运行结果为何，程序必须要等待这么久才能进行下一项操作。一般用于某些不可能或很难动态判定的操作。"));
}

BOOL CDelayPage::PreTranslateMessage(MSG* pMsg)
{
	if (pMsg->message == WM_LBUTTONDOWN
		|| pMsg->message == WM_RBUTTONDOWN
		|| pMsg->message == WM_MBUTTONDOWN
		|| pMsg->message == WM_KEYDOWN)
	{
		m_pDescDlg->DestroyWindow();
	}
	// CG: The following block was added by the ToolTips component.
	{
		// Let the ToolTip process this message.
		m_tooltip.RelayEvent(pMsg);
	}
	return CPropertyPage::PreTranslateMessage(pMsg);	// CG: This was added by the ToolTips component.
}

BOOL CDelayPage::OnSetActive() 
{
	// TODO: Add your specialized code here and/or call the base class
	g_nActivePage = PAGE_DELAY;
	return CPropertyPage::OnSetActive();
}
