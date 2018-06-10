// ExitDlg.cpp : implementation file
//

#include "stdafx.h"
#include "SIG.h"
#include "ExitDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CExitDlg dialog


CExitDlg::CExitDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CExitDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CExitDlg)
	m_bCloseD2 = FALSE;
	m_bCloseGUI = FALSE;
	//}}AFX_DATA_INIT
}


void CExitDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CExitDlg)
	DDX_Check(pDX, IDC_CLOSED2, m_bCloseD2);
	DDX_Check(pDX, IDC_CLOSEGUI, m_bCloseGUI);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CExitDlg, CDialog)
	//{{AFX_MSG_MAP(CExitDlg)
		// NOTE: the ClassWizard will add message map macros here
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CExitDlg message handlers

BOOL CExitDlg::IsCloseDiabloII() const
{
	return m_bCloseD2;
}

BOOL CExitDlg::IsCloseGUI() const
{
	return m_bCloseGUI;
}

void CExitDlg::Initialize(BOOL bCloseD2, BOOL bCloseGUI)
{
	m_bCloseD2 = bCloseD2;
	m_bCloseGUI = bCloseGUI;
}
