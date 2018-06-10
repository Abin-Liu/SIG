// RealmDlg.cpp : implementation file
//

#include "stdafx.h"
#include "D2Gateway.h"
#include "RealmDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CRealmDlg dialog


CRealmDlg::CRealmDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CRealmDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CRealmDlg)
	m_sBnetIP = _T("");
	m_sName = _T("");
	m_sPreferred = _T("");
	//}}AFX_DATA_INIT
}


void CRealmDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CRealmDlg)
	DDX_Control(pDX, IDC_PREFERRED, m_wndPreferred);
	DDX_Control(pDX, IDC_NAME, m_wndName);
	DDX_Control(pDX, IDC_BNETIP, m_wndBnetIP);
	DDX_Text(pDX, IDC_BNETIP, m_sBnetIP);
	DDX_Text(pDX, IDC_NAME, m_sName);
	DDX_Text(pDX, IDC_PREFERRED, m_sPreferred);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CRealmDlg, CDialog)
	//{{AFX_MSG_MAP(CRealmDlg)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CRealmDlg message handlers

void CRealmDlg::SetRealmInfo(LPCTSTR lpName, LPCTSTR lpBnetIP, LPCTSTR lpPreferred)
{
	m_sName = lpName;
	m_sBnetIP = lpBnetIP;
	m_sPreferred = lpPreferred;
}

CString CRealmDlg::GetRealmName() const
{
	return m_sName;
}

CString CRealmDlg::GetBnetIP() const
{
	return m_sBnetIP;
}

CString CRealmDlg::GetPreferred() const
{
	return m_sPreferred;
}

void CRealmDlg::OnOK() 
{
	// TODO: Add extra validation here
	UpdateData(TRUE);
	m_sName.TrimLeft();
	m_sName.TrimRight();
	m_sBnetIP.TrimLeft();
	m_sBnetIP.TrimRight();
	m_sPreferred.TrimLeft();
	m_sPreferred.TrimRight();

	if (m_sBnetIP.IsEmpty() || (m_sName.IsEmpty() && m_sPreferred.IsEmpty()))
		return;

	if (m_sName.IsEmpty())
		m_sName = m_sPreferred;

	if (m_sPreferred.IsEmpty())
		m_sPreferred = m_sName;

	EndDialog(IDOK);
}
