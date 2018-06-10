// SigRstDlg.cpp : implementation file
//

#include "stdafx.h"
#include "SigRst.h"
#include "SigRstDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CSigRstDlg dialog

CSigRstDlg::CSigRstDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CSigRstDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CSigRstDlg)
	m_sMsg = _T("");
	//}}AFX_DATA_INIT
	// Note that LoadIcon does not require a subsequent DestroyIcon in Win32
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
	m_nRemain = max(10, _ttoi(AfxGetApp()->m_lpCmdLine));
	m_sMsg.Format(_T("%d秒后重新启动SIG..."), m_nRemain--);
}

void CSigRstDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CSigRstDlg)
	DDX_Control(pDX, IDCANCEL, m_wndCancel);
	DDX_Text(pDX, IDC_MSG, m_sMsg);
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CSigRstDlg, CDialog)
	//{{AFX_MSG_MAP(CSigRstDlg)
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_WM_TIMER()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CSigRstDlg message handlers

BOOL CSigRstDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	// Set the icon for this dialog.  The framework does this automatically
	//  when the application's main window is not a dialog
	SetIcon(m_hIcon, TRUE);			// Set big icon
	SetIcon(m_hIcon, FALSE);		// Set small icon
	
	// TODO: Add extra initialization here
	SetWindowText(m_sMsg);
	m_wndCancel.DrawTransparent();
	SetTimer(ID_TMR_RST, 1000, NULL);
	return TRUE;  // return TRUE  unless you set the focus to a control
}

// If you add a minimize button to your dialog, you will need the code below
//  to draw the icon.  For MFC applications using the document/view model,
//  this is automatically done for you by the framework.

void CSigRstDlg::OnPaint() 
{
	if (IsIconic())
	{
		CPaintDC dc(this); // device context for painting

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
		CDialog::OnPaint();		
	}
}

// The system calls this to obtain the cursor to display while the user drags
//  the minimized window.
HCURSOR CSigRstDlg::OnQueryDragIcon()
{
	return (HCURSOR) m_hIcon;
}

void CSigRstDlg::OnTimer(UINT nIDEvent) 
{
	// TODO: Add your message handler code here and/or call default
	if (nIDEvent == ID_TMR_RST)
	{
		if (m_nRemain > 0)
		{
			m_sMsg.Format(_T("%d秒后重新启动SIG..."), m_nRemain--);
			SetWindowText(m_sMsg);
			UpdateData(FALSE);
		}
		else
		{
			KillTimer(ID_TMR_RST);
			::ShellExecute(NULL, _T("open"), _T("sig.exe"), _T("-autostart -noclean -reset"), _T(".\\"), SW_MINIMIZE);
			EndDialog(IDCANCEL);
		}
	}
	CDialog::OnTimer(nIDEvent);
}

void CSigRstDlg::OnCancel() 
{
	// TODO: Add extra cleanup here
	KillTimer(ID_TMR_RST);
	CDialog::OnCancel();
}
