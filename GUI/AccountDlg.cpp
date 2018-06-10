// AccountDlg.cpp : implementation file
//

#include "stdafx.h"
#include "SIG.h"
#include "AccountDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CAccountDlg dialog


CAccountDlg::CAccountDlg(const CRealmInfo* pri, CWnd* pParent /*=NULL*/)
	: m_pri(pri), CDialog(CAccountDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CAccountDlg)
	m_sName = _T("");
	m_nCharCount = 7;
	m_sPassword = _T("");
	m_sRepeatPassword = _T("");
	m_sBnetIP = _T("");
	m_sSubRealm = _T("");
	//}}AFX_DATA_INIT
}


void CAccountDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CAccountDlg)
	DDX_Control(pDX, IDC_REPEATPASSWORD, m_wndRepeatPassword);
	DDX_Control(pDX, IDC_PREFERRED, m_wndPreferred);
	DDX_Control(pDX, IDC_PASSWORD, m_wndPassword);
	DDX_Control(pDX, IDC_BNETIP, m_wndBNetIP);
	DDX_Control(pDX, IDC_ACCOUNT, m_wndAccount);
	DDX_Control(pDX, IDC_REALMS, m_wndRealms);
	DDX_Text(pDX, IDC_ACCOUNT, m_sName);
	DDX_CBIndex(pDX, IDC_CHARCOUNT, m_nCharCount);
	DDX_Text(pDX, IDC_PASSWORD, m_sPassword);
	DDX_Text(pDX, IDC_REPEATPASSWORD, m_sRepeatPassword);
	DDX_Text(pDX, IDC_BNETIP, m_sBnetIP);
	DDX_Text(pDX, IDC_PREFERRED, m_sSubRealm);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CAccountDlg, CDialog)
	//{{AFX_MSG_MAP(CAccountDlg)
	ON_BN_CLICKED(IDC_REALMEDIT, OnRealmedit)
	ON_BN_CLICKED(IDC_REALMREFRESH, OnRealmrefresh)
	ON_CBN_SELCHANGE(IDC_REALMS, OnSelchangeRealms)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CAccountDlg message handlers

void CAccountDlg::OnRealmedit() 
{
	// TODO: Add your control notification handler code here
	HANDLE h = ::ShellExecute(NULL, _T("open"), g_sAppDir + _T("\\D2Gateway.exe"), NULL, g_sAppDir, SW_SHOW);
	if ((long)h <= 32L)
	{
		MessageBox(_T("无法运行D2Gateway.exe。"), _T("运行错误"), MB_ICONEXCLAMATION);
	}
}

void CAccountDlg::OnRealmrefresh() 
{
	// TODO: Add your control notification handler code here
	ListRealms();
}

void CAccountDlg::OnOK() 
{
	// TODO: Add extra validation here	
	UpdateData(TRUE);
	m_sName.TrimLeft();
	m_sName.TrimRight();
	if (m_sName.IsEmpty())
	{
		MessageBox(_T("Account name must not be null."), _T("Invalid Input"), MB_ICONEXCLAMATION);
		m_wndAccount.Highlight();
		return;
	}

	m_sPassword.TrimLeft();
	m_sPassword.TrimRight();
	m_sRepeatPassword.TrimLeft();
	m_sRepeatPassword.TrimRight();
	if (m_sPassword.IsEmpty() || m_sPassword != m_sRepeatPassword)
	{
		MessageBox(_T("\"密码\"与\"重复密码\"输入的内容必须一致，并且不能为空。"), _T("Invalid Input"), MB_ICONEXCLAMATION);
		m_wndPassword.Highlight();
		return;
	}

	EndDialog(IDOK);
}

void CAccountDlg::ListRealms()
{
	int nIndex = 0;
	m_wndRealms.ResetContent();
	while (TRUE)
	{
		TCHAR szName[521] = _T("");
		TCHAR szPreferred[521] = _T("");
		TCHAR szBnetIP[521] = _T("");
		if (m_pri->GetRealmByIndex(nIndex++, szName, szBnetIP, szPreferred))
		{
			m_wndRealms.AddString(szName);
		}
		else
		{
			break;
		}
	}
	
	if (m_sBnetIP.IsEmpty())
	{
		// highlight default realm
		TCHAR szPreferred[521] = _T("");
		TCHAR szBnetIP[521] = _T("");
		if (m_pri->GetDefaultRealm(szBnetIP, szPreferred))
		{
			TCHAR szName[512] = _T("");
			if (m_pri->FindRealmName(szBnetIP, szPreferred, szName))
			{
				m_wndRealms.SetCurSel(m_wndRealms.FindStringExact(0, szName));
			}

			SetDlgItemText(IDC_BNETIP, szBnetIP);
			SetDlgItemText(IDC_PREFERRED, szPreferred);
		}
	}
	else
	{
		TCHAR szName[521] = _T("");
		if (m_pri->FindRealmName(m_sBnetIP, m_sSubRealm, szName))
		{
			m_wndRealms.SetCurSel(m_wndRealms.FindStringExact(0, szName));
		}
	}
}

BOOL CAccountDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	// TODO: Add extra initialization here
	ListRealms();

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CAccountDlg::OnSelchangeRealms() 
{
	// TODO: Add your control notification handler code here
	CString sName;
	m_wndRealms.GetLBText(m_wndRealms.GetCurSel(), sName);
	TCHAR szBnetIP[512] = _T("");
	TCHAR szPreferred[512] = _T("");

	m_pri->FindRealmDetail(sName, szBnetIP, szPreferred);
	SetDlgItemText(IDC_BNETIP, szBnetIP);
	SetDlgItemText(IDC_PREFERRED, szPreferred);
}

CString CAccountDlg::GetName() const
{
	return m_sName;
}

CString CAccountDlg::GetPassword() const
{
	return m_sPassword;
}

CString CAccountDlg::GetBNetIP() const
{
	return m_sBnetIP;
}

CString CAccountDlg::GetSubRealm() const
{
	return m_sSubRealm;
}

void CAccountDlg::SetName(LPCTSTR lpName)
{
	m_sName = lpName;
}

void CAccountDlg::SetPassword(LPCTSTR lpPassword)
{
	m_sPassword = lpPassword;
	m_sRepeatPassword = lpPassword;
}

void CAccountDlg::SetBNetIP(LPCTSTR lpBnetIP)
{
	m_sBnetIP = lpBnetIP;
}

void CAccountDlg::SetSubRealm(LPCTSTR lpSubRealm)
{
	m_sSubRealm = lpSubRealm;
}

int CAccountDlg::GetCharCount() const
{
	return m_nCharCount + 1;
}

void CAccountDlg::SetCharCount(int nCharCount)
{
	if (nCharCount > 0 && nCharCount < 9)
		m_nCharCount = nCharCount - 1;
	else
		m_nCharCount = 7;
}
