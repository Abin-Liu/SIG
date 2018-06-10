// DescDlg.cpp : implementation file
//

#include "stdafx.h"
#include "SIG.h"
#include "DescDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CDescDlg dialog


CDescDlg::CDescDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CDescDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CDescDlg)
	//}}AFX_DATA_INIT
}


void CDescDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDescDlg)
	DDX_Control(pDX, IDC_CONTENTS, m_wndContents);
	DDX_Control(pDX, IDC_HEAD, m_wndHead);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CDescDlg, CDialog)
	//{{AFX_MSG_MAP(CDescDlg)
	ON_WM_PAINT()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDescDlg message handlers

void CDescDlg::SetText(LPCTSTR lpsz)
{
	m_sContents = lpsz;
	if (IsWindow(m_hWnd))
		m_wndContents.SetText(lpsz);
}

BOOL CDescDlg::Create(CWnd* pParentWnd) 
{
	// TODO: Add your specialized code here and/or call the base class	
	return CDialog::Create(IDD, pParentWnd);
}

BOOL CDescDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	// TODO: Add extra initialization here
	m_wndHead.SetFontSize(12);
	m_wndHead.SetFontBold(TRUE);
	m_wndHead.SetText(m_sHeader);
	m_wndContents.SetText(m_sContents);
	m_wndHead.SetTransparent(TRUE);
	m_wndContents.SetTransparent(TRUE);
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CDescDlg::SetHeader(LPCTSTR lpsz)
{
	m_sHeader = lpsz;

	if (IsWindow(m_hWnd))
		m_wndHead.SetText(lpsz);
}

BOOL CDescDlg::PreTranslateMessage(MSG* pMsg) 
{
	// TODO: Add your specialized code here and/or call the base class
	if (pMsg->message == WM_LBUTTONDOWN
		|| pMsg->message == WM_RBUTTONDOWN
		|| pMsg->message == WM_MBUTTONDOWN
		|| pMsg->message == WM_KEYDOWN
		|| pMsg->message == WM_KILLFOCUS)
	{
		ShowWindow(SW_HIDE);
	}
	return CDialog::PreTranslateMessage(pMsg);
}

void CDescDlg::OnPaint() 
{
	CPaintDC dc(this); // device context for painting
	
	// TODO: Add your message handler code here
	CBrush brush;
	brush.CreateSolidBrush(::GetSysColor(COLOR_INFOBK));
	CBrush* p = dc.SelectObject(&brush);
	CRect rect;
	GetClientRect(&rect);
	dc.RoundRect(&rect, CPoint(10, 10));
	dc.SelectObject(p);
	brush.DeleteObject();

	// Do not call CDialog::OnPaint() for painting messages
}