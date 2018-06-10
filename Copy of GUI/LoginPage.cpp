// LoginPage.cpp : implementation file
//

#include "stdafx.h"
#include "SIG.h"
#include "LoginPage.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CLoginPage property page

IMPLEMENT_DYNCREATE(CLoginPage, CPropertyPage)

CLoginPage::CLoginPage() : CPropertyPage(CLoginPage::IDD)
{
	//{{AFX_DATA_INIT(CLoginPage)
	m_sAccount = _T("");
	m_sPassword = _T("");
	m_sRepeatPassword = _T("");
	m_sBnetIP = _T("");
	m_sSubRealm = _T("");
	//}}AFX_DATA_INIT

	m_bPageInitialized = FALSE;
	m_nCharPosition = 0;
}

CLoginPage::~CLoginPage()
{
}

void CLoginPage::DoDataExchange(CDataExchange* pDX)
{
	CPropertyPage::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CLoginPage)
	DDX_Control(pDX, IDC_REPEATPASSWORD, m_wndRepeatPassword);
	DDX_Control(pDX, IDC_PREFERRED, m_wndPreferred);
	DDX_Control(pDX, IDC_PASSWORD, m_wndPassword);
	DDX_Control(pDX, IDC_BNETIP, m_wndBNetIP);
	DDX_Control(pDX, IDC_ACCOUNT, m_wndAccount);
	DDX_Control(pDX, IDC_CHAR8, m_wndChar8);
	DDX_Control(pDX, IDC_CHAR7, m_wndChar7);
	DDX_Control(pDX, IDC_CHAR6, m_wndChar6);
	DDX_Control(pDX, IDC_CHAR5, m_wndChar5);
	DDX_Control(pDX, IDC_CHAR4, m_wndChar4);
	DDX_Control(pDX, IDC_CHAR3, m_wndChar3);
	DDX_Control(pDX, IDC_CHAR2, m_wndChar2);
	DDX_Control(pDX, IDC_CHAR1, m_wndChar1);
	DDX_Control(pDX, IDC_REALMS, m_wndRealms);
	DDX_Text(pDX, IDC_ACCOUNT, m_sAccount);
	DDX_Text(pDX, IDC_PASSWORD, m_sPassword);
	DDX_Text(pDX, IDC_REPEATPASSWORD, m_sRepeatPassword);
	DDX_Text(pDX, IDC_BNETIP, m_sBnetIP);
	DDX_Text(pDX, IDC_PREFERRED, m_sSubRealm);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CLoginPage, CPropertyPage)
	//{{AFX_MSG_MAP(CLoginPage)
	ON_BN_CLICKED(IDC_REALMEDIT, OnRealmedit)
	ON_BN_CLICKED(IDC_REALMREFRESH, OnRealmrefresh)
	ON_BN_CLICKED(IDC_CHAR1, OnChar1)
	ON_BN_CLICKED(IDC_CHAR2, OnChar2)
	ON_BN_CLICKED(IDC_CHAR3, OnChar3)
	ON_BN_CLICKED(IDC_CHAR4, OnChar4)
	ON_BN_CLICKED(IDC_CHAR5, OnChar5)
	ON_BN_CLICKED(IDC_CHAR6, OnChar6)
	ON_BN_CLICKED(IDC_CHAR7, OnChar7)
	ON_BN_CLICKED(IDC_CHAR8, OnChar8)
	ON_CBN_SELCHANGE(IDC_REALMS, OnSelchangeRealms)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CLoginPage message handlers

void CLoginPage::LoadSettings(const CD2ConfigData &data)
{
	m_sAccount = data.m_sAccount;
	m_sPassword = data.m_sPassword;
	m_sRepeatPassword = data.m_sPassword;
	m_nCharPosition = data.m_nCharPosition - 1;
	m_sBnetIP = data.m_sBnetIP;
	m_sSubRealm = data.m_sSubRealm;
	m_ri = data.m_ri;
}

void CLoginPage::ApplySettings(CD2ConfigData &data) const
{
	if (m_bPageInitialized)
	{
		data.m_sAccount = m_sAccount;
		data.m_sPassword = m_sPassword;
		data.m_nCharPosition = m_btns.GetCheck() + 1;
		data.m_sBnetIP = m_sBnetIP;
		data.m_sSubRealm = m_sSubRealm;
		data.m_ri = m_ri;
	}
}

BOOL CLoginPage::OnInitDialog() 
{
	CPropertyPage::OnInitDialog();

	// TODO: Add extra initialization here
	
	m_bPageInitialized = TRUE;

	m_btns.SetIconRes(IDI_YES, IDI_NO);
	m_btns.RemoveAll();
	m_btns.Add(&m_wndChar1);
	m_btns.Add(&m_wndChar2);
	m_btns.Add(&m_wndChar3);
	m_btns.Add(&m_wndChar4);
	m_btns.Add(&m_wndChar5);
	m_btns.Add(&m_wndChar6);
	m_btns.Add(&m_wndChar7);
	m_btns.Add(&m_wndChar8);

	ListRealms();
	m_btns.SetCheck(m_nCharPosition, TRUE);
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CLoginPage::ListRealms()
{
	int nIndex = 0;
	m_wndRealms.ResetContent();
	while (TRUE)
	{
		TCHAR szName[521] = _T("");
		TCHAR szPreferred[521] = _T("");
		TCHAR szBnetIP[521] = _T("");
		if (m_ri.GetRealmByIndex(nIndex++, szName, szBnetIP, szPreferred))
		{
			m_wndRealms.AddString(szName);
		}
		else
		{
			break;
		}
	}

	TCHAR szRealmName[521] = _T("");
	int nItem = -1;
	if (m_ri.FindRealmName(m_sBnetIP, m_sSubRealm, szRealmName))
	{
		nItem = m_wndRealms.FindStringExact(0, szRealmName);
	}

	if (nItem != -1)
	{
		// highlight pre set realm
		m_wndRealms.SetCurSel(nItem);
		SetDlgItemText(IDC_BNETIP, m_sBnetIP);
		SetDlgItemText(IDC_PREFERRED, m_sSubRealm);
	}
	else
	{
		// highlight default realm
		TCHAR szPreferred[521] = _T("");
		TCHAR szBnetIP[521] = _T("");
		if (m_ri.GetDefaultRealm(szBnetIP, szPreferred))
		{
			TCHAR szName[512] = _T("");
			if (m_ri.FindRealmName(szBnetIP, szPreferred, szName))
			{
				m_wndRealms.SetCurSel(m_wndRealms.FindStringExact(0, szName));
			}

			SetDlgItemText(IDC_BNETIP, szBnetIP);
			SetDlgItemText(IDC_PREFERRED, szPreferred);
		}
	}	
}

void CLoginPage::OnRealmedit() 
{
	// TODO: Add your control notification handler code here
	HANDLE h = ::ShellExecute(NULL, _T("open"), g_sAppDir + _T("\\D2Gateway.exe"), NULL, g_sAppDir, SW_SHOW);
	if ((long)h <= 32L)
	{
		MessageBox(_T("Unable to launch \"D2Gateway.exe\"!"), _T("D2Gateway.exe"), MB_ICONEXCLAMATION);
	}
}

void CLoginPage::OnRealmrefresh() 
{
	// TODO: Add your control notification handler code here
	ListRealms();
}

void CLoginPage::OnChar1() 
{
	// TODO: Add your control notification handler code here
	m_btns.SetCheck(0);
}

void CLoginPage::OnChar2() 
{
	// TODO: Add your control notification handler code here
	m_btns.SetCheck(1);
}

void CLoginPage::OnChar3() 
{
	// TODO: Add your control notification handler code here
	m_btns.SetCheck(2);
}

void CLoginPage::OnChar4() 
{
	// TODO: Add your control notification handler code here
	m_btns.SetCheck(3);
}

void CLoginPage::OnChar5() 
{
	// TODO: Add your control notification handler code here
	m_btns.SetCheck(4);
}

void CLoginPage::OnChar6() 
{
	// TODO: Add your control notification handler code here
	m_btns.SetCheck(5);
}

void CLoginPage::OnChar7() 
{
	// TODO: Add your control notification handler code here
	m_btns.SetCheck(6);
}

void CLoginPage::OnChar8() 
{
	// TODO: Add your control notification handler code here
	m_btns.SetCheck(7);
}

void CLoginPage::OnSelchangeRealms() 
{
	// TODO: Add your control notification handler code here
	CString sName;
	m_wndRealms.GetLBText(m_wndRealms.GetCurSel(), sName);
	TCHAR szBnetIP[512] = _T("");
	TCHAR szPreferred[512] = _T("");

	m_ri.FindRealmDetail(sName, szBnetIP, szPreferred);
	SetDlgItemText(IDC_BNETIP, szBnetIP);
	SetDlgItemText(IDC_PREFERRED, szPreferred);
}

BOOL CLoginPage::OnKillActive() 
{
	// TODO: Add your specialized code here and/or call the base class
	UpdateData(TRUE);
	if (m_sPassword != m_sRepeatPassword)
	{
		MessageBox(_T("\"密码\"与\"重复密码\"输入的内容必须一致。"), _T("密码不符"), MB_ICONEXCLAMATION);
		m_wndPassword.Highlight();
		return FALSE;
	}
	else
	{
		return CPropertyPage::OnKillActive();
	}
}

BOOL CLoginPage::OnSetActive() 
{
	// TODO: Add your specialized code here and/or call the base class
	g_nActivePage = PAGE_LOGIN;
	return CPropertyPage::OnSetActive();
}
